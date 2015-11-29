/*
DexEngine&Light
*/


#ifndef _DEX_LIGHT_H
#define _DEX_LIGHT_H

#include <memory.h>
#include <d3d9types.h>
#include "DexType.h"
#include "DexColor.h"
#include "../DexMath/DexVector3.h"
class DexLight
{
public:
	typedef enum _DexLightType{
		DexLight_POINT = 1,
		DexLight_SPOT = 2,
		DexLight_DIRECTIONAL = 3,
		DexLight_TOTAL
	} DexLightType;
public:
	DexLightType    type;    
	int32			id;      //灯光ID序号
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
		phi(0.0f)
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
	void SetPointLight(const D3DXVECTOR3& pos, float32 _range)
	{
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
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
	void   GetDXLight(D3DLIGHT9* dxLight) const
	{
		D3DLIGHTTYPE t = D3DLIGHT_FORCE_DWORD;
		switch (type)
		{
		case DexLight_POINT:
		{
			t = D3DLIGHT_POINT;
			break;
		}
		case DexLight_SPOT:
		{
			 t = D3DLIGHT_SPOT;
			 break;
		}
		case DexLight_DIRECTIONAL:
		{
			t = D3DLIGHT_DIRECTIONAL;
			break;
		}
		default:
			break;
		}
		dxLight->Type = t;
		memcpy(&dxLight->Ambient, &ambient, sizeof(DexColor));
		memcpy(&dxLight->Specular, &specular, sizeof(DexColor));
		memcpy(&dxLight->Diffuse, &diffuse, sizeof(DexColor));
		memcpy(&dxLight->Position, &position, sizeof(DexVector3));
		memcpy(&dxLight->Direction, &direction, sizeof(DexVector3));
		dxLight->Range = range;
		dxLight->Falloff = falloff;
		dxLight->Attenuation0 = attenuation0;
		dxLight->Attenuation1 = attenuation1;
		dxLight->Attenuation2 = attenuation2;
		dxLight->Theta = theta;
		dxLight->Phi = phi;
	}
};


#endif