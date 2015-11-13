

#include <string>
#include <vector>


#include "DexBase/DexSceneObjectModel.h"
class CPalSkillContainer;
class CPalItemContainer;

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
	EPalPoisonState poisonState; //�ж�״̬
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
	float           m_fScaleInit; //������ʾ�µ�ģ������
	float           m_fScaleCurr; //��ǰ��ģ������
	int  m_standIndex;		 //վ��index
	int  m_runIndex;         //����index
	int  m_dieIndex;         //����index
	std::vector<int>  m_attackIndex;  //����Index(���ڶ��index,�����ѡȡһ��)

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
	CPalItemContainer* m_pItemContainer; //����������ң�������������player
	std::vector<CPalPlayer*> m_pPlayers; //�����player
};