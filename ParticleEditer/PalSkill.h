
#ifndef _PAL_SKILL_H
#define _PAL_SKILL_H
#include <map>

#include "DexBase/CDelegate.h"
#include "DexBase/CDexObject.h"
#include "PalTypeDefine.h"

using namespace std;

class PalPlayer;



namespace space_skill
{
	class CPalBuffer
	{
	protected:
		bool   m_good;  //是否有益
		string m_self;  //添加buffer时  作用于自己的表达式
		string m_target;//添加buffer时  作用于目标的表达式
	};
	enum EPalSkillAttackActionType
	{
		PSAAT_NOTMOVE,  //原地施法 
		PSAAT_TARGET,   //跑到目标身边施法
	};
	enum EPalSkillAttribute
	{
		EPASA_NONE,
		EPASA_WATER,
		EPASA_FIRE
	};
	//释放技能时摄像机的动作
	enum EPalSkillAttackCameraType
	{
		PSACT_NOTMOVE,  //不移动
	};
	typedef struct _stDamageKey
	{
		int time;
		int dame;
		bool apply;
		_stDamageKey()
		{
			time = 0;
			dame = 0;
			apply = false;
		}
		_stDamageKey(int t, int d, bool a = false)
		{
			time = t;
			dame = d;
			apply = a;
		}
	}stDamageKey;
	typedef struct _stPlaSkill
	{

		int                       skillId;
		bool					  m_good;  //是否有益
		int                       damage; //先暫時固定傷害，傷害時取隨機數，之後可根據施法者的裝備及屬性進行加成
		EPalSkillAttribute        attribute;
		EPalSkillAttackActionType attackType;
		EPalSkillAttackCameraType cameraType;
		std::map<int, stDamageKey>        damageKey;
		int       preTime; //技能前置时间
		int       time;    //施法时间
		int       afterTime ; //后置时间
		std::string startEffet; //起手特效文件
		std::string flyEffet;   //飞行特效文件
		std::string attackEffet; //到达目标后攻击特效文件
		_stPlaSkill()
		{
			skillId = 0;
			m_good = false;
			damage = 100;
			attribute = EPASA_NONE;
			attackType = PSAAT_NOTMOVE;
			cameraType = PSACT_NOTMOVE;
			preTime = afterTime = 0;
			time = 1000;
		}
	}stPlaSkill;
}
using namespace space_skill;
class CDexEffectInstance;
//暂定多人攻击技能都是远程技能 因为暂时无法确定多个目标的情况下如果是近程技能的话到底要跑向那个敌人
class PalSkill:public CDexObject
{//技能的攻击动作类型
	friend class CPalSkillContainer;
	friend class PalBatterContent;
	Dex_DeclareClass(PalSkill, CDexObject, 0)
protected:
	stPlaSkill m_data;
	int damage;
	int skill_count;
	int skill_time; //test
	CDexEffectInstance* m_pEffectInstance;
	std::string m_strEffectFileName;
public:
	PalSkill();
	virtual ~PalSkill();

public:
	bool LoadEffect(string name);
	void SkillBegin();
	virtual bool Update(int delta);
	int getTriggerDamage();//技能在播放过程中 在某些帧出发伤害效果
	void SkillEnd();
	stPlaSkill* getSkillData() { return &m_data;};
	CDexEffectInstance* getSkillEffect(){ return m_pEffectInstance;};
public:
	void AddDamageKey(int time, int damage);//test
};

class CPalSkillContainer:public CDexObject
{
	Dex_DeclareClass(CPalSkillContainer, CDexObject, 0)

		//不同的戰鬥人物的技能文件裡面有不同的技能ID，加載時傳ID給任務的skillcontainer，
		//skillcontainer從擁有所有技能的skillmanager裡面加載數據
public:
	CPalSkillContainer();
	~CPalSkillContainer();
protected:
	PalSkill*  m_pSlash;   //平砍技能 ，如果資源足夠的話，不同的武器可有不同的平砍技能，這裡只設一個
	list<PalSkill*> m_listWater; //水屬性技能
	list<PalSkill*> m_listFire;  
public:
	//void AddSkill(int id);
	void AddSkill(EPalSkillAttribute type, PalSkill* skill); //之後玩家技能數據可從文件中設置
	PalSkill* getSlashSkill() ;

};

class PalGameStateBattleMain;
//战斗场景数据
class PalBatterContent
{
	friend class PalGameStateBattleMain;
protected:
	typedef struct _stContent
	{//每一個角色都可以發起一個content
		int skill_count;; //本次施法的时间
		PalPlayer* attacker;  //本次战斗的发起者
		PalSkill*  skill;     //发起本次战斗所使用的技能
		_stContent()
		{
			skill_count = 0;
			attacker = NULL;
			skill = NULL;
		}
		
	}stContent;

	//战斗场景中 敌方我方的中心位置 为了群攻技能使用
	D3DXVECTOR3 m_enemyCenterPos;
	D3DXVECTOR3 m_roleCenterPos; 
	std::list<stContent> m_listContent;
private:
	PalBatterContent();
	~PalBatterContent();

protected:
	void OnSkillBegin(const stContent* content);
	void OnSkillDamage(stContent* content, int number, bool good);
	void OnSkillEnd(const stContent* content);
public:
	void Update();
	void CreateNewContent();
	void SetContent(PalPlayer* Attacker, PalSkill* skill);
	void SetTargets(TlistPlayer targets);
	void AddTarget(PalPlayer* target);
};

#endif