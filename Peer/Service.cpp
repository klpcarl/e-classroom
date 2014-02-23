#include "stdafx.h"
#include "Service.h"
#include "ViewerDlg.h"
#include "Message.h"


CService::CService(void)
{
	bLockMode=FALSE;
	pSock=NULL;
	pSharingSession=NULL;
	pEmployer=NULL;
	pViewerDlg=NULL;
}


CService::~CService(void)
{
	StopRDPService();
	StopSockService();
	
}

void CService::SetEmployer(CWnd* pWnd){
	this->pEmployer=pWnd;
}

void CService::SetViewerDlg(CWnd* pWnd){
	this->pViewerDlg=pWnd;	
}


void CService::SetHook(BOOL use){//*************************************************未经测试
	typedef BOOL (* HookFunc)(BOOL);
	static HMODULE hModule=LoadLibrary(_T("GlobalHook.dll"));//这里真的只执行一次吗？

	if(use){
		if(bLockMode) return;//已经锁定
		if(hModule==NULL) hModule=LoadLibrary(_T("GlobalHook.dll"));
		if(hModule){
			HookFunc SetKeyHook=(HookFunc)GetProcAddress(hModule,"SetKeyHook");
			if(SetKeyHook==NULL ||  SetKeyHook(TRUE)==FALSE) return ;
			//在成功设置键盘钩子的情况下安装鼠标钩子
			HookFunc SetMouseHook=(HookFunc)GetProcAddress(hModule,"SetMouseHook");
			if(SetMouseHook) SetMouseHook(TRUE);
			bLockMode=TRUE;
		}
	}
	else{
		if(bLockMode==FALSE) return;
		if(hModule){
			HookFunc SetKeyHook=(HookFunc)GetProcAddress(hModule,"SetKeyHook");
			if(SetKeyHook) SetKeyHook(FALSE);
			HookFunc SetMouseHook=(HookFunc)GetProcAddress(hModule,"SetMouseHook");
			if(SetMouseHook) SetMouseHook(FALSE);
			FreeLibrary(hModule);
			hModule=NULL;
			bLockMode=FALSE;
		}//倘若hModule为NULL，又要解锁，是否要先装载，再调用SetKeyHook(FALSE),最后释放了呢？
	}
}



BOOL CService::GetTicket(CString* ticket){

	if(!pSharingSession) return FALSE;
	IRDPSRAPIInvitationManager * pInvitationManager;
	HRESULT  hr=pSharingSession->get_Invitations(&pInvitationManager);
	if(FAILED(hr)) return FALSE;
	IRDPSRAPIInvitation* pInvitation;
	hr=pInvitationManager->CreateInvitation(B("Trial"),B("Group"),B(""),10,&pInvitation);
	if(FAILED(hr)){
		return FALSE;
	}
	BSTR bstr=0;
	hr=pInvitation->get_ConnectionString(&bstr);
	//SetDlgItemText(IDC_EDIT1,bstr);
	*ticket=bstr;							//好吧，未经验证
	SysFreeString(bstr);

	return TRUE;	
}

void CService::StartRDPService(){
	// TODO: 在此添加控件通知处理程序代码
	if(pSharingSession) return;

	HRESULT hr=CoCreateInstance(__uuidof(RDPSession),NULL,CLSCTX_ALL,__uuidof(IRDPSRAPISharingSession),reinterpret_cast<void**>(&pSharingSession));
	if(FAILED(hr)) return;

	//添加事件通知
	IConnectionPointContainer* pConnectionPointContainer=NULL;
	IConnectionPoint* pConnectionPoint=NULL;
	unsigned long cid;//connection id, critical,use it to unadvise
	hr=pSharingSession->QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pConnectionPointContainer));

	if(FAILED(hr)) return;
	if(pConnectionPointContainer){
		pConnectionPointContainer->FindConnectionPoint(__uuidof(_IRDPSessionEvents),&pConnectionPoint);
		if(pConnectionPoint){
			pConnectionPoint->Advise(&myEvents,&cid);
		}
	}


	//这段代码的作用是什么？

	//创建频道
	wchar_t* chanelName=L"channel1";
	bool compress=false;
	int priority=3;


	
	IRDPSRAPIVirtualChannelManager* cm = 0;
	if(pSharingSession){			
		pSharingSession->get_VirtualChannelManager(&cm);
		if(cm){
			IRDPSRAPIVirtualChannel* c = 0;
			cm->CreateVirtualChannel(B(chanelName),(CHANNEL_PRIORITY)priority,compress ? 0 : CHANNEL_FLAGS_UNCOMPRESSED,&c);
		}
	}
	

	pSharingSession->Open();
}



