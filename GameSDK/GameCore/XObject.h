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

/************************************************************************/
/* �б�ϵͳ������ϵͳ���Լ�����������ʵ��                             */
/************************************************************************/
typedef _uint32 xObject;
typedef _uint32	xType;
typedef _uint32	xIndex;
typedef void*	xPosition;

#include "XAttribute.h"
#include "XObjectPool.h"

#define INVALID_OBJECT_ID -1
#define INVALID_ATTRIBID -1
#define INVALID_TYPE_VALUE -1
#define INVALID_ATTRIBUTE			(*(CAttribute*)(xgc_nullptr))
#define INVALID_ATTRIBUTE_VALUE		(*(CXVariant*)(xgc_nullptr))
#define OBJ_MAX	0xfffffe	// Ϊ��ֹ����0xffffffff��idֵ����ֵΪ�Ƿ�ID
#define OBJ_MIN	0xffff

#define OBJST_NORMAL	0 
#define OBJST_DESTROY	1

#define GET_LOCAL_VALUE( CLASS, CLASS_PTR, INDEX )		static_cast< CLASS* >( CLASS_PTR )->GetLocalAttrib( INDEX )
#define SET_LOCAL_VALUE( CLASS, CLASS_PTR, INDEX, V )	static_cast< CLASS* >( CLASS_PTR )->SetLocalAttrib( INDEX, V )

#define		TypeObjectbase				0
#define		TypeAction					0x00000001
#define		TypeStatus					0x00000002
/************************************************************************/
namespace XGC
{
	#define RUNTIME_CLASS(class_name) (class_name::GetThisClass())
	#define _RUNTIME_CLASS(class_name) ((RuntimeClass*)(&class_name::class##class_name))

	struct RuntimeClass
	{
		// Attributes
		_lpcstr m_lpszClassName;
		int m_nObjectSize;
		RuntimeClass* m_pBaseClass;

		// Operations
		bool IsDerivedFrom(const RuntimeClass* pBaseClass) const;

		//// dynamic name lookup and creation
		//static RuntimeClass* PASCAL FromName(LPCSTR lpszClassName);
		//static RuntimeClass* PASCAL FromName(LPCWSTR lpszClassName);

		// CRuntimeClass objects linked together in simple list
		RuntimeClass* m_pNextClass;       // linked list of registered classes
	};
}

//////////////////////////////////////////////////////////////////////////
// ���Զ���
#define DECLARE_DYNAMIC(class_name) \
protected: \
	static RuntimeClass* GetBaseClass(); \
public: \
	static const RuntimeClass class##class_name; \
	static RuntimeClass* GetThisClass(); \
	virtual RuntimeClass* GetRuntimeClass() const; \

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
	RuntimeClass* class_name::GetBaseClass() \
	{ return RUNTIME_CLASS(base_class_name); } \
	const RuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), RUNTIME_CLASS(base_class_name), NULL }; \
	RuntimeClass* class_name::GetThisClass() \
	{ return _RUNTIME_CLASS(class_name); } \
	RuntimeClass* class_name::GetRuntimeClass() const \
	{ return _RUNTIME_CLASS(class_name); }

