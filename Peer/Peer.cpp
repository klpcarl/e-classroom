
// Peer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Peer.h"
#include "PeerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPeerApp

BEGIN_MESSAGE_MAP(CPeerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPeerApp 构造

CPeerApp::CPeerApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPeerApp 对象

CPeerApp theApp;


// CPeerApp 初始化

BOOL CPeerApp::InitInstance()
{
	WSADATA data;
	AfxSocketInit(&data);

	HRESULT hr=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE);
	if(FAILED(hr)) return FALSE;


	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	/*
	CPeerDlg * pDlg=new CPeerDlg;
	m_pMainWnd = pDlg;
	pDlg->Create(CPeerDlg::IDD);
	pDlg->ShowWindow(SW_HIDE);
	pDlg->UpdateWindow();
	*/

	CPeerDlg dlg;
	dlg.Create(CPeerDlg::IDD);
	dlg.ShowWindow(SW_HIDE);
	dlg.RunModalLoop();

	





	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	CoUninitialize();

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
//	return TRUE;// caution : when to shut the app down ?！！！！！！！！！！！！！！！！！ 注意：如何终止程序？
}

