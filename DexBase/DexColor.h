
#ifndef _DEX_COLOR_H
#define _DEX_COLOR_H

#include "DexType.h"
/*
conversion from 'float' to 'DUChar', possible loss of data
conversion from 'double' to 'DUChar', possible loss of data
*/
#pragma warning(disable:4244) 
typedef struct _DexColor
{
	DUChar b;
	DUChar g;
	DUChar r;
	DUChar a;
	_DexColor():r(255),g(255),b(255),a(255){}
	_DexColor(DInt32 d)
	{
		a = (d >> 24) & (0xff);
		r = (d >> 16) & (0xff);
		g = (d >> 8)  & (0xff);
		b = (d >> 0)  & (0xff);
	}
	_DexColor(DUChar _r, DUChar _g, DUChar _b) :r(_r), g(_g), b(_b), a(255){}
	_DexColor(DUChar _r, DUChar _g, DUChar _b, DUChar _a) :r(_r), g(_g), b(_b), a(_a){}
	_DexColor(DInt32 _r, DInt32 _g, DInt32 _b) :r(_r), g(_g), b(_b), a(255){}
	_DexColor(DInt32 _r, DInt32 _g, DInt32 _b, DInt32 _a) :r(_r), g(_g), b(_b), a(_a){}
	//只处理0~1.0f中间的float
	_DexColor(DFloat32 _r, DFloat32 _g, DFloat32 _b) :r(_r * 255), g(_g * 255), b(_b * 255), a(255){}
	_DexColor(DFloat32 _r, DFloat32 _g, DFloat32 _b, DFloat32 _a) :r(_r * 255), g(_g * 255), b(_b * 255), a(_a * 255){}
	_DexColor(DFloat64 _r, DFloat64 _g, DFloat64 _b, DFloat64 _a) :r(_r * 255), g(_g * 255), b(_b * 255), a(_a * 255){}
	void Set(DFloat32 _r, DFloat32 _g, DFloat32 _b, DFloat32 _a)
	{
		r = _r * 255;
		g = _g * 255;
		b = _b * 255;
		a = _a * 255;
	}
	void Set(DUChar _r, DUChar _g, DUChar _b, DUChar _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	void Set(DUChar _r, DUChar _g, DUChar _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}
	DInt32 DWord() const
	{
		return (DInt32)
			(
			((((DInt32)a) & 0xff) << 24) |
			((((DInt32)r) & 0xff) << 16) |
			((((DInt32)g) & 0xff) << 8) |
			((((DInt32)b) & 0xff) << 0)
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
		DUChar _r = r + color.r;
		_r = _r > 255 ? 255 : _r;
		DUChar _g = g + color.g;
		_g = _g > 255 ? 255 : _g;
		DUChar _b = b + color.b;
		_b = _b > 255 ? 255 : _b;
		DUChar _a = a + color.a;
		_a = _a > 255 ? 255 : _a;
		return _DexColor(_r, _g, _b, _a);
	}
	_DexColor operator - (const _DexColor& color) const  
	{
		DUChar _r = r - color.r;
		_r = _r < 0 ? 0 : _r;
		DUChar _g = g - color.g;
		_g = _g < 0 ? 0 : _g;
		DUChar _b = b - color.b;
		_b = _b < 0 ? 0 : _b;
		DUChar _a = a - color.a;
		_a = _a < 0 ? 0 : _a;
		return _DexColor(_r, _g, _b, _a);
	}
	_DexColor operator * (const _DexColor& color) const  
	{
		return _DexColor(r / 255.0f * color.r, g / 255.0f * color.g,
			b / 255.0f * color.b, a / 255.0f * color.a);
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
const   DexColor  DEXCOLOR_RED    = 0xFFFF0000;  //红色
const   DexColor  DEXCOLOR_ORANGE = 0xFFFF6100;  //橙黄色
const   DexColor  DEXCOLOR_YELLOW = 0xFFFFFF00;  //黄色
const   DexColor  DEXCOLOR_GREEN  =  0xFF00FF00;  //绿色
const   DexColor  DEXCOLOR_CYAN   = 0xFF00FFFF;  //青色
const   DexColor  DEXCOLOR_BLUE   = DexColor(0.0f, 0.0f, 1.0f, 1.0f);  //蓝色
const   DexColor  DEXCOLOR_PURPLE = 0xFFA020F0; //紫色
const   DexColor  DEXCOLOR_BLACK  = DexColor(0.0f,0.0f,0.0f,1.0f);  //黑色
const   DexColor  DEXCOLOR_WHITE  = DexColor(1.0f,1.0f,1.0f,1.0f);  //白色
const   DexColor  DEXCOLOR_PINK		 = DexColor(1.0f, 0.0f, 1.0f, 1.0f); //粉红色
const   DexColor  DEXCOLOR_SILBLUE   = DexColor(1.0f, 0.0f, 1.0f, 1.0f); //银蓝
inline DInt32 getD3DColor(const DexColor& color)
{
	return (((((DInt32)(color.a)) & 0xff) << 24) | ((((DInt32)(color.r)) & 0xff) << 16) |
		((((DInt32)(color.g)) & 0xff) << 8) | ((((DInt32)(color.b)) & 0xff) << 0)
		);
}

#endif