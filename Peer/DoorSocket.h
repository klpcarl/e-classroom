#pragma once
#include "afxsock.h"

// CDoorSocket类
// 基本的套接字类

class CService;

class CDoorSocket :
	public CSocket
{
public:
	CDoorSocket(CService *);

	~CDoorSocket(void);

	CService * pService;

	void OnReceive(int);

	void OnClose(int);

};

