// MonitorSessionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "MonitorSessionDlg.h"
#include "afxdialogex.h"
#include "NewMonitorSessionDlg.h"
#include "Message.h"
#include "ViewerCtrl.h"
#include "ViewerDlg.h"

// CMonitorSessionDlg 对话框

IMPLEMENT_DYNAMIC(CMonitorSessionDlg, CDialogEx)

CMonitorSessionDlg::CMonitorSessionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorSessionDlg::IDD, pParent)
{
	pService=NULL;
	pMonitorViewerDlg=NULL;
}

CMonitorSessionDlg::~CMonitorSessionDlg()
{	
	if(pMonitorViewerDlg)
		delete pMonitorViewerDlg;
	
}

void CMonitorSessionDlg::SetService(CService* pService){
	this->pService=pService;
}



void CMonitorSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SESSION_LIST,m_list);
}


BEGIN_MESSAGE_MAP(CMonitorSessionDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_SESSION_LIST, &CMonitorSessionDlg::OnNMRClickSessionList)
	ON_MESSAGE(WM_MY_MONITOROPENED,OnMonitorOpened)
	ON_MESSAGE(WM_MY_MONITORNOTOPENED,OnMonitorNotOpened)
	ON_MESSAGE(WM_MY_MONITORCLOSED,OnMonitorClosed)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnSockClose)
	//右键菜单
	ON_COMMAND(IDM_MITEM_OPENWINDOW,OnItemOpenViewerDlg)
	ON_COMMAND(IDM_MITEM_STOP,OnItemStop)
	ON_COMMAND(IDM_MITEM_DELETE,OnItemDelete)
	ON_COMMAND(IDM_MWHITE_CREATE,OnWhiteCreate)
	ON_COMMAND(IDM_MWHITE_OPENWINDOW,OnWhiteOpenViewerDlg)
END_MESSAGE_MAP()


// CMonitorSessionDlg 消息处理程序


void CMonitorSessionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static int i=0;
	static CRect rt;

	if(i++>0){
		GetClientRect(&rt);
		m_list.MoveWindow(&rt);
	}
}


BOOL CMonitorSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	pService->SetMonitorSessionDlg(this);//注册事件
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString sColNameState,sColNameWho;
	if(!sColNameState.LoadString(IDS_MONITORLIST_COLNAME_STATE)) sColNameState=_T("状态");
	if(!sColNameWho.LoadString(IDS_MONITORLIST_COLNAME_WHO)) sColNameWho=_T("监看对象");
	m_list.InsertColumn(0,sColNameState,0,90);
	m_list.InsertColumn(1,sColNameWho,0,160);
	

	m_images.Create(16,16,ILC_COLOR32|ILC_MASK,1,0);//?
	m_images.Add(AfxGetApp()->LoadIconW(IDI_SESSIONSTARTED_ICON));
	m_images.Add(AfxGetApp()->LoadIconW(IDI_SESSIONSTOPPED_ICON));

	m_list.SetImageList(&m_images,LVSIL_SMALL);

	pMonitorViewerDlg=new CMonitorViewerDlg;
	pMonitorViewerDlg->Create(CMonitorViewerDlg::IDD,this);
	
	return TRUE;
}


int CMonitorSessionDlg::GetItemByText(CString name){
	int count=m_list.GetItemCount();
	int i=0;
	for(i=0;i<count;i++){
		if(name==m_list.GetItemText(i,1)) return i;
	}
	return -1;
}


void CMonitorSessionDlg::OnNMRClickSessionList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint cp;
	GetCursorPos(&cp);
	m_list.ScreenToClient(&cp);
	
	CMenu menu;
	menu.LoadMenuW(IDR_MONITORLIST_MENU);

	int nItem=m_list.HitTest(cp);
	m_list.ClientToScreen(&cp);

	if(nItem>=0){
		m_list.SetHotItem(nItem);
		CMenu* pMenu=menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);

	}
	else{
		CMenu* pMenu=menu.GetSubMenu(1);
		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);
	}


	*pResult = 0;
}





LRESULT CMonitorSessionDlg::OnMonitorOpened(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	CString ticket=pService->GetSockByName(name)->ticket;

	if(pMonitorViewerDlg==NULL) return 0;
	
	

	pMonitorViewerDlg->AddViewer(name,ticket);

	//界面
	CString sState;
	sState.LoadString(IDS_MONITORSTATE_STARTED);
	int nItem=m_list.GetItemCount();
	m_list.InsertItem(nItem,sState,0);
	m_list.SetItemText(nItem,1,name);

	return 1;
}

