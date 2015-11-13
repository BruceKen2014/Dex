
/*************************************************************
****DemoEx引擎空间几何直线类CLine
****作者：阚正杰(BruceKen)
****日期：2012-10-29
*************************************************************/

#ifndef _CLINE_H
#define _CLINE_H


#include "../DexBase/typedefine.h"
#include "CRay.h"

//两条直线的关系
enum 
{
	PARALLEL,    //平行
	CROSS,       //交叉
	DIFFFACE,    //different face,异面
};  


//3D空间直线类
class CLine
{
//采用对称式(点向式)方程

protected:
	D3DXVECTOR3 m_vector;    //直线向量
	D3DXVECTOR3 m_point;	 //直线上的某一点

public:
	CLine();
	CLine(D3DXVECTOR3 _vector, D3DXVECTOR3 _point);
	CLine(D3DXVECTOR3 point1, D3DXVECTOR3 point2, int flag);
	//拷贝构造
	CLine(const stRay& ray);
	virtual ~CLine();

public:
	const D3DXVECTOR3 GetVector() const;
	const D3DXVECTOR3 GetPoint() const;
	void SetVector(D3DXVECTOR3 _vector);
	void SetPoint(D3DXVECTOR3 _point);

	//处理与点的关系
	bool  IsOnLine(float x, float y, float z);       //判断某点是否在直线上
	float CalDistance(float x, float y , float z);   //计算某点到该直线的距离
	float CalDistance(D3DXVECTOR3 _point);
	D3DXVECTOR3 GetIncidence(D3DXVECTOR3 _point);     //取得点到直线的投射点

	//处理与直线的关系
	int   GetRelation(CLine line);	         //取得和另一条直线的关系

	/*以下方法暂不实现，留待扩展*/
	float CalDistance(CLine line);	         //计算两条直线的距离

	CLine& operator=(const CLine& _line); 

	//bool  
};

#endif