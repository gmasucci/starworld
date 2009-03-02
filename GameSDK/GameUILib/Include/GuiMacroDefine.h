/*********************************************************************/
/*
    Create:     2006/06/16 6:2006   15:42
    FileName:   GuiMacroDefine.h
    Author:     Albert xu

    Levle:      0
	
	���ﶨ������ϢӰ���������Ϣӳ������ת���꣬
	UI_COMMAND	Ϊ�˵���Ϣ���¼����Ϊ0
	UI_CONTROL	Ϊ�����ؼ����¼����壬���簴ť��Click�¼�DbClick�¼�
	UI_NOTIFY	Ϊ�ؼ���֪ͨ�¼������� NC_CREATE��

	��Ϣ���������ʱû�ж��壬��Ҫ���Ժ���롣
*/
/*********************************************************************/
#define OFFSET( structure, member )	((DWORD_PTR)&((structure*)0)->member)

//////////////////////////////////////////////////////////////////////////
// ��Ϣӳ�䶨��ĺ�
//////////////////////////////////////////////////////////////////////////
#define DECLARE_UIMSG_MAP() \
private: \
	static const UI_MSGMAP_ENTRY _messageEntries[]; \
protected: \
	static const UI_MSGMAP messageMap; \
	virtual const UI_MSGMAP* GetMessageMap() const; \

#define BEGIN_UIMSG_MAP(theClass, baseClass) \
const UI_MSGMAP* theClass::GetMessageMap() const \
{ return &theClass::messageMap; }; \
	__declspec( selectany ) const UI_MSGMAP theClass::messageMap = \
{ &baseClass::messageMap, &theClass::_messageEntries[0] }; \
	typedef theClass ThisClass;\
	typedef baseClass TheBassClass;\
	__declspec( selectany ) const UI_MSGMAP_ENTRY theClass::_messageEntries[] = \
{ \

#define uimsg

#define END_UIMSG_MAP() \
{ 0, 0, 0, 0, uiSig_end, (UI_PMSG)0 }\
};\

// common
//--------------------------------------------------------------------------
#define UI_LBUTTONDOWN( ID, pfnProc ) \
{	EV_MOUSELBTDOWN, 0, ID, uiSig_vwp,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::pfnProc ) },

#define UI_RBUTTONDOWN( ID, pfnProc ) \
{	EV_MOUSERBTDOWN, 0, ID, uiSig_vwp,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::pfnProc ) },

#define UI_MOUSEMOVE( ID, pfnProc ) \
{	EV_MOUSEMOVE, 0, ID, uiSig_vwp,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::pfnProc ) },

#define UI_LBUTTONUP( ID, pfnProc ) \
{	EV_MOUSELBTUP, 0, ID, uiSig_vwp,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::pfnProc ) },

#define UI_RBUTTONUP( ID, pfnProc ) \
{	EV_MOUSERBTUP, 0, ID, uiSig_vwp,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::pfnProc ) },

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
#define UI_COMMAND( id, pfnProc )\
{	WM_COMMAND, EV_COMMAND, id, id, uiSig_Cmd,\
	( UI_PMSG )static_cast< void ( XUI_Base::* )( void ) >( &ThisClass::pfnProc ) },

#define UI_COMMAND_RANGE( id, idLast, pfnProc )\
{	WM_COMMAND, EV_COMMAND, id, idLast, uiSig_Cmd_Range, ( UI_PMSG )static_cast< void ( XUI_Base::* )( void ) >( &ThisClass::pfnProc ) },

#define UI_CONTROL( id, nNotifyCode, pfnProc )\
{	WM_COMMAND, nNotifyCode, id, id, uiSig_Cmd,\
	( UI_PMSG )static_cast< void ( XUI_Base::* )( void ) >( &ThisClass::pfnProc ) },

#define UI_CONTROL_RANGE( id, idLast, nNotifyCode, pfnProc )\
{	WM_COMMAND, nNotifyCode, id, idLast, uiSig_Cmd_Range,\
	( UI_PMSG )static_cast< void ( XUI_Base::* )( UINT ) >( &ThisClass::pfnProc ) },

#define UI_NOTIFY( id, nNotifyCode, pfnProc )\
{	WM_NOTIFY, nNotifyCode, id, id, uiSig_Notify,\
	( UI_PMSG )static_cast< void ( XUI_Base::* )( NMUIHDR*, LRESULT ) >( &ThisClass::pfnProc ) },

