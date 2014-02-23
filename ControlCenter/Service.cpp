#include "stdafx.h"
#include "Service.h"
#include "Message.h"


CService::CService(void)
{
	pEmployer=NULL;
	pLisSock=NULL;
	pTreeDlg=NULL;
	pShareSessionDlg=NULL;
	pMonitorSessionDlg=NULL;
	pHandDlg=NULL;
}


CService::~CService(void)
{
}


void CService::SetEmployer(CWnd* pWnd){
	this->pEmployer=pWnd;
}

void CService::SetTreeDlg(CWnd* pWnd){
	this->pTreeDlg=pWnd;	
}

void CService::SetShareSessionDlg(CWnd* pWnd){
	this->pShareSessionDlg=pWnd;
}

void CService::SetMonitorSessionDlg(CWnd* pWnd){
	this->pMonitorSessionDlg=pWnd;
}

CWnd* CService::GetMonitorSessionDlg(){
	return this->pMonitorSessionDlg;
}

void CService::SetHandDlg(CWnd* pWnd){
	this->pHandDlg=pWnd;
}



CString CService::GetNameBySock(CDoorSock* pSock){

	NameSockMapType::iterator pNameSock=nameSockMap.begin();
	while(pNameSock!=nameSockMap.end()){
		if(pNameSock->second==pSock) return pNameSock->first;
		pNameSock++;
	}
	return _T("");
}


CDoorSock* CService::GetSockByName(CString name){
	NameSockMapType::iterator pNameSock=nameSockMap.find(name);
	if(pNameSock==nameSockMap.end()) return NULL;
	return pNameSock->second;
}



// {  socket 服务:

BOOL CService::StartSockService(int port){
	if(pLisSock!=NULL) return TRUE;	//已经开启了服务,  是什么都不做呢，还是清理并重新执行呢？
	pLisSock=new CListenSock(this);
	if(!(pLisSock->Create(port))){
		delete pLisSock;
		pLisSock=NULL;
		//::MessageBox(NULL,_T("创建套接字失败！"),_T("错误"),MB_OK);
		return FALSE;
	}
	if(!(pLisSock->Listen(20))){
		pLisSock->Close();//CAUTION: 这个 close 是需要的吗？
		delete pLisSock;
		pLisSock=NULL;
		//::MessageBox(NULL,_T("套接字监听失败!"),_T("错误"),MB_OK);
		return FALSE;
	}
	return TRUE;
}

void CService::StopSockService(){
	if(pLisSock==NULL) return;
	pLisSock->Close();
	delete pLisSock;
	pLisSock=NULL;
}

void CService::OnSockAccept(){
	CDoorSock* pDoorSock=new CDoorSock(this);

	if(!pLisSock->Accept(*pDoorSock)){
		delete pDoorSock;
		return;
	}

	SockSend(pDoorSock,_T("config disable exit"));//居然在这里写！！！！！！！！！！！！！！

	doorSockList.push_back(pDoorSock);
}


void CService::OnSockReceive(CDoorSock* pDoorSock,CString str){
	if(pDoorSock==NULL) return;


//	::MessageBox(NULL,_T("OnSockReceive: ")+str,_T("收到"),MB_OK);

	//命令处理机
	if(str.Find(_T("logname"))==0){				//主动注册名称
		CString name=str.Mid(7);
		nameSockMap.insert(NameSockMapType::value_type(name,pDoorSock));
		//通知组管理界面有新的用户登录
	
		if(pTreeDlg){		
			SendMessage(pTreeDlg->GetSafeHwnd(),WM_MY_LOGNAME,0,(LPARAM)&name);// caution : 这是不好的！！！！！！！
		}
		if(pEmployer){
			PostMessage(pEmployer->GetSafeHwnd(),WM_MY_LOGNAME,0,0);//向主对话框发消息，更新在线人数
		}
	}

	else if(str.Find(_T("logoff"))==0){//这个没用到
		delete pDoorSock;
		doorSockList.remove(pDoorSock);
		CString name=GetNameBySock(pDoorSock);
		if(name!=_T("")) nameSockMap.erase(name);
		//通知界面更新
	}

	else if(str.Find(_T("sharer started"))==0){//开启RDP分享
		pDoorSock->bSharer=TRUE;

		CString ticket=str.Mid(str.Find(_T("<E><A")));//提取ticket。注意提取的方法是基于ticket的格式，总以<E><A开头。问题是真的是这样吗？
		pDoorSock->ticket=ticket;
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;//这是谁发来的？
		//通知界面发票给viewer
		if(pShareSessionDlg==NULL) return;
		SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SHARERSTARTED,(WPARAM)&name,(LPARAM)&ticket);//SendMessage可能带来性能问题，我真的很想用PostMessage
	}

	else if(str.Find(_T("sharer not started"))==0){//这是start sharer名的失败回应
		//pDoorSock->bSharer=FALSE;

		CString name=GetNameBySock(pDoorSock);
		SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SHARERNOTSTARTED,(WPARAM)&name,0);
	}

	else if(str.Find(_T("sharer stopped"))==0){

		pDoorSock->bSharer=FALSE;
//		pDoorSock->bMonitor=FALSE;//这样写与伪造回应是冲突的
//		pDoorSock->ticket=_T("");
		CString name=GetNameBySock(pDoorSock);
		if(name!=_T("")) SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SHARERSTOPPED,(WPARAM)&name,0);
	}

	else if(str.Find(_T("viewer started"))==0){//开启了RDP观看
		pDoorSock->bViewer=TRUE;
		
	}

	else if(str.Find(_T("viewer stopped"))==0){
		pDoorSock->bViewer=FALSE;	
	}

	else if(str.Find(_T("monitor opened"))==0){//开启监控
		pDoorSock->bMonitor=TRUE;
		//提取ticket
		pDoorSock->ticket=str.Mid(str.Find(_T("<E><A")));		
		if(pMonitorSessionDlg==NULL) return;
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;
		SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_MONITOROPENED,(WPARAM)&name,0);

	}

	else if(str.Find(_T("monitor not opened"))==0){
		//pDoorSock->bMonitor=FALSE;
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return ;
		SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_MONITORNOTOPENED,(WPARAM)&name,0);
	}

	else if(str.Find(_T("monitor closed"))==0){
//		pDoorSock->bSharer=FALSE;
		pDoorSock->bMonitor=FALSE;

//		pDoorSock->ticket=_T("");

		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;
		SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_MONITORCLOSED,(WPARAM)&name,0);

	}

	else if(str.Find(_T("hand up"))==0){
	
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;
		SendMessage(pHandDlg->GetSafeHwnd(),WM_MY_HANDUP,(WPARAM)&name,0);
		PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_MY_HANDUP,0,0);//注意必须在上一句执行后这句才能执行正确
	}


}

