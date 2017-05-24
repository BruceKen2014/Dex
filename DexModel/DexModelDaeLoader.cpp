

#include "DexModelDaeLoader.h"
#include "../DexBase/DexDVector.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"
#include "../DexMath/DexQuaternion.h"
#include "DexSkinMeshFF.h"
#include "../DexBase/DexMemoryLeakCheck.h"
#include "../DexBase/DexStreamFile.h"




static const char* g_COLLADA = "COLLADA";
static const char* g_asset = "asset";
static const char* g_accessor = "accessor";
static const char* g_controller = "controller";
static const char* g_effect = "effect";
static const char* g_float_array = "float_array";
static const char* g_geometry = "geometry";
static const char* g_image = "image";
static const char* g_library_controllers = "library_controllers";
static const char* g_library_effects     = "library_effects";
static const char* g_library_geometries  = "library_geometries";
static const char* g_library_images		 = "library_images";
static const char* g_library_materials	 = "library_materials";
static const char* g_library_visual_scenes = "library_visual_scenes";
static const char* g_library_animations = "library_animations";
static const char* g_material = "material";
static const char* g_mesh = "mesh";
static const char* g_Name_array = "Name_array";
static const char* g_profile_COMMON = "profile_COMMON";
static const char* g_polylist = "polylist";
static const char* g_polygons = "polygons";
static const char* g_param = "param";
static const char* g_scene = "scene";
static const char* g_source = "source";
static const char* g_technique = "technique";
static const char* g_triangles = "triangles";
static const char* g_technique_common = "technique_common";
static const char* g_visual_scene = "visual_scene";
static const char* g_vertices = "vertices";


#define IS_Element(name)   if (strcmp(pXmlElement->Value(), name) == 0)
#define IS_Attribute(name)	if (strcmp(pXmlAttribute->Name(), name) == 0)
#define VEC_ADD(vec,value)   vec.push_back(value)

#define dae_element_imp_library(name,eType)\
DexModelDaeLoader::name::name() :DaeBase(eType)\
{\
	\
}\
DexModelDaeLoader::name::~name()\
{\
	_SafeClearVector(vecData);\
}
dae_element_imp_library(DaeLibraryImage, ECE_library_images)
dae_element_imp_library(DaeLibraryMaterials, ECE_library_meterials)
dae_element_imp_library(DaeLibraryEffects, ECE_library_effects)
dae_element_imp_library(DaeLibraryGeometries, ECE_library_geometries)
dae_element_imp_library(DaeLibraryControllers, ECE_library_controllers)
dae_element_imp_library(DaeLibraryVisualScenes, ECE_library_visual_scenes)
dae_element_imp_library(DaeLibraryAnimations, ECE_library_animations)
DexModelDaeLoader::DaeBase::DaeBase(EColladaElement _type)
{
	eType = _type;
	father = nullptr;
	child = nullptr;
	sibling = nullptr;
}
DexModelDaeLoader::DaeBase::~DaeBase()
{
	if (child != nullptr)
	{
		delete child;
	}
	if (sibling != nullptr)
	{
		delete sibling;
	}
}

void DexModelDaeLoader::DaeBase::AddChild(DaeBase* _child)
{
	child = _child;
	_child->father = this;
}

DexModelDaeLoader::DaeEffectProfile::DaeEffectProfile() :DaeBase(ECE_effect_profile)
{
	pTechnique = nullptr;
}

DexModelDaeLoader::DaeEffectProfile::~DaeEffectProfile()
{
	_SafeDelete(pTechnique);
	_SafeClearVector(vecNewParams);
}

DexModelDaeLoader::DaeSkin::DaeSkin() :DaeBase(ECE_skin)
{
	pVCountData = nullptr;
	pData = nullptr;
	pJointsName = nullptr;
	pJointsWeight = nullptr;
}

DexModelDaeLoader::DaeSkin::~DaeSkin()
{
	_SafeDeleteArr(pVCountData);
	_SafeDeleteArr(pData);
	_SafeClearVector(vSources);
}

DexModelDaeLoader::DaeNode::DaeNode() :DaeBase(ECE_node)
{
	pInstanceMesh = nullptr;
}

DexModelDaeLoader::DaeNode::~DaeNode()
{
	_SafeDelete(pInstanceMesh);
	_SafeClearVector(vNodeChildren);
}

DexModelDaeLoader::DaeVisualScene::DaeVisualScene() :DaeBase(ECE_visual_scene)
{
}

DexModelDaeLoader::DaeVisualScene::~DaeVisualScene()
{
	_SafeClearVector(vJointsSystem);
	_SafeClearVector(vMeshs);
}

DexModelDaeLoader::DaeAnimation::DaeAnimation() :DaeBase(ECE_animation)
{
	pInput = nullptr;
	pOutput = nullptr;
}

DexModelDaeLoader::DaeAnimation::~DaeAnimation()
{
	_SafeClearVector(vSources);
	vSamplerInputs.clear();
}

DexModelDaeLoader::DaeMesh::DaeMesh() :DaeBase(ECE_mesh)
{
	pVerticles = nullptr;
}

DexModelDaeLoader::DaeMesh::~DaeMesh()
{
	_SafeDelete(pVerticles);
	_SafeClearVector(vecSources);
	_SafeClearVector(vecTriangles);
	_SafeClearVector(vecPolygons);
	_SafeClearVector(vecPolylists);
}

DexModelDaeLoader::DaeGeometry::DaeGeometry() :DaeBase(ECE_geometry)
{
	pMesh = nullptr;
}

DexModelDaeLoader::DaeGeometry::~DaeGeometry()
{
	_SafeDelete(pMesh);
}

DexModelDaeLoader::DaeCollada::DaeCollada() :DaeBase(ECE_COLLADA)
{
	pAsset = nullptr;
	pLibraryImages = nullptr;
	pLibraryMaterials = nullptr;
	pLibraryEffects = nullptr;
	pLibraryGeometries = nullptr;
	pLibraryControllers = nullptr;
	pLibraryVisualScenes = nullptr;
	pLibraryAnimations = nullptr;
}
DexModelDaeLoader::DaeCollada::~DaeCollada()
{
	_SafeDelete(pAsset);
	_SafeDelete(pLibraryImages);
	_SafeDelete(pLibraryMaterials);
	_SafeDelete(pLibraryEffects);
	_SafeDelete(pLibraryGeometries);
	_SafeDelete(pLibraryControllers);
	_SafeDelete(pLibraryVisualScenes);
	_SafeDelete(pLibraryAnimations);
}
DexModelDaeLoader::DexModelDaeLoader()
{

}

DexModelDaeLoader::~DexModelDaeLoader()
{

}

