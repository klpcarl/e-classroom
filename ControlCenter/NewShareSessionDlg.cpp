// NewShareSessionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "NewShareSessionDlg.h"
#include "ShareSessionDlg.h"
#include "afxdialogex.h"
#include "Service.h"


// CNewShareSessionDlg 对话框

IMPLEMENT_DYNAMIC(CNewShareSessionDlg, CDialogEx)

CNewShareSessionDlg::CNewShareSessionDlg(CShareSession* pShareSession, CWnd* pParent /*=NULL*/)//必须指定一个包含Service的父类
	: CDialogEx(CNewShareSessionDlg::IDD, pParent),m_ShareTreeDlg(this,TRUE),m_ViewTreeDlg(this,TRUE)
{
	this->pShareSession=pShareSession;
}

CNewShareSessionDlg::~CNewShareSessionDlg()
{
}

void CNewShareSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_CHECK1, m_LockCheckbox);
}


BEGIN_MESSAGE_MAP(CNewShareSessionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewShareSessionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewShareSessionDlg 消息处理程序


BOOL CNewShareSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();

	return TRUE;
}

void CNewShareSessionDlg::InitControls(){
	CRect rt;
	GetClientRect(&rt);
	int left,top,width,height;

	left=rt.Width()/10;
	top=rt.Height()/10;
	width=rt.Width()*3.5/10;
	height=rt.Height()*3/4;

	m_ShareTreeDlg.Create(CPeerTreeDlg::IDD,this);
	m_ViewTreeDlg.Create(CPeerTreeDlg::IDD,this);
	m_ViewTreeDlg.SetTreeCheckable();
	
	m_ShareTreeDlg.SetService(theApp.GetService());//
	m_ShareTreeDlg.FilterTree(theApp.GetMainDlg()->GetPeerTreeDlg());
	m_ViewTreeDlg.SetService(theApp.GetService());//
	m_ViewTreeDlg.FilterTree(theApp.GetMainDlg()->GetPeerTreeDlg());

	m_ShareTreeDlg.MoveWindow(left,top,width,height);
	m_ShareTreeDlg.ShowWindow(SW_SHOW);

	left=rt.Width()-left-width;
	m_ViewTreeDlg.MoveWindow(left,top,width,height);
	m_ViewTreeDlg.ShowWindow(SW_SHOW);
}


void CNewShareSessionDlg::OnBnClickedOk()
{
	//重建对象
	//
	if(pShareSession){
		pShareSession->sharer=m_ShareTreeDlg.GetSelectedPeerName();
		pShareSession->viewerList.clear();
		m_ViewTreeDlg.GetCheckedPeerNames(&(pShareSession->viewerList));
//		int count=pShareSession->viewerList.size();
//		pShareSession->bLockMode=(m_LockCheckbox.GetCheck()==BST_CHECKED?TRUE:FALSE);
	}
	CDialogEx::OnOK();
}
