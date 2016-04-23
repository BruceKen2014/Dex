

#include "CLine.h"


CLine::CLine()
{
	//默认是向上的Y轴
	m_vector = DexVector3(0, 1, 0);
	m_point = DexVector3(0, 0, 0);
}

CLine::CLine(DexVector3 _vector, DexVector3 _point)
{
	m_vector = _vector;
	m_point  = _point;
	if(m_point.x == 0 && m_point.y == 0 && m_point.z ==0)
		//如果点为原点，则稍微偏移一点
		m_point += m_vector*1;
}

CLine::CLine(DexVector3 point1, DexVector3 point2, int flag)
{
	m_vector = point1 - point2;
	m_point = point1;
	if((point1.x == 0 && point1.y == 0  && point1.z ==0) || (point2.x == 0 && point2.y == 0  && point2.z ==0))
		//如果点为原点，则稍微偏移一点
		m_point += m_vector*1;
}

CLine::CLine(const stRay& ray)
{
	m_vector = ray.m_vector;
	m_point = ray.m_origin;
}

CLine::~CLine()
{
}

const DexVector3 CLine::GetVector() const
{
	return m_vector;
}
const DexVector3 CLine::GetPoint() const
{
	return m_point;
}
void CLine::SetVector(DexVector3 _vector)
{
	m_vector = _vector;
}
void CLine::SetPoint(DexVector3 _point)
{
	m_point = _point;
}

