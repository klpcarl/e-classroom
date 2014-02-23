#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"


// CSettingDlg 对话框
// 设置对话框，用于设置程序黑白名单，禁止所有exe，静默状态等。当前实现中没用到

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDlg();

// 对话框数据
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:




	BOOL AllRegKey(HKEY hKey, LPCTSTR szDesKeyItem);
	CListCtrl m_wlist;
	CListCtrl m_blist;
	virtual BOOL OnInitDialog();
};
