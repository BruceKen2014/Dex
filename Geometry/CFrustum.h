/*************************************************************
****Dex����ƽ��ͷ����CFrustum
****���ߣ�������(BruceKen)
****���ڣ�2012.12
*************************************************************/

#pragma  once

#include "CPlane.h"
#include "CBall.h"
#include "../DexMath/DexVector3.h"

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
	void   SetPlane(int index, DexVector3 _normal, DexVector3 _point); //���������
	CPlane GetPlane(int index);

	bool   CheckPointIn(DexVector3 _point); //�жϵ��Ƿ���ƽ��ͷ����
	bool   CheckPointIn(float x, float y ,float z);	   
	bool   CheckBallIn(const stBall& ball);	 //�ж����Ƿ�����ƽ��ͷ����
};