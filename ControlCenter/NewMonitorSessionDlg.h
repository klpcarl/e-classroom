#pragma once
#include "PeerTreeDlg.h"
#include "MonitorSession.h"
// CNewMonitorSessionDlg 对话框
// 当要创建新的监控会话时会弹出这个对话框，用于获得监控对象

class CNewMonitorSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewMonitorSessionDlg)

public:
	CNewMonitorSessionDlg(CMonitorSession* pMonitorSession=NULL,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewMonitorSessionDlg();

// 对话框数据
	enum { IDD = IDD_MONITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CPeerTreeDlg m_PeerTreeDlg;

	CMonitorSession* pMonitorSession;

	void InitControls();
	void LoadSession();
	void StoreSession();
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
