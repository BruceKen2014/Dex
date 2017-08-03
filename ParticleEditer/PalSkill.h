
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
		bool   m_good;  //�Ƿ�����
		string m_self;  //���bufferʱ  �������Լ��ı��ʽ
		string m_target;//���bufferʱ  ������Ŀ��ı��ʽ
	};
	enum EPalSkillAttackActionType
	{
		PSAAT_NOTMOVE,  //ԭ��ʩ�� 
		PSAAT_TARGET,   //�ܵ�Ŀ�����ʩ��
	};
	enum EPalSkillAttribute
	{
		EPASA_NONE,
		EPASA_WATER,
		EPASA_FIRE
	};
	//�ͷż���ʱ������Ķ���
	enum EPalSkillAttackCameraType
	{
		PSACT_NOTMOVE,  //���ƶ�
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
		bool					  m_good;  //�Ƿ�����
		int                       damage; //�ȕ��r�̶������������rȡ�S�C����֮��ɸ���ʩ���ߵ��b�估�����M�мӳ�
		EPalSkillAttribute        attribute;
		EPalSkillAttackActionType attackType;
		EPalSkillAttackCameraType cameraType;
		std::map<int, stDamageKey>        damageKey;
		int       preTime; //����ǰ��ʱ��
		int       time;    //ʩ��ʱ��
		int       afterTime ; //����ʱ��
		std::string startEffet; //������Ч�ļ�
		std::string flyEffet;   //������Ч�ļ�
		std::string attackEffet; //����Ŀ��󹥻���Ч�ļ�
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
//�ݶ����˹������ܶ���Զ�̼��� ��Ϊ��ʱ�޷�ȷ�����Ŀ������������ǽ��̼��ܵĻ�����Ҫ�����Ǹ�����
class PalSkill:public CDexObject
{//���ܵĹ�����������
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
	int getTriggerDamage();//�����ڲ��Ź����� ��ĳЩ֡�����˺�Ч��
	void SkillEnd();
	stPlaSkill* getSkillData() { return &m_data;};
	CDexEffectInstance* getSkillEffect(){ return m_pEffectInstance;};
public:
	void AddDamageKey(int time, int damage);//test
};

class CPalSkillContainer:public CDexObject
{
	Dex_DeclareClass(CPalSkillContainer, CDexObject, 0)

		//��ͬ�đ��Y����ļ����ļ��e���в�ͬ�ļ���ID�����d�r��ID�o�΄յ�skillcontainer��
		//skillcontainer�ē������м��ܵ�skillmanager�e����d����
public:
	CPalSkillContainer();
	~CPalSkillContainer();
protected:
	PalSkill*  m_pSlash;   //ƽ������ ������YԴ����Ԓ����ͬ���������в�ͬ��ƽ�����ܣ��@�eֻ�Oһ��
	list<PalSkill*> m_listWater; //ˮ���Լ���
	list<PalSkill*> m_listFire;  
public:
	//void AddSkill(int id);
	void AddSkill(EPalSkillAttribute type, PalSkill* skill); //֮����Ҽ��ܔ����ɏ��ļ����O��
	PalSkill* getSlashSkill() ;

};

class PalGameStateBattleMain;
//ս����������
class PalBatterContent
{
	friend class PalGameStateBattleMain;
protected:
	typedef struct _stContent
	{//ÿһ����ɫ�����԰l��һ��content
		int skill_count;; //����ʩ����ʱ��
		PalPlayer* attacker;  //����ս���ķ�����
		PalSkill*  skill;     //���𱾴�ս����ʹ�õļ���
		_stContent()
		{
			skill_count = 0;
			attacker = NULL;
			skill = NULL;
		}
		
	}stContent;

	//ս�������� �з��ҷ�������λ�� Ϊ��Ⱥ������ʹ��
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