void CService::StopRDPService(){
	// TODO: 在此添加控件通知处理程序代码
	if(pSharingSession){
		pSharingSession->Close();
		pSharingSession->Release();//这句是必要的吗，那其他的com指针呢？内存泄露？
		pSharingSession=NULL;
	}
}



BOOL CService::StartSockService(LPTSTR addr,int port){
	if(pSock!=NULL) return TRUE;//已经启动了socket服务

	pSock=new CDoorSocket(this);	// caution : 释放资源

	if(!pSock->Create()){
		
		CString sCreateSockFailText,sCreateSockFailCap;
		if(!sCreateSockFailText.LoadString(IDS_CREATESOCKFAIL_TEXT)) sCreateSockFailText=_T("套接字创建失败!");
		if(!sCreateSockFailCap.LoadString(IDS_ERROR_CAP)) sCreateSockFailCap=_T("错误");
		::MessageBox(NULL,sCreateSockFailText,sCreateSockFailCap,MB_OK|MB_ICONERROR);

		delete pSock;
		pSock=NULL;
		return FALSE;
	}

	if(!pSock->Connect(addr,port)){
		CString sConnectSockFailText,sConnectSockFailCap;
		if(!sConnectSockFailText.LoadString(IDS_CONNECTSOCKFAIL_TEXT)) sConnectSockFailText=_T("套接字连接失败!");
		if(!sConnectSockFailCap.LoadString(IDS_ERROR_CAP)) sConnectSockFailCap=_T("错误");
		::MessageBox(NULL,sConnectSockFailText,sConnectSockFailCap,MB_OK|MB_ICONWARNING);
		delete pSock;
		pSock=NULL;
		return FALSE;
	}

	//获得主机名和用户名
	TCHAR hostname[100];
	TCHAR username[100];
	DWORD hostlen=100;
	DWORD userlen=100;

	if(::GetComputerName(hostname,&hostlen) && ::GetUserName(username,&userlen)){
		hostname[hostlen]=_T('/');
		hostname[hostlen+1]=_T('\0');
		CString name(hostname);
		name+=username;
		name=_T("logname")+name;

		pSock->Send(name.GetBuffer(),sizeof(TCHAR)*name.GetLength());
	}
	else{
		CString sNameFailText,sNameFailCap;
		if(!sNameFailText.LoadString(IDS_GETNAMEFAIL_TEXT)) sNameFailText=_T("获取主机名或用户名失败！");
		if(!sNameFailCap.LoadString(IDS_ERROR_CAP)) sNameFailCap=_T("错误");
		::MessageBox(NULL,sNameFailText/*_T("获取主机名或用户名失败！")*/,sNameFailCap/*_T("错误")*/,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}



void CService::StopSockService(){
	if(pSock!=NULL){
		pSock->Close();
		delete pSock;
		pSock=NULL;
	}
}

void CService::OnReceive(CString str){

	if(str.Compare(_T("start sharer"))==0){
		this->StartRDPService();
		CString ticket;
		if(!this->GetTicket(&ticket)){
			
			ticket=_T("sharer not started ticket failed");
			this->StopRDPService();	//终止与回滚
			Send(ticket);

			CString sTickFailText,sTickFailCap;
			if(!sTickFailText.LoadString(IDS_GETTICKETFAIL_TEXT)) sTickFailText=_T("生成 Ticket 失败！");
			if(!sTickFailCap.LoadString(IDS_ERROR_CAP)) sTickFailCap=_T("错误！");
			::MessageBox(NULL,sTickFailText,sTickFailCap,MB_OK|MB_ICONERROR);

			//MessageBox(NULL,_T("生成ticket失败！"),_T("错误"),MB_OK|MB_ICONERROR);
		}else{
			ticket=_T("sharer started ticket:")+ticket;
			Send(ticket);
		}

		//通知控制台
		
	
	}


	else if(str.Find(_T("stop sharer"))==0){

		if(pSharingSession!=NULL){
			pSharingSession->Close();
			pSharingSession->Release();
			pSharingSession=NULL;
		}
		Send(_T("sharer stopped"));
	}

	else if(str.Find(_T("start viewer"))==0){

		//pViewerDlg=new CViewerDlg;//释放资源---------------------------------------
		CString ticket=str.Mid(str.Find(_T("<E><A")));//提取ticket。注意提取的方法是基于ticket的格式，总以<E><A开头。问题是真的是这样吗？
		//if(pEmployer==NULL) MessageBox(NULL,_T("ticket waisted, I am not ready now"),_T("错误"),MB_OK|MB_ICONERROR);
		SendMessage(pEmployer->GetSafeHwnd(),WM_MY_STARTVIEWER,0,(LPARAM)&ticket);

		Send(_T("viewer started"));
	}

	else if(str.Find(_T("stop viewer"))==0){

		PostMessage(pEmployer->GetSafeHwnd(),WM_MY_STOPVIEWER,0,0);
		
		Send(_T("viewer stopped"));
	}

	else if(str.Find(_T("request control"))==0){
		CString param;
		if(str.Find(_T("interactive"))>0) param=_T("interactive");
		else if(str.Find(_T("view"))>0) param=_T("view");
		else if(str.Find(_T("none"))>0) param=_T("none");

		if(pViewerDlg)
		SendMessage(pViewerDlg->GetSafeHwnd(),WM_MY_RDPCONTROL,0,(LPARAM)&param);

	}


	else if(str.Find(_T("open monitor"))==0){//这与start server命令的处理是一样的
		this->StartRDPService();
		CString ticket;

		if(!this->GetTicket(&ticket)){
			
			//通知控制台
			ticket=_T("monitor not opened ticket failed");
			this->StopRDPService();	//终止与回滚
			Send(ticket);

			CString sTickFailText,sTickFailCap;
			if(!sTickFailText.LoadString(IDS_GETTICKETFAIL_TEXT)) sTickFailText=_T("生成 Ticket 失败！");
			if(!sTickFailCap.LoadString(IDS_ERROR_CAP)) sTickFailCap=_T("错误！");
			::MessageBox(NULL,sTickFailText,sTickFailCap,MB_OK|MB_ICONERROR);

		}else{
			ticket=_T("monitor opened ticket:")+ticket;//回应的命令与start server略有不同
			Send(ticket);
		}

		
	}

	else if(str.Find(_T("close monitor"))==0){
		if(pSharingSession!=NULL){
			pSharingSession->Close();
			pSharingSession->Release();
			pSharingSession=NULL;
		}

		Send(_T("monitor closed"));

	}

	



	else if(str.Find(_T("lock"))==0){		
		PostMessage(pEmployer->GetSafeHwnd(),WM_MY_LOCK,0,0);
	}

	else if(str.Find(_T("unlock"))==0){
		PostMessage(pEmployer->GetSafeHwnd(),WM_MY_UNLOCK,0,0);
	}

	else if(str.Find(_T("silence"))==0){

	}
	else if(str.Find(_T("unsilence"))==0){

	}

	else if(str.Find(_T("config"))==0){

		SendMessage(pEmployer->GetSafeHwnd(),WM_MY_CONFIG,0,(LPARAM)&str);		
	}


}

void CService::OnSockClose(){//如何通知窗口这件事情呢？
	StopSockService();
	StopRDPService();
	SetHook(FALSE);
	PostMessage(pEmployer->GetSafeHwnd(),WM_MY_SOCKCLOSE,0,0);//it does work	
}


BOOL CService::Send(CString str){
	if(pSock==NULL) return FALSE;
	int count=pSock->Send(str.GetBuffer(str.GetLength()),sizeof(TCHAR)*str.GetLength());
	return (count==sizeof(TCHAR)*str.GetLength());
}