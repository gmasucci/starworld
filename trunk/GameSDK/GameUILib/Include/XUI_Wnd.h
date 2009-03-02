#ifndef XUI_Wnd_H
#define XUI_Wnd_H
#pragma once

#include "XUI_Base.h"
#define DECLARE_LABLE( lable )\
	virtual LPCTSTR GetLable()const { return _T(#lable); }\

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
		void Render(const CRect& clipper);

		//�ػ棬ͨ��ʵ��������������ֿռ�����
		virtual void RenderSelf(const CRect& clipper)=0;

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
		virtual	bool onMouseMove(const CPoint& pt, UINT sysKeys);
		virtual bool onMouseHover(const CPoint& pt);
		virtual bool onMouseEnter();
		virtual bool onMouseLeave();

		//��갴���¼�
		//����˵����
		//button�����µļ���0-�����1-�Ҽ���2-�м�
		//pt����������
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual	bool onButtonDown(int button, const CPoint& pt, UINT sysKeys);
		virtual	bool onButtonUp(int button, const CPoint& pt, UINT sysKeys);

		//����
		//����˵��
		//keycode�����µļ�
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual	bool onKeyDown(DWORD keycode, UINT sysKeys);
		virtual	bool onKeyUp(DWORD keycode, UINT sysKeys);
		//����˵��
		//c��������ַ�
		//sysKeys��������Ҫ������״̬���μ�MSDN
		virtual bool onChar(DWORD c, UINT sysKeys);

		//���뷨
		//�μ�MSDN
		virtual bool onImeComp(DWORD wParam, DWORD lParam);
		virtual bool onImeEndComp(DWORD wParam, DWORD lParam);
		virtual bool onImeNotify(DWORD wParam, DWORD lParam);

		//��ý���
		virtual void onGetFocus() {}
		//ʧȥ����
		virtual void onLostFocus() {}

		virtual unsigned int OnMoveWindow( CRect& rcWindow ){ return 0; }
		//////////////////////////////////////////////////////////////////////////
		// ��Ϣ����

		virtual bool OnWndMsg( UINT nMsg, WPARAM wParam, LPARAM lParam );
		virtual bool OnCommand( WPARAM wParam, LPARAM lParam );
		virtual bool OnNotify( WPARAM wParam, LPARAM lParam, HRESULT* lResult );
	public:
		virtual LPCTSTR GetLable()const = 0;

		// ������Ϣ
		unsigned int SendMessage( UINT nMsg, WPARAM wParam, LPARAM lParam );

		void Release();

		//����ת��
		CPoint ScreenToClient(const CPoint& pt) const;
		CPoint ClientToScreen(const CPoint& pt) const;

		//��������������ؼ����Թ����������Ҫ��������
		virtual CPoint AdjustPoint(const CPoint& pt, bool bClientToScreen) const {return pt;}

		//��ȡ�ռ����ʾ����
		const CRect& GetWindowRect()const { return m_wndRect; }

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
		XUI_Wnd* FindChildInPoint(const CPoint& pt) const;
		XUI_Wnd* FindChildByName(LPCTSTR sName) const;
		XUI_Wnd* FindChildByID( int nID )const;

		typedef bool (*ElementCallback)( XUI_Wnd*, void* );
		XUI_Wnd* ForAllChild( ElementCallback pfnCallback, void* pData );
		size_t GetChildCount()const{ return m_pChildren.size(); }

		//--------------------------------------------------------------------------
		// ���ڴ�С���ƶ��͵���
		//--------------------------------------------------------------------------
		
		// �������ÿռ����ʾ����
		void Move(int left, int top, int right, int bottom);
		//ƽ��
		void Offset(int x, int y);

		void BringToUp();
		void BringToDown();
		void BringToFront();
		void BringToEnd();

		//���ĳһ���Ƿ��ڿؼ�����ʾ������
		virtual BOOL IsPointIn(const CPoint& pt);

		//--------------------------------------------------------------------------
		// ���ÿؼ�����
		//--------------------------------------------------------------------------
		
		//����
		void	SetName( const _string& lpszName)	{ m_strName = lpszName;	}
		const _string& GetName()const {return m_strName;}

		void	SetID( UINT nID ){ m_nID = nID; }
		UINT	GetID(){ return m_nID; }

		//��ʾ
		bool	IsVisible() {return m_bVisible;}
		void	Show( bool bVisible = true );

		bool	IsEnable() { return m_bEnable; }
		void	Enable( bool bEnable );

		//���ý���
		void	SetFocus(bool bFocused);

		//---------------------------------------------------------------------//
		// describe	: ���桢����
		// return	:
		//---------------------------------------------------------------------//
		bool load_file( TiXmlElement* pNode );
		bool save_file( TiXmlElement* pNode );

	//--------------------------------------------------------------------------
	// �ؼ�����
	//--------------------------------------------------------------------------
	protected:
		typedef std::vector<XUI_Wnd*>	CChildList;
		XUI_Wnd*		m_pParent;			// ���ؼ�ָ��
		CChildList		m_pChildren;		// �ӿؼ��б�����һ���ǰ

		//���ƣ����ڲ���
		_string			m_strName;
		UINT			m_nID;				// �ؼ�ID������Ϣӳ��

		bool			m_bVisible;			// �Ƿ�ɼ�
		bool			m_bEnable;			// �Ƿ���Ч
		bool			m_bOwnerDraw;		// �Ƿ��Ի���

		bool			m_bFocused;			// �Ƿ��ȡ����
		bool			m_bTranslateParent;	// �Ƿ�WM_COMMAND��Ϣ�������ؼ�

		float			m_fZ;
		CRect			m_wndRect;			// λ�úʹ�С������ڸ��ؼ�

		FontAttribute	m_FontAttribute;	// ������Ϣ
		XUI_IFont*		m_pFont;

		XUI_Wnd*		m_pChildMouseOver;	// ��ǰ����µ��ӿؼ�
		XUI_Wnd*		m_pChildFocusedOn;	// ��ǰ��ý�����ӿؼ�
	};

	extern 	CGuiSystem* _afxCurrentGuiSystem;

	typedef struct DrawStruct
	{
		ICanvas*	pCanvas;
		CRect		rcClient;
		CRect		rcClipper;
		XUI_Wnd*	pCtrl;
		LPARAM		lParam1;
		LPARAM		lParam2;
	}*LPDRAWSTRUCT, DRAWSTRUCT;
};
#endif
