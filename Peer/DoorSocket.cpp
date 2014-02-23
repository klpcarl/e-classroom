#include "stdafx.h"
#include "DoorSocket.h"
#include "Service.h"


CDoorSocket::CDoorSocket(CService * pSer):pService(pSer)
{
	
}


CDoorSocket::~CDoorSocket(void)
{
}

void CDoorSocket::OnReceive(int nErrorCode){
	//1.Ð£ÑénErrorCode

	
#define BUF_SIZE 4096
	TCHAR buf[BUF_SIZE];
	int count=this->Receive(buf,BUF_SIZE);
	buf[count/(sizeof(TCHAR))]=_T('\0');

	CString str;
	str.Format(_T("%s"),buf);

	if(pService!=NULL) pService->OnReceive(str);

}

void CDoorSocket::OnClose(int nErrorCode){
	if(pService!=NULL) pService->OnSockClose();
}
