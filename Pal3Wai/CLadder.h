
#pragma  once

#include "CSoundObject.h"

/*
作者：阚正杰
2012.12
角色走上去会产生声音的梯子：主要用作高度检测用，很少直接用来渲染
*/

/*
以梯子下端的左顶点作为支点,也就是无论梯子朝上面方向，左下端顶点皆为pos所在点
*/
#define  LADDER_COUNT   4
#define  L_TITLE_X        10  //横向纹理平铺数量
#define  L_TITLE_Y        10  //纵向纹理平铺数量
class CLadder:public CSoundObject
{
protected:
	float m_width, m_length, m_height;   //长宽高
	int   m_dir;  //梯子朝向
	stVertex1 m_point[4];  //梯子四个顶点，road不用这个成员因为高度检测要简单得多，而梯子则要频繁使用四个顶点作高度检测
public:
	CLadder(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, float width, float length, float height, int dir, int id);
	~CLadder();

protected: 
	virtual bool  CreateVB();  //创建顶点

public:
	virtual void  ShutDown();
	virtual void  Update();
	virtual void  Render();
	virtual bool  IsOn(D3DXVECTOR3 pos);
	virtual float GetHeight(D3DXVECTOR3 pos);   //前提需判断是否处在梯子上，否则会出现远在别处，但高度与在梯子上时相同
};