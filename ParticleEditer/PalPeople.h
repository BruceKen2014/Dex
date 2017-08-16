

#ifndef _PAL_PEOPLE_H
#define _PAL_PEOPLE_H

#include "DexBase/DexObject.h"
#include "DexBase/DexSceneObjectModel.h"
/*
关于这里游戏中人物是继承模型还是包含模型,暂时两种方法没发现什么大的功能差距。从现实角度，人当然是一个模型，
由此看好像是继承的关系，但是从抽象的哲学上来说，人即使没有模型依然具有现实的存在，比如我们谈一个人，这个人
的身体不一定非要存在面前，脱离了肉体，精神上依然是存在的，由此看又好像是包含的关系，模型可以不存在。
people更多是一种逻辑存在
*/

class CDexEffectInstance;
class CPalPeople: public CDexSceneObjectModel
{
	Dex_DeclareClass(CPalPeople, CDexSceneObjectModel,0)
protected:
	typedef list<CDexEffectInstance*>  TlistEffectInstance;
	TlistEffectInstance m_listEffectInstance; //people身上挂的特效
	float           m_fScaleInit; //正常显示下的模型缩放
	float           m_fScaleCurr; //当前的模型缩放
	int  m_standIndex;		 //站立index
	int  m_walkIndex;
	int  m_runIndex;         //奔跑index
	int  m_attackIndex;
	int  m_dieIndex;         //死亡index
	int  m_attackAnimationStartTime; //播放攻击动画多久开始攻击（并不是等到攻击动画播放才攻击）
	int  m_attackAnimationTime; //播放攻击动画的时间
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