#define DECLARE_DYNAMICTYPE(BaseClass,ClassType)\
public:\
	static	xType	GetPointType()				{ return ClassType; }\
	static	bool	IsPointOf( xType type )		{ if( GetPointType() == type ) return true; else return BaseClass##::IsPointOf( type );}\
	virtual xType	GetType()const				{ return ClassType; }\
	virtual bool	IsTypeOf( xType type )const	{ if( GetPointType() == type ) return true; else return BaseClass##::IsPointOf( type );}

#define DECLARE_ATTRIBUTE(ATTRIBUTECOUNT)\
private:\
	CAttrib _attrib[ATTRIBUTECOUNT];\
	struct InitializeObject\
	{\
		InitializeObject( xType mType );\
	};\
	static InitializeObject _InitializeObject;\
public:\
	virtual CAttribute GetAttribute( xType nType, bool bNotifyListener = false );\
	virtual CXVariant GetAttrib( xType nType, int nIndex );\
	virtual void SetAttrib( xType nType, int nIndex, const CXVariant& Value );\
	__inline CAttribute GetLocalAttribute( bool bNotifyListener = false )\
	{\
		if( bNotifyListener )\
			return CAttribute( _attrib, _countof(_attrib), GetPointType(), m_Listener );\
		else\
			return CAttribute( _attrib, _countof(_attrib), GetPointType() );\
	}\
	__inline CXVariant GetLocalAttrib( int nIndex )const\
	{\
		ASSERT_MSG( nIndex >= 0 && nIndex < _countof(_attrib), _T("����ֵ������Χ") );\
		if( nIndex < 0 || nIndex >= _countof(_attrib) )	return INVALID_ATTRIBUTE_VALUE;\
		return _attrib[nIndex].GetValue();\
	}\
	__inline void SetLocalAttrib( int nIndex, const CXVariant& Value, bool bNotifyListener = false )\
	{\
		ASSERT_MSG( nIndex >= 0 && nIndex < _countof(_attrib), _T("����ֵ������Χ") );\
		if( nIndex < 0 || nIndex >= _countof(_attrib) )	return;\
		CXVariant xOldValue = _attrib[nIndex].GetValue();\
		_attrib[nIndex].SetValue( Value );\
		if( bNotifyListener && m_Listener )\
		{\
			m_Listener( GetPointType(), nIndex, Value, xOldValue );\
		}\
	}\

#define BEGIN_IMPLEMENT_ATTRIBUTE( ThisClass, BaseClass )\
ThisClass##::InitializeObject ThisClass##::_InitializeObject( ThisClass##::GetPointType() );\
CAttribute ThisClass##::GetAttribute( xType nType, bool bNotifyListener/* = false*/ )\
{\
	if( GetPointType() == nType )\
	{\
		return GetLocalAttribute( bNotifyListener );\
	}\
	return BaseClass##::GetAttribute( nType, bNotifyListener );\
}\
\
CXVariant ThisClass##::GetAttrib( xType nType, int nIndex )\
{\
	if( GetPointType() == nType )\
	{\
		return GetLocalAttrib( nIndex );\
	}\
	return BaseClass##::GetAttrib( nType, nIndex );\
}\
void ThisClass##::SetAttrib( xType nType, int nIndex, const CXVariant& Value )\
{\
	if( GetPointType() == nType )\
	{\
		return SetLocalAttrib( nIndex, Value );\
	}\
	return BaseClass##::SetAttrib( nType, nIndex, Value );\
}\
ThisClass##::InitializeObject::InitializeObject( xType mType )\
{\

#define MAP_ATTRIBUTE( Index )\
	AttributeNameMap::Instance().insert( std::make_pair( _T(#Index), xAttribute( mType, Index) ) );\

#define END_IMPLEMENT_ATTRIBUTE()\
}\

//////////////////////////////////////////////////////////////////////////

namespace XGC
{
	using namespace common;

	struct CORE_API xAttribute
	{
		xAttribute( xType _type, xIndex _index )
			: type( _type )
			, index( _index )
		{

		}

		xType	type;
		xIndex	index;
	};

	struct CORE_API AttributeNameMap : public  SingletonHolder< xgc_map< _string, xAttribute >, CreateUsingNew, DeletableSingleton >
	{

	};
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
		CXObject( bool bUseChildList = true, bool bUseStrAttrib = true, bool bUseValAttrib = true );
		virtual	~CXObject();

	private:
		CXObject( const CXObject& rsh );
		typedef std::list< xObject >			CChildList;
		typedef std::map< _string, _string >	CStrAttribMap;
		typedef std::map< _string, CXVariant >	CValAttribMap;

		CChildList*		m_ChildList;	// �������б�
		CStrAttribMap*	m_strAttrib;
		CValAttribMap*	m_valAttrib;

	protected:
		xObject	m_nID;
		xObject	m_nParentID;	// ������ID
		_uint32		m_nStatus;		// ��ǰ״̬

		ListenFunction	m_Listener;

		typedef CChildList::iterator		iterator;
		typedef CChildList::const_iterator	const_iterator;
	public:
		//static const RuntimeClass classCXObject;

		//static RuntimeClass* GetBaseClass();
		//static RuntimeClass* GetThisClass();

		//virtual RuntimeClass* GetRuntimeClass() const;
		//bool IsKindOf( const RuntimeClass* pRuntimeClass )const;

		const	xObject	GetObjectID()const			{ return m_nID; }
		static	xType	GetPointType()				{ return TypeObjectbase; }
		static	bool	IsPointOf( xType nType )	{ return TypeObjectbase == nType;}
		virtual xType	GetType()const				{ return TypeObjectbase; }
		virtual bool	IsTypeOf( xType nType )const{ return TypeObjectbase == nType;}

		virtual CAttribute	GetAttribute( xType nType, bool bNotifyListener = false ){ ASSERT( false ); return INVALID_ATTRIBUTE; }
		virtual CXVariant	GetAttrib( xType nType, int nIndex ){ return 0; }
		virtual void		SetAttrib( xType nType, int nIndex, const CXVariant& Value ){}

		//---------------------------------------------------//
		// [11/17/2010 Albert]
		// Description:	ʹ�����ݴ洢 
		//---------------------------------------------------//
		void Using( bool bUseChildList, bool bUseStrAttrib, bool bUseValAttrib );

		// �ַ������Ա�
		_lpctstr GetStrAttrib( _lpctstr szName )
		{
			if( m_strAttrib )
			{
				CStrAttribMap::const_iterator citer = m_strAttrib->find( szName );
				if( citer != m_strAttrib->end() )
				{
					const _string& str = citer->second;	
					return str.c_str();
				}
			}
			return xgc_nullptr;
		}

		bool SetStrAttrib( _lpctstr szName, _lpctstr strValue )
		{
			if( m_strAttrib == xgc_nullptr )
				return false;

			if( strValue == xgc_nullptr || szName == xgc_nullptr )	
				return false;

			(*m_strAttrib)[szName] = strValue;
			return true;
		}

		// ��ֵ���Ա�
		bool GetValueAttrib( _lpctstr szName, CXVariant& xVal )
		{
			if( m_valAttrib )
			{
				CValAttribMap::const_iterator citer = m_valAttrib->find( szName );
				if( citer != m_valAttrib->end() )
				{
					xVal = citer->second;
					return true;
				}
			}
			return false;
		}

		void SetValueAttrib( _lpctstr szName, const CXVariant& Value )
		{
			if( m_valAttrib == xgc_nullptr )
				return;

			if( Value == xgc_nullptr || szName == xgc_nullptr )	
				return;

			(*m_valAttrib)[szName] = Value;
			return;
		}

		//---------------------------------------------------//
		// [9/17/2009 Albert]
		// Description:	���ü�������
		//---------------------------------------------------//
		void SetAttributeListener( const ListenFunction &Fn )
		{
			m_Listener = Fn;
		}

		/************************************************************************/
		/* �㼶��ϵ����
		/************************************************************************/
		// ���ø�����
		void	SetParent( xObject nID ) { m_nParentID = nID; }
		// �õ�������ID
		xObject	GetParent()const { return m_nParentID; }
		// ����Ӷ���ID
		__inline 
		bool	AddChild( xObject nID, bool bChangeParent = true );
		bool	AddChild( CXObject* pObj, bool bChangeParent = true );
		// ɾ���Ӷ���ID
		__inline 
		void	RemoveChild( xObject nID, bool bRelease = false );
		void	RemoveChild( CXObject* pObj, bool bRelease = false );

		bool	QueryChild( CXObject* pObj );
		bool	QueryChild( xObject nID );

		template< class _Pd >
		xObject FindChild( _Pd Fn )
		{
			for( iterator i = begin(); i != end(); ++i )
			{
				if( Fn( *i ) )
					return *i;
			}

			return INVALID_OBJECT_ID;
		}
		/************************************************************************/
		// ���ٶ���
		void	Destroy();
		// ���������Ӷ���
		void	DestroyAllChild();

		// ��ȡ�Ӷ�������
		size_t	GetChildrenCount()const{ return m_ChildList?m_ChildList->size():0; }

	protected:
		iterator begin(){ return m_ChildList->begin(); };
		const_iterator begin()const{ return m_ChildList->begin(); };
		iterator end(){ return m_ChildList->end(); }
		const_iterator end()const{ return m_ChildList->end(); }

	protected:
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