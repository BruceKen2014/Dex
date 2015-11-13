

#include <string>
#include <vector>


#include "DexBase/DexSceneObjectModel.h"
class CPalSkillContainer;
class CPalItemContainer;

//中毒状态
enum EPalPoisonState
{
	PPS_None  = 1 << 1,
	PPS_Water = 1 << 2,  //水毒
	PPS_Fire  = 1 << 3,  //火毒
};
//player属性
enum EPalPlayerAttribute
{
	PPA_Water = 1 << 1,
	PPA_Fire  = 1 << 2,
};
//player战斗数据
struct stPalPlayerData
{//可添加buffer 改变某些属性的数值
	int hp;	   //精
	int hpMax; //精上限
	int mp;    //神
	int mpMax; //神上限
	int essence;    //气
	int essenceMax; //气上限 
	int level;      //等级
	int exp;	    //经验
	int attack;		//攻击
	int defense;	//防御
	float miss;     //闪避率
	float moneyRate;  //获得金钱比率
	float speed;    //敏捷
	EPalPoisonState poisonState; //中毒状态
	EPalPlayerAttribute attribute;
	CPalSkillContainer* skill;
	stPalPlayerData():hp(0),hpMax(0),mp(0),mpMax(0),essence(0),essenceMax(0),
		level(0),exp(0),attack(0),defense(0),miss(0.0f),moneyRate(1.0f),speed(1.0f),
		poisonState(PPS_None),attribute(PPA_Water),skill(NULL)
	{
	}
};

class CPalPlayer:public CDexSceneObjectModel
{
protected:
	stPalPlayerData m_data;
	float           m_fScaleInit; //正常显示下的模型缩放
	float           m_fScaleCurr; //当前的模型缩放
	int  m_standIndex;		 //站立index
	int  m_runIndex;         //奔跑index
	int  m_dieIndex;         //死亡index
	std::vector<int>  m_attackIndex;  //攻击Index(对于多个index,可随机选取一个)

public:
	CPalPlayer();
	virtual ~CPalPlayer();

public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();

public:
	void setModelId(int id);
};

class CPalUser
{
protected:
	CPalItemContainer* m_pItemContainer; //道具属于玩家，可作用于所有player
	std::vector<CPalPlayer*> m_pPlayers; //上阵的player
};