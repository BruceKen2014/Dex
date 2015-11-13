

/*************************************************************
****DemoEx引擎空间几何平面类CPlane
****作者：阚正杰(BruceKen)
****日期：2012-10-31
*************************************************************/

#ifndef _CPANEL_H
#define _CPANEL_H


#include "../DexBase/typedefine.h"
#include "CLine.h"
#include "CRay.h"

enum 
{
	CPANEL_ON,       //点在平面上
	CPANEL_FRONT,    //点在平面前面
	CPANEL_BACK,     //点在平面后面
	CPANEL_CROSS,    //直线(射线)穿过平面	平面相交
	CPANEL_PARALLEL, //直线(射线)平行平面 平面平行平面
	CPLANE_NOCROSS,  //射线与平面无焦点
};

class CPlane
{
//采用空间平面的一般方程
protected:			  
	float m_a, m_b, m_c, m_d;
public:
	CPlane();
	CPlane(float a, float b, float c, float d);	      //一般初始化
	CPlane(D3DXVECTOR3 _normal, float d);			  //一般初始化重载
	CPlane(D3DXVECTOR3 _normal, D3DXVECTOR3 point);   //点法式初始化
	CPlane(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3);  //三点初始化
	~CPlane();

public:
	void  ShutDown();

	D3DXVECTOR3 GetNormal() const;                    //取得平面法线
	void        SetNormal(D3DXVECTOR3 normal);
	void        Setm_d(float d);
	void        SetPoint(D3DXVECTOR3 point);

	//处理与点的关系
	int   GetRelation(float x, float y, float z);     //点在平面上、前面、后面
	int   GetRelation(D3DXVECTOR3 point);			  
	float CalDistance(float x, float y, float z);	  //点到平面的距离
	float CalDistance(D3DXVECTOR3 point);
	D3DXVECTOR3 GetIncidence(D3DXVECTOR3 _point);  //取得投射点

	//处理与直线的关系
	int   GetRelation(CLine line);                    //平面与直线平行还是相交
	float GetAngle(CLine line);						  //取得直线与平面的夹角
	float CalDistance(CLine line);                    //如果平面直线平行，求直线到平面的距离
	D3DXVECTOR3 GetCrossPoint(CLine line);            //如果平面直线相交，取得交点

	//处理与射线的关系
	int   GetRelation(stRay ray);                                
	D3DXVECTOR3 GetCrossPoint(stRay ray);           

	//处理与平面的关系
	int   GetRelation(CPlane panel);
	float GetAngle(const CPlane& panel);	          //取得两平面的夹角(以度数为单位)

	/*以下两个方法暂不实现，留待扩展*/
	float CalDistance(CPlane panel);				  //两平面平行，计算两平面距离
	CLine GetCrossLine(CPlane panel);                 //两平面相交，取得相交直线

	
};

#endif