#ifndef PANEL_H
#define PANEL_H
#include "XUI_Wnd.h"
//////////////////////////////////////////////////////////////////////////
// Panel Ϊ�ؼ�������ͬʱ��������ؼ���
class TiXmlNode;
namespace XGC
{
	namespace ui
	{
		class XUI_Window :    public XUI_Wnd
		{
			friend class XUI_Lua;
		public:
			XUI_Window(void);
			~XUI_Window(void);

		public:
			virtual void AdjustPoint( iPoint& pt, bool bClientToScreen )const;
			virtual void AdjustPoint( iRect& pt, bool bClientToScreen )const;

			// Panel��С
			int GetWidth()const	{ return m_nPanelWidth; }
			int GetHeight()const	{ return m_nPanelHeight; }

			// ͨ��ָ���Ľڵ㴴���ӿؼ���
			bool CreateFromXMLNode( TiXmlNode* pNode );

			// �洢��ָ���ڵ�
			bool SaveToXMLNode( TiXmlNode* pNode );

		protected:
			int m_nOffsetX;
			int m_nOffsetY;
			int m_nPanelWidth;	// ������ɵĿ��
			int m_nPanelHeight;	// ������ɵĸ߶�
		};
	}
}
#endif
