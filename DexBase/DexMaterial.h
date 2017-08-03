/*
DexEngine&Material
*/
#ifndef _DEX_MATERIAL_H
#define _DEX_MATERIAL_H

#include "DexType.h"
#include "DexColor.h"
#include "DexString.h"
class DexMaterial
{
public:
	char	 name[64];
	DexColor diffuse;
	DexColor specular;
	DexColor ambient;
	DexColor emissive;
	DFloat32  power;
public:
	DexMaterial()
	{
		dexstrcpy(name, "default");
		diffuse = DEXCOLOR_WHITE;
		ambient = DEXCOLOR_WHITE;
		specular = DEXCOLOR_BLACK;
		emissive = DEXCOLOR_BLACK;
		power = 1.0f;
	};
	DexMaterial(const DexColor& _diffuse, const DexColor& _specular, const DexColor& _ambient,
		const DexColor& _emissive, DFloat32  _power = 1.0f) :diffuse(_diffuse), specular(_specular),
		ambient(_ambient), emissive(_emissive), power(_power)
	{

	}
	~DexMaterial(){};
public:
	void Set(const DexColor& _diffuse, const DexColor& _specular, const DexColor& _ambient,
		const DexColor& _emissive, DFloat32  _power = 1.0f)
	{
		diffuse = _diffuse;
		specular = _specular;
		ambient = _ambient;
		emissive = _emissive;
		power = _power;
	}
};
#endif