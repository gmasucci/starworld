#pragma once
#include "XUI_Wnd.h"

namespace UILib
{
	class XUI_EditBox :	public UIObjTypeT< XUI_Wnd, TypeEditBox >
	{
		friend class LuaBindClass;
		DECLARE_UIMSG_MAP()
		DECLARE_LABLE( EDIT )
	public:
		XUI_EditBox(void);
		~XUI_EditBox(void);

		//�ػ棬ͨ��ʵ��������������ֿռ�����
		virtual void RenderSelf(const CRect& clipper);

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

		virtual unsigned int OnMoveWindow( CRect& rcWindow );

	protected:
		void RenderCharacter( TCHAR szChar, XUI_IFont* pFont, LONG &x, LONG &y, BOOL bRender );
		void DeleteCharacter( size_t nPos );
		bool CaratTimerUpdate( unsigned int handle, unsigned short& repeat, unsigned int& timer );

		void HandleBack( UINT nSysKey );
		void HandleDelete( UINT nSysKey );
		void HandleHome( UINT nSysKey );
		void HandleEnd( UINT nSysKey );
		void HandleWordLeft( UINT nSysKey );
		void HandleCharLeft( UINT nSysKey );
		void HandleWordRight( UINT nSysKey );
		void HandleCharRight( UINT nSysKey );
		void HandleReturn( UINT nSysKey );

		//---------------------------------------------------------------------//
		// describe	: �����ַ�λ�û���к�
		// nPos		: �ַ�����
		// return	: �к�
		//---------------------------------------------------------------------//
		size_t GetLineFromCharaterPos( size_t nPos )const;

	protected:
		// Attribute
		_string			m_strText;			// �༭������
		bool			m_bWarpText;		// ���б�־

	private:
		typedef _string::size_type Position;
		typedef	std::deque< Position >	line_recorder;

		Position		m_FirstVisiblePos;	// �༭���￴���ĵ�һ���ַ���λ��
		Position		m_CaratPos;			// ���λ��
		line_recorder	m_LineRecorder;		// ���з�λ���б�

		bool	m_bControl, m_bShift;
		CSize	m_WindowSize;				// �Ӵ���С

		struct line_info
		{
			size_t	count;		// �ַ���
			size_t	width;		// ռ�ÿ��
			size_t	tag;		// ��һ���ַ��ڴ��е�������
		};

		unsigned int	m_CaratTimerHandler;
		bool			m_bShowCarat;
	};
}
