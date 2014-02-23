#pragma once

#include "resource.h"

// CFullScreenOpDlg 对话框
// 在进入类全屏观看的时候利用这个对话框退出。
// 这个对话框停靠在屏幕上边缘偏左侧，当前实现是对话框中只有一个退出按钮

//要成功地使用这个类需满足两点：设置了父窗口，设置了消息


class CFullScreenOpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenOpDlg)

public:
	CFullScreenOpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFullScreenOpDlg();

// 对话框数据
	enum { IDD = IDD_FULLSCREENOP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


private:

	int nShowPosX;//这个对话框应该出现的x位置
	UINT nExitMessage;//当单击退出按钮时向父窗口发送这个自定义消息。这个消息应当由父窗口设置。

public:
	void SetExitMessage(UINT msg){nExitMessage=msg;}


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedExitfullscreenButton();
};