bool CLine::IsOnLine(float x, float y, float z)
{
	float _error = 1e-08f; 
	float x_minus_y, x_minus_z, y_minus_z;
	x_minus_y = x_minus_z = y_minus_z = 0;
	if(m_vector.x != 0)
	{
		if(m_vector.y != 0)
		{
			if(m_vector.z != 0)
			{
				x_minus_y = fabs((x - m_point.x)/m_vector.x - (y - m_point.y)/m_vector.y);
				x_minus_z = fabs((x - m_point.x)/m_vector.x - (z - m_point.z)/m_vector.z);
				if((x_minus_y < _error)
					&& x_minus_z < _error)
					return true;
				else
					return false;

			}
			else
			{
				if(z != m_point.z)
					return false;
				x_minus_y = fabs((x - m_point.x)/m_vector.x - (y - m_point.y)/m_vector.y);
				if( x_minus_y < _error)
					return true;
				else
					return false;
			}
		}
		else
		{
			if( y != m_point.y)
				return false;
			if(m_vector.z != 0)
			{
				x_minus_z = fabs((x - m_point.x)/m_vector.x - (z - m_point.z)/m_vector.z);
				if(( x_minus_z < _error))
					return true;
				else
					return false;

			}
			else
			{
				if(z != m_point.z)
					return false;
				return true;
			}
		}
	}
	else
	{
		if(x != m_point.x)
			return false;
		if(m_vector.y != 0)
		{
			if(m_vector.z != 0)
			{
				y_minus_z = fabs((y - m_point.y)/m_vector.y - (z - m_point.z)/m_vector.z); 
				if( y_minus_z < _error)
					return true;
				else
					return false;

			}
			else
			{
				if(z != m_point.z)
					return false;
				return true;
			}
		}
		else
		{
			if(y != m_point.y)
				return false;
			if(m_vector.z != 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

float CLine::CalDistance(float x, float y, float z)
{
	if(IsOnLine(x, y, z))
	//在直线上，距离为0
		return 0;
	//先求得两点距离
	float temp_distance = sqrt((m_point.x - x)*(m_point.x - x) + (m_point.y - y)*(m_point.y - y)	+ (m_point.z - z)*(m_point.z - z));
	
	//两点连线向量
	DexVector3 two_point_vector(x - m_point.x, y - m_point.y, z - m_point.z);
	two_point_vector.Normalize();
	m_vector.Normalize();
	float vecotr_length = m_vector.Length();
	float point_vector_length = two_point_vector.Length();
	if(vecotr_length * point_vector_length == 0)
		return 0;
	float cos_value = m_vector.Dot(two_point_vector)/ (vecotr_length * point_vector_length);
	float sin_value = sqrt(1.000f - cos_value * cos_value);

	return temp_distance * sin_value;
}

float CLine::CalDistance(DexVector3 _point)
{
	return CalDistance(_point.x, _point.y, _point.z);
}

DexVector3 CLine::GetIncidence(DexVector3 _point)
{
	/*思路：设直线上一点 m_p+t*v，取得该点到待测点的向量， 乘以直线向量为0，解出t*/
	float x0 = m_point.x;
	float y0 = m_point.y;
	float z0 = m_point.z;
	float x1 = _point.x;
	float y1 = _point.y;
	float z1 = _point.z;  
	m_vector.Normalize();
	float a = m_vector.x;
	float b = m_vector.y;
	float c = m_vector.z;
	
	//float t = (x1*x0 + y1*y0 + z1*z0 - (x0*x0 + y0*y0 + z0*z0))/(a*x0 + b*y0 + c*z0);
	if(a*a + b*b + c*c == 0)
		return _point; 
	float t= -((x0-x1)*a + (y0-y1)*b + (z0- z1)*c)/(a*a + b*b + c*c);

	return m_point + m_vector*t;
}

int CLine::GetRelation(CLine line)
{		
	//向量三个分量不可能都为0
	//为防止意外情况，特加入特殊处理,按DEMO_ERROR返回
	if(m_vector.x == 0 && m_vector.y == 0 && m_vector.z == 0)
		return DEMO_ERROR;

	DexVector3 line_vector = line.GetVector();
	if(line_vector.x == 0 && line_vector.y == 0 && line_vector.z == 0)
		return DEMO_ERROR;
	float t = rand()%100;;               //对称式方程中的t，随机取一个数
	float a_divide, b_divide, c_divide;	 //向量三个分量的比值
	DexVector3 line_point = line.GetPoint();
	

	//两直线相交，求解，看是否有解
	float a1, b1, c1, x1, y1, z1;  //方程参数
	float a2, b2, c2, x2, y2, z2;
	a1 = m_vector.x;
	b1 = m_vector.y;
	c1 = m_vector.z;
	x1 = m_point.x;
	y1 = m_point.y;
	z1 = m_point.z;

	a2 = line_vector.x;
	b2 = line_vector.y;
	c2 = line_vector.z;
	x2 = line_point.x;
	y2 = line_point.y;
	z2 = line_point.z;
	float m = a1*b2 - a2*b1;
	float k = a2*y1 - a2*y2 + x2*b2 - x1*b2;
	float left_value = b2*c1*k + z1*b2*m - z2*b2*m;
	float right_value = b1*c2*k + c2*y1*m - c2*y2*m;
	//证明平行
	//则证明两直线向量平行即可
	
	if(m_vector.x == 0)
	{//X分量为0
		if(m_vector.y == 0)
		{//Y分量为0
			//Z分量肯定不为0,因为方法入口已经有预防了
			//则原直线平行于Z轴
			if(line_vector.x == 0 && line_vector.y == 0)
			{//平行
				return PARALLEL;
			}
			else
			{//不平行
				//判断异面交叉
				if(_equal(left_value, right_value))
				{//交叉
					return CROSS;
				}
				//异面
				return DIFFFACE;
			}	
		}
		else
		{//Y分量不为0
			if(m_vector.z == 0)
			{//Z分量为0
			 //原直线平行Y轴
				if(line_vector.x == 0 && line_vector.z == 0)
				{//平行
					return PARALLEL;
				}
				else
				{//不平行
					//判断异面交叉
					if(_equal(left_value, right_value))
					{//交叉
						return CROSS;
					}
					//异面
					return DIFFFACE;
				}
			}
			else
			{//Z分量不为0
			 //原直线平行YOZ平面
				b_divide = line_vector.y/m_vector.y;
				c_divide = line_vector.z/m_vector.z;
				if(_equal(b_divide, c_divide))
				{//平行
					return PARALLEL;
				}
				else
				{//不平行
					//判断异面交叉
					if(_equal(left_value, right_value))
					{//交叉
						return CROSS;
					}	
					//异面
					return DIFFFACE;
				}
			}
		}
	}
	else
	{//X分量不为0
		if(m_vector.y == 0)
		{//Y分量为0
			if(m_vector.z == 0)
			{//Z分量为0
			 //原直线平行X轴
				if(line_vector.y == 0 && m_vector.z == 0)
				{//平行
					return PARALLEL;
				}
				else
				{//不平行
					//判断异面交叉
					if(_equal(left_value, right_value))
					{//交叉
						return CROSS;
					}
					//异面
					return DIFFFACE;
				}
			}
			else
			{//Z分量不为0
			 //原直线平行XOZ平面
				a_divide = line_vector.x/m_vector.x;
				c_divide = line_vector.z/m_vector.z;
				if(_equal(a_divide, c_divide))
				{//平行
					return PARALLEL;
				}
				else
				{//不平行
					//判断异面交叉
					if(_equal(left_value, right_value))
					{//交叉
						return CROSS;
					}
					//异面
					return DIFFFACE;
				}
			}
		}
		else
		{//Y分量不为0
			if(m_vector.z == 0)
			{//Z分量为0
			 //原直线平行XOY平面
				a_divide = line_vector.x/m_vector.x;
				b_divide = line_vector.y/m_vector.y;
				if(_equal(a_divide, b_divide))
				{//平行
					return PARALLEL;
				}
				else
				{//不平行
					//判断异面交叉
					if(_equal(left_value, right_value))
					{//交叉
						return CROSS;
					}
					//异面
					return DIFFFACE;
				}
			}
			else
			{//Z分量不为0
			 //原直线方向向量三个分量都不为0
				a_divide = line_vector.x/m_vector.x;
				b_divide = line_vector.y/m_vector.y;
				c_divide = line_vector.z/m_vector.z;
				if(_equal(a_divide, b_divide) &&
				   _equal(a_divide, c_divide))
				{//平行
					return PARALLEL;
				}
				else
				{//不平行
					//判断异面交叉
					if(_equal(left_value, right_value))
					{//交叉
						return CROSS;
					}
					//异面
					return DIFFFACE;
				}
			}
		}
	}
}

float CLine::CalDistance(CLine line)
{
	int relation = GetRelation(line);
	if(relation == CROSS)
		return 0;
	if(relation == PARALLEL)
	{
		DexVector3 line_point = line.GetPoint();
		return CalDistance(line_point.x, line_point.y, line_point.z);
	}
	//异面
	//暂时不作处理，留待扩展
	return 0;
}

CLine& CLine::operator=(const CLine& _line)
{
	m_vector = _line.GetVector();
	m_point = _line.GetPoint();
	return *this;
}