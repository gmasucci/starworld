#ifndef _XOBJECT_
#define _XOBJECT_

#include "Defines.h"
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS
#pragma warning( disable:4251 )

/************************************************************************/
/* �б�ϵͳ������ϵͳ���Լ�����������ʵ��                               */
/************************************************************************/
#define INVALID_OBJID -1
#define INVALID_ATTRIBID -1
#define OBJ_MAX	65535

#define		TypeObjectbase				0

#define		TypeScene					0x30000000

#define		TypeEvent					0x20000000
#define			TypeMapevent			0x21000000
#define			TypeObjevent			0x22000000

#define		TypeMapobj					0x10000000
#define			TypeDynamic				0x11000000
#define				TypeSprite			0x11100000
#define			TypeItem				0x12000000
#define			TypeBuild				0x13000000

#define		TypeEffect					0x00000001
/************************************************************************/

#include <list>
#include <map>
class CXObject;

//----------------------------------------------------------------------//

/************************************************************************/
/* CXObjectList �࣬ʵ����һ����̬��ָ���б�����ͨ��ID�ķ�ʽת������ָ�롣
/************************************************************************/
class CORE_API CXObjectList
{
friend class CXObject;
protected:
	CXObjectList( int nLen );
	~CXObjectList();

	int		AddObj( CXObject* pObj );
	void	DelObj( int nID );

	CXObject**	m_pObjList;
	int			m_nLen;
	int			m_nCurID;
	int			m_nCount;
public:
	static CXObjectList& GetInstance();
	bool IsValidID( int nID ) { return nID != INVALID_OBJID && nID >= 0 && nID < m_nLen; }

	CXObject*	GetObj( int nID ){ return IsValidID( nID )?m_pObjList[nID]:NULL; }
	CXObject*	operator []( int nID ){ return m_pObjList[nID]; }
};

class CAttrib;
class CORE_API CXVariant
{
friend class CAttrib;
public:
	enum var_type{ type_float, type_integer, };

private:
	var_type t;
	union
	{
		float	fValue;
		int		nValue;
	};

public:
	CXVariant(){ t = type_integer; nValue = 0; }
	CXVariant( const CXVariant& rsh ){ t = rsh.t; fValue = rsh.fValue; }
	CXVariant( float rsh ){ t = type_float; fValue = rsh; }
	CXVariant( int rsh ){ t = type_integer; nValue = rsh; }

	bool operator==( const CXVariant& val )const{ return val.t == t; }
	bool operator==( float val )const{ return t == type_float && val == fValue; }
	bool operator==( int val )const{ return t == type_integer && val == nValue; }

	CXVariant& operator=( const CXVariant& val ){ t = val.t; fValue = val.fValue; return *this; }
	CXVariant& operator=( float val ){ t = type_float; fValue = val; return *this; }
	CXVariant& operator=( int val ){ t = type_integer; nValue = val; return *this; }

	operator int()const{ if( t != type_integer ) throw( t ); return nValue; }
	operator float()const{ if( t != type_float ) throw( t ); return fValue; }
	operator bool()const{ if( t != type_integer ) throw( t ); return nValue != 0; }
};

struct CORE_API AttribValueListener
{
	virtual bool OnAttribChange( CXVariant& OldValue, CXVariant NewValue ) = 0;
};

class CORE_API CAttrib
{
public:
	CAttrib()
	: Value( 0 )
	, NowValue( 0 )
	, mListener( NULL )
	{
	}

	~CAttrib()
	{

	}

	void SetListener( AttribValueListener* pListener )
	{
		mListener = pListener;
	}

	void ChangeValue( bool bPersent, const CXVariant& Val )
	{
		// DoDefaultEffect �����ĵ��������������⡣С�ģ���ͷ�ٸİɣ�
		switch( Val.t )
		{
		case CXVariant::type_integer:
			{
				int nTmpValue = 0;

				if( bPersent )
					nTmpValue = NowValue.nValue + (int)( Value.nValue * Val.nValue / 100.0f );
				else
					nTmpValue = NowValue.nValue + Val.nValue;

				if( ( mListener == NULL ) || ( mListener && mListener->OnAttribChange( NowValue, nTmpValue ) ) )
				{
					NowValue = nTmpValue;
				}
			}
			break;
		case CXVariant::type_float:
			{
				float fTmpValue = 0.0f;

				if( bPersent )
					fTmpValue = NowValue.fValue + Value.fValue * Val.fValue / 100.0f;
				else
					fTmpValue = NowValue.fValue + Val.fValue;

				if( ( mListener == NULL ) || ( mListener && mListener->OnAttribChange( NowValue, fTmpValue ) ) )
				{
					NowValue = fTmpValue;
				}
			}
			break;
		}
	}

	void	SetValue( const CXVariant& Val )
	{ 
		if( Val == NowValue )
		{
			return;
		}
		if( ( mListener == NULL ) || ( mListener && mListener->OnAttribChange( NowValue, Val ) ) )
		{
			Value = Val;
			NowValue = Val; 
		}
	}

