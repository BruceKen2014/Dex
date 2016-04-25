/*
DexEngine& dae model loader
*/
#ifndef _DEX_MODEL_LOADER_DAE_H
#define _DEX_MODEL_LOADER_DAE_H
#include "../DexBase/DexString.h"
#include "../DexMath/DexVector4.h"
#include "DexModelLoader.h"

#define dae_element_decalare_empty(name,_type)\
class name :public DaeBase\
	{\
	public:\
	name() :DaeBase(_type){}\
	virtual ~name(){}\
};
class TiXmlNode;
class DexModelDaeLoader :public IDexModelLoader
{
public:
	DexModelDaeLoader();
	virtual ~DexModelDaeLoader();

public:
	enum EColladaElement
	{
		ECE_unknown,
		ECE_COLLADA,
		ECE_asset,
		ECE_ambient,
		ECE_controller,
		ECE_diffuse,
		ECE_effect,
		ECE_emission,
		ECE_geometry,
		ECE_images,
		ECE_library_images,
		ECE_library_meterials,
		ECE_library_effects,
		ECE_library_geometries,
		ECE_library_controllers,
		ECE_library_visual_scenes,
		ECE_material,
		ECE_profile_COMMON,
		ECE_phong,
		ECE_reflective,
		ECE_reflectivity,
		ECE_scene,
		ECE_shininess,
		ECE_specular,
		ECE_technique,
		ECE_transparent,
		ECE_transparency,
		ECE_visual_scene,

		ECE_Total
	};
	class DaeBase
	{
	public:
		DaeBase* father;
		DaeBase* child;
		DaeBase* sibling;
		EColladaElement type;
	public:
		DaeBase(EColladaElement _type);
		virtual ~DaeBase();
		void AddChild(DaeBase* _child);
	};

	class DaeCollada :public DaeBase
	{
	public:
		DString  version;
		DString  xmlns;
		DaeCollada() :DaeBase(ECE_COLLADA){}
		virtual ~DaeCollada(){}
	};
	class DaeAsset :public DaeBase
	{
	public:
		DString  created;
		DString  modified;
		DString  up_axis;
		DaeAsset() :DaeBase(ECE_asset){}
		virtual ~DaeAsset(){}
	};
	dae_element_decalare_empty(DaeLibraryImage, ECE_library_images)
	dae_element_decalare_empty(DaeLibraryMaterials, ECE_library_meterials)
	dae_element_decalare_empty(DaeLibraryEffects, ECE_library_effects)
	dae_element_decalare_empty(DaeLibraryGeometries, ECE_library_geometries)
	dae_element_decalare_empty(DaeLibraryControllers, ECE_library_controllers)
	dae_element_decalare_empty(DaeLibraryVisualScenes, ECE_library_visual_scenes)
	dae_element_decalare_empty(DaeProfileCommon, ECE_profile_COMMON)
	dae_element_decalare_empty(DaePhong, ECE_phong)
	class DaeImage :public DaeBase
	{
	public:
		DString  id;
		DString  name;
		DString  init_from;
		DaeImage() :DaeBase(ECE_images){}
		virtual ~DaeImage(){}
	};
	class DaeMaterial :public DaeBase
	{
	public:
		DString  id;
		DString  name;
		DString  instance_effect_url;
		DaeMaterial() :DaeBase(ECE_material){}
		virtual ~DaeMaterial(){}
	};
	class DaeEffect :public DaeBase
	{
	public:
		DString  id;
		DString  name;
		DaeEffect() :DaeBase(ECE_effect){}
		virtual ~DaeEffect(){}
	};
	class DaeTechnique :public DaeBase
	{
	public:
		DString  sid;
		DaeTechnique() :DaeBase(ECE_technique){}
		virtual ~DaeTechnique(){}
	};
	class DaeEmission :public DaeBase
	{
	public:
		DexVector4  color;
		DaeEmission() :DaeBase(ECE_emission){}
		virtual ~DaeEmission(){}
	};
	class DaeAmbient :public DaeBase
	{
	public:
		DexVector4  color;
		DaeAmbient() :DaeBase(ECE_ambient){}
		virtual ~DaeAmbient(){}
	};
	class DaeDiffuse :public DaeBase
	{
	public:
		DString  texture;
		DString  texcoord;
		DaeDiffuse() :DaeBase(ECE_diffuse){}
		virtual ~DaeDiffuse(){}
	};
	class DaeSpecular :public DaeBase
	{
	public:
		DexVector4  color;
		DaeSpecular() :DaeBase(ECE_specular){}
		virtual ~DaeSpecular(){}
	};
	class DaeReflective :public DaeBase
	{
	public:
		DexVector4  color;
		DaeReflective() :DaeBase(ECE_reflective){}
		virtual ~DaeReflective(){}
	};
	class DaeTransparent :public DaeBase
	{
	public:
		DexVector4  color;
		DaeTransparent() :DaeBase(ECE_transparent){}
		virtual ~DaeTransparent(){}
	};
	class DaeShininess :public DaeBase
	{
	public:
		float  shininess;
		DaeShininess() :DaeBase(ECE_shininess){}
		virtual ~DaeShininess(){}
	};
	class DaeReflectivity :public DaeBase
	{
	public:
		float  reflectivity;
		DaeReflectivity() :DaeBase(ECE_shininess){}
		virtual ~DaeReflectivity(){}
	};
	class DaeTransparency :public DaeBase
	{
	public:
		float  transparency;
		DaeTransparency() :DaeBase(ECE_transparency){}
		virtual ~DaeTransparency(){}
	};
protected:

	DaeBase* parse_COLLADA(TiXmlNode* pXmlNode);

	DaeBase* parse_asset(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_library_images(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_image(TiXmlNode* pXmlNode, DaeLibraryImage* father);
	DaeBase* parse_library_materials(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_material(TiXmlNode* pXmlNode, DaeLibraryMaterials* father);
	DaeBase* parse_library_effects(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_effect(TiXmlNode* pXmlNode, DaeLibraryEffects* father);
	DaeBase* parse_library_geometries(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_geometry(TiXmlNode* pXmlNode, DaeLibraryGeometries* father);
	DaeBase* parseMesh(TiXmlNode* pXmlNode);
	DaeBase* parseSource(TiXmlNode* pXmlNode);
	DaeBase* parse_library_controllers(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_controller(TiXmlNode* pXmlNode, DaeLibraryControllers* father);
	DaeBase* parse_library_visual_scenes(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_visual_scene(TiXmlNode* pXmlNode, DaeLibraryVisualScenes* father);
	DaeBase* parse_scene(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_profile_Common(TiXmlNode* pXmlNode, DaeEffect* father);
	DaeBase* parse_phong(TiXmlNode* pXmlNode, DaeTechnique* father);
	DaeBase* parse_technique(TiXmlNode* pXmlNode, DaeProfileCommon* father);
	DaeBase* parse_emission(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_ambient(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_diffuse(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_specular(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_shininess(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_reflective(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_reflectivity(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_transparent(TiXmlNode* pXmlNode, DaePhong* father);
	DaeBase* parse_transparency(TiXmlNode* pXmlNode, DaePhong* father);
public:
	virtual DexModelBase* LoadModel(const char* filename);
};

#endif 