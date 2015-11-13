

#ifndef _PAL_NPC_H
#define _PAL_NPC_H

#include "PalPeople.h"


class CPalNpc:public CPalPeople
{
	typedef struct _NpcNode
	{
		D3DXVECTOR3 m_point;   //����
		int       m_time;	   //�ڸõ�ͣ����ʱ��
	} NpcNode;
protected:
	std::vector<_NpcNode> m_path;  //����״̬NPC��;����
	size_t m_pathCount;  //��ǰ���������������
	D3DXVECTOR3 m_routeCurr;
	D3DXVECTOR3 m_route; //�ӵ�һ����ڶ�����ʱ���ñ�����Ϊһ����һ��ָ��ڶ��������
	bool   m_front;      //�Ƿ�����;�������㣨����;���꣬���ᷴ�����ߣ�
	bool   m_stay;       //��ǰ�Ƿ�����ͣ��
	int  m_time;        //���ڵ�ǰ���������ͣ��ʱ��

protected:
	std::string  m_strScritpFile; //npc��Ӧ�Ľű��ļ�
	//typedef list<stTask> TListTask;
	//TListTask  m_listTask;  //��Ϣ�ӽű�archive����
public:
	CPalNpc();
	virtual ~CPalNpc();

public:
	virtual void Reset();
	virtual bool Update(int delta);


	virtual void AddNode(D3DXVECTOR3 pos, int _time);
};

#endif