//#pragma once
//
//#include "UIElement.h"
//namespace UILib
//{
//	//��ʾ�༭������ʱ�Ĵ�Сд���
//	typedef enum TEditCharCases
//	{
//		XTEC_NORMAL = 0,		
//		XTEC_UPPERCASE,			
//		XTEC_LOWERCASE
//	};
//
//	class CEdit : public XObjTypeT< CUIElement, TypeEdit >
//	{
//	public:
//		CEdit(void);
//	
//		LPCTSTR	GetText() const { return m_strText.c_str(); }
//		void	SetText( LPCTSTR lpszText );
//		
//		int		GetMaxLength() const {return m_nMaxTextLength;};
//		void	SetMaxLength(int nLength){m_nMaxTextLength = nLength;};
//
//		int		GetBorderLength() const {return m_nBorderLength;};
//		void	SetBorderLength(int nLength){m_nBorderLength = nLength;};
//
//		bool	GetPWDStyle() const {return m_bPassword;};
//		void	SetPWDStyle(bool bValue){m_bPassword = bValue;};		 
//
//		LPCTSTR	GetPasswordChar() const {return m_strPasswordChar.c_str();}
//		void	SetPasswordChar(LPCTSTR strValue){m_strPasswordChar = strValue;}
//
//		DWORD	GetCaretColor() const {return m_dwSelectColor;};
//		void	SetCaretColor(DWORD dwValue){m_dwSelectColor = dwValue;};
//
//		DWORD	GetCursorColor() const {return m_dwCursorColor;};
//		void	SetCursortColor(DWORD dwValue){m_dwCursorColor = dwValue;};
//
//		bool	GetReadOnly() const {return m_bReadOnly;};
//		void	SetReadOnly(bool bValue){m_bReadOnly = bValue;};
//
//		bool	GetMultiLines() const {return m_bMultiLines;};
//		void	SetMultiLines(bool bValue){m_bMultiLines = bValue;};
//
//		static int	GetLeftOffset() {return m_sLeftOffset;};
//		static void SetLeftOffset(int nOffset){ m_sLeftOffset = nOffset;};
//		
//		void Clear();
//
//	protected:
//		virtual ~CEdit(void);
//
//		virtual void RenderSelf(const CUIRect& clipper);
//		virtual bool onChar(DWORD c, UINT sysKeys);
//		virtual bool onKeyDown(DWORD keycode, UINT sysKeys);
//
//		virtual	bool onButtonDown(int button, const CUIPoint& pt, UINT sysKeys);
//		virtual	bool onButtonUp(int button, const CUIPoint& pt, UINT sysKeys);
//		virtual	bool onMouseMove(const CUIPoint& pt, UINT sysKeys);
//		//virtual bool onMouseHover(const CUIPoint& pt);
//		//virtual bool onMouseEnter();
//		//virtual bool onMouseLeave();
//
//		virtual void OnEscape(void);   // Escape
//		virtual void OnEntry(void);    // �س���Ӧ
//		virtual void OnLeft(void);     // <-����Ӧ
//		virtual void OnRight(void);    // ->����Ӧ
//		virtual void OnDelete(void);   // Del����Ӧ
//		virtual void OnBack(void);     // Back����Ӧ
//		virtual void OnUp(void);		//VK_UP����Ӧ
//		virtual void OnDown(void);      //VK_DOWN����Ӧ
//		virtual void OnHome(void);     // Home����Ӧ
//		virtual void OnEnd(void);      // End����Ӧ
//		virtual void OnSelectAll(void); // ȫѡ
//
//		virtual void OnFocusIn();	//��ȡ���̽��㣬�����Password�༭����ôDisableIME
//		virtual void OnFocusOut();	//ʧȥ���̽��㣬a.ʧȥѡ��, b.EnablePrevIME��
//
//		//�ؼ���ǩ
//		virtual LPCTSTR GetLable(){ return _T("EDITBOX"); }
//
//		void DeleteSelectionText();		
//		void CopyToClipboard();
//		void PasteFromClipboard();
//
//		bool TestNotOverMaxLen(const std::wstring& wstrSingleText);
//
//		//�������λ��,��ȡm_strText�й����ʼλ��; 
//		int ParseCaretByMouse(const POINT& ptMouse);	
//
//		TString GetVisibleText();
//		TString GetVisibleTextReverse(int nReversePos,int* pnFirstVisible);
//		void	RefreshVisibleTextCount();
//		int		GetInputWidth();	//���ر༭�������Ŀ��,(ע:�༭���� ��һ������ ��������)
//		int		GetCharWidth( WCHAR wChar );
//		int		GetStringWidth( const std::wstring& strString );
//
//	protected:
//		TEditCharCases m_CharCase;
//
//		static int m_sLeftOffset;		//�༭����߾�
//
//		TString	m_strText;
//		TString m_strPasswordChar;
//		TString m_strSingleText;
//		TString m_strFontName;
//
//		int		m_nFirstVisible;			// ��ǰ��ʼ��ʾλ�õ�����,���WStrText
//		int		m_nVisibleCount;			//��Ⱦ�ı�= WStrText.SubStr(m_nFirstVisible,m_nFirstVisible + m_nVisibleCount)
//				
//		int		m_nCaret;					//���λ��,���strVisibleText,��Χ: 0 =< m_nCaret =< strVisibleText.length()
//		int		m_nSelStart;				//���WStrText	��Χ: 0 =< WStrText =< strVisibleText.length()
//
//		int		m_nMaxTextLength;			//������Ƴ���,-1Ϊ������(�����ڲ�Ӧ������󳤶�)
//		int		m_nBorderLength;			//�߿���
//		bool	m_bReadOnly;
//		bool	m_bMultiLines;				//�Ƿ������ʾ
//		bool	m_bPassword;
//		bool	m_bMouseDrag;
//
//
//		DWORD	m_dwSelectColor;
//		DWORD	m_dwCursorColor;
//		DWORD	m_dwTextColor;
//
//	    HANDLE	m_hOldIMC;
//	};
//}
