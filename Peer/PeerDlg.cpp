
// PeerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Peer.h"
#include "PeerDlg.h"
#include "afxdialogex.h"
#include "Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+1



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


// CPeerDlg 对话框



CPeerDlg::CPeerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPeerDlg::IDD, pParent),m_trayIcon(IDR_TRAY_MENU),pViewerDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pService=new CService();
	
	
}

CPeerDlg::~CPeerDlg(){
//	delete pDoorSock;
	if(pService!=NULL) delete pService;
	if(pViewerDlg!=NULL){
		pViewerDlg->Disconnect();
		delete pViewerDlg;
	}
}


void CPeerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPeerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION,OnTrayNotification)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnSockClose)
	ON_MESSAGE(WM_MY_STARTVIEWER,OnStartViewer)
	ON_MESSAGE(WM_MY_STOPVIEWER,OnStopViewer)
	ON_MESSAGE(WM_MY_LOCK,OnLockViewer)
	ON_MESSAGE(WM_MY_UNLOCK,OnUnlockViewer)
	ON_MESSAGE(WM_MY_CONFIG,OnConfig)
	ON_COMMAND(ID_CONNECT,OnSockConnect)
	ON_COMMAND(ID_VIEWER,OnOpenViewer)
	ON_COMMAND(ID_HAND,OnHand)
	ON_COMMAND(ID_EXIT,OnExit)
	
END_MESSAGE_MAP()


// CPeerDlg 消息处理程序

BOOL CPeerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);	
	m_trayIcon.SetIcon(IDI_OFF_ICON);	
	pService->SetEmployer(this);

	SockConnectWithConfig();

	//显示警告
	ShowWarningOnConfig();
	


	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPeerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPeerDlg::OnPaint()
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

void CPeerDlg::OnNcPaint(){
	static int i=2;
	if(i>0){
		ShowWindow(SW_HIDE);
		i--;
	}
	else{
		CDialogEx::OnNcPaint();
	}
}


BOOL CPeerDlg::SockConnectWithConfig(){

	//从配置文件中获得服端的ip地址和端号
	TCHAR* fileName=_T(".\\Config.ini");//程序所在目录下的配置文件

	TCHAR addr[35];
	UINT port;
	::GetPrivateProfileString(_T("Connection"),_T("address"),_T("localhost"),addr,35,fileName);
	port=::GetPrivateProfileInt(_T("Connection"),_T("port"),5000,fileName);
	//如果没有读取到数据（没有ini文件，ini文件数据不对，将连接 localhost 5000

	if(pService->StartSockService(addr,port)){
		m_trayIcon.SetIcon(IDI_ON_ICON);
		return TRUE;
	}
	else{
		m_trayIcon.SetIcon(IDI_OFF_ICON);
		return FALSE;
	}
}

void CPeerDlg::ShowWarningOnConfig(){
	//从配置文件中获得提示方面的配置
	TCHAR* fileName=_T(".\\Config.ini");//程序所在目录下的配置文件	
	UINT bWarn=::GetPrivateProfileInt(_T("Prompt"),_T("warning"),1,fileName);//如果没有读到数据设为1

	if(bWarn){
		CString sWarnCap,sWarnText;
		if(!sWarnCap.LoadString(IDS_WARNING_CAP)) sWarnCap=_T("警告");
		if(!sWarnText.LoadString(IDS_WARNING_TEXT)) sWarnText=_T("本软件启动并成功连接服务器后，本桌面将随时可能被分享或监控！");
		::MessageBox(NULL,sWarnText,sWarnCap,MB_OK|MB_ICONWARNING);
	}
}





//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPeerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//我的消息处理函数
LRESULT CPeerDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam){
	return m_trayIcon.OnTrayNotification(wParam,lParam);
}

LRESULT CPeerDlg::OnSockClose(WPARAM wParam,LPARAM lParam){
	
	//关闭RDP客户端和服务器端
	if(pViewerDlg){
//		pViewerDlg->SetFullScreen(FALSE);
		pViewerDlg->Disconnect();
		delete pViewerDlg;
		pViewerDlg=NULL;
	}

	/* 放在service中做
	pService->StopRDPService();
	//关闭socket
	pService->StopSockService();
	*/

	

//	MessageBox(_T("服务器断开了连接!"));

	
	return 	m_trayIcon.SetIcon(IDI_OFF_ICON);;
}

LRESULT CPeerDlg::OnStartViewer(WPARAM wParam,LPARAM pTicket){
	
	if(pViewerDlg!=NULL) OnStopViewer(0,0);//
	pViewerDlg=new CViewerDlg();	
	
	pViewerDlg->Create(CViewerDlg::IDD);
	pViewerDlg->ShowWindow(SW_NORMAL);

	CString ticket=*(CString*)pTicket;
	pViewerDlg->Connect((LPTSTR)(LPCTSTR)ticket);
	pService->SetViewerDlg(pViewerDlg);//注册事件
	return 1;
}

LRESULT CPeerDlg::OnStopViewer(WPARAM wParam, LPARAM lParam){


	if(pViewerDlg==NULL) return 0;

	OnUnlockViewer(wParam,lParam);//
	
	pViewerDlg->SetFullScreen(FALSE);
	pViewerDlg->Disconnect();
	delete pViewerDlg;
	pViewerDlg=NULL;
	pService->SetViewerDlg(NULL);
	return 1;
}





LRESULT CPeerDlg::OnLockViewer(WPARAM wParam, LPARAM lParam){
	if(pViewerDlg==NULL) return 0;
	//1.将viewer设为view模式
	pViewerDlg->m_Viewer.RequestControl(CTRL_LEVEL_VIEW);	
	//2.将viewer设为全屏
	pViewerDlg->SetFullScreen();
	//3.使用钩子
	pService->SetHook();//这个钩子未经验证

	return 1;
}

LRESULT CPeerDlg::OnUnlockViewer(WPARAM wParam, LPARAM lParam){
//	if(pViewerDlg)	pViewerDlg->SetFullScreen(FALSE);
	pService->SetHook(FALSE);
	return 1;
}


LRESULT CPeerDlg::OnConfig(WPARAM wParam, LPARAM lParam){
	CString configCmd=*(CString*)lParam;

//	MessageBox(configCmd);

	if(configCmd.Find(_T("disable exit"))>0){
		m_trayIcon.SetDiabledButton(ID_EXIT);
	}
	else if(configCmd.Find(_T("enable exit"))>0){
		m_trayIcon.SetDiabledButton(0);
	}

	return 1;
}




void CPeerDlg::OnSockConnect(){

	SockConnectWithConfig();
}

void CPeerDlg::OnHand(){
	if(!pService->Send(_T("hand up"))){
		//MessageBox(_T("电子举手失败，如有紧急事宜，请直接联系老师！"));
		AfxMessageBox(IDS_HANDUPFAIL_TEXT);
	}
}

void CPeerDlg::OnExit(){
	SendMessage(WM_CLOSE);
}

void CPeerDlg::OnOpenViewer(){
	if(pViewerDlg==NULL){
		//MessageBox(_T("当前无桌面广播会话!"));
		AfxMessageBox(IDS_OPENVIEWERFAIL_TEXT);
		return;
	}
	pViewerDlg->ShowWindow(SW_SHOW);
}
