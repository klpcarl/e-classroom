// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WLIST, m_wlist);
	DDX_Control(pDX, IDC_BLIST, m_blist);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序




// 获取系统安装程序列表   
BOOL CSettingDlg::AllRegKey(HKEY hKey, LPCTSTR szDesKeyItem)   
{   
     HKEY hSubKey;   
     DWORD dwIndex = 0;   
     long lResult;   
     int i = 1;   
    
     lResult = RegOpenKeyEx(hKey, szDesKeyItem, 0, KEY_READ, &hSubKey);      // 打开注册表   
     if(ERROR_SUCCESS == lResult) {   
         TCHAR szKeyName[256] = { 0 };   
         DWORD cbName = 256*sizeof(TCHAR);

		 TCHAR s_name[256] = {0};        // 软件名            
		 TCHAR s_DisplayName[256] = {0};         // 显示名称 
		 DWORD typeSZ = REG_SZ;          // SZ 注册表类型 

    
         lResult = RegEnumKeyEx(hSubKey, dwIndex, szKeyName, &cbName, 0, NULL, NULL, NULL);      // 获取字段   
         while (lResult == ERROR_SUCCESS && lResult != ERROR_MORE_DATA){   
             OutputDebugString(szKeyName);   
    
			
			m_wlist.InsertItem(i++,szKeyName);
			 


			 HKEY hItem;     // 下一级注册表句柄  
			 if(::RegOpenKeyEx(hSubKey, s_name, 0, KEY_READ, &hItem) == ERROR_SUCCESS){      // 提取下一级注册表程序的属性信息  
                if (::RegQueryValueEx(hItem, _T("DisplayName"), 0, &typeSZ, (LPBYTE)s_DisplayName, &cbName) == ERROR_SUCCESS) {  
                   // m_wlist.SetItemText(i-1,0,s_DisplayName);
				
                }
				      
                ::RegCloseKey(hItem);   // 关闭二级注册表（程序属性信息）  
            }  
  
    
		
    
             dwIndex = dwIndex + 1;          // 改变dwIndex，循环获取下移字段   
             cbName = 256*sizeof(TCHAR);   
             lResult = RegEnumKeyEx(hSubKey, dwIndex, szKeyName, &cbName, 0, NULL, NULL, NULL);   
    
            
    
         }   
         RegCloseKey(hSubKey);       // 关闭注册表   
         return TRUE;   
     }   
     RegCloseKey(hSubKey);   
     return FALSE;   
 }   




/*

BOOL CSettingDlg::AllRegKey(HKEY hKey, LPCTSTR szDesKeyItem)  
{  
    HKEY hSubKey;  
    DWORD dwIndex = 0;  
    long lResult;  
  
    lResult = RegOpenKeyEx(hKey, szDesKeyItem, 0, KEY_READ, &hSubKey);      // 打开注册表  
    if(ERROR_SUCCESS == lResult) {  
        int id = 1;                     // 序号  
        TCHAR s_name[256] = {0};        // 软件名            
        TCHAR s_DisplayName[256] = {0};         // 显示名称  
        TCHAR s_Publisher[256] = {0};           // 发布者  
        TCHAR s_InstallDate[12] = {0};          // 安装日期  
        DWORD s_EstimatedSize;                  // 大小  
        TCHAR s_DisplayVersion[256] = { 0 };    // 版本  
        DWORD cbName = 256*sizeof(TCHAR);     
        DWORD cbName2 = sizeof(TCHAR);  
  
        DWORD typeSZ = REG_SZ;          // SZ 注册表类型  
        DWORD typeDWORD = REG_DWORD;    // DWORD 注册表类型  
  
        lResult = RegEnumKeyEx(hSubKey, dwIndex, s_name, &cbName, 0, NULL, NULL, NULL);     // 获取字段  
        while (lResult == ERROR_SUCCESS && lResult != ERROR_MORE_DATA){  
            OutputDebugString(s_name);  
  
            CString str;  
            str.Format(_T("%d"), id);  
  
            id = m_blist.InsertItem(id, str);        // 插入到List控件显示  
            m_blist.SetItemText(id, 0, s_name);  
  
  
			
            HKEY hItem;     // 下一级注册表句柄  
            if(::RegOpenKeyEx(hSubKey, s_name, 0, KEY_READ, &hItem) == ERROR_SUCCESS){      // 提取下一级注册表程序的属性信息  
                if (::RegQueryValueEx(hItem, _T("DisplayName"), 0, &typeSZ, (LPBYTE)s_DisplayName, &cbName) == ERROR_SUCCESS) {  
                    m_blist.SetItemText(id, 1, s_DisplayName);  
				
                }  
  
                if (::RegQueryValueEx(hItem, _T("Publisher"), 0, &typeSZ, (LPBYTE)s_Publisher, &cbName) == ERROR_SUCCESS) {  
                    m_blist.SetItemText(id, 2, s_Publisher);  
                }  
  
                if (::RegQueryValueEx(hItem, _T("InstallDate"), 0, &typeSZ, (LPBYTE)s_InstallDate, &cbName) == ERROR_SUCCESS) {  
                    if (s_InstallDate != NULL) {        // 判断是否为空  
                        m_blist.SetItemText(id, 3, s_InstallDate);         
                    }  
                }  
  
                if (::RegQueryValueEx(hItem, _T("EstimatedSize"), 0, &typeDWORD, (LPBYTE)&s_EstimatedSize, &cbName2) == ERROR_SUCCESS) {  
                    float f_Size = s_EstimatedSize/1024.0;  // DWORD 转换为 CString 类型  
                    CString p_Size;  
                    p_Size.Format(_T("%.2f M"), f_Size);  
                      
                    m_blist.SetItemText(id, 4, p_Size);  
                }  
  
                if (::RegQueryValueEx(hItem, _T("DisplayVersion"), 0, &typeSZ, (LPBYTE)s_DisplayVersion, &cbName) == ERROR_SUCCESS) {  
                    m_blist.SetItemText(id, 5, s_DisplayVersion);  
                }  
                  
                ::RegCloseKey(hItem);   // 关闭二级注册表（程序属性信息）  
            }  
		
			
  
            dwIndex = dwIndex + 1;          // 改变dwIndex，循环获取下移字段  
            cbName = 256*sizeof(TCHAR);  
            lResult = RegEnumKeyEx(hSubKey, dwIndex, s_name, &cbName, 0, NULL, NULL, NULL);  
  
            id++;  
  
        }  
        RegCloseKey(hSubKey);   // 关闭一级注册表（读取程序信息）  
        return TRUE;  
    }  
    RegCloseKey(hSubKey);  
    return FALSE;  
}  

*/



BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wlist.InsertColumn(1,_T("程序名称"));
	m_blist.InsertColumn(1,_T("程序名称"));//都没用


	AllRegKey(HKEY_LOCAL_MACHINE, TEXT("SoftWare\\Microsoft\\Windows\\CurrentVersion\\Uninstall"));  

	return TRUE;
}
