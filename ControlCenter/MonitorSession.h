#pragma once
#include "afx.h"
#include "MonitorViewerDlg.h"

// CMonitorSession类
// 用于封装多个监控会话的类，用于从CNewMonitorSessionDlg对话框获得数据，并启动监控对象的RDP服务

typedef std::list<CString> MonitorListType;


class CMonitorSession :
	public CObject
{
public:
	CMonitorSession(void);
	~CMonitorSession(void);

private:
	BOOL bStateOn;//not used
	MonitorListType monitorList;
//	CMonitorViewerDlg* pMonitorViewerDlg;

public:
	void StartMonitors(int startIndex=0);
	void StartMonitors(MonitorListType::iterator startPointer);

	MonitorListType & GetMonitorList(){ return monitorList;}

};

