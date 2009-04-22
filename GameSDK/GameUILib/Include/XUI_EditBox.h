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
		virtual void RenderSelf();

		void SetText( const std::string &t );
		std::string GetText()const;

		//���
		//����ƶ��¼�
		//����˵����
		//pt���������꣬����ڿؼ�
		//sysKeys��������Ҫ������״̬���μ�MSDN	
		virtual	bool onMouseMove(const x_point& pt, UINT sysKeys);
		virtual bool onMouseHover(const x_point& pt);
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
		virtual void onGetFocus();
		//ʧȥ����
		virtual void onLostFocus();

		virtual unsigned int OnMoveWindow( x_rect& rcWindow );

		// virtual LRESULT OnWndMsg( UINT nMsg, WPARAM wParam, LPARAM lParam );
	protected:
		void RenderCharacter( _tchar szChar, XUI_IFont* pFont, LONG &x, LONG &y, BOOL bRender );
		void DeleteCharacter( size_t nLine, size_t nPos, size_t nCount );
		bool CaratTimerUpdate( unsigned int handle, unsigned short& repeat, unsigned int& timer );
		size_t NaturalLine( size_t nLine );

		void HandleBack();
		void HandleDelete();
		void HandleHome();
		void HandleEnd();
		void HandleWordLeft();
		void HandleCharLeft();
		void HandleWordRight();
		void HandleCharRight();
		void HandleReturn();
		void HandleLineUp();
		void HandleLineDown();

		//---------------------------------------------------------------------//
		// describe	: ������ǰ����ˢ��LineRecorder����
		//---------------------------------------------------------------------//
		void Analyse();

		//---------------------------------------------------------------------//
		// describe	: ���õ�ǰ��
		// line		: ������
		//---------------------------------------------------------------------//
		void SetCurLineNumber( size_t line );
		void SendCompString();

	protected:
		// Attribute
		bool			m_bWarpText;		// ���б�־

	private:
		enum end_type{ type_n, type_r }; // �������� n - Ӳ�س� r - ��س�
		struct line	:	public _string
		{
			line( const _tchar * string, end_type t = type_n )
				: cursor_position( 0 )
				, select_begin( 0 )
				, select_end( 0 )
				, type( t )
			{
				assign( string );
			}

			line( const _string& string, end_type t = type_n )
				: cursor_position( 0 )
				, select_begin( 0 )
				, select_end( 0 )
				, type( t )
			{
				assign( string );
			}

			size_t		cursor_position;
			size_t		select_begin;
			size_t		select_end;
			end_type	type;
		};

		typedef _string::size_type Position;
		typedef	std::vector< line >	text;
		size_t			m_FirstLineNumber;	// �༭���￴���ĵ�һ���ַ���λ��
		size_t			m_nCurLineNumber;	// ��ǰ������
		size_t			m_CaratPos;			// ���λ��
		text			m_text;				// �м�¼
		uint32			m_dwBorderColor, m_dwBackgroundColor;

		bool	m_bControl, m_bShift;
		x_size	m_WindowSize;				// �Ӵ���С

		unsigned int	m_CaratTimerHandler;
		bool			m_bShowCarat;
	};
}
