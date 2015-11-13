/*************************************************************
****Dex����ƽ��ͷ����CFrustum
****���ߣ�������(BruceKen)
****���ڣ�2012.12
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
	CPlane  m_planes[FT_COUNT]; //������

public:
	CFrustum();
	~CFrustum();

public:
	void   SetPlane(int index, D3DXVECTOR3 _normal, D3DXVECTOR3 _point); //���������
	CPlane GetPlane(int index);

	bool   CheckPointIn(D3DXVECTOR3 _point); //�жϵ��Ƿ���ƽ��ͷ����
	bool   CheckPointIn(float x, float y ,float z);	   
	bool   CheckBallIn(const stBall& ball);	 //�ж����Ƿ�����ƽ��ͷ����
};