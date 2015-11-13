/*************************************************************
****Dex引擎平截头体类CFrustum
****作者：阚正杰(BruceKen)
****日期：2012.12
*************************************************************/

#pragma  once

#include "CPlane.h"
#include "CBall.h"

#define FT_COUNT   6
enum
{ 
	FT_FRONT,
	FT_BACK,
	FT_TOP,
	FT_BOTTOM,
	FT_LEFT,
	FT_RIGHT
};
class CFrustum
{
protected:
	CPlane  m_planes[FT_COUNT]; //六个面

public:
	CFrustum();
	~CFrustum();

public:
	void   SetPlane(int index, D3DXVECTOR3 _normal, D3DXVECTOR3 _point); //设置面参数
	CPlane GetPlane(int index);

	bool   CheckPointIn(D3DXVECTOR3 _point); //判断点是否在平截头体中
	bool   CheckPointIn(float x, float y ,float z);	   
	bool   CheckBallIn(const stBall& ball);	 //判断球是否落在平截头体中
};