LRESULT CMonitorSessionDlg::OnMonitorNotOpened(WPARAM pName, LPARAM lParam){
	CString name=*(CString*)pName;
	//内存
	mSession.GetMonitorList().remove(name);

	//MonitorViewerDlg
	if(pMonitorViewerDlg){
		pMonitorViewerDlg->DeleteViewer(name);
	}

	//列表视图控件
	CString sState;
	sState.LoadString(IDS_MONITORSTATE_TODELETE);

	int nItem=GetItemByText(name);
	if(nItem==-1){//列表控件中没找到
		int n=m_list.GetItemCount();
		m_list.InsertItem(n,sState,1);
		m_list.SetItemText(n,1,name);

	}
	else {//找到了
		m_list.SetItemText(nItem,0,sState);
		m_list.SetItem(nItem,0,LVIF_IMAGE,0,1,0,0,0,0);
	}

	return 1;
}


LRESULT CMonitorSessionDlg::OnMonitorClosed(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	int nItem=GetItemByText(name);
	mSession.GetMonitorList().remove(name);
	if(nItem==-1) return 0;
	m_list.DeleteItem(nItem);

	if(pMonitorViewerDlg){
		pMonitorViewerDlg->DeleteViewer(name);
	}

	return 1;
}

LRESULT CMonitorSessionDlg::OnSockClose(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	int nItem=GetItemByText(name);
	if(nItem==-1) return 0;
	CString sOffTip;
	sOffTip.Format(IDS_MONITOROFFLINETIP,name);
	MessageBox(sOffTip/*name+_T(" 掉线，对应的监控即将关闭!")*/);
	mSession.GetMonitorList().remove(name);	
	m_list.DeleteItem(nItem);
	return 1;
}

void CMonitorSessionDlg::OnWhiteCreate(){
	int count=mSession.GetMonitorList().size();
	MonitorListType& monitorList=mSession.GetMonitorList();
	set<MonitorListType::value_type> monitorSet(monitorList.begin(),monitorList.end());
	
	CMonitorSession tMonitorSession;
	CNewMonitorSessionDlg dlg(&tMonitorSession,this);
	if(dlg.DoModal()!=IDOK) return;
	if(tMonitorSession.GetMonitorList().size()==0) return;

	// 验证所选的监控对象是否已存在监控列表中
	MonitorListType& tMonitorList=tMonitorSession.GetMonitorList();
	MonitorListType::iterator pMonitor=tMonitorList.begin();
	while(pMonitor!=tMonitorList.end()){
		if(monitorSet.count(*pMonitor)>0) break;
		pMonitor++;
	}
	if(pMonitor!=tMonitorList.end()){//说明监控对象有重复
		CString sMonitorRedundant;
		sMonitorRedundant.Format(IDS_MONITORREDUNDANTTIP,*pMonitor);
		//MessageBox(*pMonitor+_T(" 已在监控列表中!"));
		MessageBox(sMonitorRedundant);
		return;
	}
	//验证...
	
	monitorList.insert(monitorList.end(),tMonitorList.begin(),tMonitorList.end());//caution: not sure
	mSession.StartMonitors(count);	//这是不行的
}

void CMonitorSessionDlg::OnWhiteOpenViewerDlg(){
	if(pMonitorViewerDlg==NULL) return;
	if(pMonitorViewerDlg->GetViewerCount()==0){
		CString sText,sCap;
		sText.LoadString(IDS_MONITORNOSESSION);
		sCap.LoadString(IDS_MONITORCOMMONCAP);
		MessageBox(sText/*_T("当前没有建立任何监控")*/,sCap);
	}
	else{		
		pMonitorViewerDlg->ConnectAllViewers();
		pMonitorViewerDlg->ShowWindow(SW_SHOW);
		
	}
	
}

void CMonitorSessionDlg::OnItemOpenViewerDlg(){
	
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	int nItem=m_list.GetNextSelectedItem(pos);
	CString name=m_list.GetItemText(nItem,1);

	if(pService->GetSockByName(name)->bMonitor==FALSE) return ;

	CString ticket(pService->GetSockByName(name)->ticket);
	
	CViewerDlg dlg;	
	
	dlg.Create(CViewerDlg::IDD);
	dlg.SetWindowText(name);
	dlg.ShowWindow(SW_NORMAL);	
	dlg.Connect((LPTSTR)(LPCTSTR)ticket);
	
	dlg.RunModalLoop();

}

void CMonitorSessionDlg::OnItemStop(){


}

void CMonitorSessionDlg::OnItemDelete(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	int nItem=m_list.GetNextSelectedItem(pos);
	CString name=m_list.GetItemText(nItem,1);

	if(pMonitorViewerDlg!=NULL){		
		pMonitorViewerDlg->DeleteViewer(name);
	}

	CDoorSock* pDoorSock=pService->GetSockByName(name);
	if(pDoorSock->bSharer){
		pDoorSock->bMonitor=FALSE;
		SendMessage(WM_MY_MONITORCLOSED,(WPARAM)&name,0);
	}
	else{
		pService->SockSend(name,_T("close monitor"));
	}
	CString sState;
	sState.LoadString(IDS_MONITORSTATE_TOSTOP);
	m_list.SetItemText(nItem,0,sState);
}