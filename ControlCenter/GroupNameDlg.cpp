// GroupNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "GroupNameDlg.h"
#include "afxdialogex.h"


// CGroupNameDlg 对话框

IMPLEMENT_DYNAMIC(CGroupNameDlg, CDialogEx)

CGroupNameDlg::CGroupNameDlg(CString* groupName,CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupNameDlg::IDD, pParent)
{
	this->groupName=groupName;
}

CGroupNameDlg::~CGroupNameDlg()
{
}

void CGroupNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGroupNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGroupNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGroupNameDlg 消息处理程序


void CGroupNameDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1,*groupName);
	CDialogEx::OnOK();
}
