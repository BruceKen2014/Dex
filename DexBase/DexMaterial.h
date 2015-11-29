/*
DexEngine&Material
*/
#ifndef _DEX_MATERIAL_H
#define _DEX_MATERIAL_H

#include "DexType.h"
#include "DexColor.h"
class DexMaterial
{
public:
	DexColor diffuse;
	DexColor specular;
	DexColor ambient;
	DexColor emissive;
	float32  power;
public:
	DexMaterial()
	{
		diffuse = DEXCOLOR_WHITE;
		ambient = DEXCOLOR_BLACK;
		specular = DEXCOLOR_BLACK;
		emissive = DEXCOLOR_BLACK;
	};
	DexMaterial(const DexColor& _diffuse, const DexColor& _specular, const DexColor& _ambient,
		const DexColor& _emissive, float32  _power = 1.0f) :diffuse(_diffuse), specular(_specular),
		ambient(_ambient), emissive(_emissive), power(_power)
	{

	}
	~DexMaterial(){};
public:
	void Set(const DexColor& _diffuse, const DexColor& _specular, const DexColor& _ambient,
		const DexColor& _emissive, float32  _power = 1.0f)
	{
		diffuse = _diffuse;
		specular = _specular;
		ambient = _ambient;
		emissive = _emissive;
		power = _power;
	}
	void GetDXMaterial(D3DMATERIAL9& material)	const
	{
		size_t size = sizeof(DexColor);
		memcpy(&material.Diffuse, &diffuse, size);
		memcpy(&material.Ambient, &ambient, size);
		memcpy(&material.Emissive, &emissive, size);
		memcpy(&material.Specular, &specular, size);
		material.Power = power;
	}
};
#endif