/*************************************************************
****DemoEx引擎空间球体类CBall
****作者：阚正杰(BruceKen)
****日期：2012-12-7
*************************************************************/

#ifndef _CBALL_H
#define _CBALL_H

#include "../DexBase/typedefine.h"

#include "CRay.h"
#include "CLine.h"

typedef struct _stBall
{
	D3DXVECTOR3  m_center;
	float        m_radius;

	_stBall()
	{
		m_center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_radius = 1.0f;
	}

	//取得和点的关系
	//0： 点在球上  1：点在球内  2：点在球外
	int GetRelation(D3DXVECTOR3 point)
	{
		float length = D3DXVec3Length(&(m_center - point));
		if(_equal(length, m_radius))
			return 0;
		if(length < m_radius)
			return 1;
		return 2;
	}

	
	//取得和射线的关系
	//0：射线穿过球体  1：未穿过球体
	int GetRelation(const stRay& ray)
	{
		if(GetRelation(ray.getOrigin()) == 0
			|| GetRelation(ray.getOrigin()) == 1)
		  //起点在球内	
			return 0;
		//起点在球外
		CLine line = ray;
		float length = line.CalDistance(m_center);
		if(length <= m_radius)
		{
			//如果射线所在的直线穿过球体，那么接下来判断射线方向
			D3DXVECTOR3 temp_vector = m_center - line.GetPoint();
			D3DXVECTOR3 ray_vector = ray.getVector();
			D3DXVec3Normalize(&ray_vector,&ray_vector);
			float cross = D3DXVec3Dot(&temp_vector, &ray_vector);
			if(cross < 0.0f)
				//两向量方向相反
				return 1;
			return 0;
		}
		return 1;
	}
	//取得和另一个球的关系
	//返回值 0：相交 1:不相交
	int GetRelation(const _stBall& _other)
	{
		float length = D3DXVec3Length(&(m_center - _other.m_center));
		if(length <= (m_radius + _other.m_radius))
			return 0;
		return 1;
	}
} stBall;

#endif