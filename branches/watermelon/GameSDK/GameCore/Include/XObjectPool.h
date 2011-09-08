#pragma once
#include "XObject.h"
namespace XGC
{
	class CXObject;

	//----------------------------------------------------------------------//

	/************************************************************************/
	/* CXObjectPool �࣬ʵ����һ����̬��ָ���б�����ͨ��ID�ķ�ʽת������ָ�롣
	/************************************************************************/
	class CORE_API CXObjectPool
	{
		friend class CXObject;
	protected:
		CXObjectPool( size_t nLen );
		~CXObjectPool();

		identifier	AddObj( CXObject* pObj );
		void		DelObj( identifier nID );

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
		inline bool IsValidID( identifier id );

		//--------------------------------------------------------//
		//	created:	24:11:2009   15:02
		//	filename: 	d:\Developed\StarGame\GameSDK\GameCore\Include\XObjectList.h
		//	author:		Albert.xu
		//
		//	purpose:	ͨ��ID��ȡ����
		//--------------------------------------------------------//
		inline CXObject* GetObj( identifier id )
		{ 
			object_handle &h = (object_handle&)id;
			return IsValidID( id )?m_pObjectPool[h.position]:NULL; 
		}

		//--------------------------------------------------------//
		//	created:	24:11:2009   15:03
		//	filename: 	d:\Developed\StarGame\GameSDK\GameCore\Include\XObjectList.h
		//	author:		Albert.xu
		//
		//	purpose:	ͨ��id��ȡ����ͬʱ�������
		//--------------------------------------------------------//
		inline CXObject* GetObj( identifier id, guid type );

		inline CXObject* operator []( identifier id )
		{ 
			object_handle &h = (object_handle&)id;
			return m_pObjectPool[h.position]; 
		}
	};
}