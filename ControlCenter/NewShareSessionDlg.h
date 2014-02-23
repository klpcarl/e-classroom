#pragma once

#include "PeerTreeDlg.h"
#include "ShareSession.h"
#include "afxwin.h"


// CNewShareSessionDlg 对话框
// 当要创建新的广播会话时弹出这个对话框，用于获得分享者和观看者

class CNewShareSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewShareSessionDlg)

public:
	CNewShareSessionDlg(CShareSession* pShareSession, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewShareSessionDlg();

// 对话框数据
	enum { IDD = IDD_SHARESESSION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	CPeerTreeDlg m_ShareTreeDlg;
	CPeerTreeDlg m_ViewTreeDlg;

	BOOL bViewChanged;//指示编辑会话时有没有修改观看者

	CShareSession *pShareSession;

	void InitControls();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	CButton m_LockCheckbox;
};
