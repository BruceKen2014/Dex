
#pragma  once
/*
ս��������
@���ߣ�BruceKen
@date:2013.1
*/


#include "PalDefine.h"
#include "../Source/CModelXAni.h"
#include "../DexBase/CIni.h"  
#include "../sound/CSoundSystem.h"


typedef struct _EnmyPara
{
	D3DXVECTOR3 m_standPos;  //վ��λ�ã�ԭʼλ�ã�����ʵʱλ�ã�
	D3DXVECTOR3 m_destPos;	 //Ŀ��λ��
	int  m_time;			 //ʩ��������ʱ��	
	int  m_currTime;         //��ǰʩ��������ʱ��  
	int  m_standIndex;		 //վ��index
	int  m_runIndex;         //����index
	int  m_dieIndex;         //����index
	std::vector<int>  m_attackIndex;  //����Index(���ڶ��index,�����ѡȡһ��)
	bool m_stay; //��վ��ԭ��ʩ�������ܵ�Ŀ�ĵ�ʩ��
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
//	int   m_index;  //��Ӧ�ķ���Ч������
//	int   m_type ; //���ͣ������Ѫ����������
//	float m_count ;  //��������Ѫ�����򹥻�����
//} CSkill;
enum
{ //״̬
	ATTACK_FLAG,
	ATTACK_GO,    //����������ȥ
	ATTACK_ING,	  //���ڹ���
	ATTACK_BACK,  //����������

	SKILL_ING,    //ʩ��
	ENM_DYING,    //��������
};
class CFightHead;
class CEnemy:public CModelXAni 
{
protected:
	EnmyPara m_enmyPara;	 //��������
	CFightHead*  m_fightHead;     //��Ӧ��ս��ͷ��ָ��	
	float  m_attackVel;      //��Ŀ�걼��ʱ���ٶ�
	char   m_sndDie[64];   //������Ч
	char   m_sndAttack[64];  //����Ч��

	int  m_dieTime;	        //�����������е�ʱ��
	int  m_dieCurrTime;      //����������ǰ������ʱ��
	int    m_Stage;    //״̬
	int    m_LastStage;      //�ϸ�״̬������ϸ�״̬�ǹ����������״̬�������ܣ�����������˺�Ч��
	int    m_id;
	int  m_hp; 
	int  m_currHp;  //��ǰ����
	int  m_mp;
	int  m_currMp;   //��ǰ����ֵ
	int  m_attack;  //������
	int  m_currAttack;  //��ǰ�������������ʹ�÷������ƺ󣬹��������ͣ�
	int    m_banAttackTurn;  //���������ƻغ�
	int  m_defence;   //����
	int  m_currDefence; //��ǰ����
	float  m_banDefenseTurn;  //�������ƻغ�
	//std::vector<CSkill*> m_skill;  //�����������洢����ķ���

	int  m_attackDesire;  //��������
	int  m_defenseDesire;  //��������
	bool   m_isPlayer;  //�Ƿ�����ҿ���
	bool   m_isDead;    //�Ƿ��Ѿ�����

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
	virtual void SetDestPos(D3DXVECTOR3 pos);  //����Ŀ��λ��
	virtual void SetDestPos(float x, float y, float z);
	virtual void SetAttackVel(float vel);
	virtual void SetAttacking(bool isAttacking);
	virtual int  GetAttackState() { return m_Stage;};
	virtual D3DXVECTOR3 GetDestPos() { return m_enmyPara.m_destPos;}; 

	virtual void SetIsPlayer(bool b);
	virtual bool GetIsPlayer();
	virtual bool GetIsJustAttack();  //�Ƿ�ոչ����꣬��������˺�����Ч��
//	virtual int  GetId();
//
	virtual int  GetHp() { return m_hp;};
	virtual int  GetCurrHp() { return m_currHp;};
	virtual void SetCurrHp(int hp) ;
	virtual void ReduceHp(int hurt);   //������ҹ����˺��������ڲ����м���
	virtual void AddHp(int add); 
//
//	virtual float GetMp();
//	virtual float GetCurrMp();
//	virtual void  ReduceMp(float hurt);  //������ҹ����˺��������ڲ����м���
//
	virtual int GetAttack();
//	virtual float GetCurrAttack();
//	virtual void  DownAttack(float attack); //���ʹ����Ӧ���������͹��﹥����(���ٷֱȽ��ͣ�����0.1�򽵵�10%)�����غϺ��Զ��ָ�
//
//	virtual float GetDefense();
//	virtual float GetCurrDefense();
//	virtual void  DownDefence(float defence);//���ʹ����Ӧ���������͹������������ٷֱ� 
//
//	virtual float GetAttackDesire();
//	virtual float GetDefenseDesire();

	//virtual CSkill* Fight();   
	
};