DexModelDaeLoader::DaeCollada* DexModelDaeLoader::parse_COLLADA(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeCollada* pCollada = new DaeCollada;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("version")
			pCollada->version = DString(pXmlAttribute->Value());
		else IS_Attribute("xmlns")
			pCollada->xmlns = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute in COLLADA!");
		pXmlAttribute = pXmlAttribute->Next();
	}

	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_asset)
		{
			getLog()->LogLine(log_ok, "parse asset...");
			parse_asset(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_images)	
		{
			getLog()->LogLine(log_ok, "parse library images...");
			parse_library_images(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_materials)
		{
			getLog()->LogLine(log_ok, "parse library materials...");
			parse_library_materials(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_effects)
		{
			getLog()->LogLine(log_ok, "parse library effect...");
			parse_library_effects(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_geometries)
		{
			getLog()->LogLine(log_ok, "parse library geometries...");
			parse_library_geometries(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_controllers)
		{
			getLog()->LogLine(log_ok, "parse library controllers...");
			parse_library_controllers(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_visual_scenes)
		{
			getLog()->LogLine(log_ok, "parse library scenes...");
			parse_library_visual_scenes(pXmlChildNode, pCollada);
		}
		else IS_Element(g_library_animations)
		{
			getLog()->LogLine(log_ok, "parse library animations...");
			parse_library_animations(pXmlChildNode, pCollada);
		}
		else IS_Element(g_scene)
		{
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child %s in COLLADA!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pCollada;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_asset(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeAsset* pAsset = new DaeAsset;
	father->pAsset = pAsset;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("created")
			pAsset->created = pXmlElement->GetText();
		else IS_Element("modified")
			pAsset->modified = pXmlElement->GetText();
		else IS_Element("contributor")
		{
			TiXmlNode* pXmlContributorChildNode = pXmlChildNode->FirstChild();
			while(pXmlContributorChildNode != nullptr)
			{
				pXmlElement = pXmlContributorChildNode->ToElement();
				IS_Element("author")
				{
					if (pXmlElement->GetText() != nullptr)
						pAsset->sAuthor = DString();
				}
				pXmlContributorChildNode = pXmlContributorChildNode->NextSibling();
			}
		}
		else IS_Element("unit")
		{
			TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("meter")
					pAsset->fUnit = atof(pXmlAttribute->Value());
				else IS_Attribute("name")
					pAsset->sUnitName = pXmlAttribute->Value();
				pXmlAttribute = pXmlAttribute->Next();
			}
		}
		else IS_Element("up_axis")
		{
			if (strcmp(pXmlElement->GetText(), "Z_UP") == 0)
				pAsset->eAxisUp = EAU_Z;
			else if (strcmp(pXmlElement->GetText(), "Y_UP") == 0)
				pAsset->eAxisUp = EAU_Y;
			else if (strcmp(pXmlElement->GetText(), "X_UP") == 0)
				pAsset->eAxisUp = EAU_X;
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child %s in asset!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pAsset;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_images(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryImage* pLibraryImages = new DaeLibraryImage;
	father->pLibraryImages = pLibraryImages;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_image)
			parse_image(pXmlChildNode, pLibraryImages);
		else
			getLog()->LogLine(log_allert, "dae file unknown child in library_images!");
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryImages;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_image(TiXmlNode* pXmlNode, DaeLibraryImage* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeImage* pImage = new DaeImage;
	father->vecData.push_back(pImage);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析image的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pImage->sId = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pImage->sName = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in image!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一个节点<init_from>
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element("init_from")
			pImage->init_from = DString(pXmlChildNode->ToElement()->GetText());
		else
			getLog()->LogLine(log_allert, "dae file unknown element:%s in image!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	} 
	return pImage;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_materials(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryMaterials* pLibraryMaterials = new DaeLibraryMaterials;
	father->pLibraryMaterials = pLibraryMaterials;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_material)
			parse_material(pXmlChildNode, pLibraryMaterials);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_materials!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryMaterials;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_material(TiXmlNode* pXmlNode, DaeLibraryMaterials* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeMaterial* pMaterial = new DaeMaterial;
	father->vecData.push_back(pMaterial);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析material的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pMaterial->sId = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pMaterial->name = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in material!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一个节点<instance_effect>
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element("instance_effect")
		{
			pXmlAttribute = pXmlElement->FirstAttribute();
			pMaterial->instance_effect_url = DString(&(pXmlAttribute->Value())[1]);//跳过第一个井号'#'
		}
		else IS_Element("extra")
		{
			//getLog()->LogLine(log_ok, "dae file skip extra under material.", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown element:%s under material!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pMaterial;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_effects(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryEffects* pLibraryEffects = new DaeLibraryEffects;
	father->pLibraryEffects = pLibraryEffects;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_effect)
			parse_effect(pXmlChildNode, pLibraryEffects);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_effects!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryEffects;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_effect(TiXmlNode* pXmlNode, DaeLibraryEffects* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeEffect* pEffect = new DaeEffect;
	father->vecData.push_back(pEffect);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析effect的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pEffect->sId = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pEffect->sName = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in effect!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一个节点<profile_COMMON>
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element("profile_COMMON")
		{
			if (pEffect->pProfile != nullptr)
				getLog()->LogLine(log_allert, "dae file find multi profile:%s under effect!", pXmlElement->Value());
			else
				pEffect->pProfile = parse_effect_profile(pXmlChildNode);
		}
		else IS_Element("profile_HLSL")
		{
			if (pEffect->pProfile != nullptr)
				getLog()->LogLine(log_allert, "dae file find multi profile:%s under effect!", pXmlElement->Value());
			else
				pEffect->pProfile = parse_effect_profile(pXmlChildNode);
		}
		else
		{
			//getLog()->LogLine(log_allert, "dae file unknown element:%s under effect!", pXmlElement->Value());
		}
pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pEffect;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_geometries(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryGeometries* pLibraryGeometries = new DaeLibraryGeometries;
	father->pLibraryGeometries = pLibraryGeometries;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_geometry)
			parse_geometry(pXmlChildNode, pLibraryGeometries);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_geometries!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryGeometries;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_geometry(TiXmlNode* pXmlNode, DaeLibraryGeometries* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeGeometry* pGeometry = new DaeGeometry;
	father->vecData.push_back(pGeometry);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析geometry的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pGeometry->sId = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pGeometry->sName = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in geometry!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一个节点<mesh>
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_mesh)
			pNewDaeElement = parse_mesh(pXmlChildNode, pGeometry);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown element:%s under effect!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pGeometry;
}

DexModelDaeLoader::DaeSource* DexModelDaeLoader::parse_source(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeSource* pSource = new DaeSource;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析source的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pSource->sId = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pSource->sName = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in source!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一层节点<float_array>等
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	TiXmlNode* pXmlAccessorNode = nullptr;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_float_array)
		{//直接在此解析float_array，不再递归到下一层级
			TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
			//解析float_array的attribute
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("id")
					pSource->sFloatArrayId = DString(pXmlAttribute->Value());
				else IS_Attribute("count")
					pSource->iFloatArrayCount = atoi(pXmlAttribute->Value());
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in source float array!", pXmlAttribute->Name());
				pXmlAttribute = pXmlAttribute->Next();
			}
			//读取float_array的value
			const char* strFloatValue = pXmlElement->GetText();
			pSource->pFloatArrayValues = new float64[pSource->iFloatArrayCount];
			str_to_float64_array(strFloatValue, pSource->pFloatArrayValues);
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else IS_Element(g_Name_array)
		{//直接在此解析Name_array，不再递归到下一层级
			TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
			//解析Name_array的attribute
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("id")
					pSource->sNameArrayId = DString(pXmlAttribute->Value());
				else IS_Attribute("count")
					int i = 0;//不需要这个count,vNameArray自带size()接口
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in source Name array!", pXmlAttribute->Name());
				pXmlAttribute = pXmlAttribute->Next();
			}
			//读取Name_array的value
			const char* strFloatValue = pXmlElement->GetText();
			SplitStr(DString(strFloatValue), ' ', pSource->vNamaArray);
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else IS_Element(g_technique_common)
		{
			//technique_common本身没有属性，直接解析子节点<accessor>
			pXmlAccessorNode = pXmlChildNode->FirstChild();
			uint8 iAccessorCount = 0;
			while (pXmlAccessorNode != nullptr)
			{
				pXmlElement = pXmlAccessorNode->ToElement();
				//此处不再进行多个sibling的处理，technique_common下面应该只有一个accessor
				IS_Element(g_accessor)
				{
					pNewDaeElement = parse_accessor(pXmlAccessorNode, pSource);
					++iAccessorCount;
				}
				if (iAccessorCount > 2)
				{
					getLog()->LogLine(log_allert, "multy accessor under source %s!",pSource->sId.c_str());
					break;
				}
				pXmlAccessorNode = pXmlAccessorNode->NextSibling();
			}
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown element:%s under source!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr && pNewDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}

	return pSource;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_accessor(TiXmlNode* pXmlNode, DaeSource* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeAccessor* pAccessor = new DaeAccessor;
	pAccessor->father = father;
	if (father->child == nullptr)
		father->child = pAccessor;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析accessor的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute(g_source)
			pAccessor->sSource = DString(&pXmlAttribute->Value()[1]);//skip #
		else IS_Attribute("count")
			pAccessor->iCount = atoi(pXmlAttribute->Value());
		else IS_Attribute("stride")
			pAccessor->iStride = atoi(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in accessor!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	if (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		pXmlAttribute = pXmlElement->FirstAttribute();
		while (pXmlAttribute != nullptr)
		{
			IS_Attribute("type")
			{
				if (strcmp("Name", pXmlAttribute->Value()) == 0)
					pAccessor->EAT_type = EAT_NAME;
				else if (strcmp("float", pXmlAttribute->Value()) == 0)
					pAccessor->EAT_type = EAT_FLOAT;
				else if (strcmp("float4x4", pXmlAttribute->Value()) == 0)
					pAccessor->EAT_type = EAT_FLOAT4X4;
			}
			else IS_Attribute("name")
			{

			}
			else
				getLog()->LogLine(log_allert, "dae file unknown attribute:%s in accessor param!", pXmlAttribute->Name());
			pXmlAttribute = pXmlAttribute->Next();
		}
			
	}
	return pAccessor;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_controllers(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryControllers* pLibraryControllers = new DaeLibraryControllers;
	father->pLibraryControllers = pLibraryControllers;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_controller)
			parse_controller(pXmlChildNode, pLibraryControllers);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_controllers!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryControllers;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_controller(TiXmlNode* pXmlNode, DaeLibraryControllers* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeController* pController = new DaeController;
	father->vecData.push_back(pController);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析controller的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pController->sId = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in controller!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("skin")
			parse_skin(pXmlChildNode, pController);
		else IS_Element("extra")
			getLog()->LogLine(log_ok, "dae file skip extra under controller.", pXmlElement->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in controller!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pController;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_skin(TiXmlNode* pXmlNode, DaeController* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeSkin* pSkin = new DaeSkin;
	father->pSkin = pSkin;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析skin的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("source")
			pSkin->sSource = DString(&pXmlAttribute->Value()[1]);//skip #
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in skin!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("bind_shape_matrix")
		{
			float64* values = new float64[16];
			str_to_float64_array(pXmlElement->GetText(), values);
			for (uint16 i = 0; i < 16; ++i)
				pSkin->mMatrix.m[i] = values[i];
			delete[] values;
		}
		else IS_Element("source")
			pSkin->vSources.push_back(parse_source(pXmlChildNode));
		else IS_Element("joints")
		{
			TiXmlNode* pInputNode = pXmlChildNode->FirstChild();
			//目前joints下面只有input
			while (pInputNode != nullptr)
			{
				pXmlElement = pInputNode->ToElement();
				stDaeInput input;
				parse_input(pXmlElement, input);
				if (input.semantic == ES_INV_BIND_MATRIX)
					for (size_t j = 0; j < pSkin->vSources.size(); ++j)
					{
						if (pSkin->vSources[j]->sId == input.source)
						{
							pSkin->pJointInvMatrix = pSkin->vSources[j];
							break;
						}
					}
				pSkin->vJointsInputs.push_back(input);
				pInputNode = pInputNode->NextSibling();
			}
		}
		else IS_Element("vertex_weights")
		{
			pXmlAttribute = pXmlElement->FirstAttribute();//默认vertex_weights里面的attributes只有count
			pSkin->iVertexWeightsCount = atoi(pXmlAttribute->Value());
			TiXmlNode* pXmlVertexChildNode = pXmlChildNode->FirstChild();
			//vertex_weight下面有input也有vcount等
			uint32 i_VCount = 0;
			while (pXmlVertexChildNode != nullptr)
			{
				pXmlElement = pXmlVertexChildNode->ToElement();
				IS_Element("input")
				{
					stDaeInput input;
					parse_input(pXmlElement, input);
					if (input.semantic == ES_JOINT)
						for (size_t j = 0; j < pSkin->vSources.size(); ++j)
						{
							if (pSkin->vSources[j]->sId == input.source)
							{
								pSkin->pJointsName = pSkin->vSources[j];
								break;
							}
						}
					else if (input.semantic == ES_WEIGHT)
						for (size_t j = 0; j < pSkin->vSources.size(); ++j)
						{
							if (pSkin->vSources[j]->sId == input.source)
							{
								pSkin->pJointsWeight = pSkin->vSources[j];
								break;
							}
						}
					pSkin->vVertexWeightInputs.push_back(input);
				}
				else IS_Element("vcount")
				{
					_SafeDeleteArr(pSkin->pVCountData);
					pSkin->pVCountData = new DaeSkin::stJointData[pSkin->iVertexWeightsCount];
					//我们要统计v里面的数据个数，所以并不直接调用str_to_int32_array
					const char* pt = pXmlElement->GetText();
					char tempValue[32];
					uint8 tempValueIndex = 0; //for tempValue
					uint32 valueIndex = 0; //for value
					while (pt != nullptr && *pt != '\0')
					{
						tempValue[tempValueIndex] = *pt;
						if (*(++pt) == ' ')
						{
							++tempValueIndex;
							tempValue[tempValueIndex] = '\0';
							pSkin->pVCountData[valueIndex].count = atoi(tempValue);
							pSkin->pVCountData[valueIndex].index = i_VCount;
							i_VCount += pSkin->pVCountData[valueIndex].count * pSkin->vVertexWeightInputs.size();
							tempValueIndex = 0;
							++valueIndex;
							++pt;
						}
						else
							++tempValueIndex;
					}
					tempValue[tempValueIndex] = '\0';
					pSkin->pVCountData[valueIndex].count = atoi(tempValue);
					pSkin->pVCountData[valueIndex].index = i_VCount;
					i_VCount += pSkin->pVCountData[valueIndex].count * pSkin->vVertexWeightInputs.size();
				}
				else IS_Element("v")
				{
					pSkin->pData = new int32[i_VCount];
					str_to_int32_array(pXmlElement->GetText(),&pSkin->pData);
				}
				pXmlVertexChildNode = pXmlVertexChildNode->NextSibling();
			}
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in skin!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pSkin;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_visual_scenes(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryVisualScenes* pLibraryVisualScenes = new DaeLibraryVisualScenes;
	father->pLibraryVisualScenes = pLibraryVisualScenes;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_visual_scene)
			parse_visual_scene(pXmlChildNode, pLibraryVisualScenes);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_visual_scenes!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryVisualScenes;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_visual_scene(TiXmlNode* pXmlNode, DaeLibraryVisualScenes* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeVisualScene* pVisualScene = new DaeVisualScene;
	father->vecData.push_back(pVisualScene);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pVisualScene->sId = pXmlAttribute->Value();
		else IS_Attribute("name")
			pVisualScene->sName = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in visual_scene!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("node")
		{
			DaeNode* pNode = parse_node(pXmlChildNode);
			if (pNode->eNodeType == ENT_JOINT)
				pVisualScene->vJointsSystem.push_back(pNode);
			else if (pNode->eNodeType == ENT_NODE)
			{
				pVisualScene->vMeshs.push_back(pNode);
			}
			else
			{
				delete pNode;
			}
				
		}
		else IS_Element("extra")
			getLog()->LogLine(log_ok, "dae file skip extra under visual_scene.", pXmlElement->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in visual_scene!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pVisualScene;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_scene(TiXmlNode* pXmlNode, DaeCollada* father)
{
	return nullptr;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_animations(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryAnimations* pLibraryAnimations = new DaeLibraryAnimations;
	father->pLibraryAnimations = pLibraryAnimations;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("animation")
			parse_animation(pXmlChildNode, pLibraryAnimations);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under library_animations!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryAnimations;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_animation(TiXmlNode* pXmlNode, DaeLibraryAnimations* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeAnimation* pAnimation = new DaeAnimation;
	father->vecData.push_back(pAnimation);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pAnimation->sId = pXmlAttribute->Value();
		else IS_Attribute("name")
			pAnimation->sName = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s under animation!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//根据目前的dae格式来看，每一个animation具体的数据都在更下一级的animation里面，所以要再下一级
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();//child animation
	if (pXmlChildNode != nullptr)
	{//if is nullptr,means this joint has no key frame
		pXmlChildNode = pXmlChildNode->FirstChild();
		while (pXmlChildNode != nullptr)
		{
			pXmlElement = pXmlChildNode->ToElement();
			IS_Element("source")
				pAnimation->vSources.push_back(parse_source(pXmlChildNode));
			else IS_Element("sampler")
			{
				TiXmlNode* pXmlInputNode = pXmlChildNode->FirstChild();
				while (pXmlInputNode != nullptr)
				{
					stDaeInput input;
					parse_input(pXmlInputNode->ToElement(), input);
					pAnimation->vSamplerInputs.push_back(input);
					if (input.semantic == ES_INPUT)
						for (size_t i = 0; i < pAnimation->vSources.size(); ++i)
						{
							if (input.source == pAnimation->vSources[i]->sId)
							{
								pAnimation->pInput = pAnimation->vSources[i];
								break;
							}
						}
					else if (input.semantic == ES_OUTPUT)
						for (size_t i = 0; i < pAnimation->vSources.size(); ++i)
						{
							if (input.source == pAnimation->vSources[i]->sId)
							{
								pAnimation->pOutput = pAnimation->vSources[i];
								break;
							}
						}
					pXmlInputNode = pXmlInputNode->NextSibling();
				}
			}
			else IS_Element("channel")
			{//dont deal temporarily
			}
			else
				getLog()->LogLine(log_allert, "dae file unknown child:%s under animation!", pXmlElement->Value());
				pXmlChildNode = pXmlChildNode->NextSibling();
			}
	}
	
	return pAnimation;
}

DexModelDaeLoader::DaeEffectProfile* DexModelDaeLoader::parse_effect_profile(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeEffectProfile* pEffectProfile = new DaeEffectProfile;
	DaeNewParam* pNewParam = nullptr; 
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("technique")
			pEffectProfile->pTechnique = parse_technique(pXmlChildNode);
		else IS_Element("newparam")
		{
			pNewParam = parse_newparam(pXmlChildNode);
			if (pNewParam != nullptr)
				pEffectProfile->vecNewParams.push_back(pNewParam);
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in profile_COMMON!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pEffectProfile;
}

DexModelDaeLoader::DaeNewParam* DexModelDaeLoader::parse_newparam(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeNewParam* pNewParam = nullptr;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	const char* pId = nullptr;
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("sid")
			pId = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in newparam!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("sampler2D")
			pNewParam = parse_newparam_sampler2D(pXmlChildNode);
		else IS_Element("surface")
			pNewParam = parse_newparam_surface(pXmlChildNode);
		else
		{ }
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	if (pNewParam != nullptr)
		pNewParam->sId = pId;
	return pNewParam;
}

DexModelDaeLoader::DaeNewParam* DexModelDaeLoader::parse_newparam_surface(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeNewParamSurface* pSurface = new DaeNewParamSurface;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("type")
			pSurface->sSurfaceType = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in newparam surface!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("init_from")
			pSurface->sInitFrom = pXmlElement->GetText();
		else IS_Element("format")
			pSurface->sFormat = pXmlElement->GetText();
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under newparam surface!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pSurface;
}

DexModelDaeLoader::DaeNewParam* DexModelDaeLoader::parse_newparam_sampler2D(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeNewParamSampler2D* pSampler2D = new DaeNewParamSampler2D;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("source")
			pSampler2D->sSource = pXmlElement->GetText();
		else IS_Element("wrap_s")
			pSampler2D->sWrapS = pXmlElement->GetText();
		else IS_Element("wrap_t")
			pSampler2D->sWrapT = pXmlElement->GetText();
		else IS_Element("minfilter")
			pSampler2D->sMinFilter = pXmlElement->GetText();
		else IS_Element("magfilter")
			pSampler2D->sMagFilter = pXmlElement->GetText();
		else IS_Element("mipfilter")
			pSampler2D->sMipFilter = pXmlElement->GetText();
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under newparam sampler2D!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pSampler2D;
}

DexModelDaeLoader::DaePhong* DexModelDaeLoader::parse_phong(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaePhong* pPhong = new DaePhong;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("emission")
			parse_emission(pXmlChildNode, pPhong->colorEmission);
		else IS_Element("ambient")
			parse_ambient(pXmlChildNode, pPhong->colorAmbient);
		else IS_Element("diffuse")
			pPhong->pDiffuse = parse_diffuse(pXmlChildNode, pPhong);
		else IS_Element("specular")
			parse_specular(pXmlChildNode, pPhong->colorSpecular);
		else IS_Element("shininess")
			parse_shininess(pXmlChildNode, pPhong->fShininess);
		else IS_Element("reflective")
			parse_reflective(pXmlChildNode, pPhong->colorReflective);
		else IS_Element("reflectivity")
			parse_reflectivity(pXmlChildNode, pPhong->fReflectivity);
		else IS_Element("transparent")
			parse_transparent(pXmlChildNode, pPhong->colorTransparent);
		else IS_Element("transparency")
			parse_transparency(pXmlChildNode, pPhong->fTransparency);
		else IS_Element("index_of_refraction")
			parse_index_of_refraction(pXmlChildNode, pPhong->fIndexOfRefraction);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in phong!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pPhong;
}

DexModelDaeLoader::DaeTechnique* DexModelDaeLoader::parse_technique(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeTechnique* pTechnique = new DaeTechnique;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析technique的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("sid")
			pTechnique->sid = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in technique!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("phong")
			pTechnique->pPhong = parse_phong(pXmlChildNode);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in technique!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pTechnique;
}

void DexModelDaeLoader::parse_color(TiXmlNode* pXmlNode, DexColor& color)
{
	/*针对形如以下的color element
	<emission>
		<color> 0 0 0 1 </color>
	</emission>
	*/
	DEX_ENSURE(pXmlNode);
	TiXmlElement* pXmlNodeElement = pXmlNode->ToElement();
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("color")
		{
			const char* colorValue = pXmlElement->GetText();
			DVector<DString> out_str;
			SplitStr(colorValue, ' ', out_str);
			color.r = atof(out_str[0].c_str()) * 255;
			color.g = atof(out_str[1].c_str()) * 255;
			color.b = atof(out_str[2].c_str()) * 255;
			color.a = atof(out_str[3].c_str()) * 255;
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in %s!", pXmlElement->Value(), pXmlNodeElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
}
void DexModelDaeLoader::parse_emission(TiXmlNode* pXmlNode, DexColor& color)
{
	parse_color(pXmlNode, color);
}

void DexModelDaeLoader::parse_ambient(TiXmlNode* pXmlNode, DexColor& color)
{
	parse_color(pXmlNode, color);
}

DexModelDaeLoader::DaeDiffuse* DexModelDaeLoader::parse_diffuse(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeDiffuse* pDiffuse = new DaeDiffuse;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("texture")
		{
			TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("texture")
					pDiffuse->sTexture = pXmlAttribute->Value();
				else IS_Attribute("texcoord")
					pDiffuse->sTexcoord = pXmlAttribute->Value();
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in texture!", pXmlElement->Value());
				pXmlAttribute = pXmlAttribute->Next();
			}
		}
		else IS_Element("color")
		{
			if (pDiffuse->sTexture == "")
			{//只对于没有贴图的有效
				const char* colorValue = pXmlElement->GetText();
				DVector<DString> out_str;
				SplitStr(colorValue, ' ', out_str);
				pDiffuse->color.r = atof(out_str[0].c_str()) * 255;
				pDiffuse->color.g = atof(out_str[1].c_str()) * 255;
				pDiffuse->color.b = atof(out_str[2].c_str()) * 255;
				pDiffuse->color.a = atof(out_str[3].c_str()) * 255;
			}

		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in diffuse!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pDiffuse;
}

void DexModelDaeLoader::parse_specular(TiXmlNode* pXmlNode, DexColor& color)
{
	parse_color(pXmlNode, color);
}
void DexModelDaeLoader::parse_float(TiXmlNode* pXmlNode, float32& fValue)
{
	/*针对形如以下的float element
	<shininess>
		<float sid="shininess">10</float>
	</shininess>
	*/
	DEX_ENSURE(pXmlNode);
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlNodeElement = pXmlNode->ToElement();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("float")
		{
			const char* value = pXmlElement->GetText();
			fValue = atof(value);
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in %s!", pXmlElement->Value(), pXmlNodeElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
}
void DexModelDaeLoader::parse_shininess(TiXmlNode* pXmlNode, float32& fShininess)
{
	parse_float(pXmlNode, fShininess);
}

void DexModelDaeLoader::parse_reflective(TiXmlNode* pXmlNode, DexColor& color)
{
	parse_color(pXmlNode, color);
}

void DexModelDaeLoader::parse_reflectivity(TiXmlNode* pXmlNode, float32& fValue)
{
	parse_float(pXmlNode, fValue);
}

void DexModelDaeLoader::parse_transparent(TiXmlNode* pXmlNode, DexColor& color)
{
	parse_color(pXmlNode, color);
}

void DexModelDaeLoader::parse_transparency(TiXmlNode* pXmlNode, float32& fValue)
{
	parse_float(pXmlNode, fValue);
}

void DexModelDaeLoader::parse_index_of_refraction(TiXmlNode* pXmlNode, float32& fValue)
{
	parse_float(pXmlNode, fValue);
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_mesh(TiXmlNode* pXmlNode, DaeGeometry* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeMesh* pMesh = new DaeMesh;
	if (father->pMesh != nullptr)
	{
		getLog()->LogLine(log_allert, "dae file find multy mesh under geometry!");
		delete father->pMesh;
	}
	father->pMesh = pMesh;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_source)
			pMesh->vecSources.push_back(parse_source(pXmlChildNode));
		else IS_Element(g_vertices)
			parse_vertices(pXmlChildNode, pMesh);
		else IS_Element(g_triangles)
			parse_triangle(pXmlChildNode, pMesh);
		else IS_Element(g_polylist)
			parse_polylist(pXmlChildNode, pMesh);
		else IS_Element(g_polygons)
			parse_polygons(pXmlChildNode, pMesh);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in mesh!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pMesh;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_vertices(TiXmlNode* pXmlNode, DaeMesh* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeVertices* pVertices = new DaeVertices;
	if (father->pVerticles != nullptr)
	{
		getLog()->LogLine(log_allert, "dae file find multy verticles in mesh!");
		delete father->pVerticles;
	}
	father->pVerticles = pVertices;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析id attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pVertices->id = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in vertices!", pXmlElement->Value());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//解析Input
	stDaeInput input;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			parse_input(pXmlElement, input);
			pVertices->vInputs.push_back(input);
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under vertices!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pVertices;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_triangle(TiXmlNode* pXmlNode, DaeMesh* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeTriangles* pTriangle = new DaeTriangles;
	father->vecTriangles.push_back(pTriangle);
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("count")
			pTriangle->iCount = atoi(pXmlAttribute->Value());
		else IS_Attribute("material")
			pTriangle->material = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in triangle!", pXmlElement->Value());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//解析input & p
	uint8 real_input_counts = 0;
	uint8 triangle_input_count = 0;
	uint8 vertex_index = 0;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			stDaeInput input;
			parse_input(pXmlElement, input);
			if (input.semantic == ES_VERTEX)
			{//如果是一个VERTEX，则这个input不算，到vertices中去找具体的input
				DaeVertices* pVertices = father->pVerticles;
				int32 iVerticesInputSize = pVertices->vInputs.size();
				for (size_t i = 0; i < iVerticesInputSize; ++i)
					pTriangle->vInputs.push_back(pVertices->vInputs[i]);
				real_input_counts += iVerticesInputSize;
				vertex_index = triangle_input_count;
				++triangle_input_count;

			}
			else
			{
				pTriangle->vInputs.push_back(input);
				++real_input_counts;
				++triangle_input_count;
			}
		}
		else IS_Element("p")
		{
			pTriangle->pData = new int32[real_input_counts * pTriangle->iCount * 3];
			const char* value = pXmlElement->GetText();
			uint8 flag = 1;
			if (father->pVerticles != nullptr)
				flag = father->pVerticles->vInputs.size();
			str_to_int32_array(value, &pTriangle->pData, triangle_input_count, vertex_index, flag);
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under triangle!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pTriangle;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_polylist(TiXmlNode* pXmlNode, DaeMesh* father)
{
/*目前只支持<vcount>里面参数为3或者4的polylist，即只支持3边形和4边形
  4边分解为两个三边形
  */
	DEX_ENSURE_P(pXmlNode);
	DaePolylist* pPolylist = new DaePolylist;
	father->vecPolylists.push_back(pPolylist);
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("count")
			pPolylist->count = atoi(pXmlAttribute->Value());
		else IS_Attribute("material")
			pPolylist->material = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in polylist!", pXmlElement->Value());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//解析input & vcount & p
	uint32 p_pCount = 0;
	uint8  real_input_counts = 0;
	uint8  polylist_input_count = 0;
	uint8  vertex_index = 0;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			stDaeInput input;
			parse_input(pXmlElement, input);
			if (input.semantic == ES_VERTEX)
			{//如果是一个VERTEX，则这个input不算，到vertices中去找具体的input
				DaeVertices* pVertices = father->pVerticles;
				int32 iVerticesInputSize = pVertices->vInputs.size();
				for (size_t i = 0; i < iVerticesInputSize; ++i)
					pPolylist->vInputs.push_back(pVertices->vInputs[i]);
				real_input_counts += iVerticesInputSize;
				vertex_index = polylist_input_count;
				++polylist_input_count;
			}
			else
			{
				pPolylist->vInputs.push_back(input);
				++real_input_counts;
				++polylist_input_count;
			}
		}
		else IS_Element("vcount")
		{
			pPolylist->p_vcountData = new int32[pPolylist->count];
			//这里因为我们要记录总共的顶点数量所以就没有调用str_to_int32_array
			const char* pt = pXmlElement->GetText();
			char tempValue[32];
			uint8 tempValueIndex = 0; //for tempValue
			uint32 valueIndex = 0; //for value
			while (pt != nullptr && *pt != '\0')
			{
				tempValue[tempValueIndex] = *pt;
				if (*(++pt) == ' ')
				{
					++tempValueIndex;
					tempValue[tempValueIndex] = '\0';
					pPolylist->p_vcountData[valueIndex] = atoi(tempValue);
					p_pCount += pPolylist->p_vcountData[valueIndex] * real_input_counts;//一个多边形有几个顶点，一个顶点占几个input
					if (pPolylist->p_vcountData[valueIndex] != 3 && pPolylist->p_vcountData[valueIndex] != 4)
						getLog()->LogLine(log_allert, "dae polylist 有不支持的%d边形！", pPolylist->p_vcountData[valueIndex]);
					tempValueIndex = 0;
					++valueIndex;
					++pt;
				}
				else
					++tempValueIndex;
			}
			tempValue[tempValueIndex] = '\0';
			pPolylist->p_vcountData[valueIndex] = atoi(tempValue);
			p_pCount += pPolylist->p_vcountData[valueIndex] * real_input_counts;
		}
		else IS_Element("p")
		{
			pPolylist->pData = new int32[p_pCount];
			const char* value = pXmlElement->GetText();
			uint8 flag = 1;
			if (father->pVerticles != nullptr)
				flag = father->pVerticles->vInputs.size();
			str_to_int32_array(value, &pPolylist->pData, polylist_input_count,vertex_index, flag);
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under polylist!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pPolylist;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_polygons(TiXmlNode* pXmlNode, DaeMesh* father)
{
	/*目前只支持<p>里面有3个或者4个顶点数据的多边形，即只支持3边形和4边形
	4边分解为两个三边形
	*/
	DEX_ENSURE_P(pXmlNode);
	DaePolygons* pPolygons = new DaePolygons;
	father->vecPolygons.push_back(pPolygons);
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//解析attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("count")
			pPolygons->count = atoi(pXmlAttribute->Value());
		else IS_Attribute("material")
			pPolygons->material = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in polylist!", pXmlElement->Value());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//解析input & p
	bool beginParseP = false;
	uint32 pIndex = 0;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			stDaeInput input;
			parse_input(pXmlElement, input);
			pPolygons->inputs.push_back(input);
		}
		else IS_Element("p")
		{
			if (!beginParseP)
			{//第一个次开始解析<p>数据
				pPolygons->pData = new DaePolygons::stPData[pPolygons->count];
				beginParseP = true;
			}
			DaePolygons::stPData data;
			const char* pt = pXmlElement->GetText();
			char tempValue[32];
			uint8 tempValueIndex = 0; //for tempValue
			uint32 valueIndex = 0; //for value
			while (pt != nullptr && *pt != '\0')
			{
				tempValue[tempValueIndex] = *pt;
				if (*(++pt) == ' ')
				{
					++tempValueIndex;
					tempValue[tempValueIndex] = '\0';
					data.pData[valueIndex] = atoi(tempValue);
					tempValueIndex = 0;
					++valueIndex;
					++pt;
				}
				else
					++tempValueIndex;
			}
			tempValue[tempValueIndex] = '\0';
			data.pData[valueIndex] = atoi(tempValue);
			data.count = (valueIndex+1) / pPolygons->inputs.size();
			if (data.count != 3 && data.count != 4)
				getLog()->LogLine(log_allert, "dae file unsupported polygons edge:%d !", data.count);
			else
			{
				pPolygons->pData[pIndex] = data;
				++pIndex;
			}
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s under polylist!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pPolygons;
}
DexModelDaeLoader::DaeNode* DexModelDaeLoader::parse_node(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeNode* pNode = new DaeNode;
	pNode->eNodeType = ENT_UNKNOWN;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
		{
			if (m_bFFXIIModel)
			{//ffxii的dae 会有一个层级前缀，这是转换过程加上去的，原始的数据并没有这个前缀，.act用的也是原始名称
				uint16 index = 0;
				const char* pData = pXmlAttribute->Value();
				while (1)
				{
					if (pData[index++] == '_')
					{
						break;
					}
				}
				pNode->sId = &(pXmlAttribute->Value()[index]);
			}
			else
				pNode->sId = pXmlAttribute->Value();
		}
		else IS_Attribute("name")
		{
			if (m_bFFXIIModel)
			{//ffxii的dae 会有一个层级前缀，这是转换过程加上去的，原始的数据并没有这个前缀，.act用的也是原始名称
				uint16 index = 0;
				const char* pData = pXmlAttribute->Value();
				while (1)
				{
					if (pData[index++] == '_')
					{
						break;
					}
				}
				pNode->sName = &(pXmlAttribute->Value()[index]);
			}
			else
				pNode->sName = pXmlAttribute->Value();
		}
		else IS_Attribute("sid")
			pNode->sSId = pXmlAttribute->Value();
		else IS_Attribute("type")
		{
			if (strcmp(pXmlAttribute->Value(), "JOINT") == 0)
			{
				pNode->eNodeType = ENT_JOINT;
				if (pNode->sId != "")
				{
					m_MapJointName[pNode->sSId] = pNode->sId;
				}
			}
			else if (strcmp(pXmlAttribute->Value(), "NODE") == 0)
				pNode->eNodeType = ENT_NODE;
			else 
				getLog()->LogLine(log_allert, "dae file unknown node type:%s !", pXmlAttribute->Value());
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in node!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	float64* pFloatValue = new float64[16];
	DexMatrix4x4 tempMatrix;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("node")
			pNode->vNodeChildren.push_back(parse_node(pXmlChildNode));
		else IS_Element("translate")
		{
			str_to_float64_array(pXmlElement->GetText(),pFloatValue);
			pNode->mMatrix.Translate(pFloatValue[0], pFloatValue[1],pFloatValue[2]);
		}
		else IS_Element("rotate")
		{
			str_to_float64_array(pXmlElement->GetText(), pFloatValue);
			DexQuaternion quaternion;
			quaternion.Set(DexVector3(pFloatValue[0], pFloatValue[1], pFloatValue[2]), pFloatValue[3]);
			tempMatrix = quaternion.GetMatrix();
			//因为这些rotate都是在自身local坐标系进行旋转的，所以不能后操作，要先乘
			//先乘就相当于物体在原点先rotate再translate
			pNode->mMatrix = tempMatrix *pNode->mMatrix;
		}
		else IS_Element("scale")
		{
			str_to_float64_array(pXmlElement->GetText(), pFloatValue);
			pNode->mMatrix.Scale(DexVector3(pFloatValue[0], pFloatValue[1], pFloatValue[2]));
		}
		else IS_Element("matrix")
		{
			str_to_float64_array(pXmlElement->GetText(), pFloatValue);
			for (uint16 i = 0; i < 16; ++i)
				pNode->mMatrix.m[i] = pFloatValue[i];
			pNode->mMatrix.MakeTranspose(); //如果node下面只有matrix的话，matrix好像必须要转置一下？
		}
		else IS_Element("instance_geometry")
		{
			pNode->eNodeType = ENT_NODE;
			if (pNode->pInstanceMesh != nullptr)
			{
				getLog()->LogLine(log_allert, "dae file find multy instancemesh child under node!");
				delete pNode->pInstanceMesh;
			}
			pNode->pInstanceMesh = parse_instance_geometry(pXmlChildNode);
		}
		else IS_Element("instance_controller")
		{
			pNode->eNodeType = ENT_NODE;
			if (pNode->pInstanceMesh != nullptr)
			{
				getLog()->LogLine(log_allert, "dae file find multy instancemesh child under node!");
				delete pNode->pInstanceMesh;
			}
			pNode->pInstanceMesh = parse_instance_controller(pXmlChildNode);
		}
		else IS_Element("extra")
		{
		}
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in node!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	delete[]pFloatValue;
	return pNode;
}

DexModelDaeLoader::DaeInstanceGeometry* DexModelDaeLoader::parse_instance_geometry(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeInstanceGeometry* pInstanceGeometry = new DaeInstanceGeometry;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("url")
			pInstanceGeometry->sUrl = &pXmlAttribute->Value()[1]; //skip#
		else IS_Attribute("name")
			pInstanceGeometry->sName = pXmlAttribute->Value();
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in instance_geometry!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	stInstanceMaterial instanceMaterial;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("bind_material")
		{
			TiXmlNode* pXmlTechniqueCommonChildNode = pXmlChildNode->FirstChild();//应该是technique_common
			pXmlElement = pXmlTechniqueCommonChildNode->ToElement();
			IS_Element("technique_common")
			{
				TiXmlNode* pXmlInstanceMaterialChildNode = pXmlTechniqueCommonChildNode->FirstChild();//应该是instance_material
				while (pXmlInstanceMaterialChildNode != nullptr)
				{
					pXmlElement = pXmlInstanceMaterialChildNode->ToElement();
					IS_Element("instance_material")
					{
						parse_instance_material(pXmlElement, instanceMaterial);
						pInstanceGeometry->vecMaterials.push_back(instanceMaterial);
					}
					else
						getLog()->LogLine(log_allert, "dae file unknown child %! under technique_common!", pXmlElement->Value());
					pXmlInstanceMaterialChildNode = pXmlInstanceMaterialChildNode->NextSibling();
				}
			}
			else
				getLog()->LogLine(log_allert, "dae file bind_material has unknown child %!", pXmlElement->Value());
		}
		else IS_Element("extra")
		{
			getLog()->LogLine(log_ok, "dae file skip extra under instance_geometry.", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in instance_geometry!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pInstanceGeometry;
}

DexModelDaeLoader::DaeInstanceController* DexModelDaeLoader::parse_instance_controller(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
	DaeInstanceController* pInstanceController = new DaeInstanceController;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("url")
			pInstanceController->sUrl = &pXmlAttribute->Value()[1]; //skip#
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in instance_controller!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	stInstanceMaterial instanceMaterial;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("bind_material")
		{
			TiXmlNode* pXmlTechniqueCommonChildNode = pXmlChildNode->FirstChild();//应该是technique_common
			pXmlElement = pXmlTechniqueCommonChildNode->ToElement();
			IS_Element("technique_common")
			{
				TiXmlNode* pXmlInstanceMaterialChildNode = pXmlTechniqueCommonChildNode->FirstChild();//应该是instance_material
				while (pXmlInstanceMaterialChildNode != nullptr)
				{
					pXmlElement = pXmlInstanceMaterialChildNode->ToElement();
					IS_Element("instance_material")
					{
						parse_instance_material(pXmlElement, instanceMaterial);
						pInstanceController->vecMaterials.push_back(instanceMaterial);
					}
					else
						getLog()->LogLine(log_allert, "dae file unknown child %! under technique_common!", pXmlElement->Value());
					pXmlInstanceMaterialChildNode = pXmlInstanceMaterialChildNode->NextSibling();
				}
			}
			else
				getLog()->LogLine(log_allert, "dae file bind_material has unknown child %!", pXmlElement->Value());
		}
		else IS_Element("skeleton")
			pInstanceController->vecSkeletons.push_back(&pXmlElement->GetText()[1]);
		else IS_Element("extra")
			getLog()->LogLine(log_ok, "dae file skip extra under instance_controller.", pXmlElement->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in instance_controller!", pXmlElement->Value());
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pInstanceController;
}
DexModelDaeLoader::stDaeInput& DexModelDaeLoader::parse_input(TiXmlElement* pXmlElement, stDaeInput& input)
{
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("semantic")
		{
			if (strcmp(pXmlAttribute->Value(), "VERTEX") == 0)
				input.semantic = ES_VERTEX;
			else if (strcmp(pXmlAttribute->Value(), "POSITION") == 0)
				input.semantic = ES_POSITION;
			else if (strcmp(pXmlAttribute->Value(), "NORMAL") == 0)
				input.semantic = ES_NORMAL;
			else if (strcmp(pXmlAttribute->Value(), "TEXCOORD") == 0)
				input.semantic = ES_TEXCOORD;
			else if (strcmp(pXmlAttribute->Value(), "TEXTANGENT") == 0)
				input.semantic = ES_TEXTANGENT;
			else if (strcmp(pXmlAttribute->Value(), "TEXBINORMAL") == 0)
				input.semantic = ES_TEXBINORMAL;
			else if (strcmp(pXmlAttribute->Value(), "COLOR") == 0)
				input.semantic = ES_COLOR;
			else if (strcmp(pXmlAttribute->Value(), "JOINT") == 0)
				input.semantic = ES_JOINT;
			else if (strcmp(pXmlAttribute->Value(), "WEIGHT") == 0)
				input.semantic = ES_WEIGHT;
			else if (strcmp(pXmlAttribute->Value(), "INV_BIND_MATRIX") == 0)
				input.semantic = ES_INV_BIND_MATRIX;
			else if (strcmp(pXmlAttribute->Value(), "INPUT") == 0)
				input.semantic = ES_INPUT;
			else if (strcmp(pXmlAttribute->Value(), "OUTPUT") == 0)
				input.semantic = ES_OUTPUT;
			else if (strcmp(pXmlAttribute->Value(), "INTERPOLATION") == 0)
				input.semantic = ES_INTERPOLATION;
			else
			{
				input.semantic = ES_UNKNOWN;
				getLog()->LogLine(log_allert, "dae file unknown semantic:%s in input!", pXmlElement->Value());
			}
		}
		else IS_Attribute("source")
			input.source = &(pXmlAttribute->Value()[1]);//跳过#
		else IS_Attribute("offset")
			input.offset = atoi(pXmlAttribute->Value());
		else IS_Attribute("set")
			input.set = atoi(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in input!", pXmlElement->Value());
		pXmlAttribute = pXmlAttribute->Next();
	}
	return input;
}

DexModelDaeLoader::stInstanceMaterial& DexModelDaeLoader::parse_instance_material(TiXmlElement* pXmlElement, stInstanceMaterial& instanceMaterial)
{
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("symbol")
			instanceMaterial.sSymbol = pXmlAttribute->Value();
		else IS_Attribute("target")
			instanceMaterial.sTarget = &(pXmlAttribute->Value()[1]);//skip#
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in instance_material!", pXmlElement->Value());
		pXmlAttribute = pXmlAttribute->Next();
	}
	return instanceMaterial;
}

void DexModelDaeLoader::str_to_float64_array(const char* str, float64* value, char splitChar)
{
	const char* pt = str;
	char tempValue[32];
	uint8 tempValueIndex = 0; //for tempValue
	uint32 valueIndex = 0; //for value
	DVector<float32> testVector;
	while (pt != nullptr && *pt != '\0')
	{
		tempValue[tempValueIndex] = *pt;
		if (*(++pt) == splitChar)
		{
			++tempValueIndex;
			tempValue[tempValueIndex] = '\0';
			testVector.push_back(atof(tempValue));
			value[valueIndex] = atof(tempValue);
			tempValueIndex = 0;
			++valueIndex;
			++pt;
		}
		else
			++tempValueIndex;
	}
	tempValue[tempValueIndex] = '\0';
	testVector.push_back(atof(tempValue));
	value[valueIndex] = atof(tempValue);
}
void DexModelDaeLoader::str_to_int32_array(const char* str, int32** value, uint8 cycle, uint8 index, uint8 flag)
{
	const char* pt = str;
	char tempValue[32];
	uint8 tempValueIndex = 0; //for tempValue
	uint32 valueIndex = 0; //for value
	uint32 iCycleIndex = 0;
	while (pt != nullptr && *pt != '\0')
	{
		tempValue[tempValueIndex] = *pt;
		if (*(++pt) == ' ')
		{
			++tempValueIndex;
			tempValue[tempValueIndex] = '\0';
			(*value)[valueIndex++] = atoi(tempValue);
			tempValueIndex = 0;
			++pt;
			if (iCycleIndex == index)
				for (int i = 0; i < flag - 1; ++i)
					(*value)[valueIndex++] = atoi(tempValue);
			//iCycleIndex = ++iCycleIndex == cycle ? 0 : iCycleIndex;
			if (++iCycleIndex == cycle)
				iCycleIndex = 0;
		}
		else
			++tempValueIndex;
	}
	tempValue[tempValueIndex] = '\0';
	(*value)[valueIndex++] = atoi(tempValue);
	if (iCycleIndex == index)
		for (int i = 0; i < flag - 1; ++i)
			(*value)[valueIndex++] = atoi(tempValue);
}

void DexModelDaeLoader::TransVector3ByAxis(DexVector3& vec3)
{
	DEX_ENSURE(m_pCollada != nullptr && m_pCollada->pAsset != nullptr);
	float32 fTemp = 0.0f;
	switch (m_pCollada->pAsset->eAxisUp)
	{
	case EAU_X://TODO
		break;
	case EAU_Y:
		//默认Y向上 不作转换
		break;
	case EAU_Z:
		fTemp = vec3.y;
		vec3.y = vec3.z;
		vec3.z = fTemp;
		break;
	default:
		break;
	}
}

DexModelDaeLoader::DaeImage* DexModelDaeLoader::find_image(DString sImageId)
{
	DEX_ENSURE_P(m_pCollada != nullptr && m_pCollada->pLibraryImages != nullptr);
	for (size_t i = 0; i < m_pCollada->pLibraryImages->vecData.size(); ++i)
		if (m_pCollada->pLibraryImages->vecData[i]->sId == sImageId)
			return m_pCollada->pLibraryImages->vecData[i];
	return nullptr;
}
DexModelDaeLoader::DaeMaterial* DexModelDaeLoader::find_material(DString sMaterialId)
{
	DEX_ENSURE_P(m_pCollada != nullptr && m_pCollada->pLibraryMaterials != nullptr);
	for (size_t i = 0; i < m_pCollada->pLibraryMaterials->vecData.size(); ++i)
		if (m_pCollada->pLibraryMaterials->vecData[i]->sId == sMaterialId)
			return m_pCollada->pLibraryMaterials->vecData[i];
	return nullptr;
}
DexModelDaeLoader::DaeEffect* DexModelDaeLoader::find_effect(DString sEffectId)
{
	DEX_ENSURE_P(m_pCollada != nullptr && m_pCollada->pLibraryEffects != nullptr);
	for (size_t i = 0; i < m_pCollada->pLibraryEffects->vecData.size(); ++i)
		if (m_pCollada->pLibraryEffects->vecData[i]->sId == sEffectId)
			return m_pCollada->pLibraryEffects->vecData[i];
	return nullptr;
}
DexModelDaeLoader::DaeGeometry* DexModelDaeLoader::find_geometry(DString sGeometryId)
{
	DEX_ENSURE_P(m_pCollada != nullptr && m_pCollada->pLibraryGeometries != nullptr);
	for (size_t i = 0; i < m_pCollada->pLibraryGeometries->vecData.size(); ++i)
		if (m_pCollada->pLibraryGeometries->vecData[i]->sId == sGeometryId)
			return m_pCollada->pLibraryGeometries->vecData[i];
	return nullptr;
}
DexModelDaeLoader::DaeController* DexModelDaeLoader::find_controller(DString sControllerId)
{
	DEX_ENSURE_P(m_pCollada != nullptr && m_pCollada->pLibraryControllers != nullptr);
	for (size_t i = 0; i < m_pCollada->pLibraryControllers->vecData.size(); ++i)
		if (m_pCollada->pLibraryControllers->vecData[i]->sId == sControllerId)
			return m_pCollada->pLibraryControllers->vecData[i];
	return nullptr;
}
DexModelDaeLoader::DaeSource* DexModelDaeLoader::find_source(DVector<DaeSource*>& vData, DString sId)
{
	for (size_t i = 0; i < vData.size(); ++i)
		if (vData[i]->sId == sId)
			return vData[i];
	return nullptr;
}
DexModelDaeLoader::DaeNode* DexModelDaeLoader::find_joint(DString jointId)
{
	DEX_ENSURE_P(m_pCollada != nullptr && m_pCollada->pLibraryVisualScenes != nullptr);
	DEX_ENSURE_P(m_pCollada->pLibraryVisualScenes->vecData.size() == 1);
	DaeVisualScene* pVisualScene = m_pCollada->pLibraryVisualScenes->vecData[0];
	DEX_ENSURE_P(pVisualScene != nullptr);
	DaeNode* pRet = nullptr;
	for (size_t i = 0; i < pVisualScene->vJointsSystem.size(); ++i)
	{
		pRet = find_joint(pVisualScene->vJointsSystem[i], jointId);
		if (pRet != nullptr)
			break;
	}
	return pRet;
}
DexModelDaeLoader::DaeNode* DexModelDaeLoader::find_joint(DaeNode* pNode, DString jointId)
{
	if (pNode->sId == jointId)
		return pNode;
	DaeNode* pRet = nullptr;
	for (size_t i = 0; i < pNode->vNodeChildren.size(); ++i)
	{
		pRet = find_joint(pNode->vNodeChildren[i], jointId);
		if (pRet != nullptr)
			break;
	}
	return pRet;
}
DexSkinMesh* DexModelDaeLoader::create_SkinMeshStatic(DaeNode* pStaticMeshNode)
{
	DexSkinMesh* pDexSkinMesh = new DexSkinMesh();
	DexSkinMesh::DexMesh* pDexMesh = NULL;
	DaeInstanceGeometry* pInstanceGeometry = (DaeInstanceGeometry*)pStaticMeshNode->pInstanceMesh;
	//DaeInstanceGeometry中指明的materials就是这个skinmesh需要用到的所有material
	//处理materials
	DVector<DString> vMaterials;
	DVector<DString> vImages;
	deal_with_material_texture(pInstanceGeometry->vecMaterials, pDexSkinMesh, vMaterials, vImages);
	DaeGeometry* pGeometry = find_geometry(pInstanceGeometry->sUrl);
	if (pGeometry != nullptr)
	{
		DaeMesh* pMesh = pGeometry->pMesh;
		stIndex index;
		DaeSource* pSourcePos = nullptr;
		DaeSource* pSourceNormal = nullptr;
		DaeSource* pSourceUv = nullptr;
		DaeSource* pSourceTangent = nullptr; //暂时不支持
		DaeSource* pSourceBinormal = nullptr; //暂时不支持
		DaeSource* pSourceColor = nullptr;
		DMap<stIndex, int32> mapVertexInfo;
		if (pMesh != nullptr)
		{
			for (size_t iTriangleIndex = 0; iTriangleIndex < pMesh->vecTriangles.size(); ++iTriangleIndex)
			{
				mapVertexInfo.clear();
				DaeTriangles* pTriangle = pMesh->vecTriangles[iTriangleIndex];
				int32 iInputSize = pTriangle->vInputs.size();
				pDexMesh = pDexSkinMesh->AddMesh(pTriangle->material.c_str());//暂时用meterial的名字来命名
				//处理material
				for (size_t i = 0; i < vMaterials.size(); ++i)
				{
					if (vMaterials[i] == pTriangle->material)
					{
						pDexMesh->m_iMaterialId = pDexSkinMesh->FindMaterial(pTriangle->material);
						pDexMesh->m_iTextureId = pDexMesh->m_iMaterialId;
					}
				}
				//对应pos normal uv 等source
				for (size_t i = 0; i < pTriangle->vInputs.size(); ++i)
				{
					stDaeInput* pInput = &pTriangle->vInputs[i];
					switch (pInput->semantic)
					{
					case ES_POSITION:
						pSourcePos = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_NORMAL:
						pSourceNormal = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_TEXCOORD:
						pSourceUv = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_TEXTANGENT:
						pSourceTangent = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_TEXBINORMAL:
						pSourceBinormal = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_COLOR:
						pSourceColor = find_source(pMesh->vecSources, pInput->source);
						break;
					default:
						break;
					}
				}
				uint32 iValueIndex = 0;
				for (int32 i = 0; i < pTriangle->iCount; ++i)
				{//一次性读取3个顶点
					for (int32 j = 0; j < 3; ++j)
					{//每个顶点占据iInputSize个数据
						//默认input顺序是VERTEX NORMAL TEXCOORD TEXTANGENT TEXBINORMAL COLOR
						if (iInputSize > 0)
							index.m_iPosIndex = pTriangle->pData[iValueIndex++];
						if (iInputSize > 1)
							index.m_iNormalIndex = pTriangle->pData[iValueIndex++];
						if (iInputSize > 2)
							index.m_iUvIndex = pTriangle->pData[iValueIndex++];
						if (iInputSize > 3)
							iValueIndex++;//tangent
						if (iInputSize > 4)
							iValueIndex++;//binormal
						if (iInputSize > 5)
							index.m_iColorIndex = pTriangle->pData[iValueIndex++];
						map<stIndex, int32>::iterator ite = mapVertexInfo.find(index);
						if (ite != mapVertexInfo.end())
							pDexMesh->AddVertexIndice(ite->second);
						else
						{
							DexSkinMesh::stMeshVertex newVertex;
							if (m_bFFXIIModel)
								newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3] * 60, pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1] * 60,
								pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2] * 60);
							else
								newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3], pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1],
								pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2]);
							newVertex.normal.Set(pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3],
								pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+1], pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+2]);
							TransVector3ByAxis(newVertex.pos);
							TransVector3ByAxis(newVertex.normal);
							//newVertex.uv.x = 1.0f - newVertex.uv.x;
							//newVertex.uv.y = 1.0f - newVertex.uv.y;

							newVertex.uv.Set(pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2], pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2+1]);
							if (iInputSize > 5)
							{//有color
								uint8 r = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 0] * 255;
								uint8 g = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 1] * 255;
								uint8 b = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 2] * 255;
								uint8 a = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 3] * 255;
								newVertex.color.Set(r, g, b, a);
							}
							mapVertexInfo.insert(std::make_pair(index, pDexMesh->m_vecVertexsBuffer.size()));
							pDexMesh->AddVertexIndice(pDexMesh->m_vecVertexsBuffer.size());
							pDexMesh->m_vecVertexsBuffer.push_back(newVertex);
						}
					}

				}
			}
			//polygons todo
			//polylist
			for (size_t iPolylistIndex = 0; iPolylistIndex < pMesh->vecPolylists.size(); ++iPolylistIndex)
			{
				mapVertexInfo.clear();
				DaePolylist* pPolylist = pMesh->vecPolylists[iPolylistIndex];
				int32 iInputSize = pPolylist->vInputs.size();
				pDexMesh = pDexSkinMesh->AddMesh(pPolylist->material.c_str());//暂时用meterial的名字来命名
				//处理material
				for (size_t i = 0; i < vMaterials.size(); ++i)
				{
					if (vMaterials[i] == pPolylist->material)
					{
						pDexMesh->m_iMaterialId = pDexSkinMesh->FindMaterial(pPolylist->material);
						pDexMesh->m_iTextureId = pDexMesh->m_iMaterialId;
					}
				}
				//对应pos normal uv 等source
				for (size_t i = 0; i < pPolylist->vInputs.size(); ++i)
				{
					stDaeInput* pInput = &pPolylist->vInputs[i];
					switch (pInput->semantic)
					{
					case ES_POSITION:
						pSourcePos = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_NORMAL:
						pSourceNormal = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_TEXCOORD:
						pSourceUv = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_TEXTANGENT:
						pSourceTangent = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_TEXBINORMAL:
						pSourceBinormal = find_source(pMesh->vecSources, pInput->source);
						break;
					case ES_COLOR:
						pSourceColor = find_source(pMesh->vecSources, pInput->source);
						break;
					default:
						break;
					}
				}
				int iPos = 0;
				for (int32 i = 0; i < pPolylist->count; ++i)
				{//一次性读取iInputSize个数据
					//默认input顺序是VERTEX NORMAL TEXCOORD TEXTANGENT TEXBINORMAL COLOR
					int iVCount = pPolylist->p_vcountData[i];
					if (iVCount == 3)
					{//三角形
						for (int32 j = 0; j < 3; ++j)
						{
							if (iInputSize > 0)
								index.m_iPosIndex = pPolylist->pData[iPos++];
							if (iInputSize > 1)
								index.m_iNormalIndex = pPolylist->pData[iPos++];
							if (iInputSize > 2)
								index.m_iUvIndex = pPolylist->pData[iPos++];
							if (iInputSize > 3)
								iPos++;//tangent
							if (iInputSize > 4)
								iPos++;//binormal
							if (iInputSize > 5)
								index.m_iColorIndex = pPolylist->pData[iPos++];
							map<stIndex, int32>::iterator ite = mapVertexInfo.find(index);
							if (ite != mapVertexInfo.end())
								pDexMesh->AddVertexIndice(ite->second);
							else
							{
								DexSkinMesh::stMeshVertex newVertex;
								if (m_bFFXIIModel)
									newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3] * 60, pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1] * 60,
									pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2] * 60);
								else
									newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3], pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1],
									pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2]);
								newVertex.normal.Set(pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3], pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+1],
									pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+2]);
								TransVector3ByAxis(newVertex.pos);
								TransVector3ByAxis(newVertex.normal);

								newVertex.uv.Set(pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2], pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2+1]);
								newVertex.uv.y = 1 - newVertex.uv.y;
								//newVertex.uv.x = 1.0f - newVertex.uv.x;
								//newVertex.uv.y = 1.0f - newVertex.uv.y;
								if (iInputSize > 5)
								{//有color
									uint8 r = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 0] * 255;
									uint8 g = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 1] * 255;
									uint8 b = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 2] * 255;
									uint8 a = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 3] * 255;
									newVertex.color.Set(r, g, b, a);
								}
								mapVertexInfo.insert(std::make_pair(index, pDexMesh->m_vecVertexsBuffer.size()));
								pDexMesh->AddVertexIndice(pDexMesh->m_vecVertexsBuffer.size());
								pDexMesh->m_vecVertexsBuffer.push_back(newVertex);
							}
						}

					}
					else if (iVCount == 4)
					{//四边形
						int todo = 0; //
					}
				}
			}
		}
	}
	else
		getLog()->LogLine(log_allert, "can not find instance_geometry url %s", pInstanceGeometry->sUrl.c_str());
	return pDexSkinMesh;
}

