



#include "CPlane.h"

CPlane::CPlane()
{//默认是XOZ平面
	m_a = 0;
	m_b = 1;
	m_c = 0;
	m_d = 0;
	
}
CPlane::CPlane(float a, float b, float c, float d)
{
	m_a = a;
	m_b = b;
	m_c = c;
	m_d = d;
}
CPlane::CPlane(D3DXVECTOR3 _normal, float d)
{
	m_a = _normal.x;
	m_b = _normal.y;
	m_c = _normal.z;
	m_d = d;
}
CPlane::CPlane(D3DXVECTOR3 _normal, D3DXVECTOR3 point)
{
	m_a = _normal.x;
	m_b = _normal.y;
	m_c = _normal.z;
	m_d = -m_a*point.x - m_b*point.y - m_c*point.z;
}
CPlane::CPlane(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3)
{
	D3DXVECTOR3 vector1 = point1 - point2;
	D3DXVECTOR3 vector2 = point1 - point3;
	D3DXVec3Normalize(&vector1, &vector1);
	D3DXVec3Normalize(&vector2, &vector2);
	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal, &vector1, &vector2);
	D3DXVec3Normalize(&normal, &normal);
	m_a = normal.x;
	m_b = normal.y;
	m_c = normal.z;
	m_d = -m_a*point1.x - m_b*point1.y - m_c*point1.z;
}
CPlane::~CPlane()
{
	ShutDown();
}
void CPlane::ShutDown()
{
}
D3DXVECTOR3 CPlane::GetNormal() const
{
	return D3DXVECTOR3(m_a, m_b ,m_c);
}

void CPlane::SetNormal(D3DXVECTOR3 normal)
{
	m_a = normal.x;
	m_b = normal.y;
	m_c = normal.z;
}
void CPlane::Setm_d(float d)
{
	m_d = d;
}
void CPlane::SetPoint(D3DXVECTOR3 point)
{
	m_d = -m_a*point.x - m_b*point.y - m_c*point.z;
}
int CPlane::GetRelation(float x, float y, float z)
{
	if(m_a == 0 && m_b == 0 && m_c == 0) //异常处理
		return 0;
	float temp = sqrt(m_a*m_a + m_b*m_b + m_c*m_c);
	float distance = m_a*x + m_b*y + m_c*z + m_d/temp;
	if(distance > 0.001)
		return CPANEL_FRONT;
	if(distance < -0.001)
		return CPANEL_BACK;
	return CPANEL_ON;
}
int CPlane::GetRelation(D3DXVECTOR3 point)
{
	if(m_a == 0 && m_b == 0 && m_c == 0) //异常处理
		return 0;
	float temp = sqrt(m_a*m_a + m_b*m_b + m_c*m_c);
	float distance = m_a*point.x + m_b*point.y + m_c*point.z + m_d/temp;
	if(distance > 0.001)
		return CPANEL_FRONT;
	if(distance < -0.001)
		return CPANEL_BACK;
	return CPANEL_ON;
}
float CPlane::CalDistance(float x, float y, float z)
{
	if(m_a == 0 && m_b == 0 && m_c == 0) //异常处理
		return 0;
	float temp = sqrt(m_a*m_a + m_b*m_b + m_c*m_c);
	return fabs(m_a*x + m_b*y + m_c*z + m_d)/temp;
}
float CPlane::CalDistance(D3DXVECTOR3 point)
{
	if(m_a == 0 && m_b == 0 && m_c == 0) //异常处理
		return 0;
	float temp = sqrt(m_a*m_a + m_b*m_b + m_c*m_c);
	return fabs(m_a*point.x + m_b*point.y + m_c*point.z + m_d)/temp;
}

D3DXVECTOR3 CPlane::GetIncidence(D3DXVECTOR3 _point)
{
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	float distance = CalDistance(_point);
	D3DXVec3Normalize(&normal, &normal);
	if(GetRelation(_point) == CPANEL_FRONT)
		return _point - normal * distance; 
	else
		return _point + normal * distance; 
}

int CPlane::GetRelation(CLine line)
{
	D3DXVECTOR3 line_vector = line.GetVector();
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	float dot = D3DXVec3Dot(&line_vector, &normal); 
	if(_equal(dot, 0.0f))
		return CPANEL_PARALLEL;
	return CPANEL_CROSS;
}

