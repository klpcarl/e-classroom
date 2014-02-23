// MonitorViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "MonitorViewerDlg.h"
#include "afxdialogex.h"


// CMonitorViewerDlg 对话框

IMPLEMENT_DYNAMIC(CMonitorViewerDlg, CDialogEx)

CMonitorViewerDlg::CMonitorViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorViewerDlg::IDD, pParent)
{
	m_ViewerWidth=260;
	m_ViewerHeight=160;
	m_ViewerMargin=8;
	m_ViewerRect=CRect(0,0,m_ViewerWidth,m_ViewerHeight);//初始值
}

CMonitorViewerDlg::~CMonitorViewerDlg()
{
	ViewerCtrlListType::iterator pViewerCtrl=m_ViewerList.begin();
	while(pViewerCtrl!=m_ViewerList.end()){

		delete *pViewerCtrl;

		pViewerCtrl++;
	}
}

void CMonitorViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorViewerDlg, CDialogEx)
	ON_WM_SIZE()	
	ON_WM_VSCROLL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMonitorViewerDlg 消息处理程序

void CMonitorViewerDlg::NextRect(){

	//变换当前的绘图位置,更新m_ViewerRect
	CRect clientRect;
	GetClientRect(&clientRect);
	if(m_ViewerRect.right+m_ViewerMargin+m_ViewerWidth > clientRect.right){
		m_ViewerRect.top=m_ViewerRect.bottom+m_ViewerMargin;
		m_ViewerRect.left=0;
		m_ViewerRect.right=m_ViewerRect.left+m_ViewerWidth;
		m_ViewerRect.bottom=m_ViewerRect.top+m_ViewerHeight;
	}
	else{
		m_ViewerRect.left=m_ViewerRect.right+m_ViewerMargin;
		m_ViewerRect.right=m_ViewerRect.left+m_ViewerWidth;
	}
	//变换当前的绘图位置

}

void CMonitorViewerDlg::AddViewer(CString name, CString ticket){
	CViewerCtrl* pViewerCtrl=new CViewerCtrl;
	m_ViewerList.push_back(pViewerCtrl);//控件
	monitorList.insert(make_pair(name,ticket));//数据

	pViewerCtrl->Create(CViewerCtrl::IDD,this);
	pViewerCtrl->SetMonitorName(name);
	pViewerCtrl->MoveWindow(m_ViewerRect);
	pViewerCtrl->ShowWindow(SW_SHOW);
	//rdp连接
//	pViewerCtrl->Connect(ticket,_T("controller"),_T(""));
//	pViewerCtrl->put_SmartSizing(TRUE);	

	NextRect();
}


void CMonitorViewerDlg::DeleteViewer(CString name){
	CViewerCtrl* pViewerCtrl=GetViewerCtrl(name);
	if(pViewerCtrl==NULL) return;
//	pViewerCtrl->Disconnect();
	delete pViewerCtrl;
	m_ViewerList.remove(pViewerCtrl);
	monitorList.erase(name);
	RepaintAllViewers();
}

void CMonitorViewerDlg::RepaintAllViewers(){
	m_ViewerRect=CRect(0,0,m_ViewerWidth,m_ViewerHeight);

	ViewerCtrlListType::iterator pViewerCtrl;
	for(pViewerCtrl=m_ViewerList.begin();pViewerCtrl!=m_ViewerList.end();pViewerCtrl++){
		(*pViewerCtrl)->MoveWindow(m_ViewerRect);
		(*pViewerCtrl)->Repaint();
		NextRect();
	}
}


void CMonitorViewerDlg::RefreshAllViewers(){
	for(auto pViewerCtrl:m_ViewerList){
		pViewerCtrl->RefreshViewer();
	}
}

void CMonitorViewerDlg::ConnectAllViewers(){
	for(auto pViewerCtrl:m_ViewerList){
		pViewerCtrl->Connect();
		pViewerCtrl->put_SmartSizing(TRUE);
	}
}

void CMonitorViewerDlg::DisconnectAllViewers(){
	for(auto pViewerCtrl:m_ViewerList){
		pViewerCtrl->Disconnect();
	}
}


CViewerCtrl* CMonitorViewerDlg::GetViewerCtrl(CString name){
//	CViewerCtrl* pViewerCtrl=NULL;
	ViewerCtrlListType::iterator pViewerCtrl;
	for(pViewerCtrl=m_ViewerList.begin();pViewerCtrl!=m_ViewerList.end();pViewerCtrl++){
		if((*pViewerCtrl)->GetMonitorName()==name) return *pViewerCtrl;
	}
	return NULL;
}


void CMonitorViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static int i=0;
	if(i++>1){
		RepaintAllViewers();
	}
}


BOOL CMonitorViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SCROLLINFO vinfo;
	vinfo.cbSize =sizeof(vinfo);
	vinfo.fMask =SIF_ALL;
	vinfo.nPage =30;//滚动块自身的长短，通常有如下关系：其长度/滚动条长度（含两个箭头）=nPage/(nMax+2)，                  
	//另外nPage取值-1时，滚动条会不见了。  
	vinfo.nMax =600;//滚动条所能滚动的最大值 
	vinfo.nMin=0;//滚动条所能滚动的最小值 
	vinfo.nTrackPos =0;  
	vinfo.nPos=0;
	SetScrollInfo(SB_VERT,&vinfo);//即使上述步骤一不做，使用此条语句也可以显示滚动条 	

	return TRUE;
}




void CMonitorViewerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	SCROLLINFO scrollinfo; 
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 


	switch (nSBCode)  {   
	case SB_BOTTOM: 
		//滑块滚动到最底部  
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);//滚动屏幕
		scrollinfo.nPos = scrollinfo.nMax;//设定滑块新位置  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); //更新滑块位置 
		break;   
	case SB_TOP: //滑块滚动到最顶部 
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);  
		scrollinfo.nPos = scrollinfo.nMin;   
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;   
	case SB_LINEUP: //单击上箭头  
		scrollinfo.nPos -= 1;  
		if (scrollinfo.nPos<scrollinfo.nMin)  {  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,10); 
		break;  
	case SB_LINEDOWN: //单击下箭头  
		scrollinfo.nPos += 1;  
		if (scrollinfo.nPos>scrollinfo.nMax)  { 
			scrollinfo.nPos = scrollinfo.nMax;
			break; 
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,-10); 
		break;  
	case SB_PAGEUP: //单击滑块上方空白区域 
		scrollinfo.nPos -= 5;   
		if (scrollinfo.nPos<scrollinfo.nMin)  { 
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,10*5);
		break;  
	case SB_PAGEDOWN: //单击滑块下方空白区域  
		scrollinfo.nPos += 5;  
		if (scrollinfo.nPos>scrollinfo.nMax)  { 
			scrollinfo.nPos = scrollinfo.nMax;
			break; 
		}   
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		ScrollWindow(0,-10*5);  
		break;  
	case SB_ENDSCROLL: //鼠标离开滑块，结束滑块拖动  
		// MessageBox("SB_ENDSCROLL"); 
		break;   
	case SB_THUMBPOSITION:   
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
		// scrollinfo.nPos = nPos;  
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		break;   
	case SB_THUMBTRACK: //拖动滑块 
		ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		scrollinfo.nPos = nPos; 
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		break; 
	}


	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CMonitorViewerDlg::OnClose()
{	
	ShowWindow(SW_HIDE);
	DisconnectAllViewers();
}
