

#include "CNpc.h"


CNpc::CNpc(LPDIRECT3DDEVICE9 device):CModelXAni(device)
{
	m_type = NPC_STAND;
	m_pathCount = 0;
	m_time = 0;
	m_standIndex = 0;
	m_walkIndex = 0;
	m_front = true;
	m_stay = true;
}

CNpc::~CNpc()
{
	ShutDown();
}

void CNpc::Update()
{
	if(GetMoving())
	{//开启了移动状态
		if(m_type == NPC_WALK)
		{
			if(m_stay)
			{
				m_time++;
				if(m_time >= m_path[m_pathCount].m_time)
				{//停留时间已到，则继续向下节点行走
					m_time = 0;
					m_stay = false;
					SetAnimation(m_walkIndex);
					D3DXVECTOR3 vec;  //设置转变方向的向量
					if(m_front)
					{
						m_pathCount++;
						vec = m_path[m_pathCount].m_point - m_path[m_pathCount-1].m_point;
						if(m_pathCount == m_path.size()-1)
						{//已是最后一个节点，反向
							m_front = false;
						}
					}
					else
					{
						m_pathCount--;
						vec = m_path[m_pathCount].m_point - m_path[m_pathCount+1].m_point;
						if(m_pathCount == 0)
						{
							m_front = true;
						}
					}
					vec.y = 0;
					D3DXVec3Normalize(&vec, &vec);
					SetDirection(vec);
					SetVel(m_vel);
				}
			}
			else
			{//向下个节点移动
				m_posX += m_VelX;
				m_posY += m_VelY;
				m_posZ += m_VelZ;
				SetPosition(m_posX, m_posY, m_posZ);
				if(D3DXVec3Length(&(D3DXVECTOR3(m_posX, 0, m_posZ) - m_path[m_pathCount].m_point)) <= 10.0f)
				{//到达该节点
					m_stay = true;
					SetAnimation(m_standIndex);
				}
			}
		}
	}
	if ( m_pAnimController != NULL)
		m_pAnimController->AdvanceTime( m_aniSpeed/1000, NULL );  
}

D3DXVECTOR3 CNpc::GetNextNode()
{
	if(m_type == NPC_STAND)
		return D3DXVECTOR3(m_posX, m_posY, m_posZ);
	return m_path[m_pathCount].m_point;
}

bool CNpc::GetStay()
{
	return m_stay;
}

void CNpc::SetType(int _type)
{
	m_type = _type;
}

void CNpc::SetIndex(int stand, int walk)
{
	m_standIndex = stand;
	m_walkIndex = walk;
}

void CNpc::ChangeStandIndex()
{
	SetAnimation(m_standIndex);
}
void CNpc::ChangeWalkIndex()
{
	SetAnimation(m_walkIndex);
}

void CNpc::AddNode(float posX, float posY, float posZ, int _time)
{
	NpcNode temp;
	posY = 0;
	temp.m_point = D3DXVECTOR3(posX, posY, posZ);
	temp.m_time = _time;
	m_path.push_back(temp);
}

void CNpc::AddNode(D3DXVECTOR3 pos, int _time)
{
	NpcNode temp;
	pos.y = 0; //去掉Y高度
	temp.m_point = pos;
	temp.m_time = _time;
	m_path.push_back(temp);
}

void CNpc::AddNode(const NpcNode &node)
{				  
	NpcNode temp = node;
	temp.m_point.y = 0;
	m_path.push_back(node);
}