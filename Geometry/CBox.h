/*************************************************************
****DemoEx引擎空间立方体类CBall
****作者：阚正杰(BruceKen)
****日期：2012-12-25
*************************************************************/

#ifndef _CBOX_H
#define _CBOX_H

#include "CBall.h"
#include "CPlane.h"

/*示意图
			7					 max
            ---------------------- 6
           /|				    /|
          / |				   / |
         /  |				  /	 | height
      4 /--------------------/5	 |
        |	|3	   *中心	 |	 |
        |	|----------------|---| 2
        |  /            	 |	/
        | /0			     | / depth
        |/-------------------|/1	
   small(pos)   width
   */

class CBox
{
protected:
	float m_posX, m_posY, m_posZ;
	float m_maxX, m_maxY, m_maxZ;
	float m_width, m_height, m_depth;
	float m_centerX, m_centerY, m_centerZ;

protected:
	DexVector3 m_point0;	 //这里将这些顶点列为成员是因为程序碰撞检测时每次都要花费堆申请和释放开销，
	DexVector3 m_point1;	     //而所以那样并节省不了内存，反而会增加CPU开销
	DexVector3 m_point2;
	DexVector3 m_point3;
	DexVector3 m_point4;
	DexVector3 m_point5;
	DexVector3 m_point6;
	DexVector3 m_point7;
public:
	CBox();
	~CBox();

public:
	virtual bool Initialize(const DexVector3& pos, const DexVector3& _size);
	virtual bool Initialize(const DexVector3& center, const DexVector3& _size, bool flagcenter);
	virtual bool IsPointIn(DexVector3 _point) const; //点是否落在立方体内(包括在面上的情况)
	
	//判断是否和球碰撞，若碰撞，取得碰撞点(球圆心的投射点)
	virtual bool CheckCollide(stBall& ball, DexVector3& inci_point = DexVector3(0.0f, 0.0f, 0.0f)) const;

	virtual bool CheckCollide(const CBox* box) const;

	virtual DexVector3 GetCenter();
	void setPosition(const DexVector3& pos);
	void setCenter(const DexVector3& pos); //将center移动到pos处
	
	DexVector3 getSize();

	DexVector3 getPoint(int index) const;//取得对应index的端点
};

#endif