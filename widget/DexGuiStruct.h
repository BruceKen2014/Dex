
#ifndef DEX_GUISTRUCT_H
#define DEX_GUISTRUCT_H

#include <d3dx9math.h>
#include <list>
//开始于2013.11.02 Bruce
//重新架构GUI系统 

namespace DexGUI
{
	using namespace std;
enum EDraw_Flag 
{
	DRAW_TOP_LEFT, //正常输出
	DRAW_TOP_RIGHT, //右上角对准
	DRAW_BOTTOM_LEFT, //左下角对准
	DRAW_BOTTOM_RIGHT, //右下角对准
	DRAW_TOP_CENTER,  //上面中间对准
};
#define INVALIDE_POSITON 999999
	template<typename T>
	struct DexPointT
	{
		T x, y;
	public:
		DexPointT():x(0),y(0) {}
		DexPointT(T v1, T v2):x(v1), y(v2){}
		DexPointT(const DexPointT& point): x(point.x),y(point.y){}

		DexPointT operator +  (const DexPointT& point) const   { return DexPointT(x + point.x, y + point.y);}
		DexPointT operator -  (const DexPointT& point) const   { return DexPointT(x - point.x, y - point.y);}	
		DexPointT operator /  (int divisor) const
		{
			if(divisor == 0)
				return DexPointT(INVALIDE_POSITON, INVALIDE_POSITON);
			return DexPointT(x / divisor, y / divisor);
		}
		DexPointT& operator =  (const DexPointT& point)		
		{
			x = point.x;
			y = point.y;
			return *this;
		}
		DexPointT& operator +=  (const DexPointT& point)		
		{
			x += point.x;
			y += point.y;
			return *this;
		}
		DexPointT& operator -=  (const DexPointT& point)		
		{
			x = x - point.x;
			y = y - point.y;
			return *this;
		}
		bool operator == (const DexPointT& point) const	     { return x==point.x && y == point.y;}
		bool operator != (const DexPointT& point) const	     { return x!=point.x || y != point.y;}
	};
	typedef DexPointT<int>  DexPoint;  //这里的DexPoint既可以表示一个点，也可以表示一个二维数，如宽高、长宽等
	typedef DexPointT<float>  DexPointF;

	template<typename T>
	struct DexSizeT
	{
		T w, h;
	public:
		DexSizeT():w(0),h(0) {}
		DexSizeT(T v1, T v2):w(v1), h(v2){}
		DexSizeT(const DexSizeT& size): w(size.w),h(size.h){}

		DexSizeT operator +  (const DexSizeT& size) const   { return DexSizeT(w + size.w, h + size.h);}
		DexSizeT operator -  (const DexSizeT& size) const   { return DexSizeT(w - size.w, h - size.h);}	
		DexSizeT operator /  (int divisor) const
		{
			if(divisor == 0)
				return DexSizeT(INVALIDE_POSITON, INVALIDE_POSITON);
			return DexSizeT(w / divisor, h / divisor);
		}
		DexSizeT& operator =  (const DexSizeT& size)		
		{
			w = size.w;
			h = size.h;
			return *this;
		}
		DexSizeT& operator +=  (const DexSizeT& size)		
		{
			w += size.w;
			h += size.h;
			return *this;
		}
		DexSizeT& operator -=  (const DexSizeT& size)		
		{
			w = w - size.w;
			h = h - size.h;
			return *this;
		}
		bool operator == (const DexSizeT& size) const	     { return w==size.w && h == size.h;}
		bool operator != (const DexSizeT& size) const	     { return w!=size.w || h != size.h;}
	};
	typedef DexSizeT<int>  DexSize;
	typedef DexSizeT<float> DexSizeF;


	template<typename T>
	struct DexRectT
	{
		T left,right,top,bottom;
	public:
		DexRectT():left(0),right(0),top(0),bottom(0) {}
		DexRectT(T l, T tp,T r, T b):left(l),top(tp),right(r),bottom(b) {}
		DexRectT(const DexRectT& rect):left(rect.left),right(rect.right),top(rect.top),bottom(rect.bottom) {}

