/*
DexEngine& dae model loader
*/
#ifndef _DEX_MODEL_LOADER_DAE_H
#define _DEX_MODEL_LOADER_DAE_H
#include "../DexBase/DexString.h"
#include "../DexBase/DexDMap.h"
#include "../DexMath/DexMatrix.h"
#include "../DexMath/DexVector4.h"
#include "../DexBase/DexDVector.h"
#include "DexModelLoader.h"
#include "../DexBase/DexColor.h"

#define dae_element_decalare_library(name,member)\
class name :public DaeBase\
	{\
	public:\
	DVector<member*> vecData;\
	name();\
	virtual ~name();\
};
class TiXmlNode;
class TiXmlElement;
class DexSkinMesh;
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
		ECE_accessor,
		ECE_animation,
		ECE_asset,
		ECE_ambient,
		ECE_controller,
		ECE_diffuse,
		ECE_effect,
		ECE_effect_profile,
		ECE_emission,
		ECE_geometry,
		ECE_images,
		ECE_library_animations,
		ECE_library_images,
		ECE_library_meterials,
		ECE_library_effects,
		ECE_library_geometries,
		ECE_library_controllers,
		ECE_library_visual_scenes,
		ECE_instance_geometry,
		ECE_instance_controller,
		ECE_material,
		ECE_mesh,
		ECE_newparam,
		ECE_newparam_sampler2D,
		ECE_newparam_surface,
		ECE_node,
		ECE_phong,
		ECE_polygons,
		ECE_polylist,
		ECE_reflective,
		ECE_reflectivity,
		ECE_scene,
		ECE_shininess,
		ECE_skin,
		ECE_specular,
		ECE_source,
		ECE_technique,
		ECE_triangle,
		ECE_transparent,
		ECE_transparency,
		ECE_vertices,
		ECE_visual_scene,

		ECE_Total
	};
	enum EAxisUp
	{
		EAU_X,
		EAU_Y,
		EAU_Z,
	};
	enum ESemantic
	{
		ES_VERTEX,
		ES_POSITION,
		ES_NORMAL,
		ES_TEXCOORD,
		ES_TEXTANGENT,
		ES_TEXBINORMAL,
		ES_COLOR,
		ES_JOINT,
		ES_INV_BIND_MATRIX,
		ES_WEIGHT,
		ES_INPUT, //for animation sampler
		ES_OUTPUT,
		ES_INTERPOLATION,
		ES_UNKNOWN,
	};
	enum EAccessorType
	{
		EAT_FLOAT,
		EAT_NAME,
		EAT_FLOAT4X4,
		EAT_TOTAL
	};
	enum ENodeType
	{
		ENT_UNKNOWN,
		ENT_NODE,
		ENT_JOINT,
		ENT_TOTAL
	};
	class DaeBase
	{
	public:
		DaeBase* father;
		DaeBase* child;
		DaeBase* sibling;
		EColladaElement eType;
	public:
		DaeBase(EColladaElement _type);
		virtual ~DaeBase();
		void AddChild(DaeBase* _child);
	};
	class DaeAsset :public DaeBase
	{
	public:
		DString  sAuthor;
		DString  created;
		DString  modified;
		DString  sUnitName;
		float32  fUnit;
		
		EAxisUp  eAxisUp;
		DaeAsset() :DaeBase(ECE_asset){ eAxisUp = EAU_Y; fUnit = 1.0f; }
		virtual ~DaeAsset(){}
	};
	struct stDaeInput
	{
		ESemantic semantic;
		DString   source;
		uint32	  offset;
		uint32	  set; //for TEXCOORD attribute:"set"
		DString   flag0;
	};
	struct stInstanceMaterial
	{
		DString sSymbol;
		DString sTarget;
	};
	class DaeImage :public DaeBase
	{
	public:
		DString  sId;
		DString  sName;
		DString  init_from;
		DaeImage() :DaeBase(ECE_images){}
		virtual ~DaeImage(){}
	};

	class DaeMaterial :public DaeBase
	{
	public:
		DString  sId;
		DString  name;
		DString  instance_effect_url;
		DaeMaterial() :DaeBase(ECE_material){}
		virtual ~DaeMaterial(){}
	};
	class DaeNewParam : public DaeBase
	{
	public:
		DString sId;
	public:
		DaeNewParam() :DaeBase(ECE_newparam){}
		virtual ~DaeNewParam(){}
	};
	class DaeNewParamSurface : public DaeNewParam
	{
	public:
		DString sSurfaceType;
		DString sInitFrom;
		DString sFormat;
	public:
		DaeNewParamSurface(){ eType = ECE_newparam_surface;}
		virtual ~DaeNewParamSurface(){}
	};
	class DaeNewParamSampler2D : public DaeNewParam
	{
	public:
		DString sSource;
		DString sWrapS;
		DString sWrapT;
		DString sMinFilter;
		DString sMagFilter;
		DString sMipFilter;
	public:
		DaeNewParamSampler2D(){ eType = ECE_newparam_sampler2D; }
		virtual ~DaeNewParamSampler2D(){}
	};
	class DaeDiffuse :public DaeBase
	{
	public:
		DString  sTexture;
		DString  sTexcoord;
		DexColor color;
		DaeDiffuse() :DaeBase(ECE_diffuse){}
		virtual ~DaeDiffuse(){}
	};
	class DaePhong :public DaeBase
	{
	public:
		DexColor  colorEmission;
		DexColor  colorAmbient;
		DexColor  colorSpecular;
		DexColor  colorTransparent;
		DexColor  colorReflective;
		DaeDiffuse*  pDiffuse;
		float32   fReflectivity;
		float32	  fShininess;
		float32   fTransparency;
		float32   fIndexOfRefraction;
		DaePhong() :DaeBase(ECE_phong){ pDiffuse = nullptr; fReflectivity = fShininess = fTransparency = fIndexOfRefraction = 0.0f; }
		virtual ~DaePhong(){ if (pDiffuse != nullptr) delete pDiffuse; }
	};
	class DaeTechnique :public DaeBase
	{
	public:
		DString  sid;
		DaePhong* pPhong;
		DaeTechnique() :DaeBase(ECE_technique){ pPhong = nullptr; }
		virtual ~DaeTechnique(){ if (pPhong != nullptr) delete pPhong; }
	};
	class DaeEffectProfile :public DaeBase
	{
	public:
		DVector<DaeNewParam*> vecNewParams;
		DaeTechnique*         pTechnique;
		DaeEffectProfile();
		virtual ~DaeEffectProfile();
	};
	class DaeEffect :public DaeBase
	{
	public:
		DString  sId;
		DString  sName;
		DaeEffectProfile* pProfile;
		DaeEffect() :DaeBase(ECE_effect){ pProfile = nullptr; }
		virtual ~DaeEffect(){ if (pProfile != nullptr) delete pProfile; }
	};
	class DaeSource :public DaeBase
	{
	public:
		DString		sId;
		DString     sName;
		DString     sFloatArrayId;
		int32		iFloatArrayCount;
		float64*	pFloatArrayValues;
		DString		sNameArrayId;
		DVector<DString>	vNamaArray;

		
		DaeSource() :DaeBase(ECE_source){ iFloatArrayCount = 0; pFloatArrayValues = nullptr; }
		virtual ~DaeSource()
		{
			if (pFloatArrayValues != nullptr)	delete[] pFloatArrayValues;
		}
	};
	class DaeAccessor :public DaeBase
	{
	public:
		DString sSource;
		EAccessorType EAT_type;
		uint32  iCount;
		uint32  iStride;
		DaeAccessor() :DaeBase(ECE_accessor){}
		virtual ~DaeAccessor(){}
	};
	class DaeVertices :public DaeBase
	{
	public:
		DString  id;
		DVector<stDaeInput> vInputs; //有些文件会把除了pos之外的normal、uv等input也放在vertices里面，所以要用向量装起来
		DaeVertices() :DaeBase(ECE_vertices){}
		virtual ~DaeVertices(){ vInputs.swap(DVector<stDaeInput>()); }
	};
	class DaeTriangles :public DaeBase
	{
	public:
		int32   iCount;
		DString material;
		DVector<stDaeInput> vInputs;
		int32*  pData;
		DaeTriangles() :DaeBase(ECE_triangle){ pData = nullptr; }
		virtual ~DaeTriangles() { if (pData != nullptr) delete[]pData; }
	};
	class DaePolylist :public DaeBase
	{
	public:
		int32   count;
		DString material;
		DVector<stDaeInput> vInputs;
		int32*  p_vcountData;
		int32*  pData;
		DaePolylist() :DaeBase(ECE_polylist){ p_vcountData = pData = nullptr; }
		virtual ~DaePolylist() { if (pData != nullptr) delete[]pData; if (p_vcountData != nullptr) delete[]p_vcountData; }
	};
	class DaePolygons :public DaeBase
	{
	public:
		struct stPData
		{
			uint32 count;//顶点个数
			uint32 pData[32]; //最多支持8个input,4个顶点，所以是8*4=32个数据
			stPData(){ count = 0; memset(pData, 0, sizeof(pData)); }
		};
		int32   count;
		DString material;
		DVector<stDaeInput> inputs;
		stPData*  pData;
		DaePolygons() :DaeBase(ECE_polygons){ pData = nullptr; }
		virtual ~DaePolygons() { if (pData != nullptr) delete[]pData;}
	};
	class DaeSkin : public DaeBase
	{
	public:
		struct stJointData
		{
			uint32 count;
			uint32 index; //数据流中的起始index
		};
		DString sSource;
		DexMatrix4x4 mMatrix;
		DVector<stDaeInput> vJointsInputs;
		DVector<stDaeInput> vVertexWeightInputs;
		uint32  iVertexWeightsCount; //一定是和geometry里面的顶点数量是一样的
		stJointData*  pVCountData; //for vcount
		int32*  pData;    //for v
		DVector<DaeSource*> vSources;
		DaeSource*  pJointInvMatrix;
		DaeSource*	pJointsName; //point to vSources. no need release
		DaeSource*  pJointsWeight;//point to vSources. no need release
		DaeSkin();
		virtual ~DaeSkin();
	};
	class DaeController : public DaeBase
	{
	public:
		DString sId;
		DaeSkin* pSkin;
		DaeController() :DaeBase(ECE_controller){ pSkin = nullptr; }
		virtual ~DaeController() { if (pSkin != nullptr) delete pSkin; }
	};
	class DaeNode : public DaeBase
	{
	public:
		DString	sId;
		DString sName;
		DString sSId;
		ENodeType eNodeType;
		DexMatrix4x4 mMatrix;
		DVector<DaeNode*> vNodeChildren;
		DaeBase* pInstanceMesh; //instancegeometry or instancecontroller
		DaeNode();
		virtual ~DaeNode();
	};
	class DaeVisualScene : public DaeBase
	{
	public:
		DString	sId;
		DString sName;
		DVector<DaeNode*> vJointsSystem;
		DVector<DaeNode*> vMeshs;
		DaeVisualScene();
		virtual ~DaeVisualScene();
	};
	class DaeAnimation : public DaeBase
	{
	public:
		DString sId;
		DString sName;
		DVector<DaeSource*> vSources;
		DaeSource* pInput; //input is time
		DaeSource* pOutput; //output is matrix
		//DaeSource* pInterolation;// 目前并不用这个差值参数，现在关键帧之间全是线性插值
		DVector<stDaeInput> vSamplerInputs;
		DString sChannelSource;
		DString sChannelTarget;
		DaeAnimation();
		virtual ~DaeAnimation();
	};
	class DaeInstanceGeometry : public DaeBase
	{
	public:
		DString	sUrl;
		DString sName;
		DVector<stInstanceMaterial> vecMaterials;
		DaeInstanceGeometry() :DaeBase(ECE_instance_geometry){}
		virtual ~DaeInstanceGeometry() {}
	};
	class DaeInstanceController : public DaeBase
	{
	public:
		DString	sUrl;
		DVector<DString> vecSkeletons;
		DVector<stInstanceMaterial> vecMaterials;
		DaeInstanceController() :DaeBase(ECE_instance_controller){}
		virtual ~DaeInstanceController() {}
	};
	class DaeMesh : public DaeBase
	{
	public:
		DaeVertices*        pVerticles;
		DVector<DaeSource*> vecSources;
		DVector<DaeTriangles*> vecTriangles;
		DVector<DaePolygons*> vecPolygons;
		DVector<DaePolylist*> vecPolylists;
		DaeMesh();
		virtual ~DaeMesh();
	};
	class DaeGeometry :public DaeBase
	{
	public:
		DString  sId;
		DString  sName;
		DaeMesh* pMesh;
		DaeGeometry();
		virtual ~DaeGeometry();
	};
	dae_element_decalare_library(DaeLibraryImage, DaeImage)
	dae_element_decalare_library(DaeLibraryMaterials, DaeMaterial)
	dae_element_decalare_library(DaeLibraryEffects, DaeEffect)
	dae_element_decalare_library(DaeLibraryGeometries, DaeGeometry)
	dae_element_decalare_library(DaeLibraryControllers, DaeController)
	dae_element_decalare_library(DaeLibraryVisualScenes, DaeVisualScene)
	dae_element_decalare_library(DaeLibraryAnimations, DaeAnimation)
	class DaeCollada :public DaeBase
	{
	public:
		DString  version;
		DString  xmlns;
		DaeAsset* pAsset;
		DaeLibraryImage* pLibraryImages;
		DaeLibraryMaterials* pLibraryMaterials;
		DaeLibraryEffects* pLibraryEffects;
		DaeLibraryGeometries* pLibraryGeometries;
		DaeLibraryControllers* pLibraryControllers;
		DaeLibraryVisualScenes* pLibraryVisualScenes;
		DaeLibraryAnimations*   pLibraryAnimations;
		DaeCollada();
		virtual ~DaeCollada();
	};
