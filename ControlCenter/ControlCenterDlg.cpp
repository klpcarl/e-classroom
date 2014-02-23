
// ControlCenterDlg.cpp : 实现文件
//包含关于对话框类CAboutDlg的声明与定义和主对话框类CControlCenterDlg类的实现

#include "stdafx.h"
#include "ControlCenter.h"
#include "ControlCenterDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Message.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+20


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CControlCenterDlg 对话框




CControlCenterDlg::CControlCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlCenterDlg::IDD, pParent),pService(NULL),m_TrayIcon(IDR_TRAY_MENU)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CControlCenterDlg::~CControlCenterDlg(){
	StopService();
}



void CControlCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_HANDNUM_STATIC, m_handNumText);
	DDX_Control(pDX, IDC_ONLINENUM_STATIC, m_onlineNumStatic);
}

BEGIN_MESSAGE_MAP(CControlCenterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CControlCenterDlg::OnTcnSelchangeTab1)
	ON_STN_CLICKED(IDC_HANDNUM_STATIC, &CControlCenterDlg::OnStnClickedHandnumStatic)
	ON_MESSAGE(WM_MY_HANDUP,OnHandUp)
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION,OnTrayNotification)
	ON_MESSAGE(WM_MY_LOGNAME,OnLogOnPeer)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnLogOffPeer)
	ON_COMMAND(IDM_TRAY_MAIN,OnTrayOpenMainDlg)
	ON_COMMAND(IDM_TRAY_EXIT,OnTrayExit)

//	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CControlCenterDlg 消息处理程序

BOOL CControlCenterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	ASSERT((IDM_SETTINGBOX & 0xFFF0) == IDM_SETTINGBOX);
	ASSERT(IDM_SETTINGBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			//pSysMenu->AppendMenu(MF_STRING,IDM_SETTINGBOX,_T("设置"));//暂时不要这个功能
			CString strTemp;//用于载入字符串
			if(strTemp.LoadString(IDS_CONTROLCENTERDLG_CONFIGCLIENTEXIT))
			pSysMenu->AppendMenu(MF_STRING, IDM_CONFIGCLIENT_ENABLEEXIT, strTemp /*_T("允许客户端退出")*/);

			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	StartService();
	//注意：一定要在各个子对话框初始化前启动服务，否则会出错

	InitControls();

	/*
	CPeerTreeDlg dlg;
	pService->SetTreeDlg(&dlg);
	dlg.DoModal();
	*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


//必须在启动服务之后，因为一些子对话框可能需要服务
void CControlCenterDlg::InitControls(){

	CString strTemp;//用于载入字符串资源

	if(strTemp.LoadString(IDS_CONTROLCENTERDLG_MANAGEGROUP))
		m_tab.InsertItem(0,strTemp /*_T("分组管理")*/);
	if(strTemp.LoadString(IDS_CONTROLCENTERDLG_SHAREDESKTOP))
		m_tab.InsertItem(1,strTemp /*_T("广播桌面")*/);
	if(strTemp.LoadString(IDS_CONTROLCENTERDLG_MONITORDESKTOP))
		m_tab.InsertItem(2,strTemp /*_T("监控桌面")*/);

	m_ShareSessionDlg.SetService(pService);
	m_MonitorSessionDlg.SetService(pService);//这些是至关重要的。

	m_PeerTreeDlg.Create(CPeerTreeDlg::IDD,&m_tab);
	m_ShareSessionDlg.Create(CShareSessionDlg::IDD,&m_tab);
	m_MonitorSessionDlg.Create(CMonitorSessionDlg::IDD,&m_tab);
	m_HandDlg.Create(CHandDlg::IDD,this);

	pService->SetTreeDlg(&m_PeerTreeDlg);//这种设计是丑陋的。
	pService->SetHandDlg(&m_HandDlg);//向Service注册事件

	CRect rt;
	m_tab.GetClientRect(&rt);
	rt.DeflateRect(2,23,2,2);

	m_PeerTreeDlg.MoveWindow(&rt);
	m_ShareSessionDlg.MoveWindow(&rt);
	m_MonitorSessionDlg.MoveWindow(&rt);

	m_PeerTreeDlg.ShowWindow(SW_SHOW);
	m_tab.SetCurSel(0);

	m_TrayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);//优雅的事件注册方式
	m_TrayIcon.SetIcon(IDR_MAINFRAME);
	

}

void CControlCenterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if((nID & 0xFFF0)==IDM_SETTINGBOX){
		m_SettingDlg.DoModal();
	}
	
	else if(nID==SC_CLOSE){
		ShowWindow(SW_HIDE);
	}
	else if(nID==IDM_CONFIGCLIENT_ENABLEEXIT){
		CMenu* pMenu=this->GetSystemMenu(FALSE);
		if(pMenu->GetMenuState(IDM_CONFIGCLIENT_ENABLEEXIT,MF_CHECKED)){
			pMenu->CheckMenuItem(IDM_CONFIGCLIENT_ENABLEEXIT,MF_UNCHECKED);
			pService->SockSendAll(_T("config disable exit"));
		}
		else{
			pMenu->CheckMenuItem(IDM_CONFIGCLIENT_ENABLEEXIT,MF_CHECKED);
			pService->SockSendAll(_T("config enable exit"));
		}		
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CControlCenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CControlCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CControlCenterDlg::StartService(){
	
	pService=new CService();

	CString strTemp,strTip,strTitle;
	BOOL bTip,bTitle;
	strTip=((bTip=strTemp.LoadString(IDS_CONTROLCENTERDLG_SOCKFAILTIP))?strTemp:_T("启动服务失败，请重新启动程序！"));//倘若载入失败会使用汉语字符串
	strTitle=(bTitle=strTemp.LoadString(IDS_CONTROLCENTERDLG_SOCKFAILTITLE))?strTemp:_T("错误");
	
	if(pService==NULL){
		
		::MessageBox(NULL,strTip/*_T("启动服务失败，请重新启动程序！")*/,strTitle/*_T("错误")*/,MB_OK|MB_ICONERROR);
		return;
	}
	theApp.SetService(pService);//注册服务
	pService->SetEmployer(this);

	TCHAR * ConfigFilePath=_T(".\\Config.ini");
	UINT port;
	port=::GetPrivateProfileInt(_T("Connection"),_T("port"),5000,ConfigFilePath);
	
	if(!pService->StartSockService(port)) MessageBox(strTip/*_T("启动套接字服务失败,请重新启动程序！")*/,strTitle);
}


void CControlCenterDlg::StopService(){
	if(pService!=NULL){
		delete pService;
		pService=NULL;
	}
}



void CControlCenterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static int i=0;
	if(i++>0){

		CRect rt;
		GetClientRect(&rt);


		rt.DeflateRect(15,10,15,15);
		
		m_tab.MoveWindow(rt.left,rt.top,rt.Width(),rt.Height()-15);

		m_handNumText.MoveWindow(rt.left,rt.top+rt.Height()-13,rt.Width()/2,15);
		
		//m_onlineNumStatic.MoveWindow(rt.right-rt.Width()*4/9,rt.top+rt.Height()-13,rt.Width()*4/9,15);//这有问题

		
		m_tab.GetClientRect(&rt);
		rt.DeflateRect(2,23,2,2);
		m_PeerTreeDlg.MoveWindow(&rt);
		m_ShareSessionDlg.MoveWindow(&rt);
		m_MonitorSessionDlg.MoveWindow(&rt);

	}

	
	if(::IsWindow(m_onlineNumStatic.GetSafeHwnd())){
		CRect rt;
		m_onlineNumStatic.GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_onlineNumStatic.MoveWindow(cx-rt.Width()-15,cy-15-13,rt.Width(),rt.Height());//我是经过了精确计算
	}
	

}


void CControlCenterDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel=m_tab.GetCurSel();

	switch(sel){

	case 0:
		m_PeerTreeDlg.ShowWindow(SW_SHOW);
		m_ShareSessionDlg.ShowWindow(SW_HIDE);
		m_MonitorSessionDlg.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_PeerTreeDlg.ShowWindow(SW_HIDE);
		m_ShareSessionDlg.ShowWindow(SW_SHOW);
		m_MonitorSessionDlg.ShowWindow(SW_HIDE);
		break;

	case 2:
		m_PeerTreeDlg.ShowWindow(SW_HIDE);
		m_ShareSessionDlg.ShowWindow(SW_HIDE);
		m_MonitorSessionDlg.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CControlCenterDlg::OnStnClickedHandnumStatic()
{
//	MessageBox(_T("当前没有人举手！"));
	m_HandDlg.ShowWindow(SW_NORMAL);
}

LRESULT CControlCenterDlg::OnHandUp(WPARAM wParam, LPARAM lParam){

	CString szHandStatus;
	
	if(wParam==NULL){
		//szHandStatus.Format(_T("当前举手数：%d"),m_HandDlg.GetItemCount());
		szHandStatus.Format(IDS_CONTROLCENTERDLG_HANDNUM,m_HandDlg.GetItemCount());
	}
	else {
		szHandStatus.Format(IDS_CONTROLCENTERDLG_HANDNUM,0);
	}
	m_handNumText.SetWindowText(szHandStatus);
	m_handNumText.UpdateWindow();
	return 1;
}

LRESULT CControlCenterDlg::OnLogOnPeer(WPARAM wParam, LPARAM lParam){
	int nOnlineNum=pService->GetNameSockMap()->size();
	CString sOnlineNumText;
	sOnlineNumText.Format(IDS_ONLINENUM,nOnlineNum);
	m_onlineNumStatic.SetWindowText(sOnlineNumText);
	return 1;
}

LRESULT CControlCenterDlg::OnLogOffPeer(WPARAM wParam, LPARAM lParam){

	return OnLogOnPeer(wParam,lParam);//这两个函数做的事是一样的，都是让service算一下，写到static里
}

LRESULT CControlCenterDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam){
	return m_TrayIcon.OnTrayNotification(wParam,lParam);	
}

void CControlCenterDlg::OnTrayOpenMainDlg(){
	this->ShowWindow(SW_SHOW);
}

void CControlCenterDlg::OnTrayExit(){	
	PostMessage(WM_CLOSE);
}




/*
BOOL CControlCenterDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(pWnd==&m_handNumText){
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));		
	}
	
	else{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	
	return TRUE;
}
*/
