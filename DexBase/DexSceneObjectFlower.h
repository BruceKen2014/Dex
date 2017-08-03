


#ifndef _DEX_SCENE_OBJECT_FLOWER_H
#define _DEX_SCENE_OBJECT_FLOWER_H


//#include <list>
//#include <map>
//#include "typedefine.h"
#include "DexSceneObject.h"

class CDexTex;
class CDexSceneObjectFlower: public CDexSceneObject
{
	Dex_DeclareClass(CDexSceneObjectFlower, CDexSceneObject, 10)
protected:
	CDexTex* m_pTex;
	float m_fSwingFactor;  //摆动系数
	bool  m_bSwingFron;    //是否正在向前摆动
	float m_fCurrSwing;
	float m_fSwingSpeed;   //摆动时间
public:
	CDexSceneObjectFlower();
	virtual ~CDexSceneObjectFlower();

	virtual bool Update(int delta);
	virtual bool Render();
	virtual void Reset();

	void SetTexture(string filename);
	void setSwingFactor(float factor);
	void setSwingTime(float time);
};
#endif 