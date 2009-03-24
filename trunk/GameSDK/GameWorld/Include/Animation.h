#pragma once
// ����������ͨ��һ����ԴͼƬ����һ�鶯����
// ֡��Ӧ�У���ͨ��NextFrame����������֡
// ״̬��Ӧ�У�ͨ������״̬�����л���ͬ��״̬��
// ͬʱ�����ڶ���Ԫ���п���һ�γ����ڶ����ͬ�����ϣ������ڶ��������������һ�����ü�����
// ���ҽ���������Ϊ0ʱ�ͷŶ�������
class CSurface;
class CAnimation
{
protected:
	uint32	m_dwResID;			// ��ԴID
	int		m_nCurFrame;		// ��ǰ֡��	��Ӧ��
	int		m_nCurState;		// ��ǰ״̬�� ��Ӧ��

	uint32	m_dwAniSpeed;		// �����ٶȣ�1/1000��Ϊ��λ��
	uint32	m_dwOldTime;		// ��֡����ʱ��ʱ�䡣
	int		m_nRef;

protected:
	friend class CMap;
	// �����Ҫ������ĳ�˽�У����ҽ���CMap���Դ�����Ⱦ�������е���Ⱦ��Ҳ��ͨ��Map���й���
	// ������ÿ�η�֡�Ϳ���ͨ����ͼ���߼�һ�ν��У����ҵ�ͼ�ϵĶ��������Թ���һ����Ⱦ����
	CAnimation(void);
	~CAnimation(void);

public:
	virtual BOOL	CreateAni( LPCSTR lpszPathName );
	virtual void	SetState( int nState )			{ m_nCurState = nState;				}
	virtual void	SetCurFrame( int nFrame )		{ m_nCurFrame = nFrame;				} 

	//		��������ٶ�Ϊ0�򲻽��л�֡
	virtual void	SetAniSpeed( uint32 dwAniSpeed )	{ m_dwAniSpeed = dwAniSpeed;		}
	
	virtual int		GetCurFrame()				{ return m_nCurFrame; }
	virtual void	FrameReset()				{ m_nCurFrame = 0; }
	virtual BOOL	Update( float fDeltaTime );

	int		AddRef()	{ m_nRef++; return m_nRef; }
	int		Release()	{ m_nRef--; if( m_nRef <= 0 ){	delete this; return 0;	} else return m_nRef; }
};
