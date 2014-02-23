
// ControlCenterDlg.h : 头文件
//这是程序的主界面类CControlCenterDlg

#pragma once

#include "Service.h"
#include "afxcmn.h"
#include "PeerTreeDlg.h"
#include "ShareSessionDlg.h"
#include "MonitorSessionDlg.h"
#include "HandDlg.h"
#include "SettingDlg.h"
#include "TrayIcon.h"
#include "afxwin.h"

// CControlCenterDlg 对话框
class CControlCenterDlg : public CDialogEx
{
// 构造
public:
	CControlCenterDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CControlCenterDlg();

// 对话框数据
	enum { IDD = IDD_CONTROLCENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CService * pService;

	void StartService();
	void StopService();

	void InitControls();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CTrayIcon m_TrayIcon;

	CTabCtrl m_tab;
	CPeerTreeDlg m_PeerTreeDlg;
	CShareSessionDlg m_ShareSessionDlg;
	CMonitorSessionDlg m_MonitorSessionDlg;
	CHandDlg m_HandDlg;
	CSettingDlg m_SettingDlg;

	CStatic m_handNumText;
	CStatic m_onlineNumStatic;

public:
	CPeerTreeDlg& GetPeerTreeDlg(){return m_PeerTreeDlg;}


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnStnClickedHandnumStatic();
	afx_msg LRESULT OnHandUp(WPARAM,LPARAM);
	afx_msg LRESULT OnTrayNotification(WPARAM,LPARAM);
	afx_msg LRESULT OnLogOnPeer(WPARAM,LPARAM);//客户端登陆时发来的事件
	afx_msg LRESULT OnLogOffPeer(WPARAM,LPARAM);//客户端下线时的事件
	afx_msg void OnTrayOpenMainDlg();
	afx_msg void OnTrayExit();

	
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
