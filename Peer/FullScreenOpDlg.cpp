// FullScreenOpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FullScreenOpDlg.h"
#include "afxdialogex.h"


#define IDT_TIMER 1//定时器id

// CFullScreenOpDlg 对话框

IMPLEMENT_DYNAMIC(CFullScreenOpDlg, CDialogEx)

CFullScreenOpDlg::CFullScreenOpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFullScreenOpDlg::IDD, pParent)
{
	
	nShowPosX=::GetSystemMetrics(SM_CXSCREEN)/4;
	nExitMessage=0;
}

CFullScreenOpDlg::~CFullScreenOpDlg()
{
}

void CFullScreenOpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFullScreenOpDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EXITFULLSCREEN_BUTTON, &CFullScreenOpDlg::OnBnClickedExitfullscreenButton)
END_MESSAGE_MAP()


// CFullScreenOpDlg 消息处理程序


BOOL CFullScreenOpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowPos(&wndTopMost,nShowPosX,0,0,0,SWP_NOSIZE);
	

	SetTimer(IDT_TIMER,500,NULL);

	return TRUE;
}


void CFullScreenOpDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect rt;
	CRect rect;
	CRect art(0,-3,GetSystemMetrics(SM_CXSCREEN)/2,4);//鼠标的响应区域
	GetWindowRect(&rect);
	rt.CopyRect(&rect);	

	/*
	art.CopyRect(&rect);
	if(rect.top<-3){//若当前为隐藏状态，调整响应区域
		art.bottom+=4;
	}
	*/


	CPoint cp;
	GetCursorPos(&cp);

	if(rect.top<0 && PtInRect(&art,cp)){
		rect.top=0;
		MoveWindow(rect.left,rect.top,rt.Width(),rt.Height());
	}
	else if(rect.top>-3 && rect.top<3 && !PtInRect(rect,cp)){
		rect.top=-rect.Height();
		MoveWindow(rect.left,rect.top,rt.Width(),rt.Height());
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CFullScreenOpDlg::OnBnClickedExitfullscreenButton()
{
	if(GetParent()!=NULL && nExitMessage!=0){
		::PostMessage(GetParent()->GetSafeHwnd(),nExitMessage,0,0);
	}
}
