#pragma once
//#include "afxcmn.h"
#include "stdafx.h"
#include "resource.h"
#include "Service.h"
#include "Message.h"

using namespace std;

typedef std::map<CString,std::list<CString>* > TreeDataType;
typedef std::list<CString> PeerListType;


//#define UNKNOWN_GROUP (_T("未分组"))

// CPeerTreeDlg 对话框
// 用于显示成员分组结构的树控件所在的对话框，主界面的分组管理标签页对应这个对话框，
// 同时，在创建新的广播会话或监控会话时用于选择成员的子窗口也是这个对话框的实例

class CPeerTreeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPeerTreeDlg)

public:
	CPeerTreeDlg(CWnd* pParent = NULL, BOOL bFilterTree=FALSE);   // 标准构造函数	
	virtual ~CPeerTreeDlg();

// 对话框数据
	enum { IDD = IDD_PEERTREE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:

	virtual BOOL OnInitDialog();

private:

	/* 对于分组数据存储的说明：
	每个分组对应一个文件，文件名即为组名，每个文件中一行数据即为该组内的一个成员名
	*/
	BOOL bFilterTree;

	CService* pService;//几乎用不到

	//控件
	CTreeCtrl m_PeerTree;
	CImageList m_ImageList;

	CString UNKNOWN_GROUP;//这应当是一个常量，但需载入资源
	HTREEITEM hUnGroup;

	BOOL bTreeCheckable;//标识当前树控件是否是复选模式

	//数据
	TreeDataType * pTreeData;

	CString treeDataPath;
	

	void InitTreeControl();
	void BuildTree();
//	void FilterItems(HTREEITEM);//辅助FilterTree的递归实现
//	void SetTreeCheckable(BOOL bCheckable=TRUE);//设置是否具有复选框

	BOOL IsGroupItem(HTREEITEM);
	BOOL IsPeerItem(HTREEITEM);

	// 数据管理
	BOOL LoadTreeData();
	void FreeTreeData();
	void VerifyLogPeer(CString peerName);
	BOOL GroupHasPeer(CString group, CString peer);
	HTREEITEM ItemStringToHandle(CString text, HTREEITEM root);
	HTREEITEM GetGroupItem(CString groupName);


	//文件管理
	BOOL DeleteStringFromFile(CString str, CString fileTitle);
	BOOL AppendStringToFile(CString str, CString fileTitle);
	BOOL CreateGroupFile(CString fileTitle);
	BOOL DeleteGroupFile(CString fileTitle);
	BOOL RenameGroupFile(CString fileTitle,CString newFileTitle);


	//注意：组名应当是唯一的，不可重复
	BOOL HasGroup(CString groupName);
//	HTREEITEM GetGroupItemByText(CString groupName);

	/*
	// 组管理
	BOOL InsertGroup(HTREEITEM,CString);
	BOOL DeleteGroup(HTREEITEM);
	void RenameGroup(HTREEITEM,CString);

	// 成员管理
	void DeletePeer(HTREEITEM);
	void MovePeer(HTREEITEM sourGroup,HTREEITEM desGroup,HTREEITEM peer);
	*/
	//树管理
	void TreeTravesal(HTREEITEM hStartItem,BOOL bEvent);
	void ChangePrtItemAcdByChild( HTREEITEM hItemClicked,BOOL bEvent );



public:
	void SetService(CService*);
	void FilterTree(CPeerTreeDlg&);//这个对话框中树控件的生成方式有两种，一是由BuildTree从文件读取数据构件树，另一是由FilterTree复制另一个已存在的树控件, 过滤树将只保留在线节点
	
//	void FilterOffline();//只保留在线的，删除离线的节点
	void SetTreeCheckable(BOOL bCheckable=TRUE);//设置是否具有复选框
	void GetCheckedPeerNames(list<CString>*, HTREEITEM root);//注意：这是返回堆区空间的
	void GetCheckedPeerNames(list<CString>*);//获得以根下所有的
	CString GetSelectedPeerName();


	afx_msg void OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMovePeer();
	afx_msg void OnDelPeer();
	afx_msg void OnRenameGroup();
	afx_msg void OnInsertGroup();
	afx_msg void OnDelGroup();
	afx_msg void OnSetTreeCheckable();
	afx_msg LRESULT OnLogName(WPARAM,LPARAM);
	afx_msg LRESULT OnLogoutName(WPARAM,LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
