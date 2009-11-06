#pragma once
//////////////////////////////////////////////////////////////////////////
// Loki ͷ�ļ�
#include "loki\TypeManip.h"
#include "loki\Function.h"
#include "Loki\Singleton.h"
#include "Loki\SmallObj.h"
#include "Loki\TypeTraits.h"
#include "loki\static_check.h"
#include "loki\allocator.h"

#ifdef EXPORT_API
#define EXPORT_LIB __declspec(dllexport)
#else
#define EXPORT_LIB __declspec(dllimport)
#endif

typedef Loki::SmallObject< LOKI_DEFAULT_THREADING_NO_OBJ_LEVEL, 1024*4, 1024, 4 >  CPoolAllocator;

//////////////////////////////////////////////////////////////////////////
// ��ʱ��������
// base		:	��׼ʱ��
//////////////////////////////////////////////////////////////////////////
class EXPORT_LIB CTimerManager
{
public:
	CTimerManager(void);
	~CTimerManager(void);

public:
	void active();
	//////////////////////////////////////////////////////////////////////////
	// ��ʱ���¼��ӿ�
	//////////////////////////////////////////////////////////////////////////
	typedef Loki::Function< bool (unsigned int /*handle*/, unsigned short& /*repeat*/, unsigned int& /*timer*/) > event_function;
	struct ITimerEvent	:	public CPoolAllocator
	{
		friend class CTimerManager;
	protected:
		//------------------------------------------------//
		// [7/28/2008] Write by Albert.xu
		// Description		: ����ʱ���¼�����
		// _function	: ����ָ���������operator()�Ľṹ��
		// _repeat		: �ظ�����, ����ִ��һ��.
		// _timer		: �¼��Ĵ���ʱ����
		//------------------------------------------------//
		ITimerEvent( event_function &_function, unsigned short _repeat, unsigned int _timer )
			: function( _function )
			, handle(-1)
			, repeat(_repeat)
			, timer(_timer)
			, index(0)
		{

		};

		virtual ~ITimerEvent()
		{
			function = NULL;
			handle	= -1;
			repeat	= 0;
			timer	= 0;
			index	= 0;
		}

	public:
		__inline unsigned int get_timer()const	{ return timer; } 
		__inline unsigned int get_handle()const	{ return handle; } 

	private:
		void release(){ delete this; }

		event_function	function;
		unsigned int	handle;	// ���
		unsigned int	timer;	// ʱ����, ��׼֮��ı���
		unsigned short	repeat;	// ִ�д���, ִ�д���Ϊ��Ϊ0 ������
		unsigned short	index;	// ������, �������Ǹ�������

		size_t	round;	// ѭ�����,��ź�TimerManager��ѭ��������ʱ,�ö���ű�ִ��.
	};

	typedef ITimerEvent	event_interface;
public:
	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	��ʼ��ʱ���¼�������
	// length	:	������С,��ֵԽ��Ч��Խ��
	// return	:
	//------------------------------------------------//
	bool initialize( unsigned short length, unsigned short max_event_object );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	�����¼�
	// event	:	�¼�����
	// return	:	�¼����
	//------------------------------------------------//
	__inline unsigned int insert_event( event_interface* event, unsigned int delay = 0 );

	//---------------------------------------------//
	// [11/21/2008 write by albert]
	// ������;	:	insert event
	// function	:	�ص�����,�����ǳ�Ա����,�º�������ָ��
	// repeat	:	�ظ�����
	// timer	:	���ʱ��
	// return	:	��ʱ�����
	//---------------------------------------------//
	unsigned int insert_event( event_function function, unsigned short repeat, unsigned int timer, unsigned int delay = 0 );

	//---------------------------------------------------//
	// [9/17/2009 Albert]
	// Description:	�ӳٴ���ʱ���¼�
	// handle	:	�¼����
	// delay	:	�ӳٶ��ٵδ�
	//---------------------------------------------------//
	bool delay_event( unsigned int handle, unsigned int delay );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	�õ��¼�����
	// handle	:	�¼����
	// return	:	�¼�����,δ�ҵ�����NULL
	//------------------------------------------------//
	event_interface* get_event( unsigned int handle );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	ɾ���¼�
	// event	:	�¼�����
	//------------------------------------------------//
	unsigned int remove_event( event_interface* event );

	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	ɾ���¼�
	// handle	:	�¼����
	//------------------------------------------------//
	__inline unsigned int remove_event( unsigned int handle );

protected:
	//------------------------------------------------//
	// [7/24/2008] Write by Albert.xu
	// Description	:	������󵽶�ʱ���б���
	//	handle	:	ʱ���¼�������
	//------------------------------------------------//
	void inner_insert( unsigned int handle, unsigned int delay = 0 );

private:
	struct Impl;
	Impl* m_Impl;

	struct timer_handle
	{
		unsigned int	position:24;	// λ��λ��
		unsigned int	round:8;		// ��ѯλ��
	};

	// ��ʱ������
	size_t	m_next_list;	// ��һ���б�
	size_t	m_timer_round;	// ��һ��ѭ��

	// ����ر���
	size_t	m_object_count;	// �¼��������
	size_t	m_cur_pos;		// ��ǰ���еĵ�Ԫ
	size_t	m_round;		// ѭ������
};

template< int index >
class CTimeManagerSingleton	:	public CTimerManager
{
friend struct Loki::CreateStatic< CTimeManagerSingleton >;
private:
	CTimeManagerSingleton(): CTimerManager(){}
	~CTimeManagerSingleton(){}
	CTimeManagerSingleton( CONST CTimeManagerSingleton& src );
};

#define INVALID_TIMER_HANDLE (-1)
#define TIMER_IMMEDIATELY (-1)
typedef CTimerManager::event_interface	TimerEvent;
typedef CTimerManager::event_function	TimerFunction;
// #include "TimerManager.inl"