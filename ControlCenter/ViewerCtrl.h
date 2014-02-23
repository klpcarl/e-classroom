#pragma once
#include "rdpviewer1.h"
#include "afxwin.h"
#include "resource.h"


// CViewerCtrl 对话框
// 这是用于CMonitorViewerDlg的子窗体，包含一个RDP viewer 控件和一个静态控件
// 用于作为一个RDP客户端显示监控对象的桌面

class CViewerCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerCtrl)

public:
	CViewerCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewerCtrl();

// 对话框数据
	enum { IDD = IDD_VIEWERCTRL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CRdpviewer1 m_Viewer;
	CStatic m_Name;


public:
	void SetMonitorName(CString name){m_Name.SetWindowText(name);}
	CString GetMonitorName(){ CString name; m_Name.GetWindowText(name);return name;}
	void Connect(LPCTSTR ticket, LPCTSTR name, LPCTSTR pass){ m_Viewer.Connect(ticket,name,pass);}
	void Connect();
	void Disconnect(){m_Viewer.Disconnect();}
	void Repaint();
	void RefreshViewer();
	void put_SmartSizing(BOOL sm){ m_Viewer.put_SmartSizing(sm);}

	afx_msg void OnStnClickedNameStatic();
	afx_msg void OnSize(UINT nType, int cx, int cy);

};
