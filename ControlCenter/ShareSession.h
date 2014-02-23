#pragma once
#include "afx.h"

// CShareSession类
// 这个类用于在创建新的广播会话时传递给CNewShareSessionDlg对话框并获得数据
// 并且它也表示了广播会话这个概念的实体，并提供了管理广播绘画的一些方法


typedef std::list<CString> ViewerListType;

class CService;

class CShareSession :
	public CObject
{
public:	
	CShareSession(void);
	~CShareSession(void);


private:
	CService* pService;


public:
	BOOL bStateOn;//标示会话是否在进行中，取决于sharer是否开启RDP服务
	BOOL bPause;//标识会话是否暂停，当执行BlindViewers（TRUE）后所有的RDPviewer处在低于none的控制级别，这时视为pause
	CString sharer;
	ViewerListType viewerList;
	CString newCtrlViewer;//当前拥有交互控制级别的观看者，默认为_T("")
	CString oldCtrlViewer;
	CString ticket;

	BOOL bLockMode;


	//如果让这个类自己管理自己，怎么样，持有一个Service
	
	void StartSharer();
	void StopSharer();
	void StartViewers();
	void StopViewers();

	void GrantControl(CString name, BOOL grantOrRevoke=TRUE);
	void ChangeControl();
	void BlindViewers(BOOL enable=TRUE);
	void LockViewers(BOOL lock=TRUE);
	

	void StopSession();

	
	
};

