#pragma once
#include "afxsock.h"

// 这是基本的响应套接字类

class CService;
class CDoorSock :
	public CSocket
{
public:
	CDoorSock(CService*);
	~CDoorSock(void);


	CService* pService;
//	CString data;

	BOOL bSharer;//是否开启了分享服务
	BOOL bViewer;//是否开启了观看服务
	BOOL bMonitor;//是否在监控会话中
	CString ticket;//作为分享者所分发的票据

	void OnReceive(int nErrorCode);

	void OnClose(int nErrorCode);



};

