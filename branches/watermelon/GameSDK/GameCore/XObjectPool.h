#pragma once
#include "boost/type_traits.hpp"
namespace XGC
{
	class CXObject;

	//----------------------------------------------------------------------//

	/************************************************************************/
	/* CXObjectPool �࣬ʵ����һ����̬��ָ���б�����ͨ��ID�ķ�ʽת������ָ�롣
	/************************************************************************/
	#define ObjectPool	CXObjectPool::GetInstance()
	class CORE_API CXObjectPool
	{
		friend class CXObject;
	protected:
		CXObjectPool( size_t nLen );
		~CXObjectPool();

		xObject	AddObject( CXObject* pObj );
		void	DelObject( xObject nID );

		struct object_handle
		{
			unsigned int	position:24;	// λ��λ��
			unsigned int	round:7;		// ��ѯλ��
			unsigned int	flag:1;			// ����λ�ñ�־, 1 Ϊ������ʹ��, 0 Ϊ�ͻ���ʹ��.�������ϵĶ���,���λ������ʹ��0;
		};

		CXObject**	m_pObjectPool;
		size_t		m_nLen;
		size_t		m_nCurID;
		size_t		m_nRound;
		size_t		m_nCount;
	public:
		static CXObjectPool& GetInstance();
		inline bool IsValidObject( xObject id );

		//--------------------------------------------------------//
		//	created:	24:11:2009   15:02
		//	filename: 	d:\Developed\StarGame\GameSDK\GameCore\Include\XObjectList.h
		//	author:		Albert.xu
		//
		//	purpose:	ͨ��ID��ȡ����
		//--------------------------------------------------------//
		inline CXObject* GetObject( xObject id )
		{ 
			object_handle &h = (object_handle&)id;
			return IsValidObject( id )?m_pObjectPool[h.position]:NULL; 
		}

		//--------------------------------------------------------//
		//	created:	24:11:2009   15:03
		//	filename: 	d:\Developed\StarGame\GameSDK\GameCore\Include\XObjectList.h
		//	author:		Albert.xu
		//
		//	purpose:	ͨ��id��ȡ����ͬʱ�������
		//--------------------------------------------------------//
		inline CXObject* GetObject( xObject id, xType type );

		//---------------------------------------------------//
		// [12/11/2010 Albert]
		// Description:	ͨ��id��ȡ���� 
		//---------------------------------------------------//
		template< typename ObjectType >
		ObjectType* GetObject( xObject id )
		{
			CXObject* pObject = GetObject( id );
			if( pObject && pObject->IsTypeOf( ObjectType::GetPointType() ) )
				return static_cast< ObjectType* >( pObject );
			else
				return xgc_nullptr;
		}
	};
}