DexSkinMesh* DexModelDaeLoader::create_SkinMeshAni(DaeNode* pAniMeshNode)
{
	DEX_ENSURE_P(pAniMeshNode);
	DexSkinMesh* pDexSkinMesh = nullptr;
	if (m_bFFXIIModel)
		pDexSkinMesh = new DexSkinMeshFF();
	else
		pDexSkinMesh = new DexSkinMesh();
	DexSkinMesh::DexMesh* pDexMesh = NULL;
	DaeInstanceController* pInstanceController = (DaeInstanceController*)pAniMeshNode->pInstanceMesh;
	//DaeInstanceController中指明的materials就是这个skinmesh需要用到的所有material
	//处理materials
	DVector<DString> vMaterials;
	DVector<DString> vImages;
	deal_with_material_texture(pInstanceController->vecMaterials, pDexSkinMesh, vMaterials, vImages);

	DaeNode* pNode = nullptr;
	

	DaeController* pController = find_controller(pInstanceController->sUrl);
	if (pController != nullptr && pController->pSkin)
	{
		/*
		controller skin中的inv_bind_matrix指的是节点在模型里面的世界矩阵的逆矩阵（行矩阵）
		要正确得到一个节点的矩阵，将inv_bind_matrix转置，得到空间逆矩阵，再求逆，得到的就是模型空间矩阵(注意，这里不是相对于父节点的相对矩阵，而是实实在在的空间矩阵,与node里面的相对矩阵不一样)
		要得到相对于父节点的相对矩阵的话，以这个空间矩阵乘以父节点的空间逆矩阵
		*/

		for (size_t i = 0; i < pController->pSkin->pJointInvMatrix->iFloatArrayCount / 16; ++i)
		{
			DexMatrix4x4 invMatrix;
			for (uint16 m = 0; m < 16; ++m)
				invMatrix.m[m] = pController->pSkin->pJointInvMatrix->pFloatArrayValues[i * 16 + m];
			invMatrix.MakeTranspose();
			m_MapJointMatrix[pController->pSkin->pJointsName->vNamaArray[i]] = invMatrix;
		}
		//joint node里面只指定了父子关系，相对矩阵并无用处，真正的节点矩阵要由skin 里面的inv_bind_matrix得到
		if (pInstanceController->vecSkeletons.size() != 1)
		{//not have skeletons,then find joint node
			//must only have one joint node,bind model to this joint
			pNode = m_pCollada->pLibraryVisualScenes->vecData[0]->vJointsSystem[0];
			add_joints(pDexSkinMesh, pNode, nullptr);
		}
		else
		{
			for (size_t i = 0; i < pInstanceController->vecSkeletons.size(); ++i)
			{
				pNode = find_joint(pInstanceController->vecSkeletons[i]);
				if (pNode != nullptr)
					add_joints(pDexSkinMesh, pNode, nullptr);
			}
		}
		//读取joint动作
		if (m_pCollada->pLibraryAnimations != nullptr)
		{
			int16 iMaxAniTime = 0;
			DaeAnimation* pAnimation = nullptr;//一个animation对应一个joint
			DexSkinMesh::Joint* pJoint = nullptr;
			DexMatrix4x4 frameMatrix;
			int32 iFrameTime = 0;
			for (size_t i = 0; i < m_pCollada->pLibraryAnimations->vecData.size(); ++i)
			{
				pAnimation = m_pCollada->pLibraryAnimations->vecData[i];
				if (pAnimation->pInput != nullptr)
				{
					pJoint = pDexSkinMesh->FindJoint(pAnimation->sName);
					if (pJoint != nullptr)
					{
						for (int32 iFrame = 0; iFrame < pAnimation->pInput->iFloatArrayCount; ++iFrame)
						{
							iFrameTime = pAnimation->pInput->pFloatArrayValues[iFrame] * 1000;//从s转为ms
							for (uint16 m = 0; m < 16; ++m)
								frameMatrix.m[m] = pAnimation->pOutput->pFloatArrayValues[iFrame * 16 + m];
							frameMatrix.MakeTranspose();
							pJoint->AddKeyFrame(iFrameTime, frameMatrix);
							if (iFrameTime > iMaxAniTime)
								iMaxAniTime = iFrameTime;
						}
					}
				}
			}
			pDexSkinMesh->SetMaxAniTime(iMaxAniTime);
		}
		DaeGeometry* pGeometry = find_geometry(pController->pSkin->sSource);
		if (pGeometry != nullptr)
		{
			DaeMesh* pMesh = pGeometry->pMesh;
			
			DaeSource* pSourcePos = nullptr;
			DaeSource* pSourceNormal = nullptr;
			DaeSource* pSourceUv = nullptr;
			DaeSource* pSourceTangent = nullptr; //暂时不支持
			DaeSource* pSourceBinormal = nullptr; //暂时不支持
			DaeSource* pSourceColor = nullptr;
			DMap<stIndex, int32> mapVertexInfo;
			if (pMesh != nullptr)
			{
				for (size_t iTriangleIndex = 0; iTriangleIndex < pMesh->vecTriangles.size(); ++iTriangleIndex)
				{
					mapVertexInfo.clear();
					DaeTriangles* pTriangle = pMesh->vecTriangles[iTriangleIndex];
					int32 iInputSize = pTriangle->vInputs.size();
					pDexMesh = pDexSkinMesh->AddMesh(pTriangle->material.c_str());//暂时用meterial的名字来命名
					//处理material
					for (size_t i = 0; i < vMaterials.size(); ++i)
					{
						if (vMaterials[i] == pTriangle->material)
						{
							stInstanceMaterial m;
							find_instance_material(pInstanceController->vecMaterials, m, pTriangle->material);
							pDexMesh->m_iMaterialId = pDexSkinMesh->FindMaterial(m.sSymbol);
							pDexMesh->m_iTextureId = pDexSkinMesh->FindTexture(m.texture);
							break;
						}
					}
					//对应pos normal uv 等source
					for (size_t i = 0; i < pTriangle->vInputs.size(); ++i)
					{
						stDaeInput* pInput = &pTriangle->vInputs[i];
						switch (pInput->semantic)
						{
						case ES_POSITION:
							pSourcePos = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_NORMAL:
							pSourceNormal = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_TEXCOORD:
							pSourceUv = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_TEXTANGENT:
							pSourceTangent = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_TEXBINORMAL:
							pSourceBinormal = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_COLOR:
							pSourceColor = find_source(pMesh->vecSources, pInput->source);
							break;
						default:
							break;
						}
					}
					uint32 iValueIndex = 0;
					uint32 iVertexWeightInputSize = pController->pSkin->vVertexWeightInputs.size();
					for (int32 i = 0; i < pTriangle->iCount; ++i)
					{//一次性读取3个顶点
						for (int32 j = 0; j < 3; ++j)
						{//每个顶点占据iInputSize个数据
							//默认input顺序是VERTEX NORMAL TEXCOORD TEXTANGENT TEXBINORMAL COLOR
							stIndex index;
							if (pSourcePos != nullptr)
								index.m_iPosIndex = pTriangle->pData[iValueIndex++];
							if (pSourceNormal != nullptr)
								index.m_iNormalIndex = pTriangle->pData[iValueIndex++];
							if (pSourceUv != nullptr)
								index.m_iUvIndex = pTriangle->pData[iValueIndex++];
							if (pSourceTangent != nullptr)
								iValueIndex++;//tangent
							if (pSourceBinormal != nullptr)
								iValueIndex++;//binormal
							if (pSourceColor != nullptr)
								index.m_iColorIndex = pTriangle->pData[iValueIndex++];
							map<stIndex, int32>::iterator ite = mapVertexInfo.find(index);
							if (ite != mapVertexInfo.end())
								pDexMesh->AddVertexIndice(ite->second);
							else
							{
								DexSkinMesh::stMeshVertex newVertex;
								if (m_bFFXIIModel)
									newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3] * 60, pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1] * 60,
									pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2] * 60);
								else
									newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3], pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1],
									pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2]);
								if (pSourceNormal != nullptr)
									newVertex.normal.Set(pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3], pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+1],
									pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+2]);
								TransVector3ByAxis(newVertex.pos);
								TransVector3ByAxis(newVertex.normal);
								if (m_bFFXIIModel)
								{//右手坐标系转为左手坐标系
									newVertex.pos.z *= -1; 
									newVertex.normal.z *= -1;
								}
								if (pSourceUv != nullptr)
									newVertex.uv.Set(pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2], pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2+1]);
								if (iVertexWeightInputSize == 2)
								{//默认vertex weight 里面是joint 的index 和 weight
									//geometry里有多少个顶点，skin vertex weight 就有多少count，
									uint32 iJointCount = pController->pSkin->pVCountData[index.m_iPosIndex].count; //该顶点受影响的关节点数量
									uint32 iJointDataIndex = pController->pSkin->pVCountData[index.m_iPosIndex].index;

									for (uint32 joint = 0; joint < iJointCount; ++joint)
									{
										//默认是index在前 weight在后
										uint32 iNameIndex = pController->pSkin->pData[iVertexWeightInputSize * joint + iJointDataIndex];
										if (iNameIndex < DexSkinMesh::sGetMaxJointCount())
										{
											uint32 iWeightIndex = pController->pSkin->pData[iVertexWeightInputSize * joint + iJointDataIndex + 1];
											DString JointName = pController->pSkin->pJointsName->vNamaArray[iNameIndex];
											int32 iJointIndex = pDexSkinMesh->FindJointIndex(JointName);
											float  fJointWeight = pController->pSkin->pJointsWeight->pFloatArrayValues[iWeightIndex];
											if (iJointIndex == -1)
											{
												DMap<DString, DString>::iterator ite = m_MapJointName.find(JointName);
												if (ite != m_MapJointName.end())
													iJointIndex = pDexSkinMesh->FindJointIndex(ite->second);
											}
											if (iJointIndex != -1)
											{
												newVertex.JointIndex[joint] = iJointIndex;
												newVertex.JointWeights[joint] = fJointWeight;
											}
										}
										else
										{//not support more than 60 joint,if more than 60, then just bind to root joint
											memset(newVertex.JointIndex, 0, sizeof(newVertex.JointIndex));
											memset(newVertex.JointWeights, 0, sizeof(newVertex.JointWeights));
											newVertex.JointWeights[0] = 1.0f;
											break;
										}
									}
								}
								else
								{
									getLog()->LogLine(log_allert, "dae file unknown skin vertex weight input count! %d", pController->pSkin->vVertexWeightInputs.size());
								}
								//一些导出工具对于uv坐标是从左到右为x ,从下到上为y，而引擎使用的dx，使用的是从左到右为x，从上到下为y
								//如果y方向不一致，就要进行转换
								newVertex.uv.y = 1 - newVertex.uv.y; 
								if (pSourceColor != nullptr)
								{//有color
									uint8 r = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 0] * 255;
									uint8 g = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 1] * 255;
									uint8 b = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 2] * 255;
									uint8 a = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 3] * 255;
									newVertex.color.Set(r, g, b, a);
								}
								mapVertexInfo.insert(std::make_pair(index, pDexMesh->m_vecVertexsBuffer.size()));
								pDexMesh->AddVertexIndice(pDexMesh->m_vecVertexsBuffer.size());
								pDexMesh->m_vecVertexsBuffer.push_back(newVertex);
							}
						}

					}
				}
				//polygons todo
				//polylist
				for (size_t iPolylistIndex = 0; iPolylistIndex < pMesh->vecPolylists.size(); ++iPolylistIndex)
				{
					mapVertexInfo.clear();
					DaePolylist* pPolylist = pMesh->vecPolylists[iPolylistIndex];
					int32 iInputSize = pPolylist->vInputs.size();
					pDexMesh = pDexSkinMesh->AddMesh(pPolylist->material.c_str());//暂时用meterial的名字来命名
					//处理material
					for (size_t i = 0; i < vMaterials.size(); ++i)
					{
						if (vMaterials[i] == pPolylist->material)
						{
							pDexMesh->m_iMaterialId = pDexSkinMesh->FindMaterial(pPolylist->material);
							pDexMesh->m_iTextureId = pDexMesh->m_iMaterialId;
						}
					}
					//对应pos normal uv 等source
					for (size_t i = 0; i < pPolylist->vInputs.size(); ++i)
					{
						stDaeInput* pInput = &pPolylist->vInputs[i];
						switch (pInput->semantic)
						{
						case ES_POSITION:
							pSourcePos = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_NORMAL:
							pSourceNormal = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_TEXCOORD:
							pSourceUv = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_TEXTANGENT:
							pSourceTangent = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_TEXBINORMAL:
							pSourceBinormal = find_source(pMesh->vecSources, pInput->source);
							break;
						case ES_COLOR:
							pSourceColor = find_source(pMesh->vecSources, pInput->source);
							break;
						default:
							break;
						}
					}
					int iPos = 0;
					for (int32 i = 0; i < pPolylist->count; ++i)
					{//一次性读取iInputSize个数据
						//默认input顺序是VERTEX NORMAL TEXCOORD TEXTANGENT TEXBINORMAL COLOR
						int iVCount = pPolylist->p_vcountData[i];
						if (iVCount == 3)
						{//三角形
							for (int32 j = 0; j < 3; ++j)
							{
								stIndex index;
								if (pSourcePos != nullptr)
									index.m_iPosIndex = pPolylist->pData[iPos++];
								if (pSourceNormal != nullptr)
									index.m_iNormalIndex = pPolylist->pData[iPos++];
								if (pSourceUv != nullptr)
									index.m_iUvIndex = pPolylist->pData[iPos++];
								if (pSourceTangent != nullptr)
									iPos++;//tangent
								if (pSourceBinormal != nullptr)
									iPos++;//binormal
								if (pSourceColor != nullptr)
									index.m_iColorIndex = pPolylist->pData[iPos++];

								map<stIndex, int32>::iterator ite = mapVertexInfo.find(index);
								if (ite != mapVertexInfo.end())
									pDexMesh->AddVertexIndice(ite->second);
								else
								{
									DexSkinMesh::stMeshVertex newVertex;
									if (pSourcePos != nullptr)
										if (m_bFFXIIModel)
											//FF12的dae模型，本身太小，正好是mws模型的1/60，而所有的act文件里面的骨骼信息都是根据mws设置的，所以这里得放大60倍
											newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3] * 60, pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1] * 60,
											pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2] * 60);
										else
											newVertex.pos.Set(pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3], pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 1],
											pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3 + 2]);
									if (pSourceNormal != nullptr)
										newVertex.normal.Set(pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3], pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3 + 1], pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3+2]);
									TransVector3ByAxis(newVertex.pos);
									TransVector3ByAxis(newVertex.normal);
									if (pSourceUv != nullptr)
										newVertex.uv.Set(pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2], pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2+1]);
									//一些导出工具对于uv坐标是从左到右为x ,从下到上为y，而引擎使用的dx，使用的是从左到右为x，从上到下为y
									//如果y方向不一致，就要进行转换
									newVertex.uv.y = 1 - newVertex.uv.y;
									if (pSourceColor != nullptr)
									{//有color
										uint8 r = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 0] * 255;
										uint8 g = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 1] * 255;
										uint8 b = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 2] * 255;
										uint8 a = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 3] * 255;
										newVertex.color.Set(r, g, b, a);
									}
									mapVertexInfo.insert(std::make_pair(index, pDexMesh->m_vecVertexsBuffer.size()));
									pDexMesh->AddVertexIndice(pDexMesh->m_vecVertexsBuffer.size());
									pDexMesh->m_vecVertexsBuffer.push_back(newVertex);
								}
							}

						}
						else if (iVCount == 4)
						{//四边形
							int todo = 0; //
						}
					}
				}
			}
		}
	}
	return pDexSkinMesh;
}

