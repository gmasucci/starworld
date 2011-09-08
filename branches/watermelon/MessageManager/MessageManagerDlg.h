
// MessageManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "PropertyList.h"

// CMessageManagerDlg �Ի���
struct Property
{
	enum type { I8, I16, I32, I64, U8, U16, U32, U64, FLT, LFLT, ASTR, USTR, TSTR, BUF, SUB_DEF, ARRAY, CONTAINER, MESSAGE, NAMESPACE };
	static LPCTSTR TypeName[];
	type	m_type;				// ��������
	CString m_name;				// ������
};

struct PropertyNamespace	:	public Property
{
	PropertyNamespace()
	{
		m_type = Property::NAMESPACE;
	}
};

struct PropertyContainer	:	public Property
{
	PropertyContainer()
		: m_object_name()
		, m_array_count(0)
		, m_container_type(Property::U8)
	{
		m_type = ( Property::CONTAINER );
	}

	CString m_object_name;		// ���ɶ�����
	size_t	m_array_count;		// �����С
	type	m_container_type;	// ��������
};

struct PropertyMessage	:	public Property
{
	PropertyMessage()
	{
		m_type = Property::MESSAGE;
	}

	unsigned char msgtype;		// ��Ϣ����
	unsigned char msgcode;		// ��Ϣ����
};

class CMessageManagerDlg : public CDialog
{
// ����
public:
	CMessageManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MESSAGEMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CBitmap			m_Bitmap;
	CImageList		m_ImageList;
	CTreeCtrl		m_MessageTreeCtrl;
	CPropertyList	m_PropertyListCtrl;
	CEdit			m_CommandEditCtrl;

	CMFCPropertyGridCtrl	m_PropertyGridCtrl;
	void ExecuteCommand( CString strCmd );
	void AdjustLayout();

	afx_msg void OnTreeAppend();
	afx_msg void OnTreeDelete();
	afx_msg void OnNMRClickMessageTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditMessageTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginlabeleditMessageTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAppendProperty( UINT nID );
	afx_msg void OnTvnGetdispinfoMessageTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitemMessageTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedMessageTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
