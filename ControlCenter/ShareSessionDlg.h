#pragma once
#include "afxcmn.h"
#include "ShareSession.h"
#include "Service.h"

using namespace std;

typedef list<CShareSession*> ShareSessionListType;

// CShareSessionDlg 对话框
// 主界面的“广播桌面”标签页对应这个对话框，只包含一个列表视图控件
// 提供了管理广播会话的界面接口

class CShareSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShareSessionDlg)

public:
	CShareSessionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShareSessionDlg();

// 对话框数据
	enum { IDD = IDD_SESSIONLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:	
	CImageList m_images;
	CListCtrl m_list;
	ShareSessionListType sSessionList;

public:
	CService * pService;


public:
	void SetService(CService*);
	CService* GetService();

	CShareSession* GetShareSessionBySharerName(CString);

	afx_msg void OnNMRClickSessionList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSessionEdit();
	afx_msg void OnSessionStop();
	afx_msg void OnSessionStart();
	afx_msg void OnSessionDel();
	afx_msg void OnSessionAdd();
	afx_msg void OnSessionLock();
	afx_msg void OnSessionControl();

	afx_msg LRESULT OnSharerStarted(WPARAM,LPARAM);
	afx_msg LRESULT OnSharerNotStarted(WPARAM,LPARAM);
	afx_msg LRESULT OnSharerStopped(WPARAM,LPARAM);
	afx_msg LRESULT OnSockClose(WPARAM,LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
