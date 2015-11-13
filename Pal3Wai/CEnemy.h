
#pragma  once
/*
战斗敌人类
@作者：BruceKen
@date:2013.1
*/


#include "PalDefine.h"
#include "../Source/CModelXAni.h"
#include "../DexBase/CIni.h"  
#include "../sound/CSoundSystem.h"


typedef struct _EnmyPara
{
	D3DXVECTOR3 m_standPos;  //站立位置（原始位置，并非实时位置）
	D3DXVECTOR3 m_destPos;	 //目标位置
	int  m_time;			 //施法、攻击时间	
	int  m_currTime;         //当前施法经过的时间  
	int  m_standIndex;		 //站立index
	int  m_runIndex;         //奔跑index
	int  m_dieIndex;         //死亡index
	std::vector<int>  m_attackIndex;  //攻击Index(对于多个index,可随机选取一个)
	bool m_stay; //是站在原地施法还是跑到目的地施法
	_EnmyPara()
	{
		m_standPos = D3DXVECTOR3(0,0,0);
		m_destPos = D3DXVECTOR3(0,0,0);
		m_time  = 0;
		m_currTime = 0;
		m_standIndex = 0;
		m_runIndex = 0;
		m_dieIndex = 0;
		m_stay = false;
	}
} EnmyPara;
//typedef struct _CSkill
//{
//	int   m_index;  //对应的法术效果索引
//	int   m_type ; //类型，自身加血、攻击敌人
//	float m_count ;  //数量（加血数量或攻击力）
//} CSkill;
enum
{ //状态
	ATTACK_FLAG,
	ATTACK_GO,    //正往敌人跑去
	ATTACK_ING,	  //正在攻击
	ATTACK_BACK,  //正在往回跑

	SKILL_ING,    //施法
	ENM_DYING,    //正在死亡
};
class CFightHead;
class CEnemy:public CModelXAni 
{
protected:
	EnmyPara m_enmyPara;	 //攻击参数
	CFightHead*  m_fightHead;     //对应的战斗头像指针	
	float  m_attackVel;      //往目标奔跑时的速度
	char   m_sndDie[64];   //死亡音效
	char   m_sndAttack[64];  //攻击效果

	int  m_dieTime;	        //死亡动作进行的时间
	int  m_dieCurrTime;      //死亡动作当前经过的时间
	int    m_Stage;    //状态
	int    m_LastStage;      //上个状态，如果上个状态是攻击，而这个状态是往回跑，则添加数字伤害效果
	int    m_id;
	int  m_hp; 
	int  m_currHp;  //当前生命
	int  m_mp;
	int  m_currMp;   //当前法术值
	int  m_attack;  //攻击力
	int  m_currAttack;  //当前攻击力（被玩家使用法术禁制后，攻击力降低）
	int    m_banAttackTurn;  //攻击力禁制回合
	int  m_defence;   //防御
	int  m_currDefence; //当前防御
	float  m_banDefenseTurn;  //防御禁制回合
	//std::vector<CSkill*> m_skill;  //法术向量，存储所会的法术

	int  m_attackDesire;  //攻击欲望
	int  m_defenseDesire;  //防御欲望
	bool   m_isPlayer;  //是否由玩家控制
	bool   m_isDead;    //是否已经死亡

public:
	CEnemy(LPDIRECT3DDEVICE9 device);
	virtual ~CEnemy();

public:
	virtual void ShutDown();
	virtual void Update();
	virtual void Render();

public:
	virtual void LoadIni(char* filename, char*filed);
	virtual void SetEnmyPara(const EnmyPara& para);
    virtual void SetFightHead(CFightHead * head);
	virtual CFightHead* GetFightHead();
	virtual void SetDestPos(D3DXVECTOR3 pos);  //设置目标位置
	virtual void SetDestPos(float x, float y, float z);
	virtual void SetAttackVel(float vel);
	virtual void SetAttacking(bool isAttacking);
	virtual int  GetAttackState() { return m_Stage;};
	virtual D3DXVECTOR3 GetDestPos() { return m_enmyPara.m_destPos;}; 

	virtual void SetIsPlayer(bool b);
	virtual bool GetIsPlayer();
	virtual bool GetIsJustAttack();  //是否刚刚攻击完，用于添加伤害数字效果
//	virtual int  GetId();
//
	virtual int  GetHp() { return m_hp;};
	virtual int  GetCurrHp() { return m_currHp;};
	virtual void SetCurrHp(int hp) ;
	virtual void ReduceHp(int hurt);   //传入玩家攻击伤害，方法内部进行减除
	virtual void AddHp(int add); 
//
//	virtual float GetMp();
//	virtual float GetCurrMp();
//	virtual void  ReduceMp(float hurt);  //传入玩家攻击伤害，方法内部进行减除
//
	virtual int GetAttack();
//	virtual float GetCurrAttack();
//	virtual void  DownAttack(float attack); //玩家使用响应法术，降低怪物攻击力(按百分比降低，传入0.1则降低10%)，数回合后自动恢复
//
//	virtual float GetDefense();
//	virtual float GetCurrDefense();
//	virtual void  DownDefence(float defence);//玩家使用响应法术，降低怪物防御，传入百分比 
//
//	virtual float GetAttackDesire();
//	virtual float GetDefenseDesire();

	//virtual CSkill* Fight();   
	
};