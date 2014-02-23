// HandDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "HandDlg.h"
#include "afxdialogex.h"
#include "Message.h"


// CHandDlg 对话框

IMPLEMENT_DYNAMIC(CHandDlg, CDialogEx)

CHandDlg::CHandDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHandDlg::IDD, pParent)
{

}

CHandDlg::~CHandDlg()
{
}

void CHandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HAND_LIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CHandDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MY_HANDUP,OnHandUp)
	ON_NOTIFY(NM_RCLICK, IDC_HAND_LIST, &CHandDlg::OnNMRClickHandList)
	ON_COMMAND(IDM_WHITE_CLEAR, OnHandListClear)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHandDlg 消息处理程序


void CHandDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
//	CDialogEx::OnClose();
}


BOOL CHandDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CString sWho,sWhen;
	sWho=sWho.LoadString(IDS_HANDDLG_WHO)?sWho:_T("举手人");
	sWhen=sWhen.LoadString(IDS_HANDDLG_WHEN)?sWhen:_T("举手时间");
	m_listCtrl.InsertColumn(0,sWho/*_T("举手人")*/,LVCFMT_LEFT,150);
	m_listCtrl.InsertColumn(1,sWhen/*_T("举手时间")*/,LVCFMT_LEFT,150);	

	return TRUE;
}

LRESULT CHandDlg::OnHandUp(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	handList.push_back(name);

	int nItem=m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem(nItem,name,0);
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CString szTime;
	szTime.Format(IDS_HANDDLG_TIMESTAMP,time.wHour,time.wMinute,time.wSecond);
	m_listCtrl.SetItemText(nItem,1,szTime);

	//当有消息到来时，在合适的位置显示此对话框
	CRect rt;
	theApp.m_pMainWnd->GetWindowRect(&rt);
	SetWindowPos(NULL,rt.right,rt.top,350,rt.Height(),0);	
	ShowWindow(SW_SHOW);

	return 1;
}


void CHandDlg::OnNMRClickHandList(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	CPoint cp;
	GetCursorPos(&cp);
	m_listCtrl.ScreenToClient(&cp);
	
	CMenu menu;
	menu.LoadMenuW(IDR_HANDLIST_MENU);

	int nItem=m_listCtrl.HitTest(cp);
	m_listCtrl.ClientToScreen(&cp);

	if(nItem>=0){
		
	}
	else{
		CMenu* pMenu=menu.GetSubMenu(1);
		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);
	}


	*pResult = 0;
}


void CHandDlg::OnHandListClear(){
	handList.clear();
	m_listCtrl.DeleteAllItems();
	LPCTSTR source=_T("HandDlg");
	::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(),WM_MY_HANDUP,(WPARAM)&source,0);//这个地址只是为了与NULL区别，不要试图使用！
}

void CHandDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_listCtrl.m_hWnd){
		CRect rt;
		GetClientRect(&rt);
		rt.DeflateRect(15,15,15,15);
		m_listCtrl.MoveWindow(&rt);
	}
	
}
