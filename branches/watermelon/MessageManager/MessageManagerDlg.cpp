
// MessageManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MessageManager.h"
#include "MessageManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR Property::TypeName[] = 
{
	_T("int8"),
	_T("int16"),
	_T("int32"),
	_T("int64"),
	_T("uint8"),
	_T("uint16"),
	_T("uint32"),
	_T("uint64"),
	_T("float"),
	_T("double"),
	_T("astring"),
	_T("wstring"),
	_T("tstring"),
	_T("buffer"),
	_T("{}"),
	_T("[]"),
	_T("<>"),
	_T("message"),
	_T("namespace"),
};

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMessageManagerDlg �Ի���
CMessageManagerDlg::CMessageManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMessageManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_TREE, m_MessageTreeCtrl);
	DDX_Control(pDX, IDC_PROPERTY_LIST, m_PropertyListCtrl);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditCtrl);
}

BEGIN_MESSAGE_MAP(CMessageManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TREE_APPEND, &CMessageManagerDlg::OnTreeAppend)
	ON_COMMAND(ID_TREE_DELETE, &CMessageManagerDlg::OnTreeDelete)
	ON_COMMAND_RANGE(ID_INT8, ID_CONTAINER, &CMessageManagerDlg::OnAppendProperty )
	ON_NOTIFY(NM_RCLICK, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnNMRClickMessageTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnEndlabeleditMessageTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnBeginlabeleditMessageTree)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnGetdispinfoMessageTree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnDeleteitemMessageTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnSelchangedMessageTree)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMessageManagerDlg ��Ϣ�������

BOOL CMessageManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	m_PropertyListCtrl.ShowWindow( SW_HIDE );
	m_PropertyListCtrl.EnableWindow( FALSE );

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Bitmap.LoadBitmap( IDB_TREEIMAGE );
	m_ImageList.Create( 16, 16, ILC_COLOR24, 7, 1 );
	m_ImageList.Add( &m_Bitmap, RGB(0,0,0) );
	m_MessageTreeCtrl.SetImageList( &m_ImageList, TVSIL_NORMAL );

	AdjustLayout();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMessageManagerDlg::AdjustLayout()
{
	CRect rcWindow;
	GetClientRect( rcWindow );
	rcWindow.InflateRect( -5, -5 );

	CRect rcPart[3] = { rcWindow, rcWindow, rcWindow };

	if( m_MessageTreeCtrl.GetSafeHwnd() )
	{
		rcPart[0].right = __max( rcWindow.Width()/2, 150 );
		m_MessageTreeCtrl.MoveWindow( rcPart[0] );
	}

	if( m_PropertyGridCtrl.GetSafeHwnd() )
	{
		rcPart[1].left = __max( rcWindow.Width()/2, 150 ) + 5;
		rcPart[1].bottom -= 50;

		m_PropertyGridCtrl.MoveWindow( rcPart[1] );
	}

	if( m_CommandEditCtrl.GetSafeHwnd() )
	{
		rcPart[2].left = __max( rcWindow.Width()/2, 150 ) + 5;
		rcPart[2].top = rcPart[2].bottom - 45;

		m_CommandEditCtrl.MoveWindow( rcPart[2] );
	}
}

void CMessageManagerDlg::ExecuteCommand( CString strCmd )
{

}

void CMessageManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMessageManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMessageManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMessageManagerDlg::OnTreeAppend()
{
	// TODO: �ڴ���������������
	HTREEITEM hItem = m_MessageTreeCtrl.GetSelectedItem();

	HTREEITEM hInsert = m_MessageTreeCtrl.InsertItem( LPSTR_TEXTCALLBACK, 3, 3, hItem?hItem:TVI_ROOT );
	Property *pProperty = new PropertyMessage();
	m_MessageTreeCtrl.SetItemData( hInsert, (DWORD_PTR)pProperty );
	m_MessageTreeCtrl.SelectItem( hInsert );
	m_MessageTreeCtrl.EditLabel( hInsert );
}

void CMessageManagerDlg::OnTreeDelete()
{
	// TODO: �ڴ���������������
	m_MessageTreeCtrl.DeleteItem( m_MessageTreeCtrl.GetSelectedItem() );
}

