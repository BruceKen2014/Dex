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
		ambient = DEXCOLOR_WHITE;
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
};
#endif