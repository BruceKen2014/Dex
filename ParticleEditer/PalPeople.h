

#ifndef _PAL_PEOPLE_H
#define _PAL_PEOPLE_H

#include "DexBase/DexObject.h"
#include "DexBase/DexSceneObjectModel.h"
/*
����������Ϸ�������Ǽ̳�ģ�ͻ��ǰ���ģ��,��ʱ���ַ���û����ʲô��Ĺ��ܲ�ࡣ����ʵ�Ƕȣ��˵�Ȼ��һ��ģ�ͣ�
�ɴ˿������Ǽ̳еĹ�ϵ�����Ǵӳ������ѧ����˵���˼�ʹû��ģ����Ȼ������ʵ�Ĵ��ڣ���������̸һ���ˣ������
�����岻һ����Ҫ������ǰ�����������壬��������Ȼ�Ǵ��ڵģ��ɴ˿��ֺ����ǰ����Ĺ�ϵ��ģ�Ϳ��Բ����ڡ�
people������һ���߼�����
*/

class CDexEffectInstance;
class CPalPeople: public CDexSceneObjectModel
{
	Dex_DeclareClass(CPalPeople, CDexSceneObjectModel,0)
protected:
	typedef list<CDexEffectInstance*>  TlistEffectInstance;
	TlistEffectInstance m_listEffectInstance; //people���Ϲҵ���Ч
	float           m_fScaleInit; //������ʾ�µ�ģ������
	float           m_fScaleCurr; //��ǰ��ģ������
	int  m_standIndex;		 //վ��index
	int  m_walkIndex;
	int  m_runIndex;         //����index
	int  m_attackIndex;
	int  m_dieIndex;         //����index
	int  m_attackAnimationStartTime; //���Ź���������ÿ�ʼ�����������ǵȵ������������ŲŹ�����
	int  m_attackAnimationTime; //���Ź���������ʱ��
public:
	CPalPeople();
	virtual ~CPalPeople();

public:
	static void SetIndex(CPalPeople* people, int modelId);
public:
	int getAttackAnimationTime() {return m_attackAnimationTime;};
	int getAttackAnimationStartTime() {return m_attackAnimationStartTime;};
	int getStandIndex() { return m_standIndex;};
	int getRunIndex() { return m_runIndex;};
	int getAttackIndex() { return m_attackIndex;};
	int getDieIndex() { return m_dieIndex;};
	virtual void Reset();
	//virtual bool Update(int delta);
};

#endif
