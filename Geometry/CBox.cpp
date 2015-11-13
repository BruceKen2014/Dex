


#include "CBox.h"


CBox::CBox()
{
}

bool CBox::Initialize(const D3DXVECTOR3& pos, const D3DXVECTOR3& _size)
{
	m_posX = pos.x;	 
	m_posY = pos.y;
	m_posZ = pos.z;

	m_width = _size.x;
	m_height = _size.y;
	m_depth = _size.z;

	m_maxX = m_posX + m_width;
	m_maxY = m_posY + m_height;
	m_maxZ = m_posZ + m_depth;

	m_centerX = m_posX + m_width/2;
	m_centerY = m_posY + m_height/2;
	m_centerZ = m_posZ + m_depth/2;

	m_point0 = D3DXVECTOR3(m_posX, m_posY, m_posZ);
	m_point1 = D3DXVECTOR3(m_maxX, m_posY, m_posZ);
	m_point2 = D3DXVECTOR3(m_maxX, m_posY, m_maxZ);
	m_point3 = D3DXVECTOR3(m_posX, m_posY, m_maxZ);
	m_point4 = D3DXVECTOR3(m_posX, m_maxY, m_posZ);
	m_point5 = D3DXVECTOR3(m_maxX, m_maxY, m_posZ);
	m_point6 = D3DXVECTOR3(m_maxX, m_maxY, m_maxZ);
	m_point7 = D3DXVECTOR3(m_posX, m_maxY, m_maxZ);
	return true;
}

bool CBox::Initialize(const D3DXVECTOR3& center, const D3DXVECTOR3& _size, bool flagcenter)
{
	m_posX = center.x - _size.x;	 
	m_posY = center.y - _size.y;	 
	m_posZ = center.z - _size.z;	 

	m_width = _size.x;
	m_height = _size.y;
	m_depth = _size.z;

	m_maxX = m_posX + m_width;
	m_maxY = m_posY + m_height;
	m_maxZ = m_posZ + m_depth;

	m_centerX = center.x;
	m_centerY = center.y;
	m_centerZ = center.z;

	m_point0 = D3DXVECTOR3(m_posX, m_posY, m_posZ);
	m_point1 = D3DXVECTOR3(m_maxX, m_posY, m_posZ);
	m_point2 = D3DXVECTOR3(m_maxX, m_posY, m_maxZ);
	m_point3 = D3DXVECTOR3(m_posX, m_posY, m_maxZ);
	m_point4 = D3DXVECTOR3(m_posX, m_maxY, m_posZ);
	m_point5 = D3DXVECTOR3(m_maxX, m_maxY, m_posZ);
	m_point6 = D3DXVECTOR3(m_maxX, m_maxY, m_maxZ);
	m_point7 = D3DXVECTOR3(m_posX, m_maxY, m_maxZ);
	return true;
}
CBox::~CBox()
{
}

bool CBox::IsPointIn(D3DXVECTOR3 _point) const
{
	if( _point.x >= m_posX && _point.x <= m_maxX &&
		_point.y >= m_posY && _point.y <= m_maxY &&
		_point.z >= m_posZ && _point.z <= m_maxZ)
		return true;
	return false;
}
D3DXVECTOR3 CBox::GetCenter()
{
	return D3DXVECTOR3(m_centerX, m_centerY, m_centerZ);
}

void CBox::setPosition(const D3DXVECTOR3& pos)
{
	m_posX = pos.x;	 
	m_posY = pos.y;
	m_posZ = pos.z;

	m_maxX = m_posX + m_width;
	m_maxY = m_posY + m_height;
	m_maxZ = m_posZ + m_depth;

	m_centerX = m_posX + m_width/2;
	m_centerY = m_posY + m_height/2;
	m_centerZ = m_posZ + m_depth/2;

	m_point0 = D3DXVECTOR3(m_posX, m_posY, m_posZ);
	m_point1 = D3DXVECTOR3(m_maxX, m_posY, m_posZ);
	m_point2 = D3DXVECTOR3(m_maxX, m_posY, m_maxZ);
	m_point3 = D3DXVECTOR3(m_posX, m_posY, m_maxZ);
	m_point4 = D3DXVECTOR3(m_posX, m_maxY, m_posZ);
	m_point5 = D3DXVECTOR3(m_maxX, m_maxY, m_posZ);
	m_point6 = D3DXVECTOR3(m_maxX, m_maxY, m_maxZ);
	m_point7 = D3DXVECTOR3(m_posX, m_maxY, m_maxZ);
}

