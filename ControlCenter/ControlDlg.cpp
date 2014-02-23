// ControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "ControlDlg.h"
#include "afxdialogex.h"


// CControlDlg 对话框

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(CShareSession* pSession, CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlDlg::IDD, pParent)
{
	this->pShareSession=pSession;
}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHARER_EDIT, m_SharerNameEdit);
	DDX_Control(pDX, IDC_CONTROL_LIST, m_ViewerListCtrl);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CControlDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CControlDlg 消息处理程序


BOOL CControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();

	return TRUE;
}

void CControlDlg::InitControls(){
	if(pShareSession==NULL) return;
	m_SharerNameEdit.SetWindowText(pShareSession->sharer);

	ViewerListType& viewerList=pShareSession->viewerList;
	ViewerListType::iterator pViewer;
	for(pViewer=viewerList.begin();pViewer!=viewerList.end();pViewer++){
		m_ViewerListCtrl.InsertItem(m_ViewerListCtrl.GetItemCount(),*pViewer);
	}

}


void CControlDlg::OnBnClickedOk()
{
	pShareSession->oldCtrlViewer=pShareSession->newCtrlViewer;

	POSITION pos=m_ViewerListCtrl.GetFirstSelectedItemPosition();

	if(pos==NULL){
		pShareSession->newCtrlViewer=_T("");
	}
	else {
		int nItem=m_ViewerListCtrl.GetNextSelectedItem(pos);
		CString name=m_ViewerListCtrl.GetItemText(nItem,0);
		pShareSession->newCtrlViewer=name;
	}

	CDialogEx::OnOK();
}
