

/*
作者：阚正杰
2012.12
角色走上去会产生声音的马路、地面等
*/

#pragma  once

#include "CSoundObject.h"
 

/*顶点示意
   1				  3
   |------------------|
   |				  |length
   |				  |
   |------------------| 2
   0(pos)  width
*/
//地面横纵属性影响UV
enum
{
	RD_EW, //东西
	RD_SN, //南北
};
#define  CROAD_VERTEX   4
#define  TITLE_X        1  //横向纹理平铺数量
#define  TITLE_Y        1  //纵向纹理平铺数量
class CRoad: public CSoundObject
{
protected:
	float m_width, m_length;   //马路、地面的长宽
	float m_tileX, m_tileY;    //横向、纵向平铺数量
	int  m_dir;               //RD_EW 南北朝向 RD_SN 东西朝向
public:
	CRoad(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, float width, float length, int id, int dir, float tileX, float tileY);
	~CRoad();

protected: 
	virtual bool  CreateVB();  //创建顶点

public:
	virtual void  ShutDown();
	virtual void  Update();
	virtual void  Render();
	virtual bool  IsOn(D3DXVECTOR3 pos);
	virtual float GetHeight(D3DXVECTOR3 pos) ;
};