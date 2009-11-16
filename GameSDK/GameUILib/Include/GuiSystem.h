#ifndef GUISYSTEM_H
#define GUISYSTEM_H

#pragma once
#include "XUI_Wnd.h"
#include "timer_manager.h"
#include <loki/Singleton.h>

//����ϵͳ����Ҫ������Ϣ��ת��
#define DEFAULT_DESKTOP			0x10000
#define TIMER_PRECISION			(0.1f)
#define TIMER_SECOND(second)	int(second*1.0f/TIMER_PRECISION)
namespace UILib
{
	struct ICanvas;
	class XUI_Window;
	class CGuiSystem
	{
	friend struct Loki::CreateUsingNew< CGuiSystem >;
	friend class LuaBindClass;
	private:
		CGuiSystem();
		CGuiSystem( CONST CGuiSystem& src );
		virtual ~CGuiSystem(void);

	public:
		bool Initialize( HWND hWnd, _lpcstr lpszPath );
		void Unitialize();

		void SetEditMode( bool bMode );
		bool IsEditMode()const{ return m_bEditMode; }

		// ����ͼƬ·��
		_lpcstr	GetResourcePath();

		void SetDefaultFont( XUI_IFont *pFont ){ m_pDefaultFont	= pFont; }
		void SetDefaultCursor( XUI_IMouse *pCursor ){ m_pCursor = pCursor; }

		XUI_IFont*	GetDefaultFont()const{ return m_pDefaultFont; }
		XUI_IMouse*	GetMouseCursor()const{ return m_pCursor; }

	protected:

		// �Ƿ��ʼ��
		bool m_bInitialized;
		bool m_bPointInRoot;

		//��Ӧ�Ĵ���
		HWND	m_hWnd;
		xgcSize	m_windowsize;
		xgcPoint	m_mouse_old;

	private:
		//��
		typedef std::map< int, XUI_Window* >	CDesktopMap;
		typedef std::list< XUI_Window* >		CModalList;

		XUI_Window		*m_pDesktop;
		CDesktopMap		m_DesktopMap;
		CModalList		m_ModalList;

		XUI_IFont		*m_pDefaultFont;
		XUI_IMouse		*m_pCursor;

		std::string		m_resource_path;
		CTimerManager	m_timer;
		float			m_nowtime;
		float			m_timer_anchor;
		bool			m_bEditMode;
		int				m_nCurHandle;
		xgcPoint			m_mousedown;
		XUI_Wnd*		m_capture_element;
		XUI_Wnd*		m_mouseover_element;
	protected:
		void SetFocus		( XUI_Wnd* pElement );

		//��Ϣ������֤��Ϣ���԰���Ԥ���ķ�ʽת��
		bool onMouseMove	( XUI_Wnd* pElement, const xgcPoint& pt, _uint32 sysKeys, long_ptr *result );
		bool onMouseLeave	( XUI_Wnd* pElement );
		
		bool onButtonDown	( XUI_Wnd* pElement, _uint32 nButton, const xgcPoint& pt, _uint32 sysKeys, long_ptr *result );
		bool onButtonUp		( XUI_Wnd* pElement, _uint32 nButton, const xgcPoint& pt, _uint32 sysKeys, long_ptr *result );

		bool onKeyDown		( XUI_Wnd* pElement, _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result );
		bool onKeyUp		( XUI_Wnd* pElement, _uint32 dwVirtualCode, _uint32 sysKeys, long_ptr *result );
		bool onChar			( XUI_Wnd* pElement, _uint32 dwChar, _uint32 sysKeys, long_ptr *result );

		bool onImeComp		( XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result );
		bool onImeEndComp	( XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result );
		bool onImeNotify	( XUI_Wnd* pElement, int_ptr wParam, long_ptr lParam, long_ptr *result );

		bool HandleMouse	( _uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result );
		bool HandleKeyboard	( _uint32 uMsg, int_ptr wParam, long_ptr lParam, long_ptr *result );

	public:
		HWND GetHWND()const {return m_hWnd;}
	    
		XUI_Window* GetRoot() {return m_pDesktop;}

		//��Ⱦ
		void Render();
		void RenderEditFrame( const xgcRect &rc );
		_uint32 DetectHandler(  XUI_Wnd* pElement, const xgcPoint &pt );

		
		//�����ⲿ��Ϣ
		LRESULT HandleMessage( HWND hWnd, _uint32 uMsg, int_ptr wParam, long_ptr lParam );

		void Update( float fDelta );
		//////////////////////////////////////////////////////////////////////////
		// ���ɽ���

		void RegistDesktop( XUI_Window* pDesktop );
		void SwitchDesktop( int nDesktopID );
		XUI_Window* RemoveDesktop( int nDesktopID );
		XUI_Window* GetDesktop( UINT nDesktopID );

		bool EnterModaless( XUI_Window* pDialog );
		void LeaveModaless();

		bool LoadFromFile( _lpcstr pszFilename );
		bool SaveToFile( _lpcstr pszFilename );

		unsigned int SetTimer( TimerFunction function, unsigned short repeat, unsigned short timer );
		void KillTimer( unsigned int handle );
	};

	extern bool SetupDebuger();
	inline unsigned int GetLongevity( CGuiSystem* ){ return 20; }
	typedef Loki::SingletonHolder< CGuiSystem, Loki::CreateUsingNew, Loki::SingletonWithLongevity > GuiSystem;
}

#endif