void CMessageManagerDlg::OnNMRClickMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	
	CMenu Menu;

	CPoint ptCursor, ptTreeHit;
	GetCursorPos( &ptCursor );

	ptTreeHit = ptCursor;
	m_MessageTreeCtrl.ScreenToClient( &ptTreeHit );
	HTREEITEM hItem = m_MessageTreeCtrl.HitTest( ptTreeHit );
	m_MessageTreeCtrl.SelectItem( hItem );

	Menu.LoadMenu( IDR_MENU_TREEPOP );
	CMenu *pSub = Menu.GetSubMenu( 0 );

	pSub->EnableMenuItem( ID_TREE_DELETE, hItem?MF_ENABLED:MF_DISABLED|MF_GRAYED );
	pSub->EnableMenuItem( ID_TREE_APPEND, hItem?MF_DISABLED|MF_GRAYED:MF_ENABLED );

	pSub->TrackPopupMenu( TPM_LEFTALIGN, ptCursor.x, ptCursor.y, this );

}

void CMessageManagerDlg::OnTvnEndlabeleditMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( pTVDispInfo->item.pszText )
	{
		Property *pProperty = (Property *)pTVDispInfo->item.lParam;
		pProperty->m_name = pTVDispInfo->item.pszText;
	}
	*pResult = FALSE;
}

void CMessageManagerDlg::OnTvnBeginlabeleditMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Property *pProperty = (Property*)m_MessageTreeCtrl.GetItemData( pTVDispInfo->item.hItem );
	if( pProperty )
	{
		CEdit *pEdit = m_MessageTreeCtrl.GetEditControl();
		pEdit->SetWindowText( pProperty->m_name );
	}
	*pResult = 0;
}

BOOL CMessageManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST )
	{
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
		{
			CWnd* pWnd = GetFocus();
			if( pWnd == m_MessageTreeCtrl.GetEditControl() )
				m_MessageTreeCtrl.EndEditLabelNow(FALSE);
			else if( pWnd->GetSafeHwnd() == m_CommandEditCtrl.GetSafeHwnd() )
			{
				CString strCmd;
				m_CommandEditCtrl.GetWindowText(strCmd);
				ExecuteCommand( strCmd );
				m_CommandEditCtrl.SetWindowText( _T("") );
			}

			return TRUE;
		}
		else if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		{
			CWnd* pWnd = GetFocus();
			if( pWnd == m_MessageTreeCtrl.GetEditControl() )
				m_MessageTreeCtrl.EndEditLabelNow(TRUE);
			else if( pWnd->GetSafeHwnd() == m_CommandEditCtrl.GetSafeHwnd() )
				m_CommandEditCtrl.SetWindowText( _T("") );
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMessageManagerDlg::OnAppendProperty( UINT nID )
{
	// TODO: �ڴ���������������
	HTREEITEM hItem = m_MessageTreeCtrl.GetSelectedItem();

	HTREEITEM hInsert = NULL;
	switch( nID )
	{
	case ID_STRUCT:
		hInsert = m_MessageTreeCtrl.InsertItem( LPSTR_TEXTCALLBACK, 4, 4, hItem?hItem:TVI_ROOT );
		break;
	default:
		hInsert = m_MessageTreeCtrl.InsertItem( LPSTR_TEXTCALLBACK, 5, 5, hItem?hItem:TVI_ROOT );
		break;
	}
	
	Property *pProperty = new Property();
	m_MessageTreeCtrl.SetItemData( hInsert, (DWORD_PTR)pProperty );
	pProperty->m_type = (Property::type)(nID - ID_INT8);
	m_MessageTreeCtrl.SelectItem( hInsert );
	m_MessageTreeCtrl.EditLabel( hInsert );
}

void CMessageManagerDlg::OnTvnGetdispinfoMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Property *pProperty = (Property*)m_MessageTreeCtrl.GetItemData( pTVDispInfo->item.hItem );
	if( pProperty && pProperty->m_type < Property::MESSAGE )
	{
		_sntprintf( pTVDispInfo->item.pszText, pTVDispInfo->item.cchTextMax, _T("%s:%s"), pProperty->m_name, Property::TypeName[pProperty->m_type] );
	}
	else if( pProperty )
	{
		_sntprintf( pTVDispInfo->item.pszText, pTVDispInfo->item.cchTextMax, _T("[%s]"), pProperty->m_name );
	}

	*pResult = 0;
}

void CMessageManagerDlg::OnTvnDeleteitemMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Property *pProperty = (Property*)pNMTreeView->itemOld.lParam;
	delete pProperty;
	*pResult = 0;
}