void DexModelDaeLoader::add_joints(DexSkinMesh* pSkinMesh, DaeNode* pNode, DaeNode* pFatherNode)
{
	DEX_ENSURE(pSkinMesh != nullptr && pNode != nullptr);
	if (m_iJointCount >= DexSkinMesh::sGetMaxJointCount())
		return;
	++m_iJointCount;
	DString fatherName = pFatherNode == nullptr ? "" : pFatherNode->sId;
	DexSkinMesh::Joint* pJoint =  pSkinMesh->AddJoint(pNode->sId, fatherName, pNode->mMatrix);
	DMap<DString, DString>::iterator ite = m_MapJointName.find(pNode->sSId);
	if (ite != m_MapJointName.end())
	{
		DMap<DString, DexMatrix4x4>::iterator ite2 = m_MapJointMatrix.find(ite->first);
		if (ite2 != m_MapJointMatrix.end())
		{
			pJoint->meshMatrix = ite2->second.GetInvert();
			if (m_bFFXIIModel)
			{//右手坐标系转为左手坐标系
				pJoint->meshMatrix.ConvertHandMatrix();
				DexMatrix4x4 matrix; matrix.Scale(DexVector3(60,60,60));//FF12的模型很小，骨骼也很小，需要放大60倍
				pJoint->meshMatrix *= matrix;
				pJoint->localMeshMatrixInvert = pJoint->meshMatrix.GetInvert();
			}
			else
			{
				pJoint->localMeshMatrixInvert =  ite2->second;
			}
			pJoint->frame_matrix = pJoint->meshMatrix * pJoint->m_pFather->localMeshMatrixInvert;
			pJoint->world_matrix = pJoint->meshMatrix;
		}
	}
	for (size_t i = 0; i < pNode->vNodeChildren.size(); ++i)
		if (pNode->vNodeChildren[i] != nullptr)
			add_joints(pSkinMesh, pNode->vNodeChildren[i], pNode);
}

