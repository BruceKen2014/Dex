

#include "CEnemy.h"


CEnemy::CEnemy(LPDIRECT3DDEVICE9 device):CModelXAni(device)
{
	m_Stage = ATTACK_FLAG;
	m_LastStage = ATTACK_FLAG;
	m_fightHead = NULL;
	m_attackVel = 2.0;
	m_dieTime = 0;
	m_dieCurrTime = 0; 
	m_id = 0;
	m_hp = 0;
	m_currHp = 0;
	m_mp = 0;
	m_currMp = 0;
	m_attack = 50;
	m_currAttack = 0;
	m_banAttackTurn = 0;
	m_defence = 0;
	m_currDefence = 0;
	m_banDefenseTurn = 0;
	m_attackDesire = 0;
	m_defenseDesire = 0;
	m_isPlayer = false;
	m_isDead = false;
}

CEnemy::~CEnemy()
{
	ShutDown();
}

void CEnemy::ShutDown()
{
	CModelXAni::ShutDown();
}

void CEnemy::Update()
{
	//if(m_isDead)
	//	return;
	if(m_Stage == ATTACK_GO)
	{
		float len = D3DXVec3Length(&(D3DXVECTOR3(m_posX, m_posY, m_posZ) - m_enmyPara.m_destPos)); 
		if(len < 10.0f)
		{//到达攻击地	 
			m_LastStage = ATTACK_GO;
			m_Stage = ATTACK_ING;  //进入攻击状态
			SetVel(0);
			if(m_enmyPara.m_attackIndex.size() != 0)
				SetAnimation(m_enmyPara.m_attackIndex[0]); 
			CSound::getSoundSingleton().Play(m_sndAttack, PAL_SOUND_INDEX0);
		}
	}
	else if(m_Stage == ATTACK_ING)
	{
		if(m_enmyPara.m_currTime >= m_enmyPara.m_time)
		{//攻击时间已过
			m_enmyPara.m_currTime = 0;
			FaceTo(m_enmyPara.m_standPos);
			m_LastStage = ATTACK_ING;
			m_Stage = ATTACK_BACK; //往回跑
			//m_moving = true;
			SetVel(m_attackVel);
			SetAnimation(m_enmyPara.m_runIndex);
		}
		else
			m_enmyPara.m_currTime++;
	}
	else if(m_Stage == ATTACK_BACK)
	{
		if(D3DXVec3Length(&(D3DXVECTOR3(m_posX, m_posY, m_posZ) - m_enmyPara.m_standPos)) < 10.0f)
		{//回到原地	
			//消除误差
			m_posX = m_enmyPara.m_standPos.x; 
			m_posY = m_enmyPara.m_standPos.y; 
			m_posZ = m_enmyPara.m_standPos.z; 
			FaceTo(m_enmyPara.m_destPos);
			m_LastStage = ATTACK_FLAG; 
			m_Stage = ATTACK_FLAG;
			//m_moving = false;
			SetVel(0);
			SetAnimation(m_enmyPara.m_standIndex);
		}
	}
	else if(m_Stage == ENM_DYING)
	{
		m_dieCurrTime++;
		if(m_dieCurrTime >= m_dieTime)
		{
			m_isDead = true; 
			m_LastStage = ENM_DYING;
			m_Stage = ATTACK_FLAG;
		}
	}
	CModelXAni::Update();
}

void CEnemy::Render()
{
	if(m_isDead)
		return;
	CModelXAni::Render();
}
void CEnemy::LoadIni(char* filename, char*filed)
{
	m_currHp =  m_hp = CIni::GetInt(filename, filed, "hp");
	m_attackVel = CIni::GetFloat(filename, filed, "attack_vel");
	m_currMp =  m_mp = CIni::GetInt(filename, filed, "mp");
	m_attack = CIni::GetInt(filename, filed, "attack");
	m_defence = CIni::GetInt(filename, filed, "defence");
	m_dieTime = CIni::GetInt(filename, filed, "dTime");
	CIni::GetString(filename, filed, "attack_snd", m_sndAttack);
	CIni::GetString(filename, filed, "die_snd", m_sndDie);
}

void CEnemy::SetEnmyPara(const EnmyPara& para)
{
	m_enmyPara = para;
}  

void CEnemy::SetFightHead(CFightHead * head)
{
	m_fightHead = head;
}

CFightHead* CEnemy::GetFightHead()
{
	return m_fightHead; 
}

void CEnemy::SetAttacking(bool isAttacking)
{
	m_Stage = ATTACK_GO;
	FaceTo(m_enmyPara.m_destPos); //转向敌人
	//m_moving = true;  //开启移动状态
	SetVel(m_attackVel);
	SetAnimation(m_enmyPara.m_runIndex);
}

void CEnemy::SetIsPlayer(bool b)
{
	m_isPlayer = b;
}

bool CEnemy::GetIsPlayer()
{
	return m_isPlayer;
}

bool CEnemy::GetIsJustAttack()
{
	if(m_LastStage == ATTACK_ING && m_Stage == ATTACK_BACK)
	{//上个状态是攻击，这个状态是往回跑，则刚攻击完，重置状态
		m_LastStage = ATTACK_BACK;
		return true;
	}
	return false;
}

void CEnemy::SetCurrHp(int hp)
{
	if(hp > m_hp)
		m_currHp = m_hp;
	m_currHp = hp;
	if(m_currHp < 0)
	{
		m_currHp = 0;
		m_Stage = ENM_DYING;	
		SetAnimation(m_enmyPara.m_dieIndex);
		SetAnimationFromStart();
		CSound::getSoundSingleton().Play(m_sndDie, PAL_SOUND_INDEX3);
	}
}

void CEnemy::ReduceHp(int hurt)
{
	m_currHp -= hurt;				   
	if(m_currHp < 0)
	{
		m_currHp = 0;
		m_Stage = ENM_DYING;	
		SetAnimation(m_enmyPara.m_dieIndex);
		SetAnimationFromStart();
		CSound::getSoundSingleton().Play(m_sndDie, PAL_SOUND_INDEX3);
	}
	
}

void CEnemy::AddHp(int add)
{
	m_currHp += add;
	m_currHp = (m_currHp > m_hp ? m_hp : m_currHp);
}

void CEnemy::SetDestPos(D3DXVECTOR3 pos)
{
	m_enmyPara.m_destPos = pos;
}

void CEnemy::SetDestPos(float x, float y, float z)
{
	m_enmyPara.m_destPos = D3DXVECTOR3(x, y, z);
}

void CEnemy::SetAttackVel(float vel)
{
	m_attackVel = vel;
}

int CEnemy::GetAttack()
{
	return rand()%(int(m_attack*0.4))+m_attack*0.8;
}