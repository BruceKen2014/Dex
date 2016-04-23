



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
CPlane::CPlane(DexVector3 _normal, float d)
{
	m_a = _normal.x;
	m_b = _normal.y;
	m_c = _normal.z;
	m_d = d;
}
CPlane::CPlane(DexVector3 _normal, DexVector3 point)
{
	m_a = _normal.x;
	m_b = _normal.y;
	m_c = _normal.z;
	m_d = -m_a*point.x - m_b*point.y - m_c*point.z;
}
CPlane::CPlane(DexVector3 point1, DexVector3 point2, DexVector3 point3)
{
	DexVector3 vector1 = point1 - point2;
	DexVector3 vector2 = point1 - point3;
	vector1.Normalize();
	vector2.Normalize();
	DexVector3 normal = vector1.Cross(vector2);
	normal.Normalize();

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
DexVector3 CPlane::GetNormal() const
{
	return DexVector3(m_a, m_b, m_c);
}

void CPlane::SetNormal(DexVector3 normal)
{
	m_a = normal.x;
	m_b = normal.y;
	m_c = normal.z;
}
void CPlane::Setm_d(float d)
{
	m_d = d;
}
void CPlane::SetPoint(DexVector3 point)
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
int CPlane::GetRelation(DexVector3 point)
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
float CPlane::CalDistance(DexVector3 point)
{
	if(m_a == 0 && m_b == 0 && m_c == 0) //异常处理
		return 0;
	float temp = sqrt(m_a*m_a + m_b*m_b + m_c*m_c);
	return fabs(m_a*point.x + m_b*point.y + m_c*point.z + m_d)/temp;
}

DexVector3 CPlane::GetIncidence(DexVector3 _point)
{
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	float distance = CalDistance(_point);
	normal.Normalize();
	if(GetRelation(_point) == CPANEL_FRONT)
		return _point - normal * distance; 
	else
		return _point + normal * distance; 
}

int CPlane::GetRelation(CLine line)
{
	DexVector3 line_vector = line.GetVector();
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	float dot = line_vector.Dot(normal); 
	if(_equal(dot, 0.0f))
		return CPANEL_PARALLEL;
	return CPANEL_CROSS;
}

float CPlane::GetAngle(CLine line)
{
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	DexVector3 line_vector = line.GetVector();
	float temp1 = normal.Dot(line_vector); 
	float length1 = normal.Length();
	float length2 = line_vector.Length(); 
	float cos = temp1/(length1*length2);
	return _getDegree(acosf(cos));
}

float CPlane::CalDistance(CLine line)
{
	if(GetRelation(line) == CPANEL_CROSS)
		return 0;
	return CalDistance(line.GetPoint());
}

DexVector3 CPlane::GetCrossPoint(CLine line)
{
	if(GetRelation(line) == CPANEL_PARALLEL)
		return DexVector3(0xFF, 0xFF, 0xFF);
	DexVector3 ret;
	float a1, b1, c1, x1, y1, z1;  //直线参数
	DexVector3 _vector = line.GetVector();
	DexVector3 _point = line.GetPoint();
	a1 = _vector.x;
	b1 = _vector.y;
	c1 = _vector.z;
	x1 = _point.x;
	y1 = _point.y;
	z1 = _point.z;
	float temp1 = m_a * x1 + m_b * y1 + m_c * z1 + m_d;
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	float temp2 = _vector.Dot(normal); 
	float t = - temp1/temp2;
	ret.x = a1 * t + x1;
	ret.y = b1 * t + y1;
	ret.z = c1 * t + z1;
	return ret;
}


int CPlane::GetRelation(stRay ray)
{
	DexVector3 ray_vector = ray.getVector();
	DexVector3 ray_point = ray.getOrigin();
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	normal.Normalize();
	float dot = ray_vector.Dot(normal);
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

DexVector3 CPlane::GetCrossPoint(stRay line)
{
	if(GetRelation(line) == CPANEL_PARALLEL)
		return DexVector3(0xFF, 0xFF, 0xFF);
	DexVector3 ret;
	float a1, b1, c1, x1, y1, z1;  //直线参数
	DexVector3 _vector = line.getVector();
	DexVector3 _point = line.getOrigin();
	a1 = _vector.x;
	b1 = _vector.y;
	c1 = _vector.z;
	x1 = _point.x;
	y1 = _point.y;
	z1 = _point.z;
	float temp1 = m_a * x1 + m_b * y1 + m_c * z1 + m_d;
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	float temp2 = _vector.Dot(normal); 
	float t = - temp1/temp2;
	ret.x = a1 * t + x1;
	ret.y = b1 * t + y1;
	ret.z = c1 * t + z1;
	return ret;
}
int CPlane::GetRelation(CPlane panel)
{
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	DexVector3 panel_normal = panel.GetNormal();
	float angle = GetAngle(panel);
    if(_equal(angle, 0.0f) || _equal(angle, 180.f))
		return CPANEL_PARALLEL;
	return CPANEL_CROSS;
}

float CPlane::GetAngle(const CPlane& panel)
{
	DexVector3 normal = DexVector3(m_a, m_b, m_c);
	DexVector3 panel_normal = panel.GetNormal();
	float temp1 = normal.Dot(panel_normal);
	float length1 = normal.Length();
	float length2 = panel_normal.Length(); 
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
