
// PeerDlg.h : 头文件
//

#pragma once
#include "Service.h"
#include "TrayIcon.h"
#include "ViewerDlg.h"


// CPeerDlg 对话框
// 客户端程序的主对话框类，但这个类的界面始终为隐藏状态
// 系统托盘图标的菜单事件也有这个类处理
class CPeerDlg : public CDialogEx
{
// 构造
public:
	CPeerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CPeerDlg();

// 对话框数据
	enum { IDD = IDD_PEER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()



public:

	CService* pService;
	CTrayIcon m_trayIcon;
	CViewerDlg* pViewerDlg;
	
	

	//normal member functions

	
	

	//my message handle
	afx_msg LRESULT OnTrayNotification(WPARAM,LPARAM);
	afx_msg void OnSockConnect();
	afx_msg void OnOpenViewer();
	afx_msg void OnHand();
	afx_msg void OnExit();
	afx_msg LRESULT OnSockClose(WPARAM,LPARAM);
	afx_msg LRESULT OnStartViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnStopViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnLockViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnUnlockViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnConfig(WPARAM,LPARAM);


private:
	BOOL SockConnectWithConfig();//这个函数是读取配置文件中的地址与端口信息，再调用CService的StartSockService连接
	void ShowWarningOnConfig();
};