void DexModelDaeLoader::deal_with_material_texture(DVector<stInstanceMaterial>& vec, DexSkinMesh* pSkinMesh, DVector<DString>& vec2, DVector<DString>& vec3)
{
	DexMaterial dexMaterial;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		DaeMaterial* pMaterial = find_material(vec[i].sTarget);
		if (pMaterial != nullptr)
		{
			DaeEffect* pEffect = find_effect(pMaterial->instance_effect_url);
			if (pEffect != nullptr && pEffect->pProfile != nullptr)
			{
				DaeEffectProfile* pEffectProfile = pEffect->pProfile;
				DaeTechnique* pTechnique = pEffectProfile->pTechnique;
				if (pTechnique != nullptr && pTechnique->pPhong != nullptr)
				{
					DaePhong* pPhong = pEffectProfile->pTechnique->pPhong;
					dexstrcpy(dexMaterial.name, pMaterial->name.c_str());
					dexMaterial.emissive = pPhong->colorEmission;
					dexMaterial.diffuse = pPhong->pDiffuse->color;
					dexMaterial.ambient = pPhong->colorAmbient;
					dexMaterial.specular = pPhong->colorSpecular;
					dexMaterial.power = pPhong->fShininess;
					DaeImage* pImage = find_image(pEffectProfile, pPhong->pDiffuse->sTexture);
					vec2.push_back(vec[i].sSymbol);
					vec3.push_back(pPhong->pDiffuse->sTexcoord);
					pSkinMesh->AddMaterial(dexMaterial);
					if (pImage != nullptr)
						pSkinMesh->AddTexture(pImage->init_from.c_str());
				}
				else
				{//如果找不到technique或者technique里面没有phone，则直接找pEffectProfile里面的newparam，看有没有sampler
					bool bBreak = false;
					for (size_t j = 0; j < pEffectProfile->vecNewParams.size(); ++j)
					{
						DaeNewParamSampler2D* pNewParamSampler2D = (DaeNewParamSampler2D*)pEffectProfile->vecNewParams[j];
						if (pNewParamSampler2D != nullptr &&pNewParamSampler2D->eType == ECE_newparam_sampler2D)
						{//目前默认只有一个sampler2d
							for (size_t k = 0; k < pEffectProfile->vecNewParams.size(); ++k)
							{
								if (pEffectProfile->vecNewParams[k]->eType == ECE_newparam_surface)
								{//找到surface ,再去library image寻找
									DaeNewParamSurface* pNewParamSurface = (DaeNewParamSurface*)pEffectProfile->vecNewParams[k];
									DaeImage* pImage = find_image(pNewParamSurface->sInitFrom);
									vec2.push_back(vec[i].sSymbol);
									vec[i].texture = pImage->init_from;
									dexstrcpy(dexMaterial.name, vec[i].sSymbol.c_str());
									pSkinMesh->AddMaterial(dexMaterial);
									if (pImage != nullptr)
									{
										vec3.push_back(pImage->init_from);
										pSkinMesh->AddTexture(pImage->init_from.c_str());
									}
									bBreak = true;
									break;
								}
							}
						}
						if (bBreak)
							break;
					}
				}
			}
		}
	}
}

