
#ifndef _PAL2GAMESTATE_TOWN_H
#define _PAL2GAMESTATE_TOWN_H
#include "state/DexGameState.h"
#include <vector>
class CModel;
class CModelAlpha;
class CModelXAni;
class CCollideSphere;
class CCollideBox;
class CSoundObject;
class CParticalSystem;
class DexGameStateTown: public DexGameState
{
public:
	DexGameStateTown(int id);
	virtual ~DexGameStateTown();
public:
	std::vector<CModel*>  g_modelVector;   //城镇中所有的模型向量
	std::vector<CModelAlpha*>  g_alphaModelVector;   //带有alpha属性的模型(树、竹子等)，Z排序将根据这些物体离摄像机的距离进行排序
	std::vector<CModelXAni*>  g_aniModelVector;	    //具有动态属性的物体
	std::vector<CCollideSphere*>  g_collideSphere; //替代碰撞球
	std::vector<CCollideBox*>  g_collideBox; //替代碰撞立方体
	std::vector<CSoundObject*> g_sndObject;  //发声物体(地面、道路、桥、梯子)
	std::vector<CParticalSystem*> g_partical;  //粒子系统如雨雪、火焰
public:		  
	//virtual void OnEvent(stEvent event);
	virtual bool ApplyRes() ;     //切换到该state前申请资源
	virtual void ReleaseRes();   //释放资源

	virtual void Update(int delta);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();
	virtual void  MouseWheelFront();
	virtual void  MouseWheelBack();
	virtual void  MouseMove(int xPos, int yPos);
	virtual void  MouseLDown(int xPos, int yPos);
	virtual void  MouseLUp(int xPos, int yPos);
	virtual void  MouseRUp(int xPos, int yPos);
	virtual void  KeyUp();
	virtual void  KeyDown();

	void SetApply(bool apply) { m_bApply = apply;}
	bool NeedApply()		  { return m_bApply;}
	int  GetStateId()		  { return m_id;};
};

#endif