protected:

	DaeCollada* parse_COLLADA(TiXmlNode* pXmlNode);

	DaeBase* parse_asset(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_library_images(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_image(TiXmlNode* pXmlNode, DaeLibraryImage* father);
	DaeBase* parse_library_materials(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_material(TiXmlNode* pXmlNode, DaeLibraryMaterials* father);
	DaeBase* parse_library_effects(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_effect(TiXmlNode* pXmlNode, DaeLibraryEffects* father);
	DaeBase* parse_library_geometries(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_geometry(TiXmlNode* pXmlNode, DaeLibraryGeometries* father);
	DaeSource* parse_source(TiXmlNode* pXmlNode);
	DaeBase* parse_accessor(TiXmlNode* pXmlNode, DaeSource* father);
	DaeBase* parse_library_controllers(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_controller(TiXmlNode* pXmlNode, DaeLibraryControllers* father);
	DaeBase* parse_skin(TiXmlNode* pXmlNode, DaeController* father);
	DaeBase* parse_library_visual_scenes(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_visual_scene(TiXmlNode* pXmlNode, DaeLibraryVisualScenes* father);
	DaeBase* parse_scene(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_library_animations(TiXmlNode* pXmlNode, DaeCollada* father);
	DaeBase* parse_animation(TiXmlNode* pXmlNode, DaeLibraryAnimations* father);
	DaeEffectProfile* parse_effect_profile(TiXmlNode* pXmlNode);
	DaeNewParam* parse_newparam(TiXmlNode* pXmlNode);
	DaeNewParam* parse_newparam_surface(TiXmlNode* pXmlNode);
	DaeNewParam* parse_newparam_sampler2D(TiXmlNode* pXmlNode);
	DaePhong* parse_phong(TiXmlNode* pXmlNode);
	DaeTechnique* parse_technique(TiXmlNode* pXmlNode);
	void	 parse_color(TiXmlNode* pXmlNode, DexColor& color);
	void	 parse_emission(TiXmlNode* pXmlNode, DexColor& color);
	void	 parse_ambient(TiXmlNode* pXmlNode, DexColor& color);
	DaeDiffuse* parse_diffuse(TiXmlNode* pXmlNode, DaePhong* father);
	void	 parse_specular(TiXmlNode* pXmlNode, DexColor& color);
	void	 parse_float(TiXmlNode* pXmlNode, float32& fValue);
	void	 parse_shininess(TiXmlNode* pXmlNode, float32& fShininess);
	void	 parse_reflective(TiXmlNode* pXmlNode, DexColor& color);
	void	 parse_reflectivity(TiXmlNode* pXmlNode, float32& fValue);
	void	 parse_transparent(TiXmlNode* pXmlNode, DexColor& color);
	void	 parse_transparency(TiXmlNode* pXmlNode, float32& fValue);
	void	 parse_index_of_refraction(TiXmlNode* pXmlNode, float32& fValue);
	DaeBase* parse_mesh(TiXmlNode* pXmlNode, DaeGeometry* father);
	DaeBase* parse_vertices(TiXmlNode* pXmlNode, DaeMesh* father);
	DaeBase* parse_triangle(TiXmlNode* pXmlNode, DaeMesh* father);
	DaeBase* parse_polylist(TiXmlNode* pXmlNode, DaeMesh* father);
	DaeBase* parse_polygons(TiXmlNode* pXmlNode, DaeMesh* father);
	DaeNode* parse_node(TiXmlNode* pXmlNode);
	DaeInstanceGeometry* parse_instance_geometry(TiXmlNode* pXmlNode);
	DaeInstanceController* parse_instance_controller(TiXmlNode* pXmlNode); 
protected:
	stDaeInput& parse_input(TiXmlElement* pXmlElement, stDaeInput& input);
	stInstanceMaterial& parse_instance_material(TiXmlElement* pXmlElement, stInstanceMaterial& instanceMaterial);
	void str_to_float64_array(const char* str, float64* value, char splitChar=' ');

	/*把str里面的参数读进value中，每cycle个数据一个循环，并对一个循环中的第index个数据当flag个有效数据
	str_to_int32_array("1 2 3 4 5 6 7 8 9", value, 3, 0, 2);
	则 value = {1,1,2,3,4,4,5,6,7,7,8,9};
	*/
	void str_to_int32_array(const char* str, int32** value, uint8 cycle = 3,uint8 index = 0, uint8 flag = 1);
	void TransVector3ByAxis(DexVector3& vec3); //根据坐标系对坐标法线等进行转换
protected:
	DaeCollada* m_pCollada;
	bool		m_bFFXIIModel;
	uint32		m_iJointCount;
	//for ffxii model
	DMap<DString, DString> m_MapJointName; 
	DMap<DString, DexMatrix4x4> m_MapJointMatrix;
protected:
	DaeImage*	 find_image(DString sImageId);
	DaeMaterial* find_material(DString sMaterialId);
	DaeEffect*   find_effect(DString sEffectId);
	DaeGeometry* find_geometry(DString sGeometryId);
	DaeController* find_controller(DString sControllerId);
	DaeSource*   find_source(DVector<DaeSource*>& vData, DString sId);
	DaeImage*    find_image(DaeEffectProfile* pEffectProfile, DString sTexture);
	
	//根据instance_controller 里面的skeleton去visual_scene里面寻找骨骼节点
	DaeNode*     find_joint(DString jointId);
		DaeNode*     find_joint(DaeNode* pNode, DString jointId);//用于在上面的find_joint中进行递归


	DexSkinMesh* create_SkinMeshStatic(DaeNode* pStaticMeshNode);
	DexSkinMesh* create_SkinMeshAni(DaeNode* pAniMeshNode);

	//将pNode以及其所有的子节点关节都添加到pSkinMesh中
	//如果pFatherNode为null,则把pNode添加到根节点中
	void		 add_joints(DexSkinMesh* pSkinMesh, DaeNode* pNode, DaeNode* pFatherNode = nullptr);
	//将vec里面所有的meterial和texture数据读进mesh中，并且将名字保存到vec2(material) 和 vec3(image)
	void		 deal_with_material_texture(DVector<stInstanceMaterial>& vec, DexSkinMesh* pSkinMesh, DVector<DString>& vec2, DVector<DString>& vec3);
	
public:
	virtual bool		  SupportType(const char* fileType);
	virtual DexModelBase* LoadModel(const char* filename, int32 flag);

	void LoadFFMap(DVector<DexSkinMesh*>& vecSkinMesh, const char* filename);
	bool ReadFFSkeletonInfo(DexSkinMesh* pDexSkinMesh, DString filename);//mws file

	bool ReadActInfoFxii(DexSkinMesh* pDexSkinMesh, DString sFileName);
};

#endif 