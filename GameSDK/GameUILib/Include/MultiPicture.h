#pragma once
#include "Picture.h"
namespace UILib
{
	class CTexture;
	class CMultiPicture	:	public XObjTypeT< CPicture, TypeMultiPicture >
	{
	public:
		CMultiPicture(void);
		~CMultiPicture(void);

		virtual const char* GetLable(){ return "MULTIPICTURE"; }

		virtual void LoadProperty(TiXmlElement* pXMLElement );
		virtual void SaveProperty(TiXmlElement* pXMLElement );

		virtual bool SetProperty( const char* pszPropertyName, const char* pszValue );
		virtual bool GetProperty( const char* pszPropertyName, char* pValueBuffer, int size );

		// method
		void SetAlpha( float fAlpha );
		void SetIndex( int	nIndex );
		void SetGroup( int	nGroup );
		
		void SetAttrib( int nRow, int nCol ){ m_nRowCount = nRow; m_nColCount = nCol; }

	protected:
		void UpdatePicture();

	protected:
		std::string	m_strNameHead;
		int			m_nRowCount;	// ������
		int			m_nColCount;	// ������

		int			m_nCurGroup;	// ��ǰ������	- ��
		int			m_nCurIndex;	// ��ǰͼƬ����	- ��
		int			m_nType;		// ���� 0 - ����ͼƬ��UV�趨
									// ���� 1 - ����ͼƬ

		int			m_nUpdateTime;	// ͼƬ����ʱ�䣬�����
	};
}