#define UI_NOTIFY_RANGE( id, idLast, nNotifyCode, pfnProc )\
{	WM_NOTIFY, nNotifyCode, id, idLast, uiSig_Notify_Range,\
	( UI_PMSG )static_cast< void ( XUI_Base::* )( UINT, NMUIHDR*, LRESULT ) >( &ThisClass::pfnProc ) },
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// SystemMessage
//--------------------------------------------------------------------------
#define UI_KEYUP()\
{	WM_KEYUP, 0, 0, 0, uiSig_b_wl,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::OnKeyUp ) },

#define UI_KEYDOWN()\
{	WM_KEYUP, 0, 0, 0, uiSig_b_wl,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::OnKeyDown ) },

#define UI_CHAR()\
{	WM_KEYUP, 0, 0, 0, uiSig_b_wl,\
	( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::OnChar ) },

#define ON_UI_TIMER()\
{	UI_TIMER, 0, 0, 0, uiSig_vw, ( UI_PMSG )static_cast< VOID (XUI_Base::* )( UINT ) >( &ThisClass::OnTimer ) },

#define ON_UI_SWITCHENTER()\
{	UI_SWITCHENTER, 0, 0, 0, uiSig_vv, ( UI_PMSG )static_cast< VOID (XUI_Base::* )() >( &ThisClass::OnSwitchEnter ) },

#define ON_UI_SWITCHLEAVE()\
{	UI_SWITCHLEAVE, 0, 0, 0, uiSig_vv, ( UI_PMSG )static_cast< VOID (XUI_Base::* )() >( &ThisClass::OnSwitchLeave ) },

#define ON_UI_CREATE()\
{	UI_CREATE, 0, 0, 0, uiSig_b_wl, ( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::OnCreate ) },

#define ON_UI_DRAW()\
{	UI_OWNERDRAW, 0, 0, 0, uiSig_b_wl, ( UI_PMSG )static_cast< BOOL (XUI_Base::* )( WPARAM, LPARAM ) >( &ThisClass::OnDraw ) },

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Control Message
//--------------------------------------------------------------------------
#define ON_UI_ENABLE()\
{	WM_ENABLE, 0, 0, 0, uiSig_vb, ( UI_PMSG )static_cast< VOID (XUI_Base::* )( BOOL ) >( &ThisClass::OnEnable ) },

#define ON_UI_SHOWWINDOW()\
{	WM_SHOWWINDOW, 0, 0, 0, uiSig_vw, ( UI_PMSG )static_cast< VOID (XUI_Base::* )( BOOL ) >( &ThisClass::OnShowWindow ) },

#define ON_UI_MOVE()\
{	WM_MOVE, 0, 0, 0, uiSig_v_v_ii, ( UI_PMSG )static_cast< VOID (XUI_Base::* )( int, int ) >( &ThisClass::OnMove ) },

#define ON_UI_SIZE()\
{	WM_SIZE, 0, 0, 0, uiSig_v_v_ii, ( UI_PMSG )static_cast< VOID (XUI_Base::* )( int, int ) >( &ThisClass::OnSize ) },

// Element
#define UI_SETFOCUS( id, pfnProc )\
	UI_CONTROL( id, EV_SETFOCUS, pfnProc )

#define UI_KILLFOCUS( id, pfnProc )\
	UI_CONTROL( id, EV_KILLFOCUS, pfnProc )

// Button
#define UI_BNCLICKED( id, pfnProc )\
	UI_CONTROL( id, EV_BNCLICKED, pfnProc )
#define UI_RBNCLICKED( id, pfnProc )\
	UI_CONTROL( id, EV_RBNCLICKED, pfnProc )

// ScrollBar
#define UI_SCROLLPOSCHANGED( id, pfnProc )\
	UI_NOTIFY( id, EV_SCROLLPOSCHANGED, pfnProc )

#define UI_WINDOWPOSCHANGE() \
	UI_NOTIFY( id, EV_WINDOWPOSCHANGE��pfnProc )


// UNICODE ����
#ifdef UNICODE
typedef	std::wstring	_string;
#define INCSZ( pch )			( ++pch )
#define INCNSZ( pch, count )	( pch+=count )
#define CMPSZ( pch1, pch2 )		( pch1 == pch2 )
#define EQUSZ( pch1, pch2 )		( *pch1 = *pch2 )
#define DEFCH( ch )				( ch )
#else
typedef	std::string		_string;
#define INCSZ( pch )			( pch = _mbsinc( pch ) )
#define INCNSZ( pch, count )	( pch = _mbsninc( pch, count ) )
#define CMPSZ( pch1, pch2 )		( !_mbccmp( pch1, ( const UCHAR* )pch2 ) )
#define EQUSZ( pch1, pch2 )		( _mbccpy( pch1, pch2 ) )
#define DEFCH( ch )				( ( const unsigned char* )&ch[0] )
#endif