void CMessageManagerDlg::OnTvnSelchangedMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_PropertyListCtrl.DeleteAll();
	Property *pProperty = (Property*)pNMTreeView->itemNew.lParam;
	if( pProperty )
	{
		switch( pProperty->m_type )
		{
		case Property::MESSAGE:
			{
				m_PropertyListCtrl.AddPropItem( _T("Message"), _T("��Ϣ����"), _T("0"), PIT_EDIT, _T("0") );
				m_PropertyListCtrl.AddPropItem( _T("Message"), _T("��Ϣ����"), _T("0"), PIT_EDIT, _T("0") );
			}
			break;
		case Property::SUB_DEF:
			break;
		case Property::ARRAY:
			m_PropertyListCtrl.AddPropItem( _T("Array"), _T("�����±�"), _T("1"), PIT_EDIT, _T("1") );
			break;
		case Property::CONTAINER:
			{
				m_PropertyListCtrl.AddPropItem( _T("Container"), _T("Ƕ������"), _T(""), PIT_COMBO, _T("1|2|3") );
			}
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}

int CMessageManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rc;
	if( !m_PropertyGridCtrl.Create( WS_VISIBLE | WS_CHILD, rc, this, 1 ) )
		return -1;

	m_PropertyGridCtrl.EnableHeaderCtrl(FALSE);
	m_PropertyGridCtrl.SetVSDotNetLook();
	m_PropertyGridCtrl.MarkModifiedProperties();

	//CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���"));

	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��ά���"), (_variant_t) false, _T("ָ�����ڵ����岻ʹ�ô��壬���ҿؼ���ʹ����ά�߿�")));

	//CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("Dialog Frame"), _T("����֮һ: ��(None)��ϸ(Thin)���ɵ�����С(Resizable)���Ի������(Dialog Frame)"));
	//pProp->AddOption(_T("None"));
	//pProp->AddOption(_T("Thin"));
	//pProp->AddOption(_T("Resizable"));
	//pProp->AddOption(_T("Dialog Frame"));
	//pProp->AllowEdit(FALSE);

	//pGroup1->AddSubItem(pProp);
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));

	//m_PropertyGridCtrl.AddProperty(pGroup1);

	//CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("���ڴ�С"), 0, TRUE);

	//pProp = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
	//pProp->EnableSpinControl(TRUE, 50, 300);
	//pSize->AddSubItem(pProp);

	//pProp = new CMFCPropertyGridProperty( _T("���"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
	//pProp->EnableSpinControl(TRUE, 50, 200);
	//pSize->AddSubItem(pProp);

	//m_PropertyGridCtrl.AddProperty(pSize);

	//CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("����"));

	//LOGFONT lf;
	//CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	//font->GetLogFont(&lf);

	//lstrcpy(lf.lfFaceName, _T("����, Arial"));

	//pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ�����ڵ�Ĭ������")));
	//pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ������ʹ�á�MS Shell Dlg������")));

	//m_PropertyGridCtrl.AddProperty(pGroup2);

	//CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("����"));
	//pProp = new CMFCPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
	//pProp->Enable(FALSE);
	//pGroup3->AddSubItem(pProp);

	//CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
	//pColorProp->EnableOtherButton(_T("����..."));
	//pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	//pGroup3->AddSubItem(pColorProp);

	//static TCHAR BASED_CODE szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
	//pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));

	//pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));

	//m_PropertyGridCtrl.AddProperty(pGroup3);

	//CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));

	//CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
	//pGroup4->AddSubItem(pGroup41);

	//CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
	//pGroup41->AddSubItem(pGroup411);

	//pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
	//pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
	//pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));

	//pGroup4->Expand(FALSE);
	//m_PropertyGridCtrl.AddProperty(pGroup4);

	return 0;
}

void CMessageManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, __max( cx, 300 ), __max( cy, 300 ) );

	// TODO: �ڴ˴������Ϣ����������
	AdjustLayout();
}
