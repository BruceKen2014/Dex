
#ifndef _DEX_COLOR_H
#define _DEX_COLOR_H
//#include <d3d9types.h>

typedef struct _DexColor
{
	float r;
	float g;
	float b;
	float a;
	_DexColor():r(1.0f),g(1.0f),b(1.0f),a(1.0f){}
	_DexColor(DWORD d)
	{
		a = ((float)((d >> 24) & (0xff)))/(float)0xff;
		r = ((float)((d >> 16) & (0xff)))/(float)0xff;
		g = ((float)((d >> 8) & (0xff)))/(float)0xff;
		b = ((float)((d >> 0) & (0xff)))/(float)0xff;
	}
	_DexColor(float _r, float _g, float _b):r(_r),g(_g),b(_b),a(1.0f){}
	_DexColor(float _r, float _g, float _b, float _a):r(_r),g(_g),b(_b),a(_a){}
	void Set(float _r, float _g, float _b, float _a) 
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	_DexColor& operator = (const _DexColor& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
		return *this;
	}
	_DexColor operator + (const _DexColor& color) const  
	{
		float _r = r + color.r;
		_r = _r > 1.0f ? 1.0f : _r;
		float _g = g + color.g;
		_g = _g > 1.0f ? 1.0f : _g;
		float _b = b + color.b;
		_b = _b > 1.0f ? 1.0f : _b;
		float _a = a + color.a;
		_a = _a > 1.0f ? 1.0f : _a;
		return _DexColor(_r, _g, _b, _a);
	}
	_DexColor operator - (const _DexColor& color) const  
	{
		float _r = r - color.r;
		_r = _r < 0.0f ? 0.0f : _r;
		float _g = g - color.g;
		_g = _g < 0.0f ? 0.0f : _g;
		float _b = b - color.b;
		_b = _b < 0.0f ? 0.0f : _b;
		float _a = a - color.a;
		_a = _a < 0.0f ? 0.0f : _a;
		return _DexColor(_r, _g, _b, _a);
	}
	_DexColor operator * (const _DexColor& color) const  
	{
		return _DexColor(r * color.r, g * color.g,  b * color.b, a * color.a);
	}
	bool operator == (const _DexColor& color) const 
	{
		return (r == color.r) && (g == color.g) && (b == color.b) && (a == color.a);
	}
	bool operator != (const _DexColor& color) const
	{
		return !((r == color.r) && (g == color.g) && (b == color.b) && (a == color.a));
	}
}DexColor;
const   DexColor  DEXCOLOR_BLACK	 = DexColor(0.0f,0.0f,0.0f,1.0f);  //黑色
const   DexColor  DEXCOLOR_WHITE	 = DexColor(1.0f,1.0f,1.0f,1.0f);  //白色
const   DexColor  DEXCOLOR_RED		 = DexColor(1.0f,0.0f,0.0f,1.0f);  //红色
const   DexColor  DEXCOLOR_BLUE 	 = DexColor(0.0f,0.0f,1.0f,1.0f);  //蓝色
const   DexColor  DEXCOLOR_GREEN	 = DexColor(0.0f,1.0f,0.0f,1.0f);  //绿色
const   DexColor  DEXCOLOR_PINK		 = DexColor(1.0f, 0.0f, 1.0f, 1.0f); //粉红色
const   DexColor  DEXCOLOR_YELLOW	 = DexColor(1.0f, 1.0f, 0.0f, 1.0f); //黄色
const   DexColor  DEXCOLOR_SILBLUE   = DexColor(1.0f, 0.0f, 1.0f, 1.0f); //银蓝
inline DWORD getD3DColor(const DexColor& color)
{
	return (DWORD)(((((int)(color.a * 255)) & 0xff) << 24) | ((((int)(color.r * 255)) & 0xff) << 16) |
		((((int)(color.g * 255)) & 0xff) << 8) | ((((int)(color.b * 255)) & 0xff) << 0)
		);
}

#endif