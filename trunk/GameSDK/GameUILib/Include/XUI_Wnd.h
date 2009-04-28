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
		void Render(const x_rect& clipper);

		//�ػ棬ͨ��ʵ��������������ֿռ�����
		virtual void RenderSelf();
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
		virtual	bool onMouseMove(const x_point& pt, UINT sysKeys);
		virtual bool onMouseEnter();
		virtual bool onMouseLeave();

		//��갴���¼�
		//����˵����
		//button�����µļ���0-�����1-�Ҽ���2-�м�
		//pt����������
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual	bool onButtonDown(int button, const x_point& pt, UINT sysKeys);
		virtual	bool onButtonUp(int button, const x_point& pt, UINT sysKeys);

		//����
		//����˵��
		//keycode�����µļ�
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual	bool onKeyDown(uint32 keycode, UINT sysKeys);
		virtual	bool onKeyUp(uint32 keycode, UINT sysKeys);
		//����˵��
		//c��������ַ�
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual bool onChar(uint32 c, UINT sysKeys);

		//���뷨
		//�μ�MSDN
		virtual bool onImeComp(uint32 wParam, uint32 lParam);
		virtual bool onImeEndComp(uint32 wParam, uint32 lParam);
		virtual bool onImeNotify(uint32 wParam, uint32 lParam);

		//��ý���
		virtual void onGetFocus() {}
		//ʧȥ����
		virtual void onLostFocus() {}

		virtual unsigned int OnMoveWindow( x_rect& rcWindow ){ return 0; }
	public:
		virtual _lpctstr GetLable()const = 0;

		// ������Ϣ
		long_ptr SendUIMessage( uint32 nMsg, uint_ptr wParam, long_ptr lParam );

		void Release();

		//����ת��
		x_point ScreenToClient(const x_point& pt) const;
		x_point ClientToScreen(const x_point& pt) const;

		//��������������ؼ����Թ����������Ҫ��������
		virtual x_point AdjustPoint(const x_point& pt, bool bClientToScreen) const {return pt;}

		//��ȡ�ռ����ʾ����
		const x_rect& GetWindowRect()const { return m_WindowRect; }

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
		XUI_Wnd* FindChildInPoint(const x_point& pt) const;
		XUI_Wnd* FindChildByName(_lpctstr sName) const;
		XUI_Wnd* FindChildByID( int nID )const;

		typedef bool (*ElementCallback)( XUI_Wnd*, void* );
		XUI_Wnd* ForAllChild( ElementCallback pfnCallback, void* pData );
		size_t GetChildCount()const{ return m_pChildren.size(); }

		//--------------------------------------------------------------------------
		// ���ڴ�С���ƶ��͵���
		//--------------------------------------------------------------------------
		
		// �������ÿռ����ʾ����
		void MoveWindow(int left, int top, int right, int bottom);
		//ƽ��
		void Offset(int x, int y);

		void BringToUp();
		void BringToDown();
		void BringToFront();
		void BringToEnd();

		//���ĳһ���Ƿ��ڿؼ�����ʾ������
		virtual BOOL IsPointIn(const x_point& pt);

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

		// ����
		XUI_FontAttribute& GetFontAttribute()const{ return m_FontAttribute; }
		void SetFontAttribute( const XUI_FontAttribute& Font );

		// ����
		XUI_SpriteAttribute& GetBackgroundAttribute()const{ return m_BackgroundAttribute; }
		void SetBackgroundAttribute( const XUI_SpriteAttribute& Background );
		XUI_ISprite* GetBackground()const{ return m_pBackGround; }

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
		x_rect			m_WindowRect;		// λ�úʹ�С������ڸ��ؼ�

		mutable XUI_FontAttribute	m_FontAttribute;	// ������Ϣ
		mutable XUI_SpriteAttribute	m_BackgroundAttribute;	// ������Ϣ
		XUI_IFont*		m_pFont;
		XUI_ISprite*	m_pBackGround;			// ����ͼ

		XUI_Wnd*		m_pChildMouseOver;	// ��ǰ����µ��ӿؼ�
		XUI_Wnd*		m_pChildFocusedOn;	// ��ǰ��ý�����ӿؼ�
	};

	extern 	CGuiSystem* _afxCurrentGuiSystem;

	typedef struct DrawStruct
	{
		ICanvas*	pCanvas;
		x_rect		rcClient;
		x_rect		rcClipper;
		XUI_Wnd*	pCtrl;
		LPARAM		lParam1;
		LPARAM		lParam2;
	}*LPDRAWSTRUCT, DRAWSTRUCT;
};
#endif
