#pragma once
#ifndef _XQUEST_H_
#define _XQUEST_H_

class CXQuest;
class CXQuestObjective	:	public XObjOnlyTypeT< CXQuestObjective, CXObject, TypeQuestObjective >
{
friend class CXQuest;
public:
	enum objective_type
	{
		type_kill,	// ɱ������
		type_item,	// �õ���Ʒ
	};
	CXQuestObjective( _string strName, _uint16 nFinishCount );
	~CXQuestObjective(void);

	int OnKill( _uint32 hSource, long_ptr lParam );
	int OnItem( _uint32 hSource, long_ptr lParam );

protected:
	// ���Ӽ���,����������ɼ�����ͬ������Ŀ����.
	bool Increment();

private:
	//objective_type	m_ObjectiveType;	// Ŀ������
	_string			m_strName;			// ��������
	_uint16			m_nSerial;			// ���
	_uint16			m_nCount;			// ��ǰ����
	_uint16			m_nFinishCount;		// ��ɼ���
};

class CXQuest :	public XObjOnlyTypeT< CXQuest, CXObject, TypeQuest >
{
friend class CXQuestObjective;
public:
	CXQuest(void);
	~CXQuest(void);

protected:
	//---------------------------------------------------//
	// [9/15/2009 Albert]
	// Description:	�������Ŀ��ʱ����
	//---------------------------------------------------//
	virtual void OnAddChild( CXObject* pChild );

	//---------------------------------------------------//
	// [9/15/2009 Albert]
	// Description:	�������ʱ����
	//---------------------------------------------------//
	virtual void OnQuestFinished() = 0;

	//---------------------------------------------------//
	// [9/15/2009 Albert]
	// Description:	֪ͨ����Ŀ�����
	//---------------------------------------------------//
	void ObjectiveNotify( _uint16 nSerial );

private:
	_uint16	m_FinishMask;	// �������
	_uint16 m_Mask;			// ��ǰ����
};
#endif // _XQUEST_H_