void CService::OnSockClose(CDoorSock* pDoorSock){
	//1.内存更新
	if(pDoorSock==NULL) return;
	delete pDoorSock;
	doorSockList.remove(pDoorSock);
	CString name=GetNameBySock(pDoorSock);
	nameSockMap.erase(name);

	//3.通知界面跟新
	
	SendMessage(pTreeDlg->GetSafeHwnd(),WM_MY_SOCKCLOSE,(WPARAM)&name,0);
	SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SOCKCLOSE,(WPARAM)&name,0);
	SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_SOCKCLOSE,(WPARAM)&name,0);
	PostMessage(pEmployer->GetSafeHwnd(),WM_MY_SOCKCLOSE,0,0);//doesn't care who

	
}


BOOL CService::SockSend(CDoorSock* pDoorSock, CString str){
	if(pDoorSock==NULL) return FALSE;
	//命令过滤
	//只要返回TRUE，上层就会认为发送成功，伪造命令，使拦截时也对上层表现出一致的行为
	if(str.Find(_T("start sharer"))==0 && ( pDoorSock->bSharer || pDoorSock->bMonitor)){
		pDoorSock->bSharer=TRUE;
		OnSockReceive(pDoorSock,_T("sharer started")+pDoorSock->ticket);//伪造
		return TRUE;	
	}
	if(str.Find(_T("stop sharer"))==0){
		if(pDoorSock->bSharer==FALSE || pDoorSock->bMonitor){
			pDoorSock->bSharer=FALSE;
			OnSockReceive(pDoorSock,_T("sharer stopped"));
			return TRUE;
		}		
	}
	if(str.Find(_T("start viewer"))==0 && pDoorSock->bViewer){
		return FALSE;
	}
	if(str.Find(_T("stop viewer"))==0 && pDoorSock->bViewer==FALSE){
		OnSockReceive(pDoorSock,_T("viewer stopped"));
		return TRUE;
	}
	if(str.Find(_T("open monitor"))==0 && (pDoorSock->bMonitor || pDoorSock->bSharer)){
		pDoorSock->bMonitor=TRUE;
		OnSockReceive(pDoorSock,_T("monitor opened")+pDoorSock->ticket);
		return TRUE;
	}
	if(str.Find(_T("close monitor"))==0){
		if(pDoorSock->bMonitor==FALSE || pDoorSock->bSharer){
			pDoorSock->bMonitor=FALSE;
			OnSockReceive(pDoorSock,_T("monitor closed"));
			return TRUE;
		}
	}


	int count=pDoorSock->Send(str.GetBuffer(str.GetLength()),sizeof(TCHAR)*str.GetLength());
	if(count==sizeof(TCHAR)*str.GetLength()) return TRUE;//caution：这种验证方法未经验证
	return FALSE;
}

BOOL CService::SockSend(CString name, CString str){
	NameSockMapType::iterator pNameSock=nameSockMap.find(name);
	if(pNameSock==nameSockMap.end()) return FALSE;
	
	return SockSend(pNameSock->second,str);
}


void CService::SockSendAll(CString cmd){
	for(auto sock:doorSockList){//这是C++标准中的吗？
		SockSend(sock,cmd);
	}
}

// } socket 服务


// { 文件服务：



// } 文件服务



// { 系统服务


// } 系统服务