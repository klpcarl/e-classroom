#include "stdafx.h"
#include "MyRDPSessionEvents.h"


CMyRDPSessionEvents :: CMyRDPSessionEvents()
	{
	
		wnd=NULL;
		msg=0;
		pSharingSession=NULL;

	refNum = 0;
	AddRef();
	}

CMyRDPSessionEvents :: ~CMyRDPSessionEvents()
	{
	}

// IUnknown Methods
HRESULT _stdcall CMyRDPSessionEvents :: QueryInterface(REFIID iid,void**ppvObject)
	{
	*ppvObject = 0;
	if (iid == IID_IUnknown || iid == IID_IDispatch || iid == __uuidof(_IRDPSessionEvents))
		*ppvObject = this;
	if (*ppvObject)
		{
		((IUnknown*)(*ppvObject))->AddRef();
		return S_OK;
		}
	return E_NOINTERFACE;
	}

ULONG _stdcall CMyRDPSessionEvents :: AddRef()
	{
	refNum++;
	return refNum;
	}

ULONG _stdcall CMyRDPSessionEvents :: Release()
	{
	refNum--;
	if (!refNum)
		{
		delete this;
		return 0;
		}
	return refNum;
	}

// IDispatch Methods
HRESULT _stdcall CMyRDPSessionEvents :: GetTypeInfoCount(unsigned int * pctinfo) 
	{
	return E_NOTIMPL;
	}

HRESULT _stdcall CMyRDPSessionEvents :: GetTypeInfo(unsigned int iTInfo,LCID lcid,ITypeInfo FAR* FAR* ppTInfo) 
	{
	return E_NOTIMPL;
	}

HRESULT _stdcall CMyRDPSessionEvents :: GetIDsOfNames(
	REFIID riid,
	OLECHAR FAR* FAR*,
	unsigned int cNames,
	LCID lcid,
	DISPID FAR* ) 
	{
	return E_NOTIMPL;
	}

void CMyRDPSessionEvents :: SetNotification(HWND n,UINT m,IRDPSRAPISharingSession* p)
	{
	wnd = n;
	msg = m;
	pSharingSession = p;
	}



