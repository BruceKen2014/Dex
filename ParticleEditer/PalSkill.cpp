
#include "DexBase/CDexTime.h"
#include "DexBase/CDexPartical.h"
#include "DexBase/Dex3DScene.h"
#include "DexBase/DexObjectFactory.h"
#include "state/DexGameEngine.h"
#include "PalPlayer.h"
#include "PalStateBattleMain.h"
#include "PalNumber.h"
#include "PalSkill.h"

PalSkill::PalSkill()
{
	m_pEffectInstance = NULL;
	damage = 0;
	skill_count = 0;
	skill_time = 1000;
}

PalSkill::~PalSkill()
{

}

bool PalSkill::LoadEffect(string name)
{
	if(m_pEffectInstance != NULL)
	{
		m_pEffectInstance->Release();
	}
	m_pEffectInstance = Dex_QueryObject(CDexEffectEmitInstance);
	m_pEffectInstance->LoadInitFile(name.c_str());
	m_pEffectInstance->setCycle(false);
	return true;
}
void PalSkill::SkillBegin()
{
	if(m_pEffectInstance != NULL)
	{
		m_pEffectInstance->StartFromBegin();
		PalGameStateBattleMain::getBattleMain()->getStateScene()->registerObject(m_pEffectInstance);
	}

	skill_count = 0;
	damage = 0;
	for(std::map<int, stDamageKey>::iterator ite = m_data.damageKey.begin();
		ite!= m_data.damageKey.end(); ++ite)
	{
		ite->second.apply = false;
	}
}


void PalSkill::SkillEnd()
{
	if(m_pEffectInstance != NULL)
		PalGameStateBattleMain::getBattleMain()->getStateScene()->unRegisterObject(m_pEffectInstance);
}


bool PalSkill::Update(int delta)
{
	DEX_ENSURE_B(CDexObject::Update(delta));
	//if(skill_count == 0)
	//{
	//	stEvent event;
	//	event.push_args(stArgs(1));
	//	onSkillBegin.Handle(this,event);
	//}
	//DEX_ENSURE_B(skill_count < skill_time);
	//skill_count += getTime()->getDeltaTime();
	//for(std::map<int, stDamageKey>::iterator ite = m_data.damageKey.begin();
	//	ite!= m_data.damageKey.end(); ++ite)
	//{
	//	if(skill_count > ite->second.time &&
	//		!(ite->second.apply))
	//	{
	//		stEvent event;
	//		event.push_args(stArgs(ite->second.dame));
	//		event.push_args(stArgs(2));
	//		onSkillBegin.Handle(this,event);
	//		ite->second.apply = true;
	//	}
	//	
	//}
	//if(skill_count >= skill_time)
	//{
	//	stEvent event;
	//	event.push_args(stArgs(3));
	//	onSkillBegin.Handle(this,event);
	//}
	return true;
}

void PalSkill::AddDamageKey(int time, int damage)
{
	m_data.damageKey[time] = stDamageKey(time, damage);
}

int PalSkill::getTriggerDamage()
{
	return damage;
}

//===================================
CPalSkillContainer::CPalSkillContainer()
{
	m_pSlash = new PalSkill;
}

CPalSkillContainer::~CPalSkillContainer()
{

}

void CPalSkillContainer::AddSkill(space_skill::EPalSkillAttribute type, PalSkill* skill)
{
	switch(type)
	{
	case space_skill::EPASA_NONE:
		{
			m_pSlash = skill;
			break;
		}
	}
}

PalSkill* CPalSkillContainer::getSlashSkill()
{
	return m_pSlash;
}

//========================================
PalBatterContent::PalBatterContent()
{
	m_enemyCenterPos = D3DXVECTOR3(80,0,80);
	m_roleCenterPos = D3DXVECTOR3(-80,0,-80);
}
PalBatterContent::~PalBatterContent()
{

}

