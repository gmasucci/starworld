#ifndef _XOBJECT_
#define _XOBJECT_

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif	//CORE_EXPORTS
#pragma warning( disable:4251 )

#include <map>
#include <set>

#include "XAttribute.h"
/************************************************************************/
/* �б�ϵͳ������ϵͳ���Լ�����������ʵ��                             */
/************************************************************************/
typedef _uint32 identifier;
typedef _uint32	guid;

#define INVALID_OBJID -1
#define INVALID_ATTRIBID -1
#define INVALID_TYPE_VALUE -1
#define INVALID_ATTRIBUTE			(*(CAttribute*)(NULL))
#define INVALID_ATTRIBUTE_VALUE		(*(CXVariant*)(NULL))
#define OBJ_MAX	0xffffff
#define OBJ_MIN	0xffff

#define OBJST_NORMAL	0 
#define OBJST_DESTROY	1

#define GET_LOCAL_VALUE( CLASS, CLASS_PTR, INDEX )		static_cast< CLASS* >( CLASS_PTR )->GetLocalAttrib( INDEX )
#define SET_LOCAL_VALUE( CLASS, CLASS_PTR, INDEX, V )	static_cast< CLASS* >( CLASS_PTR )->SetLocalAttrib( INDEX, V )

#define		TypeObjectbase				0
#define		TypeAction					0x00000001
#define		TypeStatus					0x00000002
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
// ���Զ���
#define DECLARE_DYNAMICTYPE(BaseClass,ClassType)\
	static	unsigned int	GetPointType()					{ return ClassType; }\
	static	bool	IsPointOf( unsigned int dwType )		{ if( GetPointType() == dwType ) return true; else return BaseClass##::IsPointOf( dwType );}\
	virtual unsigned int	GetType()const					{ return ClassType; }\
	virtual bool	IsTypeOf( unsigned int dwType )const	{ if( GetPointType() == dwType ) return true; else return BaseClass##::IsTypeOf( dwType );}

namespace XGC
{
	struct AttribDefine
	{
		int			index;
		_string		name;
		CXVariant	init;
	};
};

#define DECLARE_ATTRIBUTE(ATTRIBUTECOUNT)\
private:\
	static AttribDefine _attribDef[ATTRIBUTECOUNT];\
	CAttrib _attrib[ATTRIBUTECOUNT];\
public:\
	virtual CAttribute GetAttribute( int nType, int nIndex, bool bNotifyListener = false );\
	virtual CXVariant GetAttrib( int nType, int nIndex )const;\
	virtual void SetAttrib( int nType, int nIndex, CXVariant Value, bool bNotifyListener = false );\
	__inline CAttribute GetLocalAttribute( int nIndex, bool bNotifyListener = false );\
	__inline CXVariant GetLocalAttrib( int nIndex )const;\
	__inline void SetLocalAttrib( int nIndex, CXVariant Value, bool bNotifyListener = false );\

#define IMPLEMENT_ATTRIBUTE( ThisClass, BaseClass )\
CAttribute ThisClass##::GetAttribute( int nType, int nIndex, bool bNotifyListener/* = false*/ )\
{\
	if( GetPointType() == nType )\
	{\
		return GetLocalAttribute( nIndex, bNotifyListener );\
	}\
	return BaseClass##::GetAttribute( nType, nIndex, bNotifyListener );\
}\
\
CXVariant ThisClass##::GetAttrib( int nType, int nIndex )const\
{\
	if( GetPointType() == nType )\
	{\
		return GetLocalAttrib( nIndex );\
	}\
	return BaseClass##::GetAttrib( nType, nIndex );\
}\
\
void ThisClass##::SetAttrib( int nType, int nIndex, CXVariant Value, bool bNotifyListener/* = false*/ )\
{\
	if( GetPointType() == nType )\
	{\
		SetLocalAttrib( nIndex, Value, bNotifyListener );\
	}\
	else\
	{\
		BaseClass##::SetAttrib( nType, nIndex, Value, bNotifyListener );\
	}\
}\
\
__inline CAttribute ThisClass##::GetLocalAttribute( int nIndex, bool bNotifyListener/* = false*/ )\
{\
	ASSERT_MSG( nIndex >= 0 && nIndex < _countof(_attrib), _T("����ֵ������Χ") );\
	if( nIndex < 0 || nIndex >= _countof(_attrib) )	return INVALID_ATTRIBUTE;\
	if( bNotifyListener )\
		return CAttribute( _attrib[nIndex], m_Listener, GetPointType(), nIndex );\
	else\
		return CAttribute( _attrib[nIndex] );\
}\
\
__inline CXVariant ThisClass##::GetLocalAttrib( int nIndex )const\
{\
	ASSERT_MSG( nIndex >= 0 && nIndex < _countof(_attrib), _T("����ֵ������Χ") );\
	if( nIndex < 0 || nIndex >= _countof(_attrib) )	return INVALID_ATTRIBUTE_VALUE;\
	return _attrib[nIndex].GetValue();\
}\
\
__inline void ThisClass##::SetLocalAttrib( int nIndex, CXVariant Value, bool bNotifyListener/* = false*/ )\
{\
	ASSERT_MSG( nIndex >= 0 && nIndex < _countof(_attrib), _T("����ֵ������Χ") );\
	if( nIndex < 0 || nIndex >= _countof(_attrib) )	return;\
	_attrib[nIndex].SetValue( Value );\
	if( bNotifyListener && m_Listener )\
	{\
		m_Listener( GetPointType(), nIndex, Value );\
	}\
}\

#define BEGIN_ATTRIBUTE_TABLE( ThisClass, BaseClass )\
	AttribDefine ThisClass##::_attribDef[] =\
	{\

#define DEFINE_ATTRIBUTE( Name, Index, DefaultValue )\
	{ Index, Name, DefaultValue },\

#define END_ATTRIBUTE_TABLE()\
	};\

//////////////////////////////////////////////////////////////////////////

namespace XGC
{
	using namespace common;
	/************************************************************************/
	/* CXObjectT ģ��ʵ��������ϵͳ����ģ����ÿ��������ʵ��һ�������б����ڴ�ȡ
	/* ʱ��Ҫ��ȷ��ȡ��������λ���ĸ������ϵģ���ͨ��һ�������ֵ��ָ��������������������
	/************************************************************************/
	//template < typename ThisClassT, typename BaseClassT, unsigned int ClassType, int nAttribCount >
	//class XObjTypeT	:	public BaseClassT
	//{
	//public:
	//	typedef XObjTypeT BaseClass;
	//protected:
	//#define XOBJECT_REPEAT(N) \
	//	template< CLIB_ENUM_D(N, class T) > \
	//	XObjTypeT( CLIB_PARAM_D(N,T,&P) )\
	//		: BaseClassT( CLIB_ENUM_D(N,P) )\
	//	{\
	//		ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );\
	//		ZeroMemory( _attrib, sizeof( _attrib ) );\
	//	}\
	//
	//// CLIB_MAIN_REPEAT(10,XOBJECT_REPEAT)
	//CLIB_MAIN_REPEAT(10,XOBJECT_REPEAT)
	//#undef SLB_REPEAT
	//#undef XOBJECT_REPEAT

	//XObjTypeT()
	//{
	//	ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );
	//	ZeroMemory( _attrib, sizeof( _attrib ) );
	//}

	//template<>
	//XObjTypeT( const ThisClassT& src )
	//	: BaseClassT( src )
	//{
	//	ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );
	//	memcpy( (void*)_attrib, (void*)src._attrib, sizeof(_attrib) );
	//}

	//private:
	//	// ����ĸ���Ԫ��,�ڵ�һʱ��ȷ����������,֮���ܸ�������.
	//	CAttrib _attrib[nAttribCount];

	//public:
	//	static	_uint32	GetPointType()				{ return ClassType; }
	//	virtual _uint32	GetType()const				{ return ClassType; }
	//	virtual bool	IsTypeOf( _uint32 dwType )const	{ if( GetPointType() == dwType ) return true; else return BaseClassT::IsTypeOf( dwType );}
	//	static	bool	IsPointOf( _uint32 dwType )		{ if( GetPointType() == dwType ) return true; else return BaseClassT::IsPointOf( dwType );}

	//	virtual CAttribute GetAttribute( int nType, int nIndex, bool bNotifyListener = false )
	//	{
	//		if( GetPointType() == nType )
	//		{
	//			return GetLocalAttribute( nIndex, bNotifyListener );
	//		}
	//		return BaseClassT::GetAttribute( nType, nIndex, bNotifyListener );
	//	}

	//	virtual CXVariant GetAttrib( int nType, int nIndex )const
	//	{
	//		if( GetPointType() == nType )
	//		{
	//			return GetLocalAttrib( nIndex );
	//		}
	//		return BaseClassT::GetAttrib( nType, nIndex );
	//	}

	//	virtual void SetAttrib( int nType, int nIndex, CXVariant Value, bool bNotifyListener = false )
	//	{
	//		if( GetPointType() == nType )
	//		{
	//			SetLocalAttrib( nIndex, Value, bNotifyListener );
	//		}
	//		else
	//		{
	//			BaseClassT::SetAttrib( nType, nIndex, Value, bNotifyListener );
	//		}
	//	}

	//	__inline CAttribute GetLocalAttribute( int nIndex, bool bNotifyListener = false )
	//	{
	//		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
	//		if( nIndex < 0 || nIndex >= nAttribCount )	return INVALID_ATTRIBUTE;
	//		if( bNotifyListener )
	//			return CAttribute( _attrib[nIndex], m_Listener, GetPointType(), nIndex );
	//		else
	//			return CAttribute( _attrib[nIndex] );
	//	}

	//	// ȡ��ı�������
	//	__inline CXVariant GetLocalAttrib( int nIndex )const
	//	{
	//		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
	//		if( nIndex < 0 || nIndex >= nAttribCount )	return INVALID_ATTRIBUTE_VALUE;
	//		return _attrib[nIndex].GetValue();
	//	}

	//	// ���ñ�������
	//	__inline void SetLocalAttrib( int nIndex, CXVariant Value, bool bNotifyListener = false )
	//	{
	//		ASSERT_MSG( nIndex >= 0 && nIndex < nAttribCount, _T("����ֵ������Χ") );
	//		if( nIndex < 0 || nIndex >= nAttribCount )	return;
	//		_attrib[nIndex].SetValue( Value );
	//		if( bNotifyListener && m_Listener )
	//		{
	//			m_Listener( GetPointType(), nIndex, Value );
	//		}
	//	}
	//};

	///************************************************************************/
	///* CXObjectT ģ��ʵ��������ϵͳ����ģ����ÿ��������ʵ��һ�������б����ڴ�ȡ
	///* ʱ��Ҫ��ȷ��ȡ��������λ���ĸ������ϵģ���ͨ��һ�������ֵ��ָ��������������������
	///************************************************************************/
	//template < typename ThisClassT, typename BaseClassT, unsigned int ClassType >
	//class XObjOnlyTypeT : public BaseClassT
	//{
	//public:
	//	typedef XObjOnlyTypeT BaseClass;
	//protected:
	//#define XOBJECT_REPEAT(N) \
	//template< CLIB_ENUM_D(N, class T) > \
	//XObjOnlyTypeT( CLIB_PARAM_D(N,T,&P) )\
	//	: BaseClassT( CLIB_ENUM_D(N,P) )\
	//{\
	//	ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );\
	//}\

	//CLIB_MAIN_REPEAT(10,XOBJECT_REPEAT)
	//#undef SLB_REPEAT
	//#undef XOBJECT_REPEAT

	//XObjOnlyTypeT()
	//{
	//	ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );
	//}

	//template<>
	//XObjOnlyTypeT( const ThisClassT& src )
	//	: BaseClassT( src )
	//{
	//	ASSERT_MSG( IsTypeOf( TypeObjectbase ), _T("����ļ̳����͡�") );
	//}

	//public:
	//	static	_uint32	GetPointType()				{ return ClassType; }
	//	virtual _uint32	GetType()const				{ return ClassType; }
	//	virtual bool	IsTypeOf( _uint32 dwType )const	{ if( GetPointType() == dwType ) return true; else return BaseClassT::IsTypeOf( dwType );}
	//	static	bool	IsPointOf( _uint32 dwType )		{ if( GetPointType() == dwType ) return true; else return BaseClassT::IsPointOf( dwType );}
	//};

	/************************************************************************/
	/* CXObject �࣬���д��ж������ԵĻ����ࡣ���ﶨ����һ������ϵͳ�Ľӿڣ�����
	/* ���˵ײ�ʵ�֡�
	/* ͬʱ��CXObject��ʵ�����Զ��б����������ɺ󣬽��Լ���������б��У�����ʼ��
	/* ��ǰID
	/************************************************************************/
	/*	:	public XObjTypeT< CXObjectBase, EN_OBJECT, 1 > */
	class CORE_API CXObject
	{
		enum { Class = 0, Type = 0 };
	protected:
		CXObject();
		CXObject( bool bIsParent, bool bIsTypeList );
		virtual	~CXObject();

	protected:
		const _byte	m_bIsParent;	// �Ƿ�Ϊ������ĸ�
		const _byte	m_bIsTypeList;	// �Ƿ����������

		_byte m_nStatus;	// ��ǰ״̬

		ListenFunction	m_Listener;
		bool IsDestroy()const{ return m_nStatus&OBJST_DESTROY; }
	private:
		identifier	m_nID;
		identifier	m_nParentID;	// ������ID

		typedef std::set< identifier >			CChildList;
		typedef std::map< int, CChildList >		CTypeList;
		typedef std::map< _string, _string >	CStrAttribMap;

		CChildList		m_ChildList;	// �������б�
		CTypeList		m_TypeList;		// ����������
		CStrAttribMap	m_strAttrib;

	public:
		const	identifier	GetObjID()const			{ return m_nID; }
		static	_uint32		GetPointType()			{ return TypeObjectbase; }
		static	bool		IsPointOf( _uint32 dwType )		{ return TypeObjectbase == dwType;}
		virtual _uint32		GetType()const			{ return TypeObjectbase; }
		virtual bool		IsTypeOf( _uint32 dwType )const	{ return TypeObjectbase == dwType;}

		virtual CXVariant	GetAttrib( int nType, int nIndex )const	{ ASSERT( false ); return INVALID_ATTRIBUTE_VALUE; }
		virtual void		SetAttrib( int nType, int nIndex, CXVariant Value, bool bNotifyListener = false ){ ASSERT( false ); }
		virtual CAttribute	GetAttribute( int nType, int nIndex, bool bNotifyListener = false ){ ASSERT( false ); return INVALID_ATTRIBUTE; }

		// �ַ������Ա�
		_lpctstr GetStrAttrib( _lpctstr szName )
		{
			CStrAttribMap::const_iterator citer = m_strAttrib.find( szName );
			if( citer != m_strAttrib.end() )
			{
				const _string& str = citer->second;	
				return str.c_str();
			}
			return NULL;
		}

		bool SetStrAttrib( _lpctstr szName, _lpctstr strValue )
		{
			if( strValue == NULL || szName == NULL )	
				return false;

			m_strAttrib[szName] = strValue;	
			return true;
		}

		// �ַ������Ա�
		bool GetStrAttribA( _lpcstr szName, _string& strValue )
		{
			CStrAttribMap::const_iterator citer = m_strAttrib.find( XA2T( szName ) );
			if( citer != m_strAttrib.end() )
			{
				strValue = citer->second;
				return true;
			}
			return false;
		}

		bool SetStrAttribA( _lpcstr szName, _lpcstr strValue )
		{
			if( szName == NULL )
				return false;

			if( strValue == NULL )
				m_strAttrib.erase( XA2T(szName) );

			m_strAttrib[XA2T(szName)] = XA2T( strValue );
			return true;
		}

		//---------------------------------------------------//
		// [9/17/2009 Albert]
		// Description:	���ü�������
		//---------------------------------------------------//
		void SetAttributeListener( ListenFunction &Fn )
		{
			m_Listener = Fn;
		}

		/************************************************************************/
		/* �㼶��ϵ����
		/************************************************************************/
		// ���ø�����
		void	SetParent( identifier nID ) { m_nParentID = nID; }
		// �õ�������ID
		identifier	GetParent()const { return m_nParentID; }
		// ����Ӷ���ID
		__inline 
		bool	AddChild( identifier nID, bool bChangeParent = true );
		bool	AddChild( CXObject* pObj, bool bChangeParent = true );
		// ɾ���Ӷ���ID
		__inline 
		void	RemoveChild( identifier nID, bool bRelease = false );
		void	RemoveChild( CXObject* pObj, bool bRelease = false );
		// ö���Ӷ���
		template< class _Func >
		_uint32	EnumChild( _Func &CallbackFunc, int nTypeFilter )
		{
			CChildList& child = ( nTypeFilter != INVALID_TYPE_VALUE )?m_TypeList[nTypeFilter]:m_ChildList;

			CChildList::const_iterator cb = child.begin();
			CChildList::const_iterator ce = child.end();
			for( ; cb != ce; ++cb )
			{
				if( CallbackFunc( *cb ) )
					return *cb;
			}

			return INVALID_OBJID;
		}

		/************************************************************************/
		// ���ٶ���
		bool	Destroy();
		// ���������Ӷ���
		void	DestroyAllChild();

		//---------------------------------------------------//
		// [9/21/2009 Albert]
		// Description:	���������Ӷ���
		//---------------------------------------------------//
		void	DestroyAllChild( int nType );

		// ��ȡ�Ӷ�������
		size_t	GetChildrenCount()const{ return m_ChildList.size(); }
		size_t	GetChildrenCount( int nType )const
		{ 
			CTypeList::const_iterator i = m_TypeList.find( nType ); 
			return i == m_TypeList.end()?0:i->second.size();
		}

	private:
		/************************************************************************/
		/* ���õ�һЩ�¼���Ӧ�麯����
		/************************************************************************/

		//---------------------------------------------------//
		// [8/3/2009 Albert]
		// Description:	�����ӽڵ�ǰ����
		// return :		true - ȷ�������ӽڵ�, false - �ӽڵ㱻���,��ӽڵ�ʧ��.
		//---------------------------------------------------//
		virtual bool PreAddChild( CXObject* pChild ){ return true; }

		//---------------------------------------------------//
		// [8/3/2009 Albert]
		// Description:	�����ӽڵ�ǰ����
		// return :		true - ȷ���Ƴ��ӽڵ�, false - �ӽڵ㱻���,�Ƴ��ӽڵ�ʧ��.
		//---------------------------------------------------//
		virtual bool PreRemoveChild( CXObject* pChild, bool bRelease ){ return true; }

		//---------------------------------------------------//
		// [8/3/2009 Albert]
		// Description:	�����ӽڵ�����
		//---------------------------------------------------//
		virtual void OnAddChild( CXObject* pChild ){}

		//---------------------------------------------------//
		// [8/3/2009 Albert]
		// Description:	ɾ���ӽڵ�����,��ʱ������δ��ɾ��
		//---------------------------------------------------//
		virtual void OnRemoveChild( CXObject* pChild, bool bRelease ){}

		//---------------------------------------------------//
		// [8/3/2009 Albert]
		// Description:	����ɾ��ʱ����,��ʱ������δ���ϳ�
		//---------------------------------------------------//
		virtual void OnDestroy(){}
	};

}
#endif //_XOBJECT_