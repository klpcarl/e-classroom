#pragma once
#include "afxwin.h"
#include <list>

// CGroupChooseDlg 对话框
// 当要移动成员时弹出的选择目的分组的对话框
using namespace std;

class CGroupChooseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupChooseDlg)

public:
	CGroupChooseDlg(CString *groupName, list<CString> * pDataList,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupChooseDlg();

// 对话框数据
	enum { IDD = IDD_GROUPCHOOSE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString *groupName;
	list<CString>* pDataList;
public:
	CComboBox m_GroupCombo;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
