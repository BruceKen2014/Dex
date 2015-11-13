

#include <string>
#include <vector>


#include "DexBase/DexSceneObjectModel.h"
#include "widget/DexGuiStruct.h"
#include "PalTypeDefine.h"
#include "PalPeople.h"
class CPalSkillContainer;
class CPalItemContainer;
class PalFightHead;
class PalSkill;
class PalPlayerAttackStateMachine;


class PalPlayer:public CPalPeople
{
protected:
	D3DXVECTOR3 m_standPos;  //站立位置（原始位置，并非实时位置）
	stPalPlayerData m_data;
	PalFightHead*   m_headFlag;
	CPalSkillContainer* m_pSkillContainer;
	PalPlayerAttackStateMachine* m_AiMachine;
	bool            m_bAuto; //是否自动AI攻击 否则由玩家手动控制
	bool            m_bCompleteSkill;
	PalSkill*        m_pCurrSkill; //本轮使用的技能
	TlistPlayer m_listTargets; //操作的目标，可以是己方的加血目标，也可以使敌方目标
	int m_iAttackAnimationTimeCount;
public:
	PalPlayer();
	virtual ~PalPlayer();

public:
	bool getAuto() { return m_bAuto;};
	void setAuto(bool b) { m_bAuto = b;};
	void ResetAttackAnimatoinTimeCount() { m_iAttackAnimationTimeCount = 0;};
	void UpdateAttackAnimationTimeCount();
	int  getAttackAnimationTimeCount() {return m_iAttackAnimationTimeCount;};
	void clearAttackTarget();
	TlistPlayer  getTargets() {return m_listTargets;};
	void AddTarget(PalPlayer* target);
	bool IsLive();
	D3DXVECTOR3 getStandOriginPosition() {return m_standPos;};
	void setStandOriginiPos(D3DXVECTOR3 pos) { m_standPos = pos;}
	stPalPlayerData* getPlayerData()  { return &m_data;};
	void             setHeadFlag(PalFightHead* head) {m_headFlag = head;};
	PalFightHead*    getHeadFlag()    { return m_headFlag;};
	CPalSkillContainer* getSkillContainer() { return m_pSkillContainer;};
	PalPlayerAttackStateMachine* getAIMachine() {return m_AiMachine;};
	void            skillBegin() { m_bCompleteSkill = false;};
	void            skillEnd(){m_bCompleteSkill = true;};
	bool            completeSkill(){return m_bCompleteSkill;};
	PalSkill*       setCurrSkill(PalSkill* skill)    { return m_pCurrSkill = skill;};
	PalSkill*       getCurrSkill()    { return m_pCurrSkill;};
	void setHeadName(string name, DexGUI::DexRect rect, DexGUI::DexSize size)
	{ m_data.headImageName = name;m_data.headImageRect =rect; m_data.headSize = size;};
	D3DXVECTOR3     getTopPosition(); //取得顶部位置
public:
	virtual void Reset();
	virtual bool UpdatePlayerData(int delta);
	virtual void ApplyDamage(int hp);
	virtual void ApplyHeal(int hp);
};

class CPalUser
{
protected:
	CPalItemContainer* m_pItemContainer; //道具属于玩家，可作用于所有player
	TVectorPlayer m_pPlayers; //上阵的player
};