#pragma once
#include "Service.h"


// CMonitorSessionDlg 对话框
// 主界面中“监控桌面”标签页对应的对话框，只包含一个列表视图控件，
// 提供了对于监控的会话的操作接口

typedef std::list<CMonitorSession*> MonitorSessionListType;

class CMonitorSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorSessionDlg)

public:
	CMonitorSessionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMonitorSessionDlg();

// 对话框数据
	enum { IDD = IDD_SESSIONLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	CListCtrl m_list;
	CImageList m_images;
	CService* pService;

	CMonitorViewerDlg* pMonitorViewerDlg;

	CMonitorSession mSession;

public:
	void SetService(CService*);

	virtual BOOL OnInitDialog();

	int GetItemByText(CString name);

	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnNMRClickSessionList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemOpenViewerDlg();
	afx_msg void OnItemStop();
	afx_msg void OnItemDelete();
	afx_msg void OnWhiteCreate();
	afx_msg void OnWhiteOpenViewerDlg();

	afx_msg LRESULT OnMonitorOpened(WPARAM,LPARAM);
	afx_msg LRESULT OnMonitorNotOpened(WPARAM pName, LPARAM);
	afx_msg LRESULT OnMonitorClosed(WPARAM,LPARAM);
	afx_msg LRESULT OnSockClose(WPARAM,LPARAM);
};
