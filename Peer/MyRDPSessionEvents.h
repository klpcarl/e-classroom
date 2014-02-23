#pragma once


// 注意：这个头文件包含 B 和CMyRDPSessionEvents两个类，这两个类为直接引用，非原创


	// Quick BSTR stuff
	class B //这个类用于字符间转换，可以将普通字符串转换为BSTR
		{
		private:
			BSTR bb;
		public:
			B(const wchar_t* s)
				{
				bb = 0;
				if (s)
					bb = SysAllocString(s);
				}
			B(const char* s)
				{
				bb = 0;
				if (!s)
					return;

				int sl = lstrlenA(s);
				int nsl = sl* 2 + 100;
				wchar_t* ws = new wchar_t[nsl];
				memset(ws,0,sizeof(wchar_t)*nsl);
				MultiByteToWideChar(CP_UTF8,0,s,-1,ws,nsl);
				bb = SysAllocString(ws);
				delete[] ws;
				}
			~B()
				{
				if (bb)
					SysFreeString(bb);
				bb = 0;
				}
			operator BSTR()
				{
				return bb;
				}

		};


	// CMyRDPSessionEvents类
	// 用于为RDP服务端提供RDP事件监听与处理

	class CMyRDPSessionEvents : public _IRDPSessionEvents
		{
		private:

			int refNum;
			HWND wnd;
			UINT msg;
			IRDPSRAPISharingSession* pSharingSession;





		private:
			void HandleEvents(DISPID idMember,DISPPARAMS* pDispParams);
		

		public:


			CMyRDPSessionEvents();
			~CMyRDPSessionEvents();
			void SetNotification(HWND,UINT,IRDPSRAPISharingSession* p);//赋值


			// IUnknown
            virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
                /* [in] */ REFIID riid,
                /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

            virtual ULONG STDMETHODCALLTYPE AddRef( void);

            virtual ULONG STDMETHODCALLTYPE Release( void);


			// IDispatch
			virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
            /* [out] */ __RPC__out UINT *pctinfo);
        
			virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
	        virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
		    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
		};