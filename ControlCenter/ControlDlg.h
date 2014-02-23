#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ShareSession.h"

// CControlDlg 对话框
// 右键点击一个广播会话的条目时会有一个授权控制选项，点击授权控制弹出这个对话框，用于选择
// 观看者，赋予控制权限

class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(CShareSession* pSession=NULL, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlDlg();

// 对话框数据
	enum { IDD = IDD_CONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_SharerNameEdit;
	CListCtrl m_ViewerListCtrl;

	CShareSession* pShareSession;


private:
	void InitControls();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