		DexRectT operator +  (const DexRectT& rect) const   { return DexRectT(left+rect.left, top+rect.top,right+rect.right,  bottom+rect.bottom);}
		DexRectT operator -  (const DexRectT& rect) const   { return DexRectT(left-rect.left, top-rect.top,right-rect.right,  bottom-rect.bottom);}
		DexRectT operator /  (int divisor) const
		{
			if(divisor == 0)
				return DexRectT(INVALIDE_POSITON, INVALIDE_POSITON, INVALIDE_POSITON, INVALIDE_POSITON);
			return DexRectT(left / divisor, right / divisor, top/divisor, bottom/divisor);
		}
		DexRectT& operator =  (const DexRectT& rect)
		{
			left  = rect.left;
			right = rect.right;
			top   = rect.top;
			bottom= rect.bottom;
			return *this;
		}
		DexRectT& operator +=  (const DexRectT& rect)
		{
			left  +=rect.left;
			right +=rect.right;
			top   +=rect.top;
			bottom+=rect.bottom;
			return *this;
		}
		DexRectT& operator -=  (const DexRectT& rect)
		{
			left  -= rect.left;
			right -= rect.right;
			top   -= rect.top;
			bottom-= rect.bottom;
			return *this;
		}
		bool IsValid() { return right >= left && bottom >= top;};
		T getWidth()  const { return right - left;};
		T getHeight() const { return bottom - top;};
		DexRectT Clip(const DexRectT& clip_rect)
		{//用clip_rect 来裁剪本rect 
			T ret_left,ret_right,ret_top,ret_bottom;
			ret_left   = clip_rect.left  <  left  ?    left  : clip_rect.left;
			ret_right  = clip_rect.right >  right ?    right : clip_rect.right;
			ret_top    = clip_rect.top   <  top   ?     top  : clip_rect.top;
			ret_bottom = clip_rect.bottom> bottom ?   bottom : clip_rect.bottom;
			ret_left = ret_left > ret_right ? ret_right: ret_left;
			ret_top = ret_top > ret_bottom ? ret_bottom: ret_top;
			return DexRectT(ret_left, ret_top, ret_right, ret_bottom);
		}
		bool operator == (const DexRectT& rect) const	     { return left==rect.left && right==rect.right && top==rect.top&& bottom==rect.bottom;}
		bool operator != (const DexRectT& rect) const	     { return left!=rect.left || right!=rect.right || top!=rect.top|| bottom!=rect.bottom;}
	};
	typedef DexRectT<int>  DexRect;
	typedef DexRectT<float>  DexRectF;


	//UI的有效区域，一个控件可能由多个CDexGuiValidArea组成
	class CDexGuiValidAreaInterface
	{
	protected:
		int    m_Id;
		static int totalAreas;
	public:
		CDexGuiValidAreaInterface();
		virtual ~CDexGuiValidAreaInterface();
	public:
		virtual bool CheckInside(const DexPoint& point) = 0; //判断一个点是否落在该区域里面
		virtual void Offset(const DexSizeF& offset) = 0;
	};
	//矩形区域:适用于方形控件
	class CDexGuiValidAreaRect   :public CDexGuiValidAreaInterface
	{
	public:
		DexRect rect;
	public:
		CDexGuiValidAreaRect(const DexRect& area):rect(area){}
	public:
		virtual bool CheckInside(const DexPoint& point); 
		virtual void Offset(const DexSizeF& offset);
	};
	//圆形区域:适用于圆形控件
	class CDexGuiValidAreaRound	   :public CDexGuiValidAreaInterface
	{
	public:
		DexPoint center;
		int      radius;
	public:
		CDexGuiValidAreaRound():radius(0){}
		CDexGuiValidAreaRound(const DexPoint& point, int radius):center(point),radius(0){}
	public:
		virtual bool CheckInside(const DexPoint& point); 
		virtual void Offset(const DexSizeF& offset);
	};
    //三角形区域:适用于不规则形状控件(通过三角形的组合而成)
	//暂时不实现 留待扩展
	class CDexGuiValidTri     :public CDexGuiValidAreaInterface
	{
	public:
		DexPoint point1;
		DexPoint point2;
		DexPoint point3;
	public:
		CDexGuiValidTri();
		CDexGuiValidTri(const DexPoint& p1,const DexPoint& p2,const DexPoint& p3):point1(p1),point2(p2),point3(p3){}
	public:
		virtual bool CheckInside(const DexPoint& point); 
		virtual void Offset(const DexSizeF& offset);
	};

