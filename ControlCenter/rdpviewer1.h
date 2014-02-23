#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CRdpviewer1 包装类

class CRdpviewer1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CRdpviewer1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x32BE5ED2, 0x5C86, 0x480F, { 0xA9, 0x14, 0xF, 0xF8, 0x88, 0x5A, 0x1B, 0x3F } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:
enum
{
    CONST_MAX_CHANNEL_MESSAGE_SIZE = 1024,
    CONST_MAX_CHANNEL_NAME_LEN = 8,
    CONST_MAX_LEGACY_CHANNEL_MESSAGE_SIZE = 409600,
    CONST_ATTENDEE_ID_EVERYONE = -1,
    CONST_ATTENDEE_ID_HOST = 0,
    CONST_CONN_INTERVAL = 50
}__MIDL___MIDL_itf_rdpencomapi_0000_0018_0001;
enum
{
    CTRL_LEVEL_MIN = 0,
    CTRL_LEVEL_INVALID = 0,
    CTRL_LEVEL_NONE = 1,
    CTRL_LEVEL_VIEW = 2,
    CTRL_LEVEL_INTERACTIVE = 3,
    CTRL_LEVEL_MAX = 3
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0001;
enum
{
    ATTENDEE_DISCONNECT_REASON_MIN = 0,
    ATTENDEE_DISCONNECT_REASON_APP = 0,
    ATTENDEE_DISCONNECT_REASON_ERR = 1,
    ATTENDEE_DISCONNECT_REASON_CLI = 2,
    ATTENDEE_DISCONNECT_REASON_MAX = 2
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0002;
enum
{
    CHANNEL_PRIORITY_LO = 0,
    CHANNEL_PRIORITY_MED = 1,
    CHANNEL_PRIORITY_HI = 2
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0003;
enum
{
    CHANNEL_FLAGS_LEGACY = 1,
    CHANNEL_FLAGS_UNCOMPRESSED = 2
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0004;
enum
{
    CHANNEL_ACCESS_ENUM_NONE = 0,
    CHANNEL_ACCESS_ENUM_SENDRECEIVE = 1
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0005;
enum
{
    ATTENDEE_FLAGS_LOCAL = 1
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0006;
enum
{
    WND_FLAG_PRIVILEGED = 1
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0007;
enum
{
    APP_FLAG_PRIVILEGED = 1
}__MIDL___MIDL_itf_rdpencomapi_0000_0000_0008;


// 操作
public:

// IRDPSRAPIViewer

// Functions
//

	void Connect(LPCTSTR bstrConnectionString, LPCTSTR bstrName, LPCTSTR bstrPassword)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrConnectionString, bstrName, bstrPassword);
	}
	void Disconnect()
	{
		InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Attendees()
	{
		LPDISPATCH result;
		InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Invitations()
	{
		LPDISPATCH result;
		InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ApplicationFilter()
	{
		LPDISPATCH result;
		InvokeHelper(0xd7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_VirtualChannelManager()
	{
		LPDISPATCH result;
		InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_SmartSizing(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xee, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_SmartSizing()
	{
		BOOL result;
		InvokeHelper(0xee, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void RequestControl(long CtrlLevel)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CtrlLevel);
	}
	void put_DisconnectedText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xed, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_DisconnectedText()
	{
		CString result;
		InvokeHelper(0xed, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void RequestColorDepthChange(long Bpp)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Bpp);
	}
	LPDISPATCH get_Properties()
	{
		LPDISPATCH result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString StartReverseConnectListener(LPCTSTR bstrConnectionString, LPCTSTR bstrUserName, LPCTSTR bstrPassword)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x74, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, bstrConnectionString, bstrUserName, bstrPassword);
		return result;
	}

// Properties
//



};
