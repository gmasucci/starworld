//#include <atlbase.h>		//Ϊ����ʹ�ÿ��ַ��Ͷ��ֽ��ַ��໥ת���ĺ�: USES_CONVERSION,A2W,W2A
//#include <iostream>
//#include <tchar.h>
//#include <windows.h>
//#include "header.h"
//#include "UIElement.h"
//#include "EditBox.h"
//#include "Canvas.h"
//
//namespace UILib
//{
//	#define ISKEYDOWN( vk_code )	( (GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0 )
//	#define MAX_EDIT_WORDS 1024		//�����û���û������MaxLength�������ܳ����ö��峤�ȡ�
//
//	int CEdit::m_sLeftOffset = 4;
//	//-----------------------------------------------------------------------
//	std::string GL_W2A( const std::wstring& wstrValue )
//	{
//		char dest[1024];
//		WideCharToMultiByte( CP_ACP, 0, wstrValue.c_str(), -1,
//			dest, 1024, NULL, NULL );
//		std::string strTest = dest;
//		return strTest;
//	}
//
//	//-----------------------------------------------------------------------
//	std::wstring GL_A2W( const std::string& strValue )
//	{
//		int size = ( int )strValue.size() * 2 + 2;
//		wchar_t* wStr = new wchar_t[ size ];
//		MultiByteToWideChar( CP_ACP, 0, strValue.c_str(), -1, wStr, size );
//
//		std::wstring Result = wStr;
//		delete [] wStr;
//		return Result;
//	}
//
//	//-----------------------------------------------------------------------
//	CEdit::CEdit(void)
//	{
//		m_hOldIMC = NULL;
//		m_nMaxTextLength = -1;
//
//		m_bReadOnly = false;
//		m_bMultiLines = false;
//		m_bMouseDrag = false;
//		
//		m_bPassword = false;
//		m_strPasswordChar = "*";
//
//		m_nFirstVisible = 0;
//		m_nVisibleCount = 0;
//		m_nCaret = 0;
//		m_nSelStart = 0;
//
//		m_nBorderLength = 0;
//
//		m_dwSelectColor = 0xff0000e0;
//		m_dwCursorColor = 0xffffffff;
//		m_dwTextColor	= 0xffffffff;
//	}
//
//	//-----------------------------------------------------------------------
//	CEdit::~CEdit(void)
//	{
//	}	
//
//	//-----------------------------------------------------------------------
//	void CEdit::Clear()
//	{
//		m_nFirstVisible = 0;
//		m_nVisibleCount = 0;
//		m_nCaret = 0;
//		m_nSelStart = 0;
//		m_strText = "";
//	}
//
//
//	//-----------------------------------------------------------------------
//	//�����ö��ֽ��ַ�,Ȼ�������Ҫת���ɿ��ַ�
//	bool CEdit::onChar(DWORD c, UINT sysKeys)
//	{
//		WCHAR wChar = (WCHAR)c;
//		if(wChar >= 0x20 )
//		{
//			DeleteSelectionText();
//			if(m_nCaret <= (int)(GetVisibleText().length()))
//			{	
//				//ȷ����Χ�Ϸ�
//				int nInsertPos = m_nCaret + m_nFirstVisible;
//				if(nInsertPos > (int)m_strText.length())
//					return true;
//
//							
//				std::string strTmp;
//				strTmp += (CHAR)c;				
//				m_strSingleText.append(strTmp);
//
//				
//				std::wstring wstrNewSingle = GL_A2W(m_strSingleText.c_str());
//				if(wstrNewSingle.length() > 0 && TestNotOverMaxLen(wstrNewSingle))	//��ֹ���ֽ��ַ�������
//				{
//					std::wstring wstrNewText = GetWStrText();
//					wstrNewText.insert(nInsertPos,wstrNewSingle);
//					m_strText = GL_W2A(wstrNewText.c_str());					
//					m_strSingleText = "";
//					
//					++m_nCaret;
//					std::wstring wstrFirst2Cursor = wstrNewText.substr(m_nFirstVisible,m_nCaret);
//					int nWidth = GetStringWidth(wstrFirst2Cursor);
//					//���(���λ��-��ʼλ��)�ĳ��� > �༭����,��m_nFirstVisible��һ
//					if(nWidth > GetInputWidth())
//					{								
//						while (true)
//						{
//							++m_nFirstVisible;
//							--m_nCaret;
//							wstrFirst2Cursor = wstrFirst2Cursor.substr(1,wstrFirst2Cursor.length());
//							nWidth = GetStringWidth(wstrFirst2Cursor );
//							if(nWidth <= GetInputWidth())
//							{
//								break;
//							}
//						}
//					}							
//					RefreshVisibleTextCount();									
//				}
//				//ע�⵱������󳤶�ʱ���������¼���ַ���
//				if(!TestNotOverMaxLen(wstrNewSingle))
//				{
//					m_strSingleText = "";
//				}
//			}
//			m_nSelStart = m_nFirstVisible + m_nCaret;
//		}
//		return true;
//	}
//
//	//-----------------------------------------------------------------------
//	bool CEdit::onButtonDown(int button, const CUIPoint& pt, UINT sysKeys)
//	{
//		if( m_bEnable )
//		{
//			m_bMouseDrag = true;
//			//CXUIManager::Instance()->SetCurrMouseCapture(this);
//			//CXUIManager::Instance()->SetCurrKeyFocus(this);
//
//			m_nCaret = ParseCaretByMouse( pt );
//			m_nSelStart = m_nFirstVisible + m_nCaret;				
//		}
//		return __super::onButtonDown( button, pt, sysKeys );
//	}
//
//	//-----------------------------------------------------------------------
//	bool CEdit::onButtonUp(int button, const CUIPoint& pt, UINT sysKeys)
//	{
//		if( m_bEnable )
//		{
//			m_bMouseDrag = false;
//			//CXUIManager::Instance()->ReleaseCapture();
//		}
//		return __super::onButtonUp( button, pt, sysKeys );
//	}
//
//	//-----------------------------------------------------------------------
//	bool CEdit::onMouseMove(const CUIPoint& pt, UINT sysKeys)
//	{
//		if( m_bEnable && m_bMouseDrag )
//		{
//			m_nCaret = ParseCaretByMouse(pt);
//		}
//		return __super::onMouseMove( pt, sysKeys );
//	}
//
//
//	//-----------------------------------------------------------------------
//	////��ȡ���̽��㣬�����Password�༭����ôDisableIME
//	void CEdit::OnFocusIn()
//	{
//		if( m_bPassword )
//		{
//			//m_hOldIMC = ImmAssociateContext( CXUIManager::Instance()->GetHWnd(), NULL );
//		}
//	}
//
//	//-----------------------------------------------------------------------
//	//ʧȥ���̽���: a.ʧȥѡ��, b.EnablePrevIME��
//	void CEdit::OnFocusOut()
//	{
//		m_nSelStart = m_nCaret + m_nFirstVisible;
//
//		if ( m_hOldIMC )
//		{
//			//ImmAssociateContext( CXUIManager::Instance()->GetHWnd(), m_hOldIMC );
//			m_hOldIMC = NULL;   //������ΪNULL,��Ϊ�û�����������򻹿��Ե����ָ�����IME
//		}
//	}
//
//	//-----------------------------------------------------------------------
//	bool CEdit::onKeyDown( DWORD keycode, UINT sysKeys )
//	{
//		switch(keycode)
//		{
//		case VK_UP:
//			{
//				OnUp();break;
//			}
//		case VK_DOWN:
//			{
//				OnDown();break;
//			}
//		case VK_LEFT:                                       // ����"<-"��
//			{
//				OnLeft();break;
//			}
//		case VK_RIGHT:                                      // ����"->"�� 
//			{
//				OnRight();break;
//			}
//		case VK_RETURN:										// �س�����                          
//			{
//				OnEntry();break;
//			}
//		case VK_ESCAPE:
//			{
//				OnEscape();break;
//			}
//		case VK_BACK:										// BACK������
//			{
//				OnBack();break;
//			}
//		case VK_DELETE:                                     // ����Del��
//			{
//				OnDelete();break;
//			}   
//		case VK_HOME:										// ����Home��
//			{
//				OnHome();break;
//			}
//		case VK_END:                                        // ����End��
//			{  
//				OnEnd();break;
//			}
//		case 'a':                                           // ȫѡCtrl + a
//		case 'A':
//			{
//				if(ISKEYDOWN(VK_CONTROL))
//					OnSelectAll();
//				break;;
//			}
//		case 'c':                                           // ����Ctrl + c
//		case 'C':
//			{
//				if(ISKEYDOWN(VK_CONTROL))
//					CopyToClipboard();
//				break;
//			}
//		case 'x':                                           // ����Ctrl + x
//		case 'X':
//			{
//				if(ISKEYDOWN(VK_CONTROL))
//				{
//					CopyToClipboard();
//					DeleteSelectionText();
//				}	
//				break;
//			}
//		case 'v':                                           // ճ��Ctrl + v
//		case 'V':
//			{
//				if(ISKEYDOWN(VK_CONTROL))
//					PasteFromClipboard();
//				break;
//			}
//		}
//		RefreshVisibleTextCount();
//		return __super::onKeyDown( keycode, sysKeys );
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnEscape(void)
//	{
//
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnEntry()
//	{
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnLeft()
//	{
//		if(m_nCaret > 0 && m_nCaret <= m_nVisibleCount)
//			--m_nCaret;
//		else if(m_nCaret == 0 && m_nFirstVisible > 0)
//		{
//			--m_nFirstVisible;
//		}
//		m_nSelStart = m_nFirstVisible + m_nCaret;
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnRight()
//	{
//		if(m_nCaret >= 0 && m_nCaret < m_nVisibleCount)
//			++m_nCaret;
//		else if(m_nCaret ==m_nVisibleCount  && ((int)GetWStrText().length() > m_nFirstVisible + m_nVisibleCount) )
//		{
//			++m_nFirstVisible;				
//			RefreshVisibleTextCount();
//			m_nCaret = m_nVisibleCount;
//		}
//		m_nSelStart = m_nFirstVisible + m_nCaret;
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnDelete()
//	{
//		if( (m_nFirstVisible + m_nCaret) != m_nSelStart )
//		{
//			DeleteSelectionText();
//			//SendEvent( EVENT_EDITBOX_CHANGE,this );
//		}
//		else
//		{
//			// Deleting one character
//			std::wstring wstrText = GetWStrText();
//			if( (m_nFirstVisible + m_nCaret) < (int)wstrText.size() )
//			{
//				wstrText.erase(m_nFirstVisible + m_nCaret,1);
//				m_strText = GL_W2A(wstrText.c_str());
//				//SendEvent( EVENT_EDITBOX_CHANGE,this );
//			}			
//		}
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnBack()
//	{
//		if( (m_nFirstVisible + m_nCaret) != m_nSelStart )	//ɾ��ѡ������
//		{
//			DeleteSelectionText();
//			//SendEvent( EVENT_EDITBOX_CHANGE,this );
//		}
//		else if(m_nCaret > 0 && m_nCaret <= m_nVisibleCount)  //Ҫ�䶯���λ�ã����䶯��һ���ɼ��ַ�
//		{			
//			std::wstring wstrText = GetWStrText();
//			wstrText.erase(m_nFirstVisible + m_nCaret-1,1);
//			m_strText = GL_W2A(wstrText.c_str());			
//			--m_nCaret;
//			RefreshVisibleTextCount(); //���¼����ȣ�ˢ��m_bVisibleCount
//			//SendEvent( EVENT_EDITBOX_CHANGE,this );
//		}
//		else if(m_nCaret == 0 && m_nFirstVisible > 0)	//Ҫ�䶯���λ�ã��ұ䶯��һ���ɼ��ַ�
//		{
//			/*��Ϊ�ַ����ɼ�����ɾ��ֻ���ƶ��ɼ��ַ���
//			--m_nFirstVisible;
//			wstring wstrText = GetWStrText();
//			wstrText.erase(m_nFirstVisible,1);
//			USES_CONVERSION;
//			m_strText = W2A(wstrText.c_str());
//			*/
//			std::wstring wstrVisible = GetVisibleTextReverse((int)GetWStrText().length(),&m_nFirstVisible);		
//			m_nCaret = (int)wstrVisible.length();
//			RefreshVisibleTextCount(); //m_nFirstVisible�Ѿ��ı䣬��ˢ��m_bVisibleCount
//			//SendEvent( EVENT_EDITBOX_CHANGE,this );
//		}
//		m_nSelStart = m_nFirstVisible + m_nCaret;
//	}
//
//	//-----------------------------------------------------------------------
//	//������Ӧ
//	void CEdit::OnUp()
//	{
//	}
//
//	//-----------------------------------------------------------------------
//	//������Ӧ
//	void CEdit::OnDown()
//	{	
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnHome()
//	{			
//		//m_nSelStart = KEYDOWN(VK_SHIFT)? m_nFirstVisible + m_nCaret : 0;
//		m_nFirstVisible = 0;
//		m_nCaret = 0;
//		m_nSelStart = ISKEYDOWN(VK_SHIFT)? m_nSelStart : 0;
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnEnd()
//	{				
//		std::wstring wstrVisible = GetVisibleTextReverse((int)GetWStrText().length(),&m_nFirstVisible);		
//		//m_nSelStart = KEYDOWN(VK_SHIFT)? m_nCaret : m_nFirstVisible + (int)wstrVisible.length();		
//		m_nCaret = (int)wstrVisible.length();		
//		m_nSelStart = ISKEYDOWN(VK_SHIFT)? m_nSelStart : m_nFirstVisible + (int)wstrVisible.length();
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::OnSelectAll()
//	{
//		//m_nSelStart = m_nCaret = m_nFirstVisible = 0;
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::DeleteSelectionText()
//	{
//		//����ѡ�з�Χ
//		int nFirst = min( m_nFirstVisible + m_nCaret, m_nSelStart );
//		int nLast = max( m_nFirstVisible + m_nCaret, m_nSelStart );
//		
//		//ɾ��ѡ���ַ�
//		std::wstring wstrText = GetWStrText();
//		wstrText = wstrText.erase(nFirst,nLast-nFirst);
//		m_strText = GL_W2A(wstrText.c_str());
//
//		m_nSelStart = nFirst;
//		m_nCaret = nFirst - m_nFirstVisible;
//		RefreshVisibleTextCount();
//	}
//	
//	//-----------------------------------------------------------------------
//	void CEdit::CopyToClipboard()
//	{
//		int nFirst = min( m_nFirstVisible + m_nCaret, m_nSelStart );
//		int nLast = max( m_nFirstVisible + m_nCaret, m_nSelStart );
//		if( !m_bPassword && nLast - nFirst > 0 )
//		{
//			std::wstring wstrCopy = GetWStrText().substr(nFirst,nLast-nFirst);
//			std::string strCopy = GL_W2A(wstrCopy.c_str());
//			HGLOBAL hClip = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,MAX_EDIT_WORDS);
//			if(!hClip)
//				return;
//
//			char *temp = (char*)GlobalLock(hClip);
//			if(!temp)
//			{
//				GlobalUnlock(hClip);
//				return;
//			}
//
//			for(int i = 0; i< (int)strCopy.length(); ++i )
//				temp[i] = strCopy[i];
//			temp[(int)strCopy.length()] = 0;
//
//			GlobalUnlock(hClip);
//
//			if(OpenClipboard(NULL))
//			{
//				EmptyClipboard();
//				SetClipboardData( CF_TEXT,hClip);
//				CloseClipboard();
//			}
//		}				
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::PasteFromClipboard()
//	{
//		DeleteSelectionText();
//
//		if(OpenClipboard(NULL))
//		{
//			HGLOBAL hClip=NULL;
//
//			if( hClip=GetClipboardData(CF_TEXT) )
//			{
//				char *temp = (char*)GlobalLock(hClip);				
//				std::string strClipboard(temp);
//				int nMaxLength = 0;
//				for(int i=nMaxLength;i<(int)strClipboard.length();i++)
//				{
//					onChar( (DWORD)strClipboard[i],NULL );	//ֱ�ӷ���Ϣ����OnChar�����꼰��һ���ɼ��ַ�
//					if(!TestNotOverMaxLen(L""))	//��������������������ѭ������̫�ࡣ
//						break;
//				}
//				GlobalUnlock(hClip);				
//			}
//			CloseClipboard();		
//		}
//	}
//
//	//-----------------------------------------------------------------------
//	//����ʹ�ö��ֽ��ַ�����
//	bool CEdit::TestNotOverMaxLen(const std::wstring& wstrSingleText)
//	{
//		std::string strText = GL_W2A( (GetWStrText() + wstrSingleText).c_str() );
//		return  (m_nMaxTextLength != -1 && (int)strText.length() <= m_nMaxTextLength)
//			|| (m_nMaxTextLength == -1 && (int)strText.length() <= MAX_EDIT_WORDS) ;
//	}
//
//
//	//-----------------------------------------------------------------------
//	//���ؿ����ı��еĹ��λ��
//	int CEdit::ParseCaretByMouse(const POINT& ptMouse)
//	{
//		int nCaretPos = 0;
//		if(m_strText.length() == 0)
//		{
//			m_nCaret = 0;
//			m_nSelStart = 0;
//			return nCaretPos;
//		}
//
//		//ptCaret = ���λ�� - ��߾�
//		POINT ptCaret;
//		ptCaret.x = ptMouse.x - m_sLeftOffset;
//		ptCaret.y = ptMouse.y;
//
//		//��ȡ�õ�ǰ��ʾ���ı�
//		std::wstring wstrText = GetVisibleText();
//		int nSingleWCharWidth,nTmpWidth = 0;
//
//		
//		for(int i=0;i<(int)wstrText.length();++i)
//		{								
//			nSingleWCharWidth = GetCharWidth( wstrText[i] );
//			nTmpWidth += nSingleWCharWidth;
//			if(ptCaret.x > nTmpWidth)
//			{
//				//++m_nSelStart;
//				++nCaretPos;
//			}
//			else
//			{
//				if(ptCaret.x > 0 && abs(nTmpWidth - ptCaret.x) < (nSingleWCharWidth / 2) )
//				{				
//					//++m_nSelStart;
//					++nCaretPos;
//				}				
//				break;
//			}
//		}
//		return nCaretPos;
//	}
//
//	//-----------------------------------------------------------------------
//	//��ȡ�༭���ڿ����ı�
//	std::wstring CEdit::GetVisibleText()
//	{
//		std::wstring wstrText = GetWStrText();
//		if( m_nFirstVisible <= (int)wstrText.length() 
//			&& (m_nFirstVisible + m_nVisibleCount) <= (int)wstrText.length() )
//			return wstrText.substr(m_nFirstVisible,m_nVisibleCount);
//		else
//			return L"";
//	}
//
//	//-----------------------------------------------------------------------
//	//�������һ�������ַ�λ��,���ƻ�ȡ�༭���ڿ����ı�
//	std::wstring CEdit::GetVisibleTextReverse(int nReversePos,int* pnFirstVisible)
//	{		
//		std::wstring wstrText = GetWStrText();
//		*pnFirstVisible = 0;
//
//		if(nReversePos <= (int)wstrText.length())
//		{
//			int nSingleWCharWidth, nTmpWidth = 0;
//			for(int i=nReversePos-1;i>=0;--i)
//			{
//				nSingleWCharWidth = GetCharWidth( wstrText[i] );
//				nTmpWidth += nSingleWCharWidth;
//				if( nTmpWidth >= GetInputWidth() )
//				{
//					*pnFirstVisible = i+1;
//					return wstrText.substr(i+1,nReversePos-i-1);
//				}
//			}		 
//			return wstrText;	//��������ı����<�༭����,�򷵻������ַ���
//		}
//
//		return L"";
//	}
//
//	//-----------------------------------------------------------------------
//	std::wstring CEdit::GetWStrText()
//	{
//		return GL_A2W(m_strText.c_str());
//	}
//
//	//-----------------------------------------------------------------------
//	int CEdit::GetInputWidth()	//���ر༭�������Ŀ��,(ע:�༭���� ��һ������ ��������)
//	{
//		return GetRect().Width() - m_sLeftOffset;
//	}
//
//	//-----------------------------------------------------------------------
//	//ˢ��m_nVisibleCount
//	void CEdit::RefreshVisibleTextCount()
//	{
//		m_nVisibleCount = 0;
//		int nSingleWCharWidth, nTmpWidth = 0;
//		std::wstring wstrText = GetWStrText();
//		for(int i=m_nFirstVisible;i<(int)wstrText.length();++i)
//		{
//			nSingleWCharWidth = GetCharWidth(wstrText[i] );
//			nTmpWidth += nSingleWCharWidth;
//			if( nTmpWidth <= GetInputWidth() )
//				++m_nVisibleCount;
//			else
//				return;
//		}
//	}
//
//	//-----------------------------------------------------------------------
//	void CEdit::RenderSelf( const CUIRect& clipper )
//	{
//		//CUIPoint Pos( m_wndRect.left, m_wndRect.top );
//		CUIPoint Pos( 0, 0 );
//		Pos = ClientToScreen( Pos );
//
//		////���Ʊ���
//		m_pCanvas->DrawTexture( m_pBkTex, Pos.x, Pos.y, m_wndRect.Width(), m_wndRect.Height(), clipper );
//
//		//��ȡ�ͻ�����
//		CUIRect rcClient = GetRect();
//
//		Pos.y += rcClient.Height()/2;
//		//����ѡ�б���
//		if( (m_nFirstVisible + m_nCaret) != m_nSelStart )
//		{						
//			int nFirst = min( m_nFirstVisible + m_nCaret, m_nSelStart );
//			int nLast = max( m_nFirstVisible + m_nCaret, m_nSelStart );			
//			std::wstring wstrText = GetWStrText();
//			if(nLast >= (int)wstrText.length() + 1)
//				return;
//
//			int nSelLeftWidth,nSelLenght;
//			if ((nFirst-m_nFirstVisible) > 0)
//			{
//				//����ѡ���ַ�����߾�
//				nSelLeftWidth = GetStringWidth(wstrText.substr(m_nFirstVisible,nFirst-m_nFirstVisible) );
//			}
//			else
//			{
//				//�����һ���ɼ��ַ����Ѿ���ѡ�з�Χ����ôѡ����߾�Ϊ0
//				nSelLeftWidth = 0;
//			}
//			//GetWStrWidth(wstrText.substr(m_nFirstVisible,abs(nFirst-m_nFirstVisible)),&nSelLeftWidth);
//			nSelLenght = GetStringWidth( wstrText.substr( nFirst, nLast-nFirst ) );
//			
//			RECT rcSelection;
//			rcSelection.left = nSelLeftWidth + m_sLeftOffset;
//			rcSelection.right = rcSelection.left + nSelLenght;
//			rcSelection.top = rcClient.top;
//			rcSelection.bottom = rcClient.bottom;
//
//			IntersectRect( &rcSelection, &rcClient, &rcSelection );
//			OffsetRect(&rcSelection,Pos.x,Pos.y);
//
//			m_pCanvas->DrawRect( rcSelection, m_dwSelectColor, NULL );	
//		}
//		
//		
//
//		//��������	
//		std::wstring wstrVisibleText = GetVisibleText();
//		if(m_bPassword)
//		{
//			std::fill(strVisibleText.begin(), strVisibleText.end(), m_strPasswordChar[0]);
//		}
//		m_pCanvas->DrawText(strVisibleText.c_str(), m_pFont, Pos.x + m_sLeftOffset,Pos.y, m_dwTextColor, &rcClient );
//
//		// �н����������		
//		if( m_bFocused )
//		{
//			if((GetTickCount()/500)%2)
//			{
//				//����m_nCaret������λ��
//				if(m_nCaret > (int)wstrVisibleText.length())
//					m_nCaret = (int)wstrVisibleText.length();
//							
//				int nX;
//				nX = GetStringWidth( wstrVisibleText.substr(0,m_nCaret) );
//				nX += (Pos.x + m_sLeftOffset - 4);
//				m_pCanvas->DrawText( _T("|"), m_pFont, nX, Pos.y, m_dwCursorColor, &rcClient );
//			}
//		}
//	}
//
//	void CEdit::SetText( LPCTSTR lpszText ) 
//	{
//		m_nFirstVisible = 0;
//		m_nVisibleCount = 0;
//		m_nCaret = 0;
//		m_nSelStart = 0;
//
//		if( lpszText == NULL )
//		{
//			m_strText = "";
//		}
//		else
//		{
//			int nLen = (int)_tcslen( lpszText ); // �ַ�����
//			for( int i = 0; i < nLen; ++i )
//			{
//				onChar( (DWORD)lpszText[i],NULL );	//ֱ�ӷ���Ϣ����OnChar�����꼰��һ���ɼ��ַ�
//				if(!TestNotOverMaxLen(L""))	//��������������������ѭ������̫�ࡣ
//					break;
//			}
//		}
//	}
//
//	//-----------------------------------------------------------------------
//	int CEdit::GetCharWidth( WCHAR wChar )
//	{		
//		if(m_bPassword)
//		{
//			std::wstring wstrTmp;
//			wstrTmp.append(&wChar,1);
//			std::string strTmp = GL_W2A( wstrTmp );
//			return m_pFont?m_pFont->GetCharWidth( m_strPasswordChar[0] ):0;
//		}
//		return m_pFont?m_pFont->GetCharWidth( m_strPasswordChar[0] ):0;
//	}
//
//	//-----------------------------------------------------------------------
//	int CEdit::GetStringWidth( const std::wstring& strString )
//	{
//		if( m_bPassword )
//		{
//			std::wstring wstrTestTmp = strString;
//			std::string strTest = GL_W2A( strString ) ;
//			fill(strTest.begin(), strTest.end(), m_strPasswordChar[0]);			
//			wstrTestTmp = GL_A2W(strTest.c_str());
//			return m_pFont?m_pFont->GetCharHeight( m_strPasswordChar[0] ):0;
//		}
//		return m_pFont?m_pFont->GetCharHeight( m_strPasswordChar[0] ):0;
//	}
//}