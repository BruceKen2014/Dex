/*
DexEngine&Light
*/


#ifndef _DEX_LIGHT_H
#define _DEX_LIGHT_H

#include <memory.h>
//#include <d3d9types.h>
#include "DexType.h"
#include "DexColor.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexVector4.h"

#define DEXRENDER_LIGHT_ENABLE	  (1 << 0)
#define DEXRENDER_LIGHT_AMBIENT   (1 << 1)
#define DEXRENDER_LIGHT_POINT	  (1 << 2)
#define DEXRENDER_LIGHT_DIRECTION (1 << 3)
#define DEXRENDER_LIGHT_ALL_ON    (DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | \
	DEXRENDER_LIGHT_POINT | DEXRENDER_LIGHT_DIRECTION)
#define DEXRENDER_LIGHT_MAX_COUNT 8
class DexLight
{
public:
	typedef enum _DexLightType{
		DexLight_POINT = 0,
		DexLight_SPOT = 1,
		DexLight_DIRECTIONAL = 2,
		DexLight_TOTAL
	} DexLightType;
public:
	DexLightType    type;    
	int32			id;      //灯光ID序号
	bool			enable;
	DexColor	    diffuse;        
	DexColor	    specular;       
	DexColor	    ambient;      
	DexVector3      position; //------------点光源---------聚光灯------------------------    
	DexVector3      direction;//---------------------------聚光灯---------方向光---------   
	float32         range;    //------------点光源---------聚光灯------------------------
	float32         falloff;  //衰减-----------------------聚光灯------------------------
	float32         attenuation0;     /* Constant attenuation */
	float32         attenuation1;     /* Linear attenuation */
	float32         attenuation2;     /* Quadratic attenuation */
	float32         theta;    //---------------------------聚光灯光源内部锥体弧度角------
	float32         phi;      //---------------------------聚光灯光源外部锥体弧度角------
	
public:
	DexLight() :type(DexLight_TOTAL), id(-1), diffuse(DEXCOLOR_WHITE), specular(DEXCOLOR_BLACK), 
		ambient(DEXCOLOR_BLACK), position(DexVector3(0.0f, 0.0f, 0.0f)), direction(DexVector3(0.0f, -1.0f, 0.0f)),
		range(100.0f), falloff(0.0f), attenuation0(0.0f), attenuation1(0.0f), attenuation2(0.0f), theta(0.0f),
		phi(0.0f), enable(true)
	{
	};
	DexLight(DexLightType _type, const DexColor& color)  //无论哪种灯光，这些属性都是共同的 
	{
		type = _type;
		diffuse = color;
		specular = color;
		ambient = color;
	}
public:
	void SetPointLight(const DexVector3& pos, float32 _range)
	{
		position = pos;
		range = _range;
	}
	void SetDirectionLight(const DexVector3& dir)
	{
		direction = dir;
	}
	void SetSpotLight(const DexVector3& pos, const DexVector3& dir, float32 _range, float32 _fallof,
		float _theta, float _phi)
	{
		position = pos;
		direction = dir;
		range = _range;
		falloff = _fallof;
		theta = _theta;
		phi = _phi;
	}
};
struct stDexPointLight
{
	DexVector4  color;
	DexVector4  position;
	DexVector4  rangeAtte;
	stDexPointLight()
	{}
	stDexPointLight(const DexVector4& _color, const DexVector4& _pos, const DexVector4& _range) :color(_color),
		position(_pos), rangeAtte(_range)
	{}
};
struct stDexDirectionLight
{
	DexVector4  color;
	DexVector4  direction;
	stDexDirectionLight(){}
	stDexDirectionLight(const DexVector4& _color, const DexVector4& _direction) :color(_color), direction(_direction)
	{}
};

#endif