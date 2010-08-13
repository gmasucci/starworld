#ifndef XUI_Wnd_H
#define XUI_Wnd_H
#pragma once

#include "XUI_Object.h"
#include "XUI_Canvas.h"

namespace XGC
{
	namespace ui
	{
		struct XUI_Font;
		class XUI_Wnd	:	public XUI_Object
		{
			friend class XUI_System;
			friend class XUI_Factory;
			friend class XUI_Lua;
		public:
			enum GUI_ELEMENT_FLAG
			{
				FLAGS_EDIT = 0,
			};

		protected:
			XUI_Wnd(void);
			virtual ~XUI_Wnd(void);

			void Destroy();
		protected:
			//����
			void Render(const iRect& AreaRect );
			void RenderEdit( const iPoint &adjust );

			//�ػ棬ͨ��ʵ��������������ֿռ�����
			virtual void RenderSelf( const iPoint& adjust );
			virtual void Update( float timer, float delta );

			// ����,����
			virtual void OnSavePropertys( const char* name, TiXmlElement* pNode );
			virtual void OnLoadPropertys( const char* name, TiXmlElement* pNode );

			virtual	bool OnMouseMove(const iPoint& pt, _uint32 sysKeys){ return false; }
			virtual bool OnMouseEnter(){ return false; }
			virtual bool OnMouseLeave(){ return false; }

			//��갴���¼�
			//����˵����
			//button�����µļ���0-�����1-�Ҽ���2-�м�
			//pt����������
			//sysKeys��������Ҫ������״̬���μ�MSDN
			virtual	bool OnButtonDown(int button, const iPoint& pt, _uint32 sysKeys){ return false; }
			virtual	bool OnButtonUp(int button, const iPoint& pt, _uint32 sysKeys){ return false; }

			//����
			//����˵��
			//keycode�����µļ�
			//sysKeys��������Ҫ������״̬���μ�MSDN
			virtual	bool OnKeyDown(_uint32 keycode, _uint32 sysKeys){ return false; }
			virtual	bool OnKeyUp(_uint32 keycode, _uint32 sysKeys){ return false; }
			//����˵��
			//c��������ַ�
			//sysKeys��������Ҫ������״̬���μ�MSDN
			virtual bool OnChar(_uint32 c, _uint32 sysKeys){ return false; }

			//���뷨
			//�μ�MSDN
			virtual bool OnImeComp(_uint32 wParam, _uint32 lParam){ return false; }
			virtual bool OnImeEndComp(_uint32 wParam, _uint32 lParam){ return false; }
			virtual bool OnImeNotify(_uint32 wParam, _uint32 lParam){ return false; }

			//��ý���
			virtual void OnSetFocus() {}
			//ʧȥ����
			virtual void OnLostFocus() {}

			virtual void OnMoveWindow( const iRect& rcWindow ){}
		public:
			void Release();

			//����ת��
			void ScreenToClient( iPoint& pt) const;
			void ClientToScreen( iPoint& pt) const;
			void ScreenToClient( iRect& rc ) const;
			void ClientToScreen( iRect& rc ) const;

			//��������������ؼ����Թ����������Ҫ��������
			virtual void AdjustPoint( iPoint& pt, bool bClientToScreen )const { if( m_pParent ) m_pParent->AdjustPoint( pt, bClientToScreen ); }
			virtual void AdjustWindow( iRect& rc, bool bClientToScreen )const { if( m_pParent ) m_pParent->AdjustWindow( rc, bClientToScreen ); }

			//��ȡ�ռ����ʾ����
			const iRect		GetWindowRect()const { return iRect( m_WindowPosition, m_WindowSize ); }
			const iPoint	GetWindowPosition()const{ return m_WindowPosition; }
			const iSize		GetWindowSize()const{ return m_WindowSize; }

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
			XUI_Wnd* FindChildInPoint( const iPoint &pt );

			//--------------------------------------------------------//
			//	created:	19:11:2009   18:29
			//	filename: 	d:\Develop\StarGame\GameSDK\GameUILib\Source\XUI_Wnd.cpp
			//	author:		Albert.xu
			//
			//	purpose:	�����ܰ����������ε�����Wnd
			//--------------------------------------------------------//
			bool FindRectIn( const iRect &rc, std::list< XUI_Wnd* >& l );
			XUI_Wnd* FindChildByName( const _string& sName )const;
			XUI_Wnd* FindChildByID( int nID )const;

			typedef bool (*ElementCallback)( XUI_Wnd*, void* );
			XUI_Wnd* ForAllChild( ElementCallback pfnCallback, void* pData );
			_uint32 GetChildCount()const{ return m_pChildren.size(); }

			//--------------------------------------------------------------------------
			// ���ڴ�С���ƶ��͵���
			//--------------------------------------------------------------------------

			// �������ÿռ����ʾ����
			void MoveWindow(int left, int top, int right, int bottom, bool notify = true );
			//ƽ��
			void Offset( int x, int y );

			void BringToUp();
			void BringToDown();
			void BringToFront();
			void BringToEnd();

			//���ĳһ���Ƿ��ڿؼ�����ʾ������
			virtual bool IsPointIn(const iPoint& pt);

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

			//--------------------------------------------------------//
			//	created:	25:11:2009   10:48
			//	filename: 	d:\Developed\StarGame\GameSDK\GameUILib\Include\XUI_Wnd.h
			//	author:		Albert.xu
			//
			//	purpose:	��־����
			//--------------------------------------------------------//
			void SetFlags( _uint16 nFlag );	// ���ñ�־
			void ClrFlags( _uint16 nFlag );	// �����־
			bool GetFlags( _uint16 nFlag )const; // ��ȡ��־

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
			iPoint			m_WindowPosition;		// λ�úʹ�С������ڸ��ؼ�
			iSize			m_WindowSize;

			XUI_Font*		m_pFont;
			XUI_ISprite*	m_pBackGround;			// ����ͼ
			_byte			m_Flags[1];				// ��־
		};

		extern 	XUI_System* _afxCurrentGuiSystem;
	}
};
#endif
