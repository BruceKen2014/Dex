
#pragma once
/*
npc类
作者：BruceKen
*/

#include "../Source/CModelXAni.h"


enum
{
	NPC_STAND,  //站着状态的NPC
	NPC_WALK,   //行走状态的NPC
};
typedef struct _NpcNode
{
	D3DXVECTOR3 m_point;   //坐标
	int       m_time;	   //在该点停留的时间
} NpcNode;
class CNpc:public CModelXAni
{
protected:
	int m_type;  //NPC类型，是站着的还是还是行走的
	std::vector<_NpcNode> m_path;  //行走状态NPC的途径点
	size_t m_pathCount;  //当前正在走向的索引点
	bool   m_front;      //是否正向途径索引点（正向途径完，将会反向行走）
	bool   m_stay;       //当前是否正在停留
	int  m_time;        //已在当前我索引点的停留时间
	int  m_standIndex;  //站立Index
	int  m_walkIndex;   //行走Index

public:
	CNpc(LPDIRECT3DDEVICE9 device);
	~CNpc();

public:
	virtual void Update();
	virtual D3DXVECTOR3 GetNextNode();  //获得要前往的那个节点坐标 
	virtual bool GetStay();             //当前是否正停留
	virtual void SetType(int _type);
	virtual void SetIndex(int stand, int walk);
	virtual void ChangeWalkIndex();
	virtual void ChangeStandIndex();
	virtual void AddNode(float posX, float posY, float posZ, int _time);  //添加npc节点时计算是否到达不算y，因为地形会实时更改y值，可能造成偏差
	virtual void AddNode(D3DXVECTOR3 pos, int _time);
	virtual void AddNode(const NpcNode& node);
};