void PalBatterContent::OnSkillBegin(const stContent* content)
{
	content->skill->SkillBegin();
	D3DXVECTOR3 sill_pos;
	if(content->attacker->getTargets().size() == 1)
	{
		PalPlayer* target = content->attacker->getTargets().front();
		sill_pos = target->getStandOriginPosition();
	}
	else
	{//群体技能
		if(content->attacker->getPlayerData()->m_PlayerType == EPT_ENEMY)
		{//enemy使用
			if(content->skill->getSkillData()->m_good)
			{//有益技能
				sill_pos = m_enemyCenterPos;
			}
			else
			{
				sill_pos = m_roleCenterPos;
			}
		}
		else
		{//role使用
			if(content->skill->getSkillData()->m_good)
			{//有益技能
				sill_pos = m_roleCenterPos;
			}
			else
			{
				sill_pos = m_enemyCenterPos;
			}
		}
	}
	CDexEffectInstance* effect = content->skill->getSkillEffect();
	if(effect != NULL)
		effect->setPosition(sill_pos);
}

void PalBatterContent::OnSkillDamage(stContent* content,int number, bool good)
{
	PalPlayer* target = NULL;
	D3DXVECTOR3 _3Dpos;
	DexGUI::DexPoint _2Dpnt;
	TlistPlayer targets = content->attacker->getTargets();
	PalNumberAni* numberAni = NULL;
	for(TlistPlayer::iterator ite = targets.begin();ite != targets.end(); ++ite)
	{
		target = *ite;
		_3Dpos = target->getTopPosition();
		_2Dpnt = DexGameEngine::getEngine()->GetXY(_3Dpos);
		EPlayerType target_type = target->getPlayerData()->m_PlayerType;
		bool good = content->skill->getSkillData()->m_good;
		EPalPlayerHpType hp_type = EPHP_HP;
		numberAni = PalGameStateBattleMain::getBattleMain()->getNumberManager()->createNumberAni(target_type, good,hp_type);
		numberAni->SetNumber(number);
		numberAni->SetPos(_2Dpnt.x, _2Dpnt.y);
		numberAni->SetSpeed(1);
		numberAni->SetEndPos(_2Dpnt.x, _2Dpnt.y - 70);
		if(good)
		{
			target->ApplyHeal(number);
		}
		else
		{
			target->ApplyDamage(number);
		}
	}
}

void PalBatterContent::OnSkillEnd(const stContent* content)
{
	content->skill->SkillEnd();
	content->attacker->skillEnd();
}
void PalBatterContent::CreateNewContent()
{
	m_listContent.push_back(stContent());
}
void PalBatterContent::Update()
{
	stContent* content;
	for(std::list<stContent>::iterator ite = m_listContent.begin(); ite != m_listContent.end();)
	{
		content = &(*ite);
		if(content!=NULL && content->attacker != NULL && !content->attacker->completeSkill())
		{
			if(content->skill_count == 0)
			{
				OnSkillBegin(content);
			}
			content->skill_count += getTime()->getDeltaTime();
			for(std::map<int, stDamageKey>::iterator ite2 = (content->skill->m_data.damageKey).begin();
				ite2 != (content->skill->m_data.damageKey).end(); ++ite2)
			{
				if(content->skill_count > ite2->second.time &&
					!(ite2->second.apply))
				{
					OnSkillDamage(content, ite2->second.dame, content->skill->m_data.m_good);
					ite2->second.apply = true;
				}

			}
			if(content->skill_count >= content->skill->m_data.time)
			{
				OnSkillEnd(content);
				ite = m_listContent.erase(ite);
				continue;
			}
		}
		++ite;
	}
}

void PalBatterContent::SetContent(PalPlayer* Attacker, PalSkill* skill)
{
	if(m_listContent.size() != 0)
	{
		m_listContent.back().attacker = Attacker;
		m_listContent.back().skill = skill;
	}
}

void PalBatterContent::SetTargets(TlistPlayer targets)
{
}
void PalBatterContent::AddTarget(PalPlayer* target)
{
}