void CMyRDPSessionEvents :: HandleEvents(DISPID id,DISPPARAMS* pDispParams)
	{
	IDispatch* j = 0;
	IUnknown* ju = 0;
	VARIANT x;
	unsigned int y = 0;
	HRESULT hr = 0;

	switch(id)
		{
		case DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_CONNECTED:
			{			

			x.vt = VT_DISPATCH;//指定VARIANT 的类型
			x.pdispVal = NULL;////虽然我不清楚为什么但是这句话是至关重要的，没有的话会引发内存读写错误
	
			hr = DispGetParam(pDispParams,0,VT_DISPATCH,&x,&y);
			if (FAILED(hr))return;			
			IRDPSRAPIAttendee* pAttendee=(IRDPSRAPIAttendee*)x.pdispVal;
			pAttendee->put_ControlLevel(CTRL_LEVEL_VIEW);

			break;
			}


		/*	
		case DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_DISCONNECTED:
			{
			x.vt = VT_DISPATCH;
			x.pdispVal = j;
			hr = DispGetParam(p,0,VT_DISPATCH,&x,&y);
			if (FAILED(hr))
				return;
			IDispatch* d = x.pdispVal;
			if (!d)
				return;

			IRDPSRAPIAttendeeDisconnectInfo* a = 0;
			d->QueryInterface(__uuidof(IRDPSRAPIAttendeeDisconnectInfo),(void**)&a);
			if (!a)
				return;

			IRDPSRAPIAttendee* at = 0;
			a->get_Attendee(&at);
			if (at)
				{
				RemoveAttendee(at);
				at->Release();
				}

			a->Release();
			break;
			}
			*/
			

		case DISPID_RDPSRAPI_EVENT_ON_CTRLLEVEL_CHANGE_REQUEST:
			{

				
			x.vt = VT_INT;
			x.pdispVal = NULL;
			hr = DispGetParam(pDispParams,1,VT_INT,&x,&y);
			if (FAILED(hr)) return;				
			int Lev = x.intVal;//客户的发过来的控制级别
			


			x.vt = VT_INT;
			x.pdispVal = j;
			hr = DispGetParam(pDispParams,0,VT_DISPATCH,&x,&y);
			if (FAILED(hr)) return;
			IRDPSRAPIAttendee * pAttendee=(IRDPSRAPIAttendee*)x.pdispVal;
			pAttendee->put_ControlLevel((CTRL_LEVEL)Lev);
			
			/*
			IDispatch* d = x.pdispVal;
			if (!d)
				return;
			IRDPSRAPIAttendee* a = 0;
			d->QueryInterface(__uuidof(IRDPSRAPIAttendee),(void**)&a);
			if (!a)
				return;

			ATTENDEE* sa = FindAtt(a);
			if (sa)
				sa->SetControl(Lev);
			a->Release();
			*/
			break;
			}


			/*
		case DISPID_RDPSRAPI_EVENT_ON_VIRTUAL_CHANNEL_JOIN:
			{
			break;
			}
		case DISPID_RDPSRAPI_EVENT_ON_VIRTUAL_CHANNEL_LEAVE:
			{
			break;
			}
		case DISPID_RDPSRAPI_EVENT_ON_VIRTUAL_CHANNEL_SENDCOMPLETED:
			{
			x.vt = VT_INT;
			hr = DispGetParam(p,2,VT_INT,&x,&y);
			if (FAILED(hr))
				return;
			int sz = x.intVal;
			x.vt = VT_INT;
			x.pdispVal = j;
			hr = DispGetParam(p,1,VT_INT,&x,&y);
			if (FAILED(hr))
				return;
			int lid = x.intVal;
			x.vt = VT_INT;
			x.punkVal = ju;
			hr = DispGetParam(p,0,VT_DISPATCH,&x,&y);
			if (FAILED(hr))
				return;
			IUnknown* u = x.punkVal;
			if (!u)
				return;

			IRDPSRAPIVirtualChannel* C = 0;
			u->QueryInterface(__uuidof(IRDPSRAPIVirtualChannel),(void**)&C);
			if (!C)
				return;

			RAS::CHANNEL* CC = 0;
			for(unsigned int i = 0 ; i < GetChannels().size() ; i++)
				{
				if (GetChannels()[i].GetInterface() == (void*)C)
					{
					CC = &GetChannels()[i];
					break;
					}
				}
	

			// Data notification
			OnReceiveData(CC,0,0,sz);

			// Check if this Channel has pending data
			CC->Flush();
	
			break;
			}
		case DISPID_RDPSRAPI_EVENT_ON_VIRTUAL_CHANNEL_DATARECEIVED:
			{
			x.vt = VT_BSTR;
			BSTR xa = 0;
			x.bstrVal = xa;

			hr = DispGetParam(p,2,VT_BSTR,&x,&y);
			if (FAILED(hr))
				return;
			BSTR data = x.bstrVal;
			x.vt = VT_INT;
			x.pdispVal = j;
			hr = DispGetParam(p,1,VT_INT,&x,&y);
			if (FAILED(hr))
				return;
			int lid = x.intVal;
			x.vt = VT_INT;
			x.punkVal = ju;
			hr = DispGetParam(p,0,VT_DISPATCH,&x,&y);
			if (FAILED(hr))
				return;
			IUnknown* u = x.punkVal;
			if (!u)
				return;

			IRDPSRAPIVirtualChannel* C = 0;
			u->QueryInterface(__uuidof(IRDPSRAPIVirtualChannel),(void**)&C);
			if (!C)
				return;

			RAS::CHANNEL* CC = 0;
			for(unsigned int i = 0 ; i < GetChannels().size() ; i++)
				{
				if (GetChannels()[i].GetInterface() == (void*)C)
					{
					CC = &GetChannels()[i];
					break;
					}
				}
	
			int rsz = SysStringByteLen(data);
			// Find Attendee
			ATTENDEE* A = 0;
			for(unsigned int jj = 0 ; jj < GetAttendees().size() ; jj++)
				{
				if (GetAttendees()[jj].GetID() == lid)
					{
					A = &GetAttendees()[jj];
					break;
					}
				}
			OnReceiveData(CC,A,(char*)data,rsz);

			C->Release();
			break;
			}

			*/
		}
	}


HRESULT _stdcall CMyRDPSessionEvents :: Invoke(
										DISPID dispIdMember,
										REFIID riid,
										LCID lcid,
										WORD wFlags,
										DISPPARAMS FAR* pDispParams,
										VARIANT FAR* pVarResult,
										EXCEPINFO FAR* pExcepInfo,
										unsigned int FAR* puArgErr)
	{


	LRESULT r = 0;
	if (wnd && msg)
		r = SendMessage(wnd,msg,dispIdMember,(LPARAM)pDispParams);//通知窗口消息
	if (r == 1)//如果窗口处理了这个消息并成功返回
		return S_OK;

	// Else, default implementation
	//pSharingSession->HandleNotification(dispIdMember,pDispParams);

	HandleEvents(dispIdMember,pDispParams);
	
	return S_OK;
	}


