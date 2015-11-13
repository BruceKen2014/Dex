

#include "../DexBase/typedefine.h"
#include "DexGuiStruct.h"
#include <math.h>


namespace DexGUI
{
//--------------------CDexGuiValidAreaInterface------------------
	int CDexGuiValidAreaInterface::totalAreas = 0;
	CDexGuiValidAreaInterface::CDexGuiValidAreaInterface()
	{
		m_Id = totalAreas++;
	}
	CDexGuiValidAreaInterface::~CDexGuiValidAreaInterface()
	{
	}
//--------------------CDexGuiValidAreaRect-------------------
	bool CDexGuiValidAreaRect::CheckInside(const DexPoint& point)
	{
		return (point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom);
	}
	void CDexGuiValidAreaRect::Offset(const DexSizeF& offset)
	{
		rect.left += offset.w;
		rect.top += offset.h;
		rect.right += offset.w;
		rect.bottom += offset.h;
	}
//--------------------CDexGuiValidTri-------------------
	bool CDexGuiValidTri::CheckInside(const DexPoint& point)
	{
		return CDexFun::PointInTriangle(D3DXVECTOR3(point1.x, point1.y, 0), D3DXVECTOR3(point2.x, point2.y, 0),
			D3DXVECTOR3(point3.x, point3.y, 0),D3DXVECTOR3(point.x, point.y, 0));
	}
	void CDexGuiValidTri::Offset(const DexSizeF& offset)
	{
		point1.x += offset.w;
		point1.y += offset.h;
		point2.x += offset.w;
		point2.y += offset.h;
		point3.x += offset.w;
		point3.y += offset.h;
	}
//--------------------CDexGuiValidAreaRound-------------------
	bool CDexGuiValidAreaRound::CheckInside(const DexPoint& point)
	{
		return CDexFun::CalPointDistance(center, point)<=radius;
	}
	void CDexGuiValidAreaRound::Offset(const DexSizeF& offset)
	{
		center.x += offset.w;
		center.y += offset.h;
	}


//----------------------------CDexFun---------------------------
	int CDexFun::CalPointDistance(const DexPoint& p1, const DexPoint& p2)
	{
		int temp = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.x - p2.y);
		return (int)sqrt((float)temp);
	}
	//判断点P是否落在△ABC内(前提点P必须在△ABC所处的平面内)，如果是2D平面处理，则将Z置为0即可
	bool  CDexFun::PointInTriangle(const D3DXVECTOR3& A, const D3DXVECTOR3& B,const D3DXVECTOR3& C,const D3DXVECTOR3& P)
	{
		D3DXVECTOR3 v0 = C - A ;
		D3DXVECTOR3 v1 = B - A ;
		D3DXVECTOR3 v2 = P - A ;

		float dot00 = D3DXVec3Dot(&v0, &v0) ;
		float dot01 = D3DXVec3Dot(&v0, &v1) ;
		float dot02 = D3DXVec3Dot(&v0, &v2) ;
		float dot11 = D3DXVec3Dot(&v1, &v1) ;
		float dot12 = D3DXVec3Dot(&v1, &v2) ;

		float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

		float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
		if (u < 0 || u > 1) // if u out of range, return directly
		{
			return false ;
		}

		float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
		if (v < 0 || v > 1) // if v out of range, return directly
		{
			return false ;
		}
		return u + v <= 1 ;
	}
	string CDexFun::IntToStr(int i)
	{
		char ch[32];
		sprintf(ch, "%d", i);
		return string(ch);
	}
	int CDexFun::str_to_int(string str)
	{
		return atoi(str.c_str());
	}
	bool CDexFun::strcat(string& ret, string str)
	{
		ret += str;
		return true;
	}
	bool CDexFun::strcat(char* ret, const char* str)
	{
		::strcat(ret, str);
		return true;
	}
	int CDexFun::str_to_int(char* str)
	{
		return atoi(str);
	}
	float CDexFun::str_to_float(string str)
	{
		return atof(str.c_str());
	}
	float CDexFun::str_to_float(char* str)
	{
		return atof(str);
	}
	string CDexFun::FloatToStr(float f)
	{
		char ch[32];
		sprintf(ch, "%.3f",  f);
		return string(ch);
	}
	string CDexFun::D3DVector3toStr(const D3DXVECTOR3& vec, char split_char)
	{
		string str;
		string temp;
		temp = FloatToStr(vec.x);
		str += temp;
		str.append(1, split_char);
		temp.clear();
		temp = FloatToStr(vec.y);
		str += temp;
		str.append(1, split_char);
		temp.clear();
		temp = FloatToStr(vec.z);
		str += temp;
		return str;
	}
	bool CDexFun::StrToD3DVector3(string str, D3DXVECTOR3& vec, char split_char)
	{
		std::vector<string> str_arr;
		SplitStr(str, split_char, str_arr);
		DEX_ENSURE_B(str_arr.size() == 3);
		vec.x  = atof(str_arr[0].c_str());
		vec.y  = atof(str_arr[1].c_str());
		vec.z  = atof(str_arr[2].c_str());
		return true;
	}
	bool CDexFun::check_rect_croos(const DexRectF& one, const DexRectF& two)
	{
		if(one.right < two.left || one.left > two.right)
			return false;
		if(one.bottom < two.top || one.top > two.bottom)
			return false;
		return true;
	}
	bool CDexFun::check_point_in_rect(const DexPointF& pnt, const DexRectF& rect)
	{
		return pnt.x >= rect.left && pnt.x <= rect.right
			&& pnt.y >= rect.top  && pnt.y <= rect.bottom;
	}
	bool CDexFun::equal(const char* str1, const char* str2)
	{
		return strcmp(str1,str2) == 0;
	}
	float CDexFun::get_catmull_value(float x0, float x1, float x2, float x3, float middle, float factor /* = 0.5f */)
	{
		float c0 = x1;
		float c1 = (x2 - x0) * factor;
		float c2 = (x2 - x1)*3.0f - (x3 - x1) * factor - (x2 - x0) * 2.0f * factor;
		float c3 = (x2 - x1)*(-2.0f) + (x3 - x1) * factor + (x2 - x0) * factor;
		return c3*middle*middle*middle + c2*middle*middle + c1*middle + c0;
	}
	DexPointF CDexFun::get_catmull_value(const DexPointF& p0,const DexPointF& p1,const DexPointF& p2,const DexPointF& p3,float middle, float factor /* = 0.5f */)
	{
		float x = get_catmull_value(p0.x, p1.x, p2.x, p3.x, middle, factor);
		float y = get_catmull_value(p0.y, p1.y, p2.y, p3.y, middle, factor);
		return DexPointF(x, y);
	}
	D3DXVECTOR3 CDexFun::get_catmull_value(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3,float middle, float factor/* = 0.5f */)
	{
		float x = get_catmull_value(p0.x, p1.x, p2.x, p3.x, middle, factor);
		float y = get_catmull_value(p0.y, p1.y, p2.y, p3.y, middle, factor);
		float z = get_catmull_value(p0.z, p1.z, p2.z, p3.z, middle, factor);
		return D3DXVECTOR3(x, y, z);
	}
	float CDexFun::get_catmull_tangent(float x0, float x1, float x2, float x3, float middle, float factor /* = 0.5f */)
	{
		float c0 = x1;
		float c1 = (x2 - x0) * factor;
		float c2 = (x2 - x1)*3.0f - (x3 - x1) * factor - (x2 - x0) * 2.0f * factor;
		float c3 = (x2 - x1)*(-2.0f) + (x3 - x1) * factor + (x2 - x0) * factor;
		return c3*middle*middle*3 + c2*middle*2 + c1;
	}
	DexPointF CDexFun::get_catmull_tangent(const DexPointF& p0,const DexPointF& p1,const DexPointF& p2,const DexPointF& p3,float middle, float factor /* = 0.5f */)
	{
		float x = get_catmull_tangent(p0.x, p1.x, p2.x, p3.x, middle, factor);
		float y = get_catmull_tangent(p0.y, p1.y, p2.y, p3.y, middle, factor);
		return DexPointF(x, y);
	}
	D3DXVECTOR3 CDexFun::get_catmull_tangent(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3,float middle, float factor/* = 0.5f */)
	{
		float x = get_catmull_tangent(p0.x, p1.x, p2.x, p3.x, middle, factor);
		float y = get_catmull_tangent(p0.y, p1.y, p2.y, p3.y, middle, factor);
		float z = get_catmull_tangent(p0.z, p1.z, p2.z, p3.z, middle, factor);
		return D3DXVECTOR3(x, y, z);
	}
	void CDexFun::RotatePoint(const DexPointF& center, DexPointF& wait_pnt, float radian)
	{
		DexPointF temp_pnt = wait_pnt;
		wait_pnt.x = (temp_pnt.x - center.x)*cos(radian) - (temp_pnt.y - center.y)*sin(radian) + center.x; 
		wait_pnt.y = (temp_pnt.x - center.x)*sin(radian) + (temp_pnt.y- center.y)*cos(radian) + center.y; 
	}
	void CDexFun::RotatePoint(const DexPointF& center, DexPoint& wait_pnt, float radian)
	{
		DexPoint temp_pnt = wait_pnt;
		wait_pnt.x = (temp_pnt.x - center.x)*cos(radian) - (temp_pnt.y - center.y)*sin(radian) + center.x; 
		wait_pnt.y = (temp_pnt.x - center.x)*sin(radian) + (temp_pnt.y- center.y)*cos(radian) + center.y; 
	}
	void CDexFun::RotatePoint(const DexPoint& center, DexPointF& wait_pnt, float radian)
	{
		DexPointF temp_pnt = wait_pnt;
		wait_pnt.x = (temp_pnt.x - center.x)*cos(radian) - (temp_pnt.y - center.y)*sin(radian) + center.x; 
		wait_pnt.y = (temp_pnt.x - center.x)*sin(radian) + (temp_pnt.y- center.y)*cos(radian) + center.y; 
	}
	void CDexFun::RotatePoint(const DexPoint& center, DexPoint& wait_pnt, float radian)
	{
		DexPoint temp_pnt = wait_pnt;
		wait_pnt.x = (temp_pnt.x - center.x)*cos(radian) - (temp_pnt.y - center.y)*sin(radian) + center.x; 
		wait_pnt.y = (temp_pnt.x - center.x)*sin(radian) + (temp_pnt.y- center.y)*cos(radian) + center.y; 
	}
