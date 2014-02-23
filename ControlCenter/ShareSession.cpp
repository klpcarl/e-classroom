#include "stdafx.h"
#include "ShareSession.h"
#include "ControlCenter.h"


CShareSession::CShareSession(void)
{
	bStateOn=FALSE;
	bPause=FALSE;
	bLockMode=FALSE;
	oldCtrlViewer=_T("");
	newCtrlViewer=_T("");
	pService=theApp.GetService();
}


CShareSession::~CShareSession(void)
{
	if(bStateOn){//如果还在会话中
		StopViewers();
		StopSharer();
	}
}

void CShareSession::StartSharer(){	
	pService->SockSend(sharer,_T("start sharer"));
	//应当在收到 sharer started 将 bStateOn 设为 TRUE
}

void CShareSession::StopSharer(){
	pService->SockSend(sharer,_T("stop sharer"));
	//注意，应当在收到 sharer stopped 时 将bStateOn设为 FALSE
}

void CShareSession::StartViewers(){
	CString cmd=_T("start viewer")+ticket;
	ViewerListType::iterator pViewer=viewerList.begin();
	while(pViewer!=viewerList.end()){
		pService->SockSend(*pViewer,cmd);
		pViewer++;
	}
}

void CShareSession::StopViewers(){
	CString cmd=_T("stop viewer");
	ViewerListType::iterator pViewer=viewerList.begin();
	while(pViewer!=viewerList.end()){
		pService->SockSend(*pViewer,cmd);
		pViewer++;
	}
}


void CShareSession::GrantControl(CString name, BOOL grant){
	if(name==_T("")) return;
	CString cmd;
	if(grant){
		cmd=_T("request control interactive");
		//解锁：
		if(bLockMode) pService->SockSend(name,_T("unlock"));

	}
	else{
		cmd=_T("request control view");
		//恢复锁定
		if(bLockMode) pService->SockSend(name,_T("lock"));
	}
	pService->SockSend(name,cmd);

}

void CShareSession::BlindViewers(BOOL enable){
	CString cmd;
	if(enable){
		bPause=TRUE;//收到回复消息时设置，not now
		cmd=_T("request control none");
	}
	else {
		bPause=FALSE;
		cmd=_T("request control view");
	}
	ViewerListType::iterator pViewer;
	for(pViewer=viewerList.begin();pViewer!=viewerList.end();pViewer++){
		pService->SockSend(*pViewer,cmd);
	}

	oldCtrlViewer=_T("");
	newCtrlViewer=_T("");
}


void CShareSession::LockViewers(BOOL lock){
	if(bStateOn==FALSE) return;
	CString cmd;
	if(lock){
		bLockMode=TRUE;//其实这些应当在收到socket的回应时设置，而不是现在
		cmd=_T("lock");
	}
	else {
		bLockMode=FALSE;
		cmd=_T("unlock");
	}

	ViewerListType::iterator pViewer;
	for(pViewer=viewerList.begin();pViewer!=viewerList.end();pViewer++){
		pService->SockSend(*pViewer,cmd);
	}
}





void CShareSession::ChangeControl(){

	GrantControl(oldCtrlViewer,FALSE);
	GrantControl(newCtrlViewer,TRUE);
}


/* 
很明显这个函数这么写是有问题的，首先没有关联bStateOn，
其次关闭会话这个动作本身就是分离的，就像你无法写出StartSession()一样
*/
void CShareSession::StopSession(){
	if(bStateOn){
		StopViewers();
		StopSharer();//这样写的问题是可能sharer先于 viewer关闭
	}
}


