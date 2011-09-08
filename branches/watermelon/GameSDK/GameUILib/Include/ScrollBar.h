#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "UIElement.h"
#include "Texture.h"
#define SCROLL_BTN_DELAY 500
namespace UILib
{
	// class CPanel;
	struct NMHDR_SCROLL
	{
		NMUIHDR hdr;
		int		pos;
		int		oldpos;
	};

	class CButton;
	class CScrollBar	:	public XObjTypeT< CUIElement, TypeScroll >
	{
	DECLARE_UIMSG_MAP();
	public:
		enum	Direction{ Vertical, Horizontal };
		CScrollBar(void);
		~CScrollBar(void);

		// ������������
		void	SetRange( int nMin, int nMax );
		void	SetPosition( int nPosition );
		void	SetPageSize( int nPageSize );

		int		GetPosition(){ return m_nPosition; }
		int		GetPageSize(){ return m_nPageSize; }

		// ��������
		void	Scroll( int nDelta );

		// ���ù�������
		void		SetDirection( Direction enDirection ){ m_enDirection = enDirection; }
		Direction	GetDirection(){ return m_enDirection; }

		// ���ص��鷽��
		virtual const char* GetLable(){ return "SCROLLBAR"; }

		// �ӽڵ㴴������������
		virtual void	LoadProperty( TiXmlElement* pXMLElement );
		virtual void	SaveProperty( TiXmlElement* pXMLElement );

		// �༭���ӿ�
		virtual bool	SetProperty(const char* pszPropertyName, const char* pszValue );
		virtual bool	GetProperty(const char* pszPropertyName, char* pValueBuffer, int size );

	protected:
		// ���»���λ��
		void	UpdateThumb();
		// ����λ����Ϣ
		void	ReCalc();

		virtual bool onButtonDown(int button, const CUIPoint& pt, UINT sysKeys);
		virtual bool onMouseMove(const CUIPoint& pt, UINT sysKeys);
		virtual bool onButtonUp(int button, const CUIPoint& pt, UINT sysKeys);
		
		virtual void RenderSelf(const CUIRect& clipper);
		
		uimsg void OnSize( int nWidth, int nHeight );

	protected:
		enum	BtnState{ StateNone, StateUp, StateDown, StateThumb, StateHoldUp, StateHoldDown };

		int		m_nMin;
		int		m_nMax;
		int		m_nPosition;
		int		m_nPageSize;
		bool	m_bShowThumb;

		BtnState	m_enBtnState;
		Direction	m_enDirection;

		CUIRect		m_rcThumb;
		CUIRect		m_rcUpButton;
		CUIRect		m_rcDownButton;
		CUIRect		m_rcTrack;

		ITexture*	m_pUpTexture;
		ITexture*	m_pDownTexture;
		ITexture*	m_pThumbTexture;

	private:
		CUIPoint	m_ptLast;
		int			m_nThumbOffset;
		DWORD		m_dwBtnTime;
	};
}
#endif
