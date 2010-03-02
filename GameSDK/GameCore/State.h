#pragma once

template< class E >
class CState
{
public:
	CState(void);
	virtual ~CState(void);
	//--------------------------------------------------------//
	//	created:	21:1:2010   14:33
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬ǰ����
	//	pEntity:	����״̬�Ķ���ָ��
	//	return:		TRUE ����״̬�ɹ��� false����״̬ʧ��
	//--------------------------------------------------------//
	virtual bool OnEnter( E *pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   14:34
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬
	//	pEntity:	����״̬�Ķ���ָ��
	//--------------------------------------------------------//
	virtual void OnUpdate( E *pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   17:02
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�ж�
	//--------------------------------------------------------//
	virtual void OnInterrupt( E* pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   17:02
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϻָ��ֳ�
	//--------------------------------------------------------//
	virtual void OnIret( E* pEntity ) = 0;

	//--------------------------------------------------------//
	//	created:	21:1:2010   14:35
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�뿪״̬
	//	pEntity:	����״̬�Ķ���ָ��
	//--------------------------------------------------------//
	virtual void OnLeave( E *pEntity ) = 0;
};

template< class E, int C = 8 >
class CStateTemplate
{
public:
	typedef E _entity;

	typedef CState< _entity >				_state;	// ״̬
	typedef CStateTemplate< _entity, C >	_this;

	typedef Loki::Function< _uint8 ( _entity* ) >	_guardfun;// �ػ�����
	typedef std::list< _guardfun >	_guardlist; // �ػ������б�

public:
	CStateTemplate( _entity *pOwner )
		: m_pOwner( pOwner )
		, m_pCurStage( NULL )
	{
		memset( m_StatePool, 0, sizeof(m_StatePool) );
	}

	~CStateTemplate()
	{

	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬ӳ���
	//--------------------------------------------------------//
	bool MapState( _uint8 nIndex, _state *pState )
	{
		if( nIndex >= _countof(m_StatePool) ) 
			return false;

		m_StatePool[nIndex] = pState;
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	ע��״̬Ǩ��·�����ػ�����
	//--------------------------------------------------------//
	bool RegistStateGuard( _uint8 nIndex, _guardfun& Func )
	{
		if( nIndex >= _countof( m_StatePool ) )
			return false;

		m_Guards[nIndex].push_back( Func );
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	����״̬��
	//--------------------------------------------------------//
	void Update()
	{
		if( !m_pCurStage )
			return;

		_state* pState = m_StatePool[m_pCurStage->index];
		if( pState )
		{
			pState->OnUpdate( m_pOwner );
			_guardlist::iterator i = m_Guards[m_pCurStage->index].begin();
			while( i != m_Guards[m_pCurStage->index].end() )
			{
				_uint8 nNewState = (*i)( m_pOwner );
				if( nNewState != -1 )
				{
					ChangeTo( nNewState );
					break;
				}
				++i;
			}
		}
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:10
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	������Ǩ��״̬
	//--------------------------------------------------------//
	bool ChangeTo( _uint8 nIndex )
	{
		if( nIndex >= _countof(m_StatePool) )
			return false;

		if( m_pCurStage == NULL )
			m_pCurStage = new stage;
		_state *pOldState = m_StatePool[m_pCurStage->index];
		_state *pNewState = m_StatePool[nIndex];
		if( pNewState == NULL )
			return false;

		if( pNewState->OnEnter( m_pOwner ) == false )
			return false;

		if( pOldState )
			pOldState->OnLeave( m_pOwner );

		m_pCurStage->index = nIndex;
		m_pCurStage->interrupt = false;
		return true;
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:11
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϵ�ǰ״̬�����жϵ�״̬���Զ�����
	//				��ǰ״̬�������ģ��ȵ��ж��˳���ʱ�򱻻ָ���
	//--------------------------------------------------------//
	bool Interrupt( _uint8 nIndex )
	{

		if( nIndex >= _countof(m_StatePool) )
			return false;

		_state *pOldState  = m_StatePool[m_pCurStage->index];
		_state *pNewState = m_StatePool[nIndex];

		if( pNewState == NULL )
			return false;

		if( pNewState->OnInterrupt( m_pOwner ) == false )
			return false;

		pstage s = new stage;
		s->index = m_pCurStage->index;
		s->interrupt = m_pCurStage->interrupt;
		s->next = m_CurStage->next;

		m_pCurStage
		return true;
	}

	//--------------------------------------------------------//
	//	created:	22:1:2010   14:14
	//	filename: 	State
	//	author:		Albert.xu
	//
	//	purpose:	�жϷ��أ����жϵ�״̬Ҫ�����жϷ��غ����л���ԭ�ȵ�״̬��
	//--------------------------------------------------------//
	bool Iret()
	{
		if( m_ss.empty() )
			return false;

		_uint8 nIndex = m_ss.top();
		m_ss.pop();

		if( nIndex >= _countof(m_StatePool) )
			return false;

		_state *pOldState  = m_StatePool[nIndex];
		_state *pCurState  = m_StatePool[m_pCurStage->index];

		// ֪ͨ��ǰ״̬���жϹ����з���
		if( pCurState )
			pCurState->OnIret( m_pOwner );

		m_pCurStage->index = nIndex;

		return true;
	}

private:
	typedef struct _stage
	{
		_uint8	interrupt;
		_uint8	index;

		struct _stage *next;
	}stage, *pstage;

	_entity		*m_pOwner;
	_state		*m_StatePool[C];
	_guardlist	m_Guards[C];

	pstage		m_pCurStage;
};