float CPlane::GetAngle(CLine line)
{
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	D3DXVECTOR3 line_vector = line.GetVector();
	float temp1 = D3DXVec3Dot(&normal, &line_vector);
	float length1 = D3DXVec3Length(&normal);
	float length2 = D3DXVec3Length(&line_vector);
	float cos = temp1/(length1*length2);
	return _getDegree(acosf(cos));
}

float CPlane::CalDistance(CLine line)
{
	if(GetRelation(line) == CPANEL_CROSS)
		return 0;
	return CalDistance(line.GetPoint());
}

D3DXVECTOR3 CPlane::GetCrossPoint(CLine line)
{
	if(GetRelation(line) == CPANEL_PARALLEL)
		return D3DXVECTOR3(0xFF, 0xFF, 0xFF);
	D3DXVECTOR3 ret;
	float a1, b1, c1, x1, y1, z1;  //直线参数
	D3DXVECTOR3 _vector = line.GetVector();
	D3DXVECTOR3 _point  = line.GetPoint();
	a1 = _vector.x;
	b1 = _vector.y;
	c1 = _vector.z;
	x1 = _point.x;
	y1 = _point.y;
	z1 = _point.z;
	float temp1 = m_a * x1 + m_b * y1 + m_c * z1 + m_d;
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	float temp2 = D3DXVec3Dot(&_vector, &normal); 
	float t = - temp1/temp2;
	ret.x = a1 * t + x1;
	ret.y = b1 * t + y1;
	ret.z = c1 * t + z1;
	return ret;
}


int CPlane::GetRelation(stRay ray)
{
	D3DXVECTOR3 ray_vector = ray.getVector();
	D3DXVECTOR3 ray_point = ray.getOrigin();
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	D3DXVec3Normalize(&normal, &normal);
	float dot = D3DXVec3Dot(&ray_vector, &normal); 
	if(_equal(dot, 0.0f))
		return CPANEL_PARALLEL;

	int relate = GetRelation(ray_point) ; 
	if(relate == CPANEL_ON)
		return CPANEL_CROSS;
	if(dot > 0)
	{//两向量夹角为正
		if(relate == CPANEL_BACK)
			return CPANEL_CROSS;
	}
	if(dot < 0)
	{//夹角为负
		if(relate == CPANEL_FRONT)
			return CPANEL_CROSS;
	}
	return CPLANE_NOCROSS;
}

D3DXVECTOR3 CPlane::GetCrossPoint(stRay line)
{
	if(GetRelation(line) == CPANEL_PARALLEL)
		return D3DXVECTOR3(0xFF, 0xFF, 0xFF);
	D3DXVECTOR3 ret;
	float a1, b1, c1, x1, y1, z1;  //直线参数
	D3DXVECTOR3 _vector = line.getVector();
	D3DXVECTOR3 _point  = line.getOrigin();
	a1 = _vector.x;
	b1 = _vector.y;
	c1 = _vector.z;
	x1 = _point.x;
	y1 = _point.y;
	z1 = _point.z;
	float temp1 = m_a * x1 + m_b * y1 + m_c * z1 + m_d;
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	float temp2 = D3DXVec3Dot(&_vector, &normal); 
	float t = - temp1/temp2;
	ret.x = a1 * t + x1;
	ret.y = b1 * t + y1;
	ret.z = c1 * t + z1;
	return ret;
}
int CPlane::GetRelation(CPlane panel)
{
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	D3DXVECTOR3 panel_normal = panel.GetNormal();
	float angle = GetAngle(panel);
    if(_equal(angle, 0.0f) || _equal(angle, 180.f))
		return CPANEL_PARALLEL;
	return CPANEL_CROSS;
}

float CPlane::GetAngle(const CPlane& panel)
{
	D3DXVECTOR3 normal = D3DXVECTOR3(m_a, m_b, m_c);
	D3DXVECTOR3 panel_normal = panel.GetNormal();
	float temp1 = D3DXVec3Dot(&normal, &panel_normal);
	float length1 = D3DXVec3Length(&normal);
	float length2 = D3DXVec3Length(&panel_normal);
	float cos = temp1/(length1*length2);
	return _getDegree(acosf(cos));
}

float CPlane::CalDistance(CPlane panel)
{
	return 0;
}
CLine CPlane::GetCrossLine(CPlane panel)
{
	CLine line;
	return line;
}
