#include "stdafx.h"
#include "timer_manager.h"

struct CTimerManager::Impl
{
	typedef std::list	< unsigned int, ALLOCATOR< unsigned int > >		CTimerEventList;
	typedef std::vector	< CTimerEventList, ALLOCATOR< CTimerEventList > >	CTimerEventListVec;
	typedef std::vector	< event_interface*, ALLOCATOR< event_interface* > >	CTimerEventObjectVec;

	CTimerEventListVec		m_TimerVec;
	CTimerEventObjectVec	m_EventVec;
};

CTimerManager::CTimerManager(void)
: m_object_count(0)
, m_cur_pos(0x01000000)
, m_round(1)
, m_next_list(0)
, m_timer_round(0)
, m_Impl( new Impl() )
{
}

CTimerManager::~CTimerManager(void)
{
	m_Impl->m_TimerVec.clear();
	for( size_t i = 0; i < m_Impl->m_EventVec.size(); ++i )
	{
		TimerEvent* event = m_Impl->m_EventVec.at(i);
		delete event;
	}
	m_Impl->m_EventVec.clear();

	SAFE_DELETE( m_Impl );
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	��ʼ��ʱ���¼�������
// length	:	������С,��ֵԽ��Ч��Խ��
// return	:
//------------------------------------------------//
bool CTimerManager::initialize( unsigned short length, unsigned short max_event_object )
{
	m_Impl->m_TimerVec.resize( length );
	m_Impl->m_EventVec.resize( max_event_object, 0 );
	return true;
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	�����¼�
// event	:	�¼�����
// return	:	�¼����
//------------------------------------------------//
unsigned int CTimerManager::insert_event( event_interface* event, unsigned int delay )
{
	if( m_object_count == m_Impl->m_EventVec.size() ) return -1;
	if( !event ) return -1;

	if( delay == TIMER_IMMEDIATELY )
	{
		// ����ִ�е�timer�¼�
		bool ret = event->function( INVALID_TIMER_HANDLE, event->repeat, event->timer );
		if( ret || event->repeat == 0 )
		{
			return INVALID_TIMER_HANDLE;
		}
	}

	// ������ע�ᵽ�������
	event->handle = (unsigned int)m_cur_pos;
	timer_handle& h = (timer_handle&)m_cur_pos;

	m_Impl->m_EventVec[h.position] = event;
	do
	{
		++h.position;
		if( h.position >= m_Impl->m_EventVec.size() )
		{
			++h.round;
			++m_round;
			h.position = 0;
		}
	}while( m_Impl->m_EventVec[h.position] != NULL );

	++m_object_count;

	// �������е����ݲ��뵽ˢ���б���
	inner_insert( event->handle, delay );
//	printf("******************* inner_insert(%d)\n", event->handle);
	return event->handle;
}

//---------------------------------------------//
// [11/21/2008 write by albert]
// ������;	:	insert event
// function	:	�ص�����,�����ǳ�Ա����,�º�������ָ��
// repeat	:	�ظ�����
// timer	:	���ʱ��
// return	:	��ʱ�����
//---------------------------------------------//
unsigned int CTimerManager::insert_event( event_function function, unsigned short repeat, unsigned int timer, unsigned int delay )
{
	return insert_event( new TimerEvent( function, repeat, timer ), delay );
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	�õ��¼�����
// handle	:	�¼����
// return	:	�¼�����,δ�ҵ�����NULL
//------------------------------------------------//
CTimerManager::event_interface* CTimerManager::get_event( unsigned int handle )
{
	timer_handle& h = (timer_handle&)handle;
	if( h.position < m_Impl->m_EventVec.size() )
	{
		ITimerEvent* event = m_Impl->m_EventVec[h.position];
		if( event && event->handle == handle )
		{
			return event;
		}
	}
	return NULL;
}

//---------------------------------------------------//
// [9/17/2009 Albert]
// Description:	�ӳٴ���ʱ���¼�
// handle	:	�¼����
// delay	:	�ӳٶ��ٵδ�
//---------------------------------------------------//
bool CTimerManager::delay_event( unsigned int handle, unsigned int delay )
{
	timer_handle& h = (timer_handle&)handle;
	if( h.position < m_Impl->m_EventVec.size() )
	{
		ITimerEvent* event = m_Impl->m_EventVec[h.position];
		if( event && event->handle == handle )
		{
			inner_insert( handle, event->timer + delay );
			return true;
		}
	}

	return false;
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	ɾ���¼�
// event	:	�¼�����
//------------------------------------------------//
unsigned int CTimerManager::remove_event( event_interface* event )
{
	if( event )
	{
		timer_handle& h = (timer_handle&)event->handle;
		event_interface* _event = m_Impl->m_EventVec[h.position];
		if( _event == event )
		{
			// ����ʣ��δ�
			unsigned int ret = event->index - m_next_list + ( event->round - m_timer_round )*m_Impl->m_TimerVec.size();

			m_Impl->m_EventVec[h.position] = NULL;
			event->release();
			--m_object_count;

			return ret;
		}
	}
	return 0;
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	ɾ���¼�
// handle	:	�¼����
//------------------------------------------------//
unsigned int CTimerManager::remove_event( unsigned int handle )
{
	ITimerEvent* event = get_event( handle );
	return remove_event( event );
}

///function	:	��ʱ������
///return	:	��
void CTimerManager::active()
{
	Impl::CTimerEventList& timer_list = m_Impl->m_TimerVec[m_next_list++];
	Impl::CTimerEventList::iterator iter = timer_list.begin();
	while( iter != timer_list.end() )
	{
		Impl::CTimerEventList::iterator olditer = iter++;
		unsigned int handle = *olditer;
		ITimerEvent* event = get_event( handle );
		if( !event )
		{
			timer_list.erase( olditer );
			continue;
		}

		if( event->round == m_timer_round )
		{
			unsigned int handle = event->handle;
			bool ret = event->function( handle, event->repeat==-1?event->repeat:--event->repeat, event->timer);
			// [9/18/2008 Albert]
			// �п����Ѿ���ɾ����, ������Ҫ�ж��Ƿ�ɾ��.
			if( get_event( handle ) )
			{
				if( ret || event->repeat == 0 )
				{
					remove_event( event );
				}
				else
				{
					inner_insert( event->handle, event->timer );
				}
			}
			timer_list.erase( olditer );
		}
	}

	if( m_next_list >= m_Impl->m_TimerVec.size() )
	{
		++m_timer_round;
		m_next_list = 0;
	}
}

//------------------------------------------------//
// [7/24/2008] Write by Albert.xu
// Description	:	������󵽶�ʱ���б���
//	handle	:	ʱ���¼�������
//------------------------------------------------//
void CTimerManager::inner_insert( unsigned int handle, unsigned int delay )
{
	ITimerEvent* event = get_event( handle );
	ASSERT( event );
	if( !event ) 
		return;
	
	size_t index = delay + m_next_list;
	size_t round = m_timer_round + index/m_Impl->m_TimerVec.size();
	index = index%m_Impl->m_TimerVec.size();
	event->index = (unsigned short)index;
	event->round = (unsigned short)round;
	m_Impl->m_TimerVec[index].push_back( handle );
}