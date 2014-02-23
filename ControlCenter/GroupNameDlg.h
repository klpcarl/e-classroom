#pragma once


// CGroupNameDlg 对话框
// 当要新增分组或重命名分组时弹出的用于获取新的分组名的对话框

class CGroupNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupNameDlg)

public:
	CGroupNameDlg(CString* groupName,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupNameDlg();

// 对话框数据
	enum { IDD = IDD_GROUPNAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString * groupName;
	
public:
	afx_msg void OnBnClickedOk();
};
