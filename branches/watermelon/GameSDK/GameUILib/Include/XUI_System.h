#ifndef GUISYSTEM_H
#define GUISYSTEM_H

#pragma once
#include "XUI_Wnd.h"
#include "timer_manager.h"
#include "singleton.h"

//����ϵͳ����Ҫ������Ϣ��ת��
#define DEFAULT_DESKTOP			0x10000
#define TIMER_PRECISION			(0.1f)
#define TIMER_SECOND(second)	int(second*1.0f/TIMER_PRECISION)
namespace XGC
{
	namespace ui
	{
		struct ICanvas;
		class XUI_Window;
		class XUI_System
		{
			friend struct CreateUsingNew< XUI_System >;
			friend class XUI_Lua;
		private:
			XUI_System();
			XUI_System( CONST XUI_System& src );
			virtual ~XUI_System(void);

		public:
			bool Initialize( HWND hWnd, _lpcstr lpszPath );
			void Unitialize();

			void SetEditMode( bool bMode );
			bool IsEditMode()const{ return m_bEditMode; }

			// ����ͼƬ·��
			_lpcstr	GetResourcePath();

			void SetDefaultFont( xuiFont hFont ){ m_hDefaultFont = hFont; }
			xuiFont	GetDefaultFont()const{ return m_hDefaultFont; }

			bool IsButtonPress( int nKey ){ return (GetKeyState( nKey ) & 0x8000) != 0; }
			void SetMouseCursor( int nIndex ){}
		protected:

			// �Ƿ��ʼ��
			bool m_bInitialized;
			bool m_bPointInRoot;

			//��Ӧ�Ĵ���
			HWND		m_hWnd;
			WNDPROC		m_OldProc;
			iSize		m_WindowSize;
			iPoint		m_ptMouse;
			iPoint		m_ptMouseDown;

		private:
			//��
			typedef std::list< XUI_Window* >		CTopWindowList;
			typedef std::list< XUI_Window* >		CModalList;
			typedef std::list< XUI_Wnd* >			CWndList;

			CTopWindowList	m_TopWindowList;
			CModalList		m_ModalList;

			xuiFont			m_hDefaultFont;

			std::string		m_strResourcePath;
			timer			m_Timer;
			float			m_fNowTime;
			float			m_timer_anchor;
			bool			m_bEditMode;
			int				m_nCurrentHandler;
			XUI_Wnd			*m_pCaptureElement, *m_pOverElement;
			CWndList		m_capture_list;
		protected:
			void SetFocus		( XUI_Wnd* pElement );

			//��Ϣ������֤��Ϣ���԰���Ԥ���ķ�ʽת��
			bool OnMouseMove	( const iPoint& pt, _uint32 sysKeys, long_ptr *result );
			bool OnMouseLeave	( XUI_Wnd* pElement );

			bool OnButtonDown	( _uint32 nButton, const iPoint& pt, _uint32 sysKeys, long_ptr *result );
			bool OnButtonUp		( _uint32 nButton, const iPoint& pt, _uint32 sysKeys, long_ptr *result );

			bool OnKeyDown		( _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result );
			bool OnKeyUp		( _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result );
			bool OnChar			( _uint32 dwChar, _uint32 sysKeys, long_ptr *result );

			bool OnImeComp		( int_ptr wParam, long_ptr lParam, long_ptr *result );
			bool OnImeEndComp	( int_ptr wParam, long_ptr lParam, long_ptr *result );
			bool OnImeNotify	( int_ptr wParam, long_ptr lParam, long_ptr *result );

			bool HandleMouse	( _uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result );
			bool HandleKeyboard	( _uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result );

		public:
			HWND	GetHWND()const			{ return m_hWnd; }
			iSize	GetClientSize()const	{ return m_WindowSize; }
			//��Ⱦ
			void Render();
			void RenderEditFrame();
			_uint32 DetectHandler(  XUI_Wnd* pElement, const iPoint &pt );


			//�����ⲿ��Ϣ
			LRESULT HandleMessage( HWND hWnd, _uint32 uMsg, int_ptr wParam, long_ptr lParam );

			void Update( float fDelta );
			//////////////////////////////////////////////////////////////////////////
			// ���ɽ���

			void RegistTopWindow( XUI_Window* pTopWindow );
			XUI_Window* RemoveTopWindow( XUI_Window *pTopWindow );
			XUI_Window* GetTopWindow( _lpctstr lpszWindowName );

			bool EnterModaless( XUI_Window* pDialog );
			void LeaveModaless();

			bool LoadFromFile( _lpcstr pszFilename );
			bool SaveToFile( _lpcstr pszFilename );

			unsigned int SetTimer( _tfunction function, unsigned short repeat, unsigned short timer );
			void KillTimer( unsigned int handle );
		};

		extern bool SetupDebuger();
		inline unsigned int GetLongevity( XUI_System* ){ return 20; }
		typedef SingletonHolder< XUI_System, CreateUsingNew, SingletonWithLongevity > XUI;
	}
}

#endif