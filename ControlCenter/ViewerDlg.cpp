// ViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "ControlCenter.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "resource.h"



#define WM_MY_EXIT_FULLSCREEN WM_USER+23//注意，不要重复


// CViewerDlg 对话框

IMPLEMENT_DYNAMIC(CViewerDlg, CDialogEx)

CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerDlg::IDD, pParent),m_bFullScrMode(FALSE)
{

}

CViewerDlg::~CViewerDlg()
{
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDPVIEWER, m_Viewer);
}


BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_SMARTSIZE & 0xFFF0)==IDM_SMARTSIZE);
	ASSERT(IDM_SMARTSIZE<0xF000);

	CString sThumbnail,sControl;
	if(!sThumbnail.LoadString(IDS_VIEWERDLG_MENU_THUMBNAIL)) sThumbnail=_T("缩略图");
	if(!sControl.LoadString(IDS_VIEWERDLG_MENU_CONTROL)) sControl=_T("控制");


	CMenu* pMenu=GetSystemMenu(FALSE);
	ASSERT_VALID(pMenu);
	pMenu->AppendMenuW(MF_SEPARATOR);
	pMenu->AppendMenuW(MF_STRING,IDM_SMARTSIZE,sThumbnail/*_T("缩略图")*/);
	pMenu->AppendMenuW(MF_STRING,IDM_REQUESTCONTROL,sControl/*_T("控制")*/);

	return TRUE;
}






BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_MY_EXIT_FULLSCREEN,OnExitFullScreen)
END_MESSAGE_MAP()


// CViewerDlg 消息处理程序






void CViewerDlg::Connect(LPTSTR ticket,LPTSTR username,LPTSTR userpass){
	m_Viewer.Connect(ticket,username,userpass);
}

void CViewerDlg::Disconnect(){
	m_Viewer.Disconnect();
}

void CViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	

	if(this->m_hWnd && m_Viewer.m_hWnd){//注意：第一次的WM_SIZE消息处理是不能使用以下代码，会出错
		CRect rt;
		GetClientRect(&rt);
		m_Viewer.MoveWindow(&rt);
	}	
}




void CViewerDlg::SetFullScreen(BOOL enable)
{
	
	static WINDOWPLACEMENT m_OldPlacement;

	if(enable){//进入全屏

		if (m_bFullScrMode)
		{
			return;
		}
		int Scr_cx=GetSystemMetrics(SM_CXSCREEN);
		int Scr_cy=GetSystemMetrics(SM_CYSCREEN);
		GetWindowPlacement(&m_OldPlacement);
		CRect rc_FullScr,rc_Client,rc_WholeDlg;
		GetWindowRect(&rc_WholeDlg);
		RepositionBars(0,0xFFFF,AFX_IDW_PANE_FIRST,reposQuery,&rc_Client);
		ClientToScreen(&rc_Client);
		rc_FullScr.left=rc_WholeDlg.left-rc_Client.left;
		rc_FullScr.top=rc_WholeDlg.top-rc_Client.top;
		rc_FullScr.right=rc_WholeDlg.right+Scr_cx-rc_Client.right;
		rc_FullScr.bottom=rc_WholeDlg.bottom+Scr_cy-rc_Client.bottom;
		WINDOWPLACEMENT new_wdp;
		new_wdp.length=sizeof(WINDOWPLACEMENT);
		new_wdp.flags=0;
		new_wdp.showCmd=SW_SHOWNORMAL;
		new_wdp.rcNormalPosition=rc_FullScr;
		SetWindowPlacement(&new_wdp);
		m_bFullScrMode=true;


		//全屏操作的对话框
		m_fsOpDlg.Create(CFullScreenOpDlg::IDD,this);
		m_fsOpDlg.SetExitMessage(WM_MY_EXIT_FULLSCREEN);
		m_fsOpDlg.ShowWindow(SW_SHOW);


	}

	else {//退出全屏
		if(m_bFullScrMode==FALSE) return;
		SetWindowPlacement(&m_OldPlacement);
		m_bFullScrMode=FALSE;

		m_fsOpDlg.DestroyWindow();
	}
}


LRESULT CViewerDlg::OnExitFullScreen(WPARAM wParam, LPARAM lParam){
	SetFullScreen(FALSE);
	return 1;
}





void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nID==SC_MAXIMIZE){
		SetFullScreen();
	}
	
	else if(nID==IDM_SMARTSIZE){
		HMENU hMenu=::GetSystemMenu(this->GetSafeHwnd(),FALSE);

		if(m_Viewer.get_SmartSizing()){
			m_Viewer.put_SmartSizing(FALSE);
			::CheckMenuItem(hMenu,IDM_SMARTSIZE,MF_UNCHECKED);
		}
		else {
			m_Viewer.put_SmartSizing(TRUE);
			::CheckMenuItem(hMenu,IDM_SMARTSIZE,MF_CHECKED);
		}
	}
	else if(nID==IDM_REQUESTCONTROL){

		CMenu* pMenu=GetSystemMenu(FALSE);
		BOOL bChecked=pMenu->GetMenuState(IDM_REQUESTCONTROL,MF_CHECKED);
		if(bChecked){		
			m_Viewer.RequestControl(CRdpviewer1::CTRL_LEVEL_VIEW);
			pMenu->CheckMenuItem(IDM_REQUESTCONTROL,MF_UNCHECKED);
		}
		else{
			m_Viewer.RequestControl(CRdpviewer1::CTRL_LEVEL_INTERACTIVE);
			pMenu->CheckMenuItem(IDM_REQUESTCONTROL,MF_CHECKED);
		}
	}
	else{
		CDialogEx::OnSysCommand(nID,lParam);
	}
}


BOOL CViewerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE){
		SetFullScreen(FALSE);
		return TRUE;
	}
	else{
		return CDialogEx::PreTranslateMessage(pMsg);
	}
}


void CViewerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	
	CRect rt(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	rt.InflateRect(10,20,50,20);
	lpMMI->ptMaxSize=CPoint(rt.Size());
	lpMMI->ptMaxTrackSize=CPoint(rt.Size());

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
