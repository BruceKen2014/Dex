/*************************************************************
****DemoEx����ռ�������CBall
****���ߣ�������(BruceKen)
****���ڣ�2012-12-7
*************************************************************/

#ifndef _CBALL_H
#define _CBALL_H

#include "../DexBase/typedefine.h"

#include "CRay.h"
#include "CLine.h"

typedef struct _stBall
{
	DexVector3  m_center;
	float        m_radius;

	_stBall()
	{
		m_center = DexVector3(0.0f, 0.0f, 0.0f);
		m_radius = 1.0f;
	}

	//ȡ�ú͵�Ĺ�ϵ
	//0�� ��������  1����������  2����������
	int GetRelation(DexVector3 point)
	{
		float length = (m_center - point).Length();
		if(_equal(length, m_radius))
			return 0;
		if(length < m_radius)
			return 1;
		return 2;
	}

	
	//ȡ�ú����ߵĹ�ϵ
	//0�����ߴ�������  1��δ��������
	int GetRelation(const stRay& ray)
	{
		if(GetRelation(ray.getOrigin()) == 0
			|| GetRelation(ray.getOrigin()) == 1)
		  //���������	
			return 0;
		//���������
		CLine line = ray;
		float length = line.CalDistance(m_center);
		if(length <= m_radius)
		{
			//����������ڵ�ֱ�ߴ������壬��ô�������ж����߷���
			DexVector3 temp_vector = m_center - line.GetPoint();
			DexVector3 ray_vector = ray.getVector();
			ray_vector.Normalize();
			float cross = temp_vector.Dot(ray_vector); 
			if(cross < 0.0f)
				//�����������෴
				return 1;
			return 0;
		}
		return 1;
	}
	//ȡ�ú���һ����Ĺ�ϵ
	//����ֵ 0���ཻ 1:���ཻ
	int GetRelation(const _stBall& _other)
	{
		float length = (m_center - _other.m_center).Length();
		if(length <= (m_radius + _other.m_radius))
			return 0;
		return 1;
	}
} stBall;

#endif