void DexModelDaeLoader::find_instance_material(DVector<stInstanceMaterial>& vec, stInstanceMaterial& material, DString materialSymbol)
{
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].sSymbol == materialSymbol)
		{
			material.sSymbol =  vec[i].sSymbol;
			material.sTarget = vec[i].sTarget;
			material.texture = vec[i].texture;
			break;
		}
	}
}
DexModelDaeLoader::DaeImage* DexModelDaeLoader::find_image(DaeEffectProfile* pEffectProfile, DString sTexture)
{
	DEX_ENSURE_P(pEffectProfile != nullptr);
	DaeImage* pImage = nullptr;
	DaeNewParamSampler2D* pNewParamSampler2D = nullptr;
	DaeNewParamSurface* pNewParamSurface = nullptr;
	//先从profile里面寻找是否有sample2D
	for (size_t i = 0; i < pEffectProfile->vecNewParams.size(); ++i)
	{
		if (pEffectProfile->vecNewParams[i]->eType == ECE_newparam_sampler2D)
		{
			pNewParamSampler2D = (DaeNewParamSampler2D*) pEffectProfile->vecNewParams[i];
			if (pNewParamSampler2D->sId == sTexture)
			{//找到sTexture对应的sampler2D，寻找Surface
				for (size_t j = 0; j < pEffectProfile->vecNewParams.size(); ++j)
				{
					if (pEffectProfile->vecNewParams[j]->eType == ECE_newparam_surface)
					{//找到surface ,再去library image寻找
						pNewParamSurface = (DaeNewParamSurface*)pEffectProfile->vecNewParams[j];
						pImage = find_image(pNewParamSurface->sInitFrom);
						break;
					}
				}
				break;
			}
		}
	}
	if (pImage == nullptr)
		//sTexture所指的不在sampler2D,中，则直接去library images寻找
		pImage = find_image(sTexture);
	return pImage;
}

