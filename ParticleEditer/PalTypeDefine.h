
#ifndef _PAL_TYPE_DEFINE_H
#define _PAL_TYPE_DEFINE_H

#include "widget/DexGuiStruct.h"
class PalFightHead;
class PalPlayer;
class CPalSkillContainer;
typedef list<PalPlayer*> TlistPlayer;
typedef list<PalFightHead*> TlistFlighHead ;
typedef std::vector<PalPlayer*> TVectorPlayer;
enum PalStateEventArgs
{
	PalArgs_PlayerCancelSkill, //玩家主动取消技能重新选择技能
	PalArgs_Total,
};
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
enum EPalPlayerHpType
{
	EPHP_HP,
	EPHP_MP,
	EPHP_QP,
};
enum EPlayerType
{
	EPT_ENEMY, //怪物方
	EPT_ROLE,   //我方
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
	string headImageName; //玩家头像对应的图片名称
	DexGUI::DexRect headImageRect; //头像在图片里面的占据矩形
	DexGUI::DexSize headSize;
	EPalPoisonState poisonState; //中毒状态
	EPlayerType   m_PlayerType;
	EPalPlayerAttribute attribute;
	stPalPlayerData():hp(10000),hpMax(10000),mp(0),mpMax(0),essence(0),essenceMax(0),
		level(0),exp(0),attack(0),defense(0),miss(0.0f),moneyRate(1.0f),speed(1.0f),
		poisonState(PPS_None),attribute(PPA_Water)
	{
		headImageName = "";
	}
};
#endif