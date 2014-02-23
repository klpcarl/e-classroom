////////////////////////////////////////////////////////////////
// CTrayIcon Copyright 1996 Microsoft Systems Journal.
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.

#ifndef _TRAYICON_H
#define _TRAYICON_H



////////////////
// CTrayIcon manages an icon in the Windows 95 system tray. 
// 这个类是直接引用的网上的一个类，用于提供系统托盘功能


class CTrayIcon : public CCmdTarget {
protected:
	DECLARE_DYNAMIC(CTrayIcon)
	NOTIFYICONDATA m_nid;			// struct for Shell_NotifyIcon args

public:
	CTrayIcon(UINT uID);
	~CTrayIcon();

	// Call this to receive tray notifications
	void SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg);

	// SetIcon functions. To remove icon, call SetIcon(0)
	//
	BOOL SetIcon(UINT uID); // main variant you want to use
	BOOL SetIcon(HICON hicon, LPCWSTR lpTip);
	BOOL SetIcon(LPCTSTR lpResName, LPCWSTR lpTip)
		{ return SetIcon(lpResName ? 
			AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip); }
	BOOL SetStandardIcon(LPCTSTR lpszIconName, LPCWSTR lpTip)
		{ return SetIcon(::LoadIcon(NULL, lpszIconName), lpTip); }

	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
};

#endif
