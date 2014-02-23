#include "stdafx.h"
#include "MonitorSession.h"
#include "ControlCenter.h"


CMonitorSession::CMonitorSession(void)
{
	bStateOn=FALSE;
//	pMonitorViewerDlg=NULL;
}


CMonitorSession::~CMonitorSession(void)
{
}


void CMonitorSession::StartMonitors(int startIndex){
	CService* pService=theApp.GetService();
	MonitorListType::iterator pMonitor=monitorList.begin();
	for(int i=0;i<startIndex && pMonitor!=monitorList.end();i++,pMonitor++);//移针
	for(;pMonitor!=monitorList.end();pMonitor++){
		CDoorSock* pDoorSock=pService->GetSockByName(*pMonitor);
		if(pDoorSock==NULL) continue;
		if(pDoorSock->bSharer){//这就不必发命令了
			pDoorSock->bMonitor=TRUE;
			SendMessage(pService->GetMonitorSessionDlg()->GetSafeHwnd(),WM_MY_MONITOROPENED,(WPARAM)&(*pMonitor),(LPARAM)&pDoorSock->ticket);
		}
		else{
			pService->SockSend(pDoorSock,_T("open monitor"));			
		}		
	}
}

void CMonitorSession::StartMonitors(MonitorListType::iterator pMonitor){
	CService* pService=theApp.GetService();
	while(pMonitor!=monitorList.end()){
		pService->SockSend(*pMonitor,_T("open monitor"));
		pMonitor++;
	}
}
