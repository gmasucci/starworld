
// MessageManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MessageManager.h"
#include "MessageManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

BEGIN_MESSAGE_MAP(CMessageManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TREE_APPEND, &CMessageManagerDlg::OnTreeAppend)
	ON_COMMAND(ID_TREE_DELETE, &CMessageManagerDlg::OnTreeDelete)
	ON_NOTIFY(NM_RCLICK, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnNMRClickMessageTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnEndlabeleditMessageTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_MESSAGE_TREE, &CMessageManagerDlg::OnTvnBeginlabeleditMessageTree)
	ON_COMMAND_RANGE(ID_INT8, ID_STRUCT, &CMessageManagerDlg::OnAppendProperty )
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Bitmap.LoadBitmap( IDB_TREEIMAGE );
	m_ImageList.Create( 16, 16, ILC_COLOR24, 7, 1 );
	m_ImageList.Add( &m_Bitmap, RGB(0,0,0) );
	m_MessageTreeCtrl.SetImageList( &m_ImageList, TVSIL_NORMAL );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

	HTREEITEM hInsert = m_MessageTreeCtrl.InsertItem( _T(""), 3, 3, hItem?hItem:TVI_ROOT );
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
		m_MessageTreeCtrl.SetItemText( pTVDispInfo->item.hItem, pTVDispInfo->item.pszText );
		*pResult = TRUE;
	}
	else
	{
		*pResult = FALSE;
	}
}

void CMessageManagerDlg::OnTvnBeginlabeleditMessageTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}
BOOL CMessageManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST )
	{
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
		{
			m_MessageTreeCtrl.EndEditLabelNow(FALSE);
			return TRUE;
		}
		else if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		{
			m_MessageTreeCtrl.EndEditLabelNow(TRUE);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMessageManagerDlg::OnAppendProperty( UINT nID )
{
	// TODO: �ڴ���������������
	HTREEITEM hItem = m_MessageTreeCtrl.GetSelectedItem();

	HTREEITEM hInsert = m_MessageTreeCtrl.InsertItem( _T(""), 5, 5, hItem?hItem:TVI_ROOT );
	m_MessageTreeCtrl.SelectItem( hInsert );
	m_MessageTreeCtrl.EditLabel( hInsert );
}
