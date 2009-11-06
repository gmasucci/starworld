#ifndef XUI_Wnd_H
#define XUI_Wnd_H
#pragma once

#include "XUI_Base.h"
#include "XUI_Canvas.h"
#define DECLARE_LABLE( lable )\
	virtual _lpctstr GetLable()const { return _T(#lable); }\

namespace UILib
{
	struct XUI_IFont;
	class XUI_Wnd	:	public UIObjTypeT< XUI_Base, TypeElement >
	{
	friend class CGuiSystem;
	friend class CUIFactory;
	friend class LuaBindClass;

	protected:
		XUI_Wnd(void);
		virtual ~XUI_Wnd(void);

		void Destroy();
	protected:
		//����
		void Render(const xgcRect& clipper);

		//�ػ棬ͨ��ʵ��������������ֿռ�����
		virtual void RenderSelf( const xgcPoint& adjust );
		virtual void Update( float timer, float delta );

		//���
		virtual void Validate();

		// ����,����
		virtual void OnSavePropertys( const char* name, TiXmlElement* pNode );
		virtual void OnLoadPropertys( const char* name, TiXmlElement* pNode );

		//virtual void OnDestroy();
		//���
		//����ƶ��¼�
		//����˵����
		//pt���������꣬����ڿؼ�
		//sysKeys��������Ҫ������״̬���μ�MSDN	
		virtual	bool onMouseMove(const xgcPoint& pt, UINT sysKeys);
		virtual bool onMouseEnter();
		virtual bool onMouseLeave();

		//��갴���¼�
		//����˵����
		//button�����µļ���0-�����1-�Ҽ���2-�м�
		//pt����������
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual	bool onButtonDown(int button, const xgcPoint& pt, UINT sysKeys);
		virtual	bool onButtonUp(int button, const xgcPoint& pt, UINT sysKeys);

		//����
		//����˵��
		//keycode�����µļ�
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual	bool onKeyDown(_uint32 keycode, UINT sysKeys);
		virtual	bool onKeyUp(_uint32 keycode, UINT sysKeys);
		//����˵��
		//c��������ַ�
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual bool onChar(_uint32 c, UINT sysKeys);

		//���뷨
		//�μ�MSDN
		virtual bool onImeComp(_uint32 wParam, _uint32 lParam);
		virtual bool onImeEndComp(_uint32 wParam, _uint32 lParam);
		virtual bool onImeNotify(_uint32 wParam, _uint32 lParam);

		//��ý���
		virtual void onGetFocus() {}
		//ʧȥ����
		virtual void onLostFocus() {}

		virtual unsigned int OnMoveWindow( xgcRect& rcWindow ){ return 0; }
	public:
		virtual _lpctstr GetLable()const = 0;

		// ������Ϣ
		long_ptr SendUIMessage( _uint32 nMsg, uint_ptr wParam, long_ptr lParam );

		void Release();

		//����ת��
		void ScreenToClient( xgcPoint& pt) const;
		void ClientToScreen( xgcPoint& pt) const;
		void ScreenToClient( xgcRect& rc ) const;
		void ClientToScreen( xgcRect& rc ) const;

		//��������������ؼ����Թ����������Ҫ��������
		virtual void AdjustPoint( xgcPoint& pt, bool bClientToScreen )const { if( m_pParent ) m_pParent->AdjustPoint( pt, bClientToScreen ); }
		virtual void AdjustWindow( xgcRect& rc, bool bClientToScreen )const { if( m_pParent ) m_pParent->AdjustWindow( rc, bClientToScreen ); }

		//��ȡ�ռ����ʾ����
		const xgcRect& GetWindowRect()const { return m_WindowRect; }
		const xgcPoint& GetTopLeft()const{ return m_WindowRect.TopLeft(); }

		//--------------------------------------------------------------------------
		//��ι�ϵ
		//--------------------------------------------------------------------------
		int  FindChild(XUI_Wnd* pElement) const;
		void AddChild(XUI_Wnd* pElement );
		void AddChildAt(XUI_Wnd* pElement, int x, int y);
		void RemoveChild(XUI_Wnd* pElement, bool bDestroy = true );
		void DestroyAllChild();
		void SetParent( XUI_Wnd* pParent ){ m_pParent = pParent; }
		XUI_Wnd* GetParent(){return m_pParent;}
		XUI_Wnd* FindChildInPoint( const xgcPoint &pt, _uint32* deep = NULL );
		XUI_Wnd* FindChildByName( const _string& sName )const;
		XUI_Wnd* FindChildByID( int nID )const;

		typedef bool (*ElementCallback)( XUI_Wnd*, void* );
		XUI_Wnd* ForAllChild( ElementCallback pfnCallback, void* pData );
		size_t GetChildCount()const{ return m_pChildren.size(); }

		//--------------------------------------------------------------------------
		// ���ڴ�С���ƶ��͵���
		//--------------------------------------------------------------------------
		
		// �������ÿռ����ʾ����
		void MoveWindow(int left, int top, int right, int bottom, bool notify = true );
		//ƽ��
		void Offset(int x, int y);

		void BringToUp();
		void BringToDown();
		void BringToFront();
		void BringToEnd();

		//���ĳһ���Ƿ��ڿؼ�����ʾ������
		virtual BOOL IsPointIn(const xgcPoint& pt);

		//--------------------------------------------------------------------------
		// ���ÿؼ�����
		//--------------------------------------------------------------------------
		
		//����
		void	SetName( const _string& lpszName)	{ m_strName = lpszName;	}
		const _string& GetName()const {return m_strName;}

		//��ʾ
		bool	IsVisible()const {return m_bVisible;}
		void	ShowWindow( bool bVisible = true );

		bool	IsEnable()const { return m_bEnable; }
		void	EnableWindow( bool bEnable );

		//���ý���
		void	SetFocus(bool bFocused);

		//---------------------------------------------------------------------//
		// describe	: ���桢����
		// return	:
		//---------------------------------------------------------------------//
		bool load_file( TiXmlElement* pNode );
		bool save_file( TiXmlElement* pNode );
		void show_members( int indent );

	//--------------------------------------------------------------------------
	// �ؼ�����
	//--------------------------------------------------------------------------
	protected:
		typedef std::vector<XUI_Wnd*>	CChildList;
		XUI_Wnd*		m_pParent;			// ���ؼ�ָ��
		CChildList		m_pChildren;		// �ӿؼ��б�����һ���ǰ

		//���ƣ����ڲ���
		_string			m_strName;
		std::string		m_strUpdateFunc;

		bool			m_bVisible;			// �Ƿ�ɼ�
		bool			m_bEnable;			// �Ƿ���Ч
		bool			m_bOwnerDraw;		// �Ƿ��Ի���

		bool			m_bFocused;			// �Ƿ��ȡ����
		bool			m_bTranslateParent;	// �Ƿ�WM_COMMAND��Ϣ�������ؼ�

		float			m_fZ;
		xgcRect			m_WindowRect;		// λ�úʹ�С������ڸ��ؼ�

		XUI_IFont*		m_pFont;
		XUI_ISprite*	m_pBackGround;			// ����ͼ

		//XUI_Wnd*		m_pChildMouseOver;	// ��ǰ����µ��ӿؼ�
		//XUI_Wnd*		m_pChildFocusedOn;	// ��ǰ��ý�����ӿؼ�
	};

	extern 	CGuiSystem* _afxCurrentGuiSystem;

	typedef struct DrawStruct
	{
		ICanvas*	pCanvas;
		xgcRect		rcClient;
		xgcRect		rcClipper;
		XUI_Wnd*	pCtrl;
		LPARAM		lParam1;
		LPARAM		lParam2;
	}*LPDRAWSTRUCT, DRAWSTRUCT;
};
#endif
