

#include "CFrustum.h"

CFrustum::CFrustum()
{
}

CFrustum::~CFrustum()
{
}


void CFrustum::SetPlane(int index, D3DXVECTOR3 _normal, D3DXVECTOR3 _point)
{
	if(index < FT_FRONT || index > FT_RIGHT)
		return;
	m_planes[index].SetNormal(_normal);
	m_planes[index].SetPoint(_point);
}

CPlane CFrustum::GetPlane(int index)
{
	if(index < FT_FRONT || index > FT_RIGHT)
		return CPlane();
	return m_planes[index]; 
}

bool CFrustum::CheckPointIn(D3DXVECTOR3 _point)
{
	for(short i = 0 ; i < FT_COUNT; i++)
	{
		if(m_planes[i].GetRelation(_point) == CPANEL_BACK)
			//点落在平面后面
			return false;
	}
	return true;
}
bool CFrustum::CheckPointIn(float x, float y ,float z)
{
	for(short i = 0 ; i < FT_COUNT; i++)
	{
		if(m_planes[i].GetRelation(x, y, z) == CPANEL_BACK)
			//点落在平面后面
			return false;
	}
	return true;
}

bool CFrustum::CheckBallIn(const stBall& ball)
{
	for(short i = 0 ; i < FT_COUNT; i++)
	{
		if(m_planes[i].GetRelation(ball.m_center.x, ball.m_center.y, ball.m_center.z) == CPANEL_BACK)
		{
			//点落在平面后面
			if(m_planes[i].CalDistance(ball.m_center) <= ball.m_radius)
			{//球边缘可见
				return true;
			}
			return false;
		}
	}
	return true;
}