#ifndef PANEL_H
#define PANEL_H

#include "XUI_Picture.h"
#include "GuiMacroDefine.h"

//////////////////////////////////////////////////////////////////////////
// Panel Ϊ�ؼ�������ͬʱ��������ؼ���
class TiXmlNode;
namespace UILib
{
	class Canvas;
	class XUI_Window :    public UIObjTypeT< XUI_Wnd, TypeWindow >
	{
		DECLARE_UIMSG_MAP()
		DECLARE_LABLE( WINDOW )
		friend class LuaBindClass;
	public:
		XUI_Window(void);
		~XUI_Window(void);

	public:
		virtual x_point AdjustPoint(const x_point& pt, bool bClientToScreen) const;

		// Panel��С
		int		GetWidth()const		{ return m_nPanelWidth; }
		int		GetHeight()const	{ return m_nPanelHeight; }

		// ͨ��ָ���Ľڵ㴴���ӿؼ���
		bool CreateFromXMLNode( TiXmlNode* pNode );

		// �洢��ָ���ڵ�
		bool SaveToXMLNode( TiXmlNode* pNode );

	protected:
		virtual void RenderSelf(const x_rect& clipper){};
		void	BindScrollBar();
		int		ScrollVerticle(int iOffset);
		int		ScrollHorizontal(int iOffset);

		// uimsg
		BOOL OnScrollCreate( WPARAM wParam, LPARAM lParam );
		BOOL OnScrollPosChange( WPARAM wParam, LPARAM lParam );
	protected:
		int m_nOffsetX;
		int m_nOffsetY;
		int m_nPanelWidth;	// ������ɵĿ��
		int m_nPanelHeight;	// ������ɵĸ߶�

	private:
		int m_nVScrollID;
		int m_nHScrollID;

		//CScrollBar*	m_pVScrollBar;	// ��ֱ����Ĺ�����
		//CScrollBar* m_pHScrollBar;	// ˮƽ����Ĺ�����
	};
}
#endif
