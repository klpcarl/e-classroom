#pragma once
#include "afxsock.h"

// 这是监听套接字类

class CService;

class CListenSock :
	public CSocket
{
public:
	CListenSock(CService*);
	~CListenSock(void);

	CService * pService;

	void OnAccept(int);
};

