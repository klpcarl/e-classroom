#include "stdafx.h"
#include "DoorSock.h"
#include "Service.h"

CDoorSock::CDoorSock(CService* holder)
{
	this->pService=holder;
	bSharer=FALSE;
	bViewer=FALSE;
	bMonitor=FALSE;
	ticket=_T("");
}


CDoorSock::~CDoorSock(void)
{
}


void CDoorSock::OnReceive(int nErrorCode){
	//ErrorCode 校验


	#define BUF_SIZE 4096
	TCHAR buf[BUF_SIZE];
	int count=this->Receive(buf,BUF_SIZE);
	if(count<=0) return;
	buf[count/(sizeof(TCHAR))]=_T('\0');

	CString str;
	str.Format(_T("%s"),buf);

	pService->OnSockReceive(this,str);

}

void CDoorSock::OnClose(int nErrorCode){
	pService->OnSockClose(this);//这样转型破坏了替换原则
}