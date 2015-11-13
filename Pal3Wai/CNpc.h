
#pragma once
/*
npc��
���ߣ�BruceKen
*/

#include "../Source/CModelXAni.h"


enum
{
	NPC_STAND,  //վ��״̬��NPC
	NPC_WALK,   //����״̬��NPC
};
typedef struct _NpcNode
{
	D3DXVECTOR3 m_point;   //����
	int       m_time;	   //�ڸõ�ͣ����ʱ��
} NpcNode;
class CNpc:public CModelXAni
{
protected:
	int m_type;  //NPC���ͣ���վ�ŵĻ��ǻ������ߵ�
	std::vector<_NpcNode> m_path;  //����״̬NPC��;����
	size_t m_pathCount;  //��ǰ���������������
	bool   m_front;      //�Ƿ�����;�������㣨����;���꣬���ᷴ�����ߣ�
	bool   m_stay;       //��ǰ�Ƿ�����ͣ��
	int  m_time;        //���ڵ�ǰ���������ͣ��ʱ��
	int  m_standIndex;  //վ��Index
	int  m_walkIndex;   //����Index

public:
	CNpc(LPDIRECT3DDEVICE9 device);
	~CNpc();

public:
	virtual void Update();
	virtual D3DXVECTOR3 GetNextNode();  //���Ҫǰ�����Ǹ��ڵ����� 
	virtual bool GetStay();             //��ǰ�Ƿ���ͣ��
	virtual void SetType(int _type);
	virtual void SetIndex(int stand, int walk);
	virtual void ChangeWalkIndex();
	virtual void ChangeStandIndex();
	virtual void AddNode(float posX, float posY, float posZ, int _time);  //���npc�ڵ�ʱ�����Ƿ񵽴ﲻ��y����Ϊ���λ�ʵʱ����yֵ���������ƫ��
	virtual void AddNode(D3DXVECTOR3 pos, int _time);
	virtual void AddNode(const NpcNode& node);
};