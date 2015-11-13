

#ifndef _PAL_NPC_H
#define _PAL_NPC_H

#include "PalPeople.h"


class CPalNpc:public CPalPeople
{
	typedef struct _NpcNode
	{
		D3DXVECTOR3 m_point;   //坐标
		int       m_time;	   //在该点停留的时间
	} NpcNode;
protected:
	std::vector<_NpcNode> m_path;  //行走状态NPC的途径点
	size_t m_pathCount;  //当前正在走向的索引点
	D3DXVECTOR3 m_routeCurr;
	D3DXVECTOR3 m_route; //从第一点向第二点走时，该变量即为一个第一点指向第二点的向量
	bool   m_front;      //是否正向途径索引点（正向途径完，将会反向行走）
	bool   m_stay;       //当前是否正在停留
	int  m_time;        //已在当前我索引点的停留时间

protected:
	std::string  m_strScritpFile; //npc对应的脚本文件
	//typedef list<stTask> TListTask;
	//TListTask  m_listTask;  //信息从脚本archive进出
public:
	CPalNpc();
	virtual ~CPalNpc();

public:
	virtual void Reset();
	virtual bool Update(int delta);


	virtual void AddNode(D3DXVECTOR3 pos, int _time);
};

#endif