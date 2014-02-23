#pragma once
#include "stdafx.h"
#include "DoorSocket.h"
#include "MyRdpSessionEvents.h"

// CService类
// 提供套接字服务、RDP服务和安装系统钩子的服务

class CService :
	public CObject
{
public:
	CService(void);
	~CService(void);

	void SetEmployer(CWnd*);
	void SetViewerDlg(CWnd*);

	void SetHook(BOOL use=TRUE);

	

	//rdp
	BOOL GetTicket(CString*);
	void StartRDPService();
	void StopRDPService();

	//socket
	BOOL StartSockService(LPTSTR addr=_T("localhost"),int port=5000);
	void StopSockService();
	void OnReceive(CString cmd);
	void OnSockClose();
	BOOL Send(CString);

	BOOL GetSockState(){return pSock!=NULL;}//是否连接上



private:
	BOOL bLockMode;//是否被锁定

	CWnd* pEmployer;
	CWnd* pViewerDlg;

	IRDPSRAPISharingSession* pSharingSession;
	CMyRDPSessionEvents myEvents;
	CDoorSocket* pSock;//根据这个值是否为NULL判读是否连接上了socket

	

};