void CBox::setCenter(const D3DXVECTOR3& pos)
{
	D3DXVECTOR3 new_pos;
	new_pos.x = pos.x - m_width/2;
	new_pos.y = pos.y - m_height/2;
	new_pos.z = pos.z - m_depth/2;
	setPosition(new_pos);
}


D3DXVECTOR3 CBox::getSize()
{
	return D3DXVECTOR3(m_width, m_height, m_depth);
}

D3DXVECTOR3 CBox::getPoint(int index) const
{
	switch(index)
	{
	case 0:
		return m_point0;
	case 1:
		return m_point1;
	case 2:
		return m_point2;
	case 3:
		return m_point3;
	case 4:
		return m_point4;
	case 5:
		return m_point5;
	case 6:
		return m_point6;
	case 7:
		return m_point7;
	default:
		break;
	}
	return D3DXVECTOR3(0,0,0);
}

bool CBox::CheckCollide(const CBox* box) const
{
	if(box == NULL)
		return false;
	for(int i = 0; i < 8; i++)
	{
		if(IsPointIn(box->getPoint(i)))
			return true;
		if(box->IsPointIn(getPoint(i)))
			return true;
	}
	return false;
}


bool CBox::CheckCollide(stBall &ball, D3DXVECTOR3& inci_point) const
{
	D3DXVECTOR3 ball_center = ball.m_center;
	float ball_radius = ball.m_radius;
	//根据圆球的中心位置将立方体所在空间分为26个区间
	//正上方、正下方、正前方、正后方、正左方、正右方 6
	//左上方、左下方、右上方、右下方、前上方、前下方、后上方、后下方  8
	//左前方、左后方、右前方、右后方  4
	//左前上方、左前下方、左后上方、左后下方、右前上方、右前下方、右后上方、右后下方   8

	if( ball_center.x >= m_posX)
	{
		if(ball_center.y >= m_posY)
		{
			if(ball_center.z >= m_posZ)
			{
				if(ball_center.x >= m_maxX)
				{
					if(ball_center.y >= m_maxY)
					{
						if(ball_center.z >= m_maxZ)
						{// x>x1 y>y1 z>z1
						 //右前上方,若碰撞必然和 顶点6 碰撞
							if(D3DXVec3Length(&(m_point6 - ball_center)) <= ball_radius)
							{
								inci_point = m_point6;
								return true;
							}
							return false;
						}
						else
						{//x>x1 y>y1 z<z1
							//右上方，若碰撞必然和线段56碰撞
							CLine line(m_point5, m_point6, 0);
							if(line.CalDistance(ball_center) <= ball_radius)
							{
								inci_point = line.GetIncidence(ball_center);
								return true;
							}
							return false;
						}
					}
					else
					{
						if(ball_center.z >= m_maxZ)
						{//x>x1 y<y1 z>z1
						 //右前方，若碰撞必然和线段26碰撞
							CLine line(m_point2, m_point6, 0);
							if(line.CalDistance(ball_center) <= ball_radius)
							{
								inci_point = line.GetIncidence(ball_center);
								return true;
							}
							return false;
						}
						else
						{//x>x1 y<y1 z<z1
						//正右方,若碰撞必然和右面1256碰撞
							CPlane plane(m_point2, m_point5, m_point6);
							if(plane.CalDistance(ball_center) <= ball_radius) 
							{
								inci_point = plane.GetIncidence(ball_center);
								return true;
							}
							return false;
						}
					}
				}
				else
				{
					if(ball_center.y >= m_maxY)
					{
						if(ball_center.z >= m_maxZ)
						{//x<x1 y>y1 z>z1
						//前上方,若碰撞必然和 线段67 碰撞
							CLine line(m_point6, m_point7, 0);
							if(line.CalDistance(ball_center) <= ball_radius)
							{
								inci_point = line.GetIncidence(ball_center);
								return true;
							}
							return false;
						}
						else
						{//x<x1 y>y1 z<z1 
						//正上方，若碰撞必然和上面4567碰撞
							CPlane plane(m_point4, m_point5, m_point6);
							if(plane.CalDistance(ball_center) <= ball_radius) 
							{
								inci_point = plane.GetIncidence(ball_center);
								return true;
							}
							return false;
						}
					}
					else
					{
						if(ball_center.z >= m_maxZ)
						{//x<x1 y<y1 z>z1
						//正前方，若碰撞必然前面2367碰撞
							CPlane plane(m_point2, m_point3, m_point6);
							if(plane.CalDistance(ball_center) <= ball_radius) 
							{
								inci_point = plane.GetIncidence(ball_center);
								return true;
							}
							return false;
						}
						else
						{//x<x1 y<y1 z<z1
							//中心在立方体内
							inci_point = ball_center;
							return true;
						}
					}
				}
			}
			else
			{//z<z0 故没必要在比较maxZ了
				if(ball_center.x >= m_maxX)
				{
					if(ball_center.y >= m_maxY)
					{//x>x0 y>y0 z<z0   x>x1 y>y1
						//右后上方,若碰撞必然和顶点5碰撞
						if(D3DXVec3Length(&(m_point5 - ball_center)) <= ball_radius)
						{
							inci_point = m_point5;
							return true;
						}
						return false;
					}
					else
					{//x>x0 y>y0 z<z0   x>x1 y<y1
						//右后方，若碰撞必然和 线段15碰撞
						CLine line(m_point1, m_point5, 0);
						if(line.CalDistance(ball_center) <= ball_radius)
						{
							inci_point = line.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
				}
				else
				{
					if(ball_center.y >= m_maxY)
					{//x>x0 y>y0 z<z0   x<x1 y>y1
						//后上方,若碰撞必然和 线段45碰撞
						CLine line(m_point3, m_point5, 0);
						if(line.CalDistance(ball_center) <= ball_radius)
						{
							inci_point = line.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
					else
					{//x>x0 y>y0 z<z0   x<x1 y<y1
						//正后方，和面0154碰撞
						CPlane plane(m_point0, m_point1, m_point5);
						if(plane.CalDistance(ball_center) <= ball_radius) 
						{
							inci_point = plane.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
				}
			}
		}
		else
		{//y<y0，故没必要再比较maxY了
			if(ball_center.z >= m_posZ)
			{
				if(ball_center.x >= m_maxX)
				{
					if(ball_center.z >= m_maxZ)
					{//x>x0 y<y0 z>z0 x>x1 z>z1
						//右前下方，和 顶点2 碰撞 
						if(D3DXVec3Length(&(m_point2 - ball_center)) <= ball_radius)
						{
							inci_point = m_point2;
							return true;
						}
						return false;
					}
					else
					{//x>x0 y<y0 z>z0 x>x1 z<z1
						//右下方,和 线段12 碰撞 
						CLine line(m_point1, m_point2, 0);
						if(line.CalDistance(ball_center) <= ball_radius)
						{
							inci_point = line.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
				}
				else
				{
					if(ball_center.z >= m_maxZ)
					{//x>x0 y<y0 z>z0 x<x1 z>z1
						//前下方，和 线段23 碰撞
						CLine line(m_point2, m_point3, 0);
						if(line.CalDistance(ball_center) <= ball_radius)
						{
							inci_point = line.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
					else
					{//x>x0 y<y0 z>z0 x<x1 z<z1
						//正下方, 和 面0123 碰撞
						CPlane plane(m_point0, m_point1, m_point2);
						if(plane.CalDistance(ball_center) <= ball_radius) 
						{
							inci_point = plane.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
				}
			}
			else
			{//z<z0 故没必要在比较maxZ了
				if(ball_center.x >= m_maxX)
				{//x>x0 y<y0 z<z0     x>x1
					//右后下方 和顶点1碰撞
					if(D3DXVec3Length(&(m_point1 - ball_center)) <= ball_radius)
					{
						inci_point = m_point1;
						return true;
					}
					return false;
				}
				else
				{//x>x0 y<y0 z<z0   x<x1
					//后下方， 和 线段01 碰撞
					CLine line(m_point0, m_point1, 0);
					if(line.CalDistance(ball_center) <= ball_radius)
					{
						inci_point = line.GetIncidence(ball_center);
						return true;
					}
					return false;
				}
			}
		}
	}
	else
	{//x<x0 没必要比较maxX了
		if(ball_center.y >= m_posY)
		{
			if(ball_center.z >= m_posZ)
			{
				if(ball_center.y >= m_maxY)
				{
					if(ball_center.z >= m_maxZ)
					{//x<x0 y>y0 z>z0    y>y1 z>z1
						//左前上方,若碰撞必然和 顶点7 碰撞
						if(D3DXVec3Length(&(m_point7 - ball_center)) <= ball_radius)
						{
							inci_point = m_point7;
							return true;
						}
						return false;
					}
					else
					{//x<x0 y>y0 z>z0    y>y1 z<z1 
						//左上方， 线段47
						CLine line(m_point4, m_point7, 0);
						if(line.CalDistance(ball_center) <= ball_radius)
						{
							inci_point = line.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
				}
				else
				{
					if(ball_center.z >= m_maxZ)
					{//x<x0 y>y0 z>z0    y<y1 z>z1
						//左前方 线段37
						CLine line(m_point3, m_point7, 0);
						if(line.CalDistance(ball_center) <= ball_radius)
						{
							inci_point = line.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
					else
					{//x<x0 y>y0 z>z0    y<y1 z<z1
						//正左方 ，面0374
						CPlane plane(m_point0, m_point3, m_point7);
						if(plane.CalDistance(ball_center) <= ball_radius) 
						{
							inci_point = plane.GetIncidence(ball_center);
							return true;
						}
						return false;
					}
				}
			}
			else
			{//z<z0 故没必要在比较maxZ了
				if(ball_center.y >= m_maxY)
				{//x<x0 y>y0 z<z0   y>y1
					//左后上方,顶点4
					if(D3DXVec3Length(&(m_point4 - ball_center)) <= ball_radius)
					{
						inci_point = m_point4;
						return true;
					}
					return false;
				}
				else
				{//x<x0 y>y0 z<z0   y<y1
					//左后方 线段04
					CLine line(m_point0, m_point4, 0);
					if(line.CalDistance(ball_center) <= ball_radius)
					{
						inci_point = line.GetIncidence(ball_center);
						return true;
					}
					return false;
				}
			}
		}
		else
		{//y<y0，故没必要再比较maxY了
			if(ball_center.z >= m_posZ)
			{
				if(ball_center.z >= m_maxZ)
				{//x<x0 y<y0 z>z0  z>z1
					//左前下方，和 顶点3 碰撞
					if(D3DXVec3Length(&(m_point3 - ball_center)) <= ball_radius)
					{
						inci_point = m_point3;
						return true;
					}
					return false;
				}
				else
				{//x<x0 y<y0 z>z0  z>z1
					//左下方 线段03
					CLine line(m_point0, m_point3, 0);
					if(line.CalDistance(ball_center) <= ball_radius)
					{
						inci_point = line.GetIncidence(ball_center);
						return true;
					}
					return false;
				}
			}
			else
			{//z<z0 故没必要在比较maxZ了
				//左后下方 顶点0
				if(D3DXVec3Length(&(m_point0 - ball_center)) <= ball_radius)
				{
					inci_point = m_point0;
					return true;
				}
				return false;
			}
		}
	}
	
}