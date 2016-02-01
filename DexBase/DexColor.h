
#ifndef _DEX_COLOR_H
#define _DEX_COLOR_H
//#include <d3d9types.h>

typedef struct _DexColor
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
	_DexColor():r(255),g(255),b(255),a(255){}
	_DexColor(DWORD d)
	{
		a = (d >> 24) & (0xff);
		r = (d >> 16) & (0xff);
		g = (d >> 8)  & (0xff);
		b = (d >> 0)  & (0xff);
	}
	_DexColor(unsigned char _r, unsigned char _g, unsigned char _b) :r(_r), g(_g), b(_b), a(255){}
	_DexColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) :r(_r), g(_g), b(_b), a(_a){}
	_DexColor(int _r, int _g, int _b) :r(_r), g(_g), b(_b), a(255){}
	_DexColor(int _r, int _g, int _b, int _a) :r(_r), g(_g), b(_b), a(_a){}
	//只处理0~1.0f中间的float
	_DexColor(float _r, float _g, float _b) :r(_r * 255), g(_g * 255), b(_b * 255), a(255){}
	_DexColor(float _r, float _g, float _b, float _a) :r(_r * 255), g(_g * 255), b(_b * 255), a(_a * 255){}
	_DexColor(double _r, double _g, double _b, double _a) :r(_r * 255), g(_g * 255), b(_b * 255), a(_a * 255){}
	void Set(float _r, float _g, float _b, float _a) 
	{
		r = _r * 255;
		g = _g * 255;
		b = _b * 255;
		a = _a * 255;
	}
	void Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	DWORD DWord()
	{
		return (DWORD)
			(
				((((int)a) & 0xff) << 24) | 
				((((int)r) & 0xff) << 16) |
				((((int)g) & 0xff) << 8 ) | 
				((((int)b) & 0xff) << 0 )
			);
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
		return _DexColor(r / 255.0f * color.r / 255.0f, g / 255.0f * color.g / 255.0f,
			b / 255.0f * color.b / 255.0f, a / 255.0f * color.a / 255.0f);
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
	return (DWORD)(((((int)(color.a)) & 0xff) << 24) | ((((int)(color.r)) & 0xff) << 16) |
		((((int)(color.g)) & 0xff) << 8) | ((((int)(color.b)) & 0xff) << 0)
		);
}

#endif