	//平面直线方程   y = ax +b
	//class CDex2DLine
	//{
	//public:
	//	int a, b;
	//	CDex2DLine(int v1, int v2):a(v1),b(2){}
	//	CDex2DLine(const DexPoint& p1,const DexPoint& p2);
	//};
	class CDexFun
	{
	public:
		static int    CalPointDistance(const DexPoint& p1, const DexPoint& p2);
		//判断点P是否落在△ABC内(前提点P必须在△ABC所处的平面内)，如果是2D平面处理，则将Z置为0即可
		static bool   PointInTriangle(const D3DXVECTOR3& A, const D3DXVECTOR3& B,const D3DXVECTOR3& C,const D3DXVECTOR3& P);
		static string IntToStr(int i);
		static int    str_to_int(string str);
		static int    str_to_int(char* str);
		static bool   strcat(string& ret, string str);
		static bool   strcat(char* ret,const char* str);
		static string FloatToStr(float f);	  
		static float  str_to_float(string str);
		static float  str_to_float(char* str);
		static string D3DVector3toStr(const D3DXVECTOR3& vec, char split_char=';');
		static bool   StrToD3DVector3(string str, D3DXVECTOR3& vec, char split_char=';');
		static bool   check_rect_croos(const DexRectF& one, const DexRectF& two);//查看两个矩形是否相交
		static bool   check_point_in_rect(const DexPointF& pnt, const DexRectF& rect);
		static bool   equal(const char* str1, const char* str2);
		
		//取得x1 x2之间的catmull-rom值 middle∈(0.0f,1.0f) factor弯曲系数 
		//factor < 0 则形状往里面凹 ； factor = 0 时 为直线； factor > 0时往外面凸； factor = 1最接近圆圈弧形
		static float  get_catmull_value(float x0, float x1, float x2, float x3, float middle, float factor = 0.5f);
		
		//取得平面p1 p2之间的catmull点, n个点能决定n-3段区间曲线 要获得一个圆形 最少要画四段圆弧，需要4+3个控制点
		static DexPointF get_catmull_value(const DexPointF& p0,const DexPointF& p1,const DexPointF& p2,const DexPointF& p3,float middle, float factor = 0.5f);
		
		//取得空间p1 p2之间的catmull点
		static D3DXVECTOR3 get_catmull_value(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3,float middle, float factor = 0.5f);
		
		//取得x1 x2之间的catmull-rom值 的切值 即把相应的catmull-rom值对middle进行一次求导
		static float  get_catmull_tangent(float x0, float x1, float x2, float x3, float middle, float factor = 0.5f);
		
		//取得平面p1 p2之间的catmull点所在的切向量
		static DexPointF get_catmull_tangent(const DexPointF& p0,const DexPointF& p1,const DexPointF& p2,const DexPointF& p3,float middle, float factor = 0.5f);
		
		//取得空间p1 p2之间的catmull点所在的切向量
		static D3DXVECTOR3 get_catmull_tangent(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,const D3DXVECTOR3& p2,const D3DXVECTOR3& p3,float middle, float factor = 0.5f);
		
		//wait_pnt绕center旋转radian弧度
		static void   RotatePoint(const DexPointF& center, DexPointF& wait_pnt, float radian);
		static void   RotatePoint(const DexPointF& center, DexPoint& wait_pnt, float radian);
		static void   RotatePoint(const DexPoint& center, DexPointF& wait_pnt, float radian);
		static void   RotatePoint(const DexPoint& center, DexPoint& wait_pnt, float radian);
	};

	class CDexGuiArea
	{
	protected:
		list<CDexGuiValidAreaInterface*> m_pAreas;
	public:
		CDexGuiArea();
		~CDexGuiArea();
	public:
		void AddArea(const DexRect& rect); //添加一个矩形区域
		void AddArea(const DexPoint& point, int radius); //添加一个圆形区域
		void AddArea(const DexPoint& pnt1, const DexPoint& pnt2, const DexPoint& pnt3);//添加一个三角形区域
		bool CheckInside(const DexPoint& point); 
		void Offset(const DexSizeF& offset);
	};

	
}



#endif