
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
	PalArgs_PlayerCancelSkill, //�������ȡ����������ѡ����
	PalArgs_Total,
};
//�ж�״̬
enum EPalPoisonState
{
	PPS_None  = 1 << 1,
	PPS_Water = 1 << 2,  //ˮ��
	PPS_Fire  = 1 << 3,  //��
};
//player����
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
	EPT_ENEMY, //���﷽
	EPT_ROLE,   //�ҷ�
};
//playerս������
struct stPalPlayerData
{//�����buffer �ı�ĳЩ���Ե���ֵ
	int hp;	   //��
	int hpMax; //������
	int mp;    //��
	int mpMax; //������
	int essence;    //��
	int essenceMax; //������ 
	int level;      //�ȼ�
	int exp;	    //����
	int attack;		//����
	int defense;	//����
	float miss;     //������
	float moneyRate;  //��ý�Ǯ����
	float speed;    //����
	string headImageName; //���ͷ���Ӧ��ͼƬ����
	DexGUI::DexRect headImageRect; //ͷ����ͼƬ�����ռ�ݾ���
	DexGUI::DexSize headSize;
	EPalPoisonState poisonState; //�ж�״̬
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