bool DexModelDaeLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".dae") == 0 || dexstricmp(fileType, ".xml") == 0;
}

DexModelBase* DexModelDaeLoader::LoadModel(const char* filename, int32 flag)
{
	//一个JOINT类型的node是一个骨骼系统，一个NODE类型的node是一个模型
	//静态模型下面有instance_geometry,动态模型下面有instance_controller
	//无论instance_geometry还是instantce_controller都会引用一个geometry,一个geometry就是一个skinmesh的mesh数据
	//geometry里面有多组triangles或者polylist等，每一组都对应一个材质，也就是DexMesh
	int64 Time = getTime()->GetTotalMillSeconds();
	bool ret = false;
	DexMem mem(filename);
	DEX_ENSURE_P(mem.GetLength() != 0);
	TiXmlDocument Xml;
	if (Xml.Parse((char*)mem.GetData()) == 0)
	{
		getLog()->LogLine(log_error, "open dae model error!:%s", Xml.ErrorDesc());
		return DexNull;
	}
	DexSkinMesh* pSkinMesh = nullptr;
	TiXmlNode* pXmlNode = Xml.FirstChild();
	if (pXmlNode->Type() == TiXmlNode::TINYXML_DECLARATION)
	{
		pXmlNode = pXmlNode->NextSibling();
		if (pXmlNode->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			getLog()->LogLine(log_error, "dae file format error !");
			return nullptr;
		}
		TiXmlElement* pXmlElement = pXmlNode->ToElement();
		if (strcmp(pXmlElement->Value(), g_COLLADA) != 0)
		{
			getLog()->LogLine(log_error, "dae file not find COLLADA element!");
			return nullptr;
		}
		DexCheckMemLeak::getDexMemoryLeakCheck()->BeginCheck();
		m_iJointCount = 1;
		m_bFFXIIModel = flag == 1;
		m_pCollada = parse_COLLADA(pXmlNode);

		if (1)
		{
			DaeNode* pNode = nullptr;
			DaeInstanceGeometry* pInstanceGeometry = nullptr;
			DaeInstanceController* pInstanceController = nullptr;
			for (size_t i = 0; i < m_pCollada->pLibraryVisualScenes->vecData[0]->vMeshs.size(); ++i)
			{
				pNode = m_pCollada->pLibraryVisualScenes->vecData[0]->vMeshs[i];
				if (pNode != nullptr)
				{
					if (pNode->pInstanceMesh->eType == ECE_instance_geometry)
					{//静态模型
						pInstanceGeometry = (DaeInstanceGeometry*)pNode->pInstanceMesh;
						pSkinMesh = create_SkinMeshStatic(pNode);
					}
					else if (pNode->pInstanceMesh->eType == ECE_instance_controller)
					{//带有骨骼的模型
						pInstanceController = (DaeInstanceController*)pNode->pInstanceMesh;
						pSkinMesh = create_SkinMeshAni(pNode);
					}
				}
			}
		}



		delete m_pCollada;
		m_MapJointName.clear();
		m_MapJointMatrix.clear();
		DexCheckMemLeak::getDexMemoryLeakCheck()->EndCheck();
	}
	if (pSkinMesh == nullptr)
		getLog()->LogLine(log_error, "load dae model can not find mesh !");
	else
	{
		Time = getTime()->GetTotalMillSeconds() - Time;
		getLog()->LogLine(log_ok, "load dae model %s ok, use time %d ms", filename, Time);
	}

	return pSkinMesh;
}

