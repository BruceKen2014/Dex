

/*************************************************************
****Dex引擎空间碰撞球
****作者：阚正杰(BruceKen)
****日期：2012-12-25
*************************************************************/
#pragma  once

#include "CBall.h"


/*为了调节有些物体的不规则造成碰撞检测误差过大,取消这些物体
  参加碰撞检测，然后新建一个适合的collide球放在这里代替碰撞检测
  */
class CCollideSphere:public stBall
{
protected:
	LPDIRECT3DDEVICE9 m_device;
#ifdef DEX_DEBGU   //开发模式下需要显示碰撞球,但发布的程序并不需要，发布时取消DEX_DEBGU宏，这样便没有了m_mesh，也能节省内存
	               //前提是必须所有地方都没有m_mesh的引用了，否则会出现错误
	LPD3DXMESH m_mesh;
#endif
	bool       m_render;

public:
	CCollideSphere(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 center, float radius, bool render);
	~CCollideSphere();

public:
	virtual void Shutdown();
	virtual void Render();

	virtual void SetRender(bool render);
	virtual bool GetRender();
};