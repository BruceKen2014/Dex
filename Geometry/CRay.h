/*************************************************************
****DemoEx����ռ�������CRay
****���ߣ�������(BruceKen)
****���ڣ�2012-12-6
*************************************************************/

#ifndef _CRAY_H
#define _CRAY_H
//���߽ṹ
#include "../DexMath/DexVector3.h"
typedef struct _stRay
{
	DexVector3 m_vector;  //���䷽��
	DexVector3 m_origin;  //�����

	_stRay()
	{
		m_vector = DexVector3(0, 0, 1);
		m_origin = DexVector3(0, 0, 0);
	}
	DexVector3 getVector() const
	{
		//D3DXVec3Normalize(&m_vector, &m_vector);
		return m_vector;
	}
	DexVector3 getOrigin() const
	{
		return m_origin;
	}
} stRay;

class CRay
{
protected:
	stRay   m_Ray;
};
#endif