bool DexModelDaeLoader::SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int32 flag)
{
	return true;
}


void DexModelDaeLoader::LoadFFMap(DVector<DexSkinMesh*>& vecSkinMesh, const char* filename)
{
	getLog()->LogLine(log_ok, "load ff map %s...\n", filename);
	int64 Time = getTime()->GetTotalMillSeconds();
	TiXmlDocument Xml(filename);
	Xml.LoadFile();
	DexSkinMesh* pSkinMesh = nullptr;
	TiXmlNode* pXmlNode = Xml.FirstChild();
	if (Xml.Error())
	{
		getLog()->LogLine(log_error, "open dae model error!:%s", Xml.ErrorDesc());
	}
	else if (pXmlNode->Type() == TiXmlNode::TINYXML_DECLARATION)
	{
		pXmlNode = pXmlNode->NextSibling();
		if (pXmlNode->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			getLog()->LogLine(log_error, "dae file format error !");
			return ;
		}
		TiXmlElement* pXmlElement = pXmlNode->ToElement();
		if (strcmp(pXmlElement->Value(), g_COLLADA) != 0)
		{
			getLog()->LogLine(log_error, "dae file not find COLLADA element!");
			return ;
		}
		DexCheckMemLeak::getDexMemoryLeakCheck()->BeginCheck();
		m_pCollada = parse_COLLADA(pXmlNode);
		m_iJointCount = 1;
		m_bFFXIIModel = 1;
		if (1)
		{
			DaeNode* pNode = nullptr;
			DaeInstanceGeometry* pInstanceGeometry = nullptr;
			DaeInstanceController* pInstanceController = nullptr;
			for (size_t i = 0; i < m_pCollada->pLibraryVisualScenes->vecData[0]->vMeshs.size(); ++i)
			{
				pNode = m_pCollada->pLibraryVisualScenes->vecData[0]->vMeshs[i];
				if (pNode != nullptr)
				{
					if (pNode->pInstanceMesh->eType == ECE_instance_geometry)
					{//静态模型
						pInstanceGeometry = (DaeInstanceGeometry*)pNode->pInstanceMesh;
						pSkinMesh = create_SkinMeshStatic(pNode);
					}
					else if (pNode->pInstanceMesh->eType == ECE_instance_controller)
					{//带有骨骼的模型
						pInstanceController = (DaeInstanceController*)pNode->pInstanceMesh;
						pSkinMesh = create_SkinMeshAni(pNode);
					}
					vecSkinMesh.push_back(pSkinMesh);
				}
			}
		}
		delete m_pCollada;
		m_MapJointName.clear();
		m_MapJointMatrix.clear();
		DexCheckMemLeak::getDexMemoryLeakCheck()->EndCheck();
	}
	if (pSkinMesh == nullptr)
		getLog()->LogLine(log_error, "load dae model can not find mesh !");
	else
	{
		Time = getTime()->GetTotalMillSeconds() - Time;
		getLog()->LogLine(log_ok, "load dae model %s ok, use time %d ms", filename, Time);
	}
}
bool DexModelDaeLoader::ReadFFSkeletonInfo(DexSkinMesh* pDexSkinMesh, DString filename)
{
	DEX_ENSURE_B(pDexSkinMesh != nullptr && pDexSkinMesh->getType() == DexSkinMeshFF::getClassType());

	DexSkinMeshFF* pSkinMeshFF = (DexSkinMeshFF*)pDexSkinMesh;
	DexMem mem;
	DEX_ENSURE_B(mem.IniFromFile(filename.c_str()));

	DexSkinMesh::Joint* pFatherJoint = nullptr;
	DexSkinMesh::Joint* pJoint = nullptr;
	const int16  iMaxLineByte = 256;
	const int16  iMaxJointNameByte = 64;
	char tempData[iMaxLineByte];
	char jointName[iMaxJointNameByte];
	DVector<DString> values;
	
	bool bReadSkeletonInfo = false;
	while (!mem.End())
	{
		mem.ReadLine(tempData);
		if (bReadSkeletonInfo)
		{
			if (tempData[0] == '\r' || tempData[0] == '\n')
				break;
			values.clear();
			SplitStr(tempData, ',', values);
			if (values.size() > 3)
			{
				if (values[2] == " \"null_skeleton\"" || values[2] == " \"joint\""
					|| values[2] == " \"chain\"" || values[2] == " \"effector\"")
				{
					int32 index = atoi(values[1].c_str());
					DexVector3 translate;
					translate.x = atof(values[3].c_str());
					translate.y = atof(values[4].c_str());
					translate.z = atof(values[5].c_str());
					DexQuaternion quatenion;
					DexMatrix4x4 matrix;
					DexMatrix4x4 tempMatrix;
					quatenion.Set(DexVector3(1, 0, 0), -_getRadian(atof(values[6].c_str())));
					matrix = matrix * quatenion.GetMatrix();
					quatenion.Set(DexVector3(0, 1, 0), -_getRadian(atof(values[7].c_str())));
					matrix = matrix * quatenion.GetMatrix();
					quatenion.Set(DexVector3(0, 0, 1), -_getRadian(atof(values[8].c_str())));
					matrix = matrix * quatenion.GetMatrix();
					matrix.Translate(translate);
					matrix.ConvertHandMatrix();
					DexSkinMesh::Joint* OldJoint = pSkinMeshFF->FindMwsJoint(index);
					if (OldJoint != nullptr)
						pFatherJoint = OldJoint->m_pFather;
					int32 iTempIndex = 0;
					const char* ptr = values[0].c_str();
					for (;;)
					{
						++ptr; //skip the first "
						char cValue = *ptr;
						if (cValue == '"')
						{
							jointName[iTempIndex++] = 0;
							break;
						}
						else
						{
							jointName[iTempIndex++] = cValue;
						}
					}
					pJoint = pSkinMeshFF->AddMwsJoint(jointName, index, pFatherJoint, matrix);
					pFatherJoint = pJoint;
				}
			}

		}
		else if (tempData[0] == '[' && tempData[1] == 'T'&& tempData[5] == ']')
		{ //"[TREE]"
			bReadSkeletonInfo = true;
		}
	}
	return true;
}

bool DexModelDaeLoader::ReadActInfoFxii(DexSkinMesh* pDexSkinMesh, DString sFileName)
{
	DEX_ENSURE_B(pDexSkinMesh != nullptr && pDexSkinMesh->getType() == DexSkinMeshFF::getClassType());
	DexSkinMeshFF* pSkinMeshFF = (DexSkinMeshFF*)pDexSkinMesh;

	DexMem mem;
	DEX_ENSURE_B(mem.IniFromFile(sFileName.c_str()));
	//将之前的动画信息清除
	for (size_t i = 0; i < pSkinMeshFF->m_vecMwsJoints.size(); ++i)
		pSkinMeshFF->m_vecMwsJoints[i]->m_vecKeyFrames.clear();

	DexSkinMesh::Joint* pJoint = nullptr;
	const int16  iMaxLineByte = 256;
	const int16  iMaxJointNameByte = 64;
	const int16  iFps = 1000 / 30; //每秒60帧
	int16        iFrames = 0;//该动画有多少帧
	char tempData[iMaxLineByte];
	char jointName[iMaxJointNameByte];
	DexVector3 oldTranslate;
	DexVector3 oldRotation;
	DVector<DString> values;
	while (!mem.End())
	{
		mem.ReadLine(tempData);
		if (pJoint != nullptr)
		{
			values.clear();
			SplitStr(tempData, ',', values);
			if (strlen(tempData) == 0)
			{
				pJoint = nullptr;
				continue;
			}
			DexMatrix4x4 matrix;
			DexQuaternion quatenion;

			DexVector3 translate;
			if (values[1].length() == 1)
				translate.x = oldTranslate.x;
			else
				translate.x = atof(values[1].c_str());
			if (values[2].length() == 1)
				translate.y = oldTranslate.y;
			else
				translate.y = atof(values[2].c_str());
			if (values[3].length() == 1)
				translate.z = oldTranslate.z;
			else
				translate.z = atof(values[3].c_str());

			DexVector3 rotation;
			if (values[4].length() == 1)
				rotation.x = oldRotation.x;
			else
				rotation.x = -_getRadian(atof(values[4].c_str()));
			if (values[5].length() == 1)
				rotation.y = oldRotation.y;
			else
				rotation.y = -_getRadian(atof(values[5].c_str()));
			if (values[6].length() == 1)
				rotation.z = oldRotation.z;
			else
				rotation.z = -_getRadian(atof(values[6].c_str()));

			quatenion.Set(DexVector3(1, 0, 0), rotation.x);
			matrix = matrix * quatenion.GetMatrix();
			quatenion.Set(DexVector3(0, 1, 0), rotation.y);
			matrix = matrix * quatenion.GetMatrix();
			quatenion.Set(DexVector3(0, 0, 1), rotation.z);
			matrix = matrix * quatenion.GetMatrix();

			matrix.Translate(translate);
			matrix.ConvertHandMatrix();

			pJoint->AddKeyFrame((atoi(values[0].c_str()) - 1) * iFps, matrix);
			oldTranslate = translate;
			oldRotation = rotation;
		}
		else
		{
			switch (tempData[0])
			{
			case '[':
				for (int16 i = 1; i < iMaxJointNameByte; ++i)
				{
					if (tempData[i] == ']')
					{
						jointName[i - 1] = 0;
						break;
					}
					else
						jointName[i - 1] = tempData[i];
				}
				//if (strcmp(jointName, "n_hara") == 0)
				pJoint = pSkinMeshFF->FindMwsJoint(jointName);
				break;
			case '$':
				//like $timeLength=300
				iFrames = atoi(&tempData[12]);
				break;
			default:
				break;
			}
		}

	}
	pDexSkinMesh->SetMaxAniTime((iFrames - 1) * iFps);
	return true;
}

//#undef IS_NODE
