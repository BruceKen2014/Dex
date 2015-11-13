/*************************************************************
****DemoEx����ռ�������CRay
****���ߣ�������(BruceKen)
****���ڣ�2012-12-6
*************************************************************/

#ifndef _CRAY_H
#define _CRAY_H
//���߽ṹ
typedef struct _stRay
{
	D3DXVECTOR3 m_vector;  //���䷽��
	D3DXVECTOR3 m_origin;  //�����

	_stRay()
	{
		m_vector = D3DXVECTOR3(0,0,1);
		m_origin = D3DXVECTOR3(0,0,0);
	}
	D3DXVECTOR3 getVector() const
	{
		//D3DXVec3Normalize(&m_vector, &m_vector);
		return m_vector;
	}
	D3DXVECTOR3 getOrigin() const
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