//--------------------CDexGuiArea-------------------
	CDexGuiArea::CDexGuiArea()
	{
	}
	CDexGuiArea::~CDexGuiArea()
	{
		for(list<CDexGuiValidAreaInterface*>::iterator ite = m_pAreas.begin(); ite != m_pAreas.end();)
		{
			_SafeDelete(*ite);
			ite = m_pAreas.erase(ite);
		}
	}
	void CDexGuiArea::AddArea(const DexRect& rect)
	{
		CDexGuiValidAreaRect* rect_area = new CDexGuiValidAreaRect(rect);
		m_pAreas.push_back(rect_area);
	}
	void CDexGuiArea::AddArea(const DexPoint& point, int radius)
	{
		CDexGuiValidAreaRound* round = new CDexGuiValidAreaRound(point, radius);
		m_pAreas.push_back(round);
	}
	void CDexGuiArea::AddArea(const DexPoint& pnt1, const DexPoint& pnt2, const DexPoint& pnt3)
	{
		CDexGuiValidTri* tri = new CDexGuiValidTri(pnt1, pnt2, pnt3);
		m_pAreas.push_back(tri);
	}
	bool CDexGuiArea::CheckInside(const DexPoint& point)
	{
		foreach(list<CDexGuiValidAreaInterface*>, ite, m_pAreas)
		{
			if((*ite)->CheckInside(point))
				return true;
		}
		return false;
	}
	void CDexGuiArea::Offset(const DexSizeF& offset)
	{
		foreach(list<CDexGuiValidAreaInterface*>, ite, m_pAreas)
		{
			(*ite)->Offset(offset);
		}
	}
}