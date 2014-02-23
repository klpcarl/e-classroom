// ViewerCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "ViewerCtrl.h"
#include "afxdialogex.h"
#include "resource.h"


// CViewerCtrl 对话框

IMPLEMENT_DYNAMIC(CViewerCtrl, CDialogEx)

CViewerCtrl::CViewerCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerCtrl::IDD, pParent)
{

}

CViewerCtrl::~CViewerCtrl()
{
}

void CViewerCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDPVIEWER1, m_Viewer);
	DDX_Control(pDX, IDC_NAME_STATIC, m_Name);
}


BEGIN_MESSAGE_MAP(CViewerCtrl, CDialogEx)
	ON_STN_CLICKED(IDC_NAME_STATIC, &CViewerCtrl::OnStnClickedNameStatic)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CViewerCtrl 消息处理程序


void CViewerCtrl::OnStnClickedNameStatic()
{
//	MessageBox(_T("you win viewer ctrl"));
}


void CViewerCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(this->m_hWnd && m_Viewer.m_hWnd && m_Name.m_hWnd){//我都服气了
		CRect rt;
		GetClientRect(&rt);
		m_Viewer.MoveWindow(rt.left,rt.top,rt.Width(),rt.Height()-15);
		m_Name.MoveWindow(rt.left,rt.Height()-15,rt.Width(),15);
	}
}


void CViewerCtrl::RefreshViewer(){//断开与再次连接之间的时间过短，很可能失败！这是一种错误的刷新方法
	m_Viewer.Disconnect();
	CString name;
	m_Name.GetWindowText(name);
	CString ticket=(theApp.GetService())->GetSockByName(name)->ticket;
	BOOL bMonitor=(theApp.GetService())->GetSockByName(name)->bMonitor;
	if(bMonitor)
	m_Viewer.Connect(ticket,name,_T(""));
}

void CViewerCtrl::Connect(){
	CString name;
	m_Name.GetWindowText(name);
	CString ticket=(theApp.GetService())->GetSockByName(name)->ticket;
	BOOL bMonitor=(theApp.GetService())->GetSockByName(name)->bMonitor;
	if(bMonitor)
	m_Viewer.Connect(ticket,name,_T(""));
}


void CViewerCtrl::Repaint(){

}