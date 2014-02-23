#pragma once
#include "stdafx.h"
#include "ListenSock.h"
#include "DoorSock.h"
#include "ShareSession.h"
#include "MonitorSession.h"



typedef std::map<CString,CDoorSock*> NameSockMapType;
typedef std::list<CDoorSock*> DoorSockListType;

//两种不同的会话类型
typedef std::list<CShareSession*> ShareSessionListType;//可能没用到


// CService类
// 服务类，提供基本的套接字数据发送与接收服务，并分发消息，通知界面


class CService :
	public CObject
{
public:
	CService(void);
	~CService(void);


private:
	CWnd* pEmployer;

	CWnd* pTreeDlg;
	CWnd* pShareSessionDlg;
	CWnd* pMonitorSessionDlg;
	CWnd* pHandDlg;
	

	// socket 服务
	CListenSock* pLisSock;
	DoorSockListType doorSockList;
	NameSockMapType nameSockMap;



	// 文件与存储服务



	/*
	// 会话管理	
	ShareSessionListType sSessionList;
	MonitorSessionListType mSessionList;
	*/



public:
	void SetEmployer(CWnd* );
	//这三个函数以及对应变量的设置是为了为对应的界面注册事件通知
	void SetTreeDlg(CWnd*);
	void SetShareSessionDlg(CWnd*);
	void SetMonitorSessionDlg(CWnd*);
	CWnd* GetMonitorSessionDlg();
	void SetHandDlg(CWnd*);

	NameSockMapType* GetNameSockMap(){ return &nameSockMap; }
	CString GetNameBySock(CDoorSock*);
	CDoorSock* GetSockByName(CString);

	// socket服务
	BOOL StartSockService(int port=5000);
	void StopSockService();

	BOOL SockSend(CDoorSock*,CString);
	BOOL SockSend(CString name, CString cmd);
	void SockSendAll(CString cmd);
	void OnSockAccept();
	void OnSockReceive(CDoorSock*,CString);
	void OnSockClose(CDoorSock*);





};

