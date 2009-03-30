#pragma once
#include <string>
#include <vector>
#include "GuiMacroDefine.h"
#include "EventDefine.h"
#include "UIObject.h"
#include "DataHelper.h"
#include "tinyxml.h"
using namespace XGC;

class TiXmlElement;
namespace UILib{ class XUI_Base; }

typedef void ( UILib::XUI_Base::* UI_PMSG)(void);
struct UI_MSGMAP_ENTRY
{
	UINT		nMessage;   // windows message
	UINT		nEvent;     // control event
	UINT		nID;		// control ID (or 0 for windows messages)
	UINT		nIDLast;
	UINT_PTR	nSig;       // signature type (action) or pointer to message #
	UI_PMSG		pfn;        // routine to call (or special value)
};

struct UI_MSGMAP
{
	const UI_MSGMAP*		pBaseMap;
	const UI_MSGMAP_ENTRY*	lpEntry;
};

enum uiSig
{
	uiSig_begin	=	0,		// Sig begin!

	uiSig_Cmd,				// VOID	(VOID)
	uiSig_Cmd_Range,		// VOID (UINT)
	uiSig_Notify,			// BOOL (NMUIHDR, LRESULT)
	uiSig_Notify_Range,		// BOOL (UINT, NMUIHDR, LRESULT)

	uiSig_vv,				// VOID (VOID)
	uiSig_vw,				// VOID (WPARAM)
	uiSig_vb = uiSig_vw,	// VOID (BOOL)

	uiSig_b_wl,				// BOOL (WPARAM,LPARAM)
	uiSig_vwp,				// VOID (UINT,CPoint)
	uiSig_v_v_ii,			// VOID (short,short)
	uiSig_end
};

namespace UILib
{
	//�¼������������¼�����Ȥ�������̳�����ӿ�
	class CGuiSystem;
	struct ICanvas;
	struct XUI_ISprite;
	class XUI_Base;
	class XUI_Wnd;

	struct UI_HANDLE_INFO
	{
		XUI_Base*	pCmdTraget;
		void ( XUI_Base::*pmf )(void);
	};

	// ֪ͨ��Ϣ�ṹ
	struct NMUIHDR
	{
		XUI_Wnd* pCtrl;
		UINT		idFrom;
		UINT		code;
		LRESULT		hResult;
	};

	union	pfnSig
	{
		UI_PMSG	pfnNormal;

		VOID ( XUI_Base::* pfn_Cmd)(VOID);
		VOID ( XUI_Base::* pfn_Cmd_Range)(UINT);

		VOID ( XUI_Base::* pfn_Notify)(UILib::NMUIHDR*, LRESULT);
		VOID ( XUI_Base::* pfn_Notify_Range)(UINT, UILib::NMUIHDR*, LRESULT);

		VOID ( XUI_Base::* pfn_vv )( VOID );
		VOID ( XUI_Base::* pfn_vw )( WPARAM );

		BOOL ( XUI_Base::* pfn_b_wl)(WPARAM, LPARAM);
		VOID ( XUI_Base::* pfn_vwp )(UINT, CPoint);

		VOID ( XUI_Base::* pfn_v_v_ii)(short, short);
	};

	class XUI_Base	:	public UIObjTypeT< CUIObject, TypeUI >
	{
	public:
		XUI_Base();
		virtual ~XUI_Base();
	private:
		static const UI_MSGMAP_ENTRY _messageEntries[];

	protected:
		static const UI_MSGMAP messageMap;
		virtual const UI_MSGMAP* GetMessageMap() const;

	protected:
		const UI_MSGMAP_ENTRY*	FindMessageEntry( const UI_MSGMAP_ENTRY* lpEntry, UINT nMsg, INT nEvent, UINT nID );
		//�¼���Ӧ
		std::vector<XUI_Base*> m_pListeners;

	public:
		//�¼�����������true��ʾ��ֹ��������
		HRESULT DispatchCmdMsg( UINT nID, int nCode, UI_PMSG pfn, void* lpExtra, UINT_PTR nSig );
		virtual bool OnCmdMsg( UINT nID, int nCode, void* lpExtra, UI_HANDLE_INFO* pHandler );

		//�¼����������е��¼����ᱻ������Щ������
		void AddCommonListener(XUI_Base* pListener);
		void RemoveCommonListener(XUI_Base* pListener);

		virtual bool DefMsgProc( UINT nMsg, WPARAM wParam, LPARAM lParam ){ return FALSE; }
	};
}