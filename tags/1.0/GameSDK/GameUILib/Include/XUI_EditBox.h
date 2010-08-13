#pragma once
#include "XUI_Wnd.h"
#define LINE_MAX_CHAR (32)
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
		virtual void RenderSelf( const xgcPoint& adjust );

		void SetText( const std::string &t );
		std::string GetText()const;

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
		virtual void onGetFocus();
		//ʧȥ����
		virtual void onLostFocus();

		virtual unsigned int OnMoveWindow( xgcRect& rcWindow );

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
		struct line
		{
			line( const _tchar * string, end_type t = type_n )
				: cursor_position( 0 )
				, select_begin( 0 )
				, select_end( 0 )
				, type( t )
				, string_( (_tchar*)malloc(LINE_MAX_CHAR*sizeof(_tchar)) )
				, size_(0)
				, capacity_(LINE_MAX_CHAR)
			{
				memset( string_, 0, sizeof(_tchar)*LINE_MAX_CHAR);
				append( string );
			}

			line( const _tchar *string, size_t beg, size_t end, end_type t = type_n )
				: cursor_position( 0 )
				, select_begin( 0 )
				, select_end( 0 )
				, type( t )
				, string_( (_tchar*)malloc(LINE_MAX_CHAR*sizeof(_tchar)) )
				, size_(0)
				, capacity_(LINE_MAX_CHAR)
			{
				memset( string_, 0, sizeof(_tchar)*LINE_MAX_CHAR);
				erase(0, -1);
				insert( 0, string + beg, string + end );
			}

			line( const line& rsh )
				: cursor_position( rsh.cursor_position )
				, select_begin( rsh.select_begin )
				, select_end( rsh.select_end )
				, type( rsh.type )
				, string_( (_tchar*)malloc(LINE_MAX_CHAR*sizeof(_tchar)) )
				, size_( rsh.size_ )
				, capacity_(rsh.capacity_)
			{
				memset( string_, 0, sizeof(_tchar)*LINE_MAX_CHAR);
				memcpy( string_, rsh.string_, size_*sizeof(_tchar) );
			}

			~line()
			{
				free(string_);
			}

			line& operator=( line&& rsh )
			{
				cursor_position = rsh.cursor_position;
				select_begin = rsh.select_begin;
				select_end = rsh.select_end;
				type = rsh.type;
				size_ = rsh.size_;
				capacity_ = rsh.capacity_;
				memcpy( string_, rsh.string_, size_*sizeof(_tchar) );

				return *this;
			}

			size_t size()const
			{
				return size_;
			}

			void insert( size_t beg, const _tchar* str )
			{
				size_t len = _tcslen( str );
				if( size_ + len > capacity_ )
				{
					string_ = (_tchar*)realloc( string_, (size_+len+1)*sizeof(_tchar) );
					capacity_ = (size_+len+1);
				}

				memmove( string_ + beg + len, string_ + beg, len*sizeof(_tchar) );
				_tcsncpy( string_ + beg, str, len );
				size_ += len;
			}

			void insert( size_t beg, int count, _tchar ch )
			{
				size_t len = count;
				if( size_ + len > capacity_ )
				{
					string_ = (_tchar*)realloc( string_, (size_+len+1)*sizeof(_tchar) );
					capacity_ = (size_+len+1);
				}

				memmove( string_ + beg + len, string_ + beg, (size_ - beg - count + 1)*sizeof(_tchar) );
				while(count)
				{
					*(string_ + beg + --count) = ch;
				}
				size_ += len;
			}

			void insert( size_t cur, const _tchar* beg, const _tchar* end )
			{
				if( end < beg )
					std::swap( end, beg );

				size_t len = end - beg;
				if( size_ + len > capacity_ )
				{
					string_ = (_tchar*)realloc( string_, (size_+len+1)*sizeof(_tchar) );
					capacity_ = (size_+len+1);
				}

				memmove( string_ + cur + len, string_ + cur, len*sizeof(_tchar) );
				_tcsncpy( string_ + cur, beg, len );
				size_ += len;
			}

			void append( const _tchar *str )
			{
				int len = _tcslen(str);
				if( size_ + len >= capacity_ )
				{
					string_ = (_tchar*)realloc( string_, (size_+len+1)*sizeof(_tchar) );
					capacity_ = (size_+len+1);
				}
				_tcsncat_s( string_, capacity_, str, _TRUNCATE );
				size_ += len;
			}

			const _tchar* c_str()const
			{
				return string_;
			}

			operator const _tchar*()const
			{
				return string_;
			}

			line subline( size_t beg, size_t end )
			{
				if( end < beg )
					std::swap( end, beg );
				if( beg > size_ ) 
					beg = size_;
				if( end < beg )
					end = beg;
				if( end > size_ )
					end = size_;
				return line( string_, beg, end );
			}

			void erase( size_t beg, size_t count )
			{
				memmove( string_ + beg, string_ + beg + count, (size_ - beg - count + 1)*sizeof(_tchar) );
				size_ -= count;
			}

			const _tchar* begin()const
			{
				return string_;
			}

			const _tchar* end()const
			{
				return string_ + size_;
			}

			_tchar*		string_;
			size_t		size_;
			size_t		capacity_;

			size_t		cursor_position;
			size_t		select_begin;
			size_t		select_end;
			end_type	type;
		};

		typedef _string::size_type Position;
		typedef	std::vector< line >	text;
		bool			m_bSingleLine;
		size_t			m_FirstLineNumber;	// �༭���￴���ĵ�һ���ַ���λ��
		size_t			m_nCurLineNumber;	// ��ǰ������
		size_t			m_CaratPos;			// ���λ��
		text			m_text;				// �м�¼
		_uint32			m_dwBorderColor, m_dwBackgroundColor;

		bool			m_bControl, m_bShift;
		xgcSize			m_TextBufferSize;				// �ַ������С

		unsigned int	m_CaratTimerHandler;
		bool			m_bShowCarat;
	};
}
