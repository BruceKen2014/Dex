


#include "PalNpc.h"

CPalNpc::CPalNpc()
{
	m_time = 0;
	m_pathCount = 0;
	m_route = D3DXVECTOR3(0,0,0);
	m_routeCurr = D3DXVECTOR3(0,0,0);
	m_front = true;
	m_stay = true;
}

CPalNpc::~CPalNpc()
{

}

void CPalNpc::Reset()
{

}

bool CPalNpc::Update(int delta)
{
	DEX_ENSURE_B(CPalPeople::Update(delta));
	CDexModelXAni* modelAni = getAniModel();
	if(m_stay)
	{
		m_time += delta;
		if(m_time >= m_path[m_pathCount].m_time)
		{//停留时间已到，则继续向下节点行走
			m_time = 0;
			m_stay = false;
			if(m_walkIndex != -1)
				modelAni->SetAnimation(m_walkIndex);
			else
				modelAni->SetAnimation(m_runIndex);
			D3DXVECTOR3 vec;  //设置转变方向的向量
			if(m_front)
			{
				m_pathCount++;
				m_route = m_path[m_pathCount].m_point - m_path[m_pathCount-1].m_point;
				vec = m_path[m_pathCount].m_point - m_path[m_pathCount-1].m_point;
				if(m_pathCount == m_path.size()-1)
				{//已是最后一个节点，反向
					m_front = false;
				}
			}
			else
			{
				m_pathCount--;
				m_route = m_path[m_pathCount].m_point - m_path[m_pathCount+1].m_point;
				vec = m_path[m_pathCount].m_point - m_path[m_pathCount+1].m_point;
				if(m_pathCount == 0)
				{
					m_front = true;
				}
			}
			vec.y = 0;
			D3DXVec3Normalize(&vec, &vec);
			setDirection(vec);
		}
	}
	else
	{//向下个节点移动
		D3DXVECTOR3 del = m_route * 0.005f;
		Move(del);

		D3DXVECTOR3	curr_pos = getPosition();
		curr_pos.y = 0;
		if(D3DXVec3Length(&(curr_pos - m_path[m_pathCount].m_point)) <= 10.0f)
		{//到达该节点
			m_stay = true;
			modelAni->SetAnimation(m_standIndex);
		}
	}
	return true;
}

void CPalNpc::AddNode(D3DXVECTOR3 pos, int _time)
{
	NpcNode temp;
	pos.y = 0; //去掉Y高度
	temp.m_point = pos;
	temp.m_time = _time;
	m_path.push_back(temp);
}