	const CXVariant&	GetValue()const { return NowValue; }

private:
	AttribValueListener* mListener;
	CXVariant Value;		// ��׼ֵ
	CXVariant NowValue;	// ��ǰֵ,���������ֵ
};

/************************************************************************/
/* CXObjectT ģ��ʵ��������ϵͳ����ģ����ÿ��������ʵ��һ�������б����ڴ�ȡ
/* ʱ��Ҫ��ȷ��ȡ��������λ���ĸ������ϵģ���ͨ��һ�������ֵ��ָ��������������������
/************************************************************************/
template < typename TBase, int TYPE, int nAttribCount = 1 >
class XObjTypeT	:	public TBase
{
protected:
	XObjTypeT()
	{
		ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );
		ZeroMemory( _attrib, sizeof( _attrib ) );
	}

private:
	// ����ĸ���Ԫ��,�ڵ�һʱ��ȷ����������,֮���ܸ�������.
	CAttrib _attrib[nAttribCount];

	typedef std::map< _string, _string >	CStrAttribMap;
	typedef std::map< _string, LPVOID >		CBufAttribMap;

	CStrAttribMap	_strAttrib;
	CBufAttribMap	_bufAttrib;

public:
	static	int		GetPointType()				{ return TYPE; }
	virtual int		GetType()const				{ return TYPE; }
	virtual bool	IsType( uint32 dwType )const	{ if( GetPointType() == dwType ) return true; else return TBase::IsType( dwType );}
	static	bool	IsTypeOf( uint32 dwType )	{ if( GetPointType() == dwType ) return true; else return TBase::IsTypeOf( dwType );}

	virtual bool SetAttribListener( int nType, int nIndex, AttribValueListener* pListener )
	{
		if( GetPointType() == nType )
		{
			return SetLocalAttribListener( nIndex, pListener );
		}
		if( GetPointType() == TBase::GetPointType() )	return false;
		return TBase::SetAttribListener( nType, nIndex, pListener );
	}

	virtual bool GetAttrib( int nType, int nIndex, CXVariant& Value )
	{
		if( GetPointType() == nType )
		{
			return GetLocalAttrib( nIndex, Value );
		}
		if( GetPointType() == TBase::GetPointType() )	return false;
		return TBase::GetAttrib( nType, nIndex, Value );
	}

	virtual bool SetAttrib( int nType, int nIndex, CXVariant Value )
	{
		if( GetPointType() == nType )
		{
			return SetLocalAttrib( nIndex, Value );
		}
		if( GetPointType() == TBase::GetPointType() )	return false;
		return TBase::SetAttrib( nType, nIndex, Value );
	}

	virtual bool ChangeValue( int nType, int nIndex, CXVariant Value, bool bPersent )
	{
		if( GetPointType() == nType )
		{
			return ChangeLocalValue( nIndex, Value, bPersent );
		}
		if( GetPointType() == TBase::GetPointType() )	return false;
		return TBase::ChangeValue( nType, nIndex, Value, bPersent );
	}

	bool SetLocalAttribListener( int nIndex, AttribValueListener* pListener )
	{
		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
		if( nIndex < 0 || nIndex >= nAttribCount )	return false;
		_attrib[nIndex].SetListener( pListener );
		return true;
	}

	// ȡ��ı�������
	bool GetLocalAttrib( int nIndex, CXVariant& Value )const
	{
		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
		if( nIndex < 0 || nIndex >= nAttribCount )	return false;
		Value = _attrib[nIndex].GetValue();
		return true;
	}

	// ���ñ�������
	bool SetLocalAttrib( int nIndex, const CXVariant& Value )
	{
		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
		if( nIndex < 0 || nIndex >= nAttribCount )	return false;
		_attrib[nIndex].SetValue( Value );
		return true;
	}

	bool ChangeLocalValue( int nIndex, const CXVariant& Value, bool bPersent )
	{
		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
		if( nIndex < 0 || nIndex >= nAttribCount )	return false;
		_attrib[nIndex].ChangeValue( bPersent, Value );
		return true;
	}

	// �ַ������Ա�
	virtual _lpctstr GetStrAttrib( int nType, _lpctstr szName )
	{
		if( GetPointType() == nType )
		{
			CStrAttribMap::const_iterator citer = _strAttrib.find( szName );
			if( citer != _strAttrib.end() )
			{
				const _string& str = citer->second;	return str.c_str();
			}
		}
		if( GetPointType() == TBase::GetPointType() )	return NULL;
		return TBase::GetStrAttrib( nType, szName );
	}

	virtual bool SetStrAttrib( int nType, _lpctstr szName, _lpctstr strValue )
	{
		if( strValue == NULL || szName == NULL )	return false;
		if( GetPointType() == nType )
		{
			_strAttrib[szName] = strValue;	return true;
		}
		if( GetPointType() == TBase::GetPointType() )	return false;
		return TBase::SetStrAttrib( nType, szName, strValue );
	}

	// ���������Ա�
	virtual LPVOID GetBufAttrib( int nType, _lpctstr szName )
	{
		if( GetPointType() == nType )
		{
			CBufAttribMap::const_iterator citer = _bufAttrib.find( szName );
			if( citer != _bufAttrib.end() )
			{
				LPVOID p = citer->second;	return p;
			}
			return NULL;
		}
		if( GetPointType() == TBase::GetPointType() )	return NULL;
		return TBase::GetBufAttrib( nType, szName );
	}

	virtual bool SetBufAttrib( int nType, _lpctstr szName, LPVOID lpData )
	{
		if( GetPointType() == nType )
		{
			_bufAttrib[szName] = lpData;
			return true;
		}
		if( GetPointType() == TBase::GetPointType() )	return NULL;
		return TBase::SetBufAttrib( nType, szName, lpData );
	}

};

/************************************************************************/
/* CXObject �࣬���д��ж������ԵĻ����ࡣ���ﶨ����һ������ϵͳ�Ľӿڣ�����
/* ���˵ײ�ʵ�֡�
/* ͬʱ��CXObject��ʵ�����Զ��б����������ɺ󣬽��Լ���������б��У�����ʼ��
/* ��ǰID
/************************************************************************/
/*	:	public XObjTypeT< CXObjectBase, EN_OBJECT, 1 > */
typedef bool (*EnumChildCallback)( int nID, CXObject* pObj, LPVOID pParam );
class CORE_API CXObject
{
protected:
	CXObject();
	virtual	~CXObject();

private:
	int m_nID;
	int	m_nParentID;	// ������ID

	typedef std::list< int >	CChildList;

	CChildList	m_ChildList;	// �������б�
	_string		m_strName;		// ��������

public:
	void			SetName( _lpctstr lpszName )	{ m_strName = lpszName;			}
	_lpctstr		GetName()const				{ return m_strName.c_str();		}

	const	int		GetObjID()const				{ return m_nID;					}
	static	int		GetPointType()				{ return TypeObjectbase;		}
	virtual int		GetType()const				{ return TypeObjectbase;		}
	virtual bool	IsType( uint32 dwType )const{ return TypeObjectbase == dwType;}
	static	bool	IsTypeOf( uint32 dwType )	{ return TypeObjectbase == dwType;}

	bool	IsKindOf( uint32 dwType )			{ if( this == NULL ) return false;else return IsType( dwType ); }

	virtual bool	SetAttribListener( int nType, int nIndex, AttribValueListener* pListener )	{ return false; }
	virtual bool	GetAttrib( int nType, int nIndex, CXVariant& Value )const					{ return false; }
	virtual bool	SetAttrib( int nType, int nIndex, CXVariant Value )						{ return false; }
	virtual bool	ChangeValue( int nType, int nIndex, CXVariant Value, bool bPersent )		{ return false; }

	virtual _lpctstr	GetStrAttrib( int nType, _lpctstr szName )const					{ return NULL; }
	virtual bool		SetStrAttrib( int nType, _lpctstr szName, _lpctstr strValue )	{ return false;}

	virtual void*	GetBufAttrib( int nType, _lpctstr szName )const					{ return NULL; }
	virtual bool	SetBufAttrib( int nType, _lpctstr szName, LPVOID lpData )		{ return false; }

	/************************************************************************/
	/* �㼶��ϵ����
	/************************************************************************/
	// ���ø�����
	void	SetParent( int nID ) { m_nParentID = nID; }
	// �õ�������ID
	int		GetParent()const { return m_nParentID; }
	// ����Ӷ���ID
	void	AddChild( int nID );
	// ɾ���Ӷ���ID
	void	RemoveChild( int nID, bool bRelease = false );
	// ö���Ӷ���
	void	EnumChild( EnumChildCallback CallbackFunc, LPVOID pParam = NULL );
	/************************************************************************/
	// ���ٶ���
	bool	Destroy();
	// ���������Ӷ���
	void	DestroyAllChild();
protected:
	/************************************************************************/
	/* ���õ�һЩ�¼���Ӧ�麯����
	/************************************************************************/
	virtual bool OnParentChange( int nOldID, int nNewID ){ return true; }
	virtual bool OnAddChild( int nID ){ return true; }
	virtual bool OnRemoveChild( int nID, bool bRelease ){ return true; }
	virtual bool OnDestroy(){ return true; }
	// ���Ա��ʱ���ã�����֪ͨ���Ա��
	// nType	:	���Ե����Ͳ��
	// nIndex	:	���Ե�����
	// nNewValue:	���Ե�ֵ
	// return	:	-ture �������	-false ��ֹ����
	// ��ʱ����
	//virtual bool OnAttribChange( int nType, int nIndex, int nNewValue ){ return true; }
};

/************************************************************************/
/* һ������ϵͳ����ʾ��ͨ��ģ�����������Ժ����׵ļ̳�����ϵͳ���������ظ�����
/* ģ�����ָ�����������࣬ ����ֵ�����������Ը���
/************************************************************************/
//class CXItem	:	public XObjTypeT< CXObject, TypeItem, 20 >
//{
//public:
//	CXItem(){};
//	~CXItem(){};
//};

#endif //_XOBJECT_