

#include "DexModelDaeLoader.h"
#include "../DexBase/DexDVector.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"
#include "../DexMath/DexQuaternion.h"
#include "../DexBase/DexMemoryLeakCheck.h"
#include "DexSkinMesh.h"
#include "../DexBase/DexDMap.h"



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
	pVCountData = pData = nullptr;
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
			parse_asset(pXmlChildNode, pCollada);
		else IS_Element(g_library_images)	
			parse_library_images(pXmlChildNode, pCollada);
		else IS_Element(g_library_materials)
			parse_library_materials(pXmlChildNode, pCollada);
		else IS_Element(g_library_effects)
			parse_library_effects(pXmlChildNode, pCollada);
		else IS_Element(g_library_geometries)
			parse_library_geometries(pXmlChildNode, pCollada);
		else IS_Element(g_library_controllers)
			parse_library_controllers(pXmlChildNode, pCollada);
		else IS_Element(g_library_visual_scenes)
			parse_library_visual_scenes(pXmlChildNode, pCollada);
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
			pAsset->created = DString(pXmlElement->GetText());
		else IS_Element("modified")
			pAsset->modified = DString(pXmlElement->GetText());
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
			getLog()->LogLine(log_ok, "dae file skip extra under material.", pXmlElement->Value());
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
		else
			getLog()->LogLine(log_allert, "dae file unknown element:%s under effect!", pXmlElement->Value());
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
			pSource->pFloatArrayValues = new float32[pSource->iFloatArrayCount];
			str_to_float_array(strFloatValue, &pSource->pFloatArrayValues);
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
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("skin")
			pNewDaeElement = parse_skin(pXmlChildNode, pController);
		else IS_Element("extra")
		{
			getLog()->LogLine(log_ok, "dae file skip extra under controller.", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in controller!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pController;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_skin(TiXmlNode* pXmlNode, DaeController* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeSkin* pSkin = new DaeSkin;
	pSkin->father = father;
	if (father->child == nullptr)
		father->child = pSkin;
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
			float32* values = new float32[16];
			str_to_float_array(pXmlElement->GetText(), &values);
			pSkin->mMatrix = DexMatrix4x4(values);
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
					pSkin->vVertexWeightInputs.push_back(input);
				}
				else IS_Element("vcount")
				{
					_SafeDeleteArr(pSkin->pVCountData);
					pSkin->pVCountData = new int32[pSkin->iVertexWeightsCount];
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
							pSkin->pVCountData[valueIndex] = atoi(tempValue);
							i_VCount += pSkin->pVCountData[valueIndex] * pSkin->vVertexWeightInputs.size();
							tempValueIndex = 0;
							++valueIndex;
							++pt;
						}
						else
							++tempValueIndex;
					}
					tempValue[tempValueIndex] = '\0';
					pSkin->pVCountData[valueIndex] = atoi(tempValue);
					i_VCount += pSkin->pVCountData[valueIndex] * pSkin->vVertexWeightInputs.size();
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
				if(pNode->pInstanceMesh != nullptr)
					pVisualScene->vMeshs.push_back(pNode);
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
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("sampler2D")
			pNewParam = parse_newparam_sampler2D(pXmlChildNode);
		else IS_Element("surface")
			pNewParam = parse_newparam_surface(pXmlChildNode);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s under newparam!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
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
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pNode->sId = pXmlAttribute->Value();
		else IS_Attribute("name")
			pNode->sName = pXmlAttribute->Value();
		else IS_Attribute("sid")
			pNode->sSId = pXmlAttribute->Value();
		else IS_Attribute("type")
		{
			if (strcmp(pXmlAttribute->Value(), "JOINT") == 0)
				pNode->eNodeType = ENT_JOINT;
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
	float* pFloatValue = new float[16];
	DexMatrix4x4 tempMatrix;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("node")
			pNode->vNodeChildren.push_back(parse_node(pXmlChildNode));
		else IS_Element("translate")
		{
			str_to_float_array(pXmlElement->GetText(),&pFloatValue);
			pNode->mMatrix.Translate(DexVector3(pFloatValue));
		}
		else IS_Element("rotate")
		{
			str_to_float_array(pXmlElement->GetText(), &pFloatValue);
			DexQuaternion quaternion(pFloatValue);
			tempMatrix = quaternion.GetMatrix();
			pNode->mMatrix *= tempMatrix;
		}
		else IS_Element("scale")
		{
			str_to_float_array(pXmlElement->GetText(), &pFloatValue);
			pNode->mMatrix.Scale(DexVector3(pFloatValue));
		}
		else IS_Element("scale")
		{
			str_to_float_array(pXmlElement->GetText(), &pFloatValue);
			pNode->mMatrix.Scale(DexVector3(pFloatValue));
		}
		else IS_Element("matrix")
		{
			str_to_float_array(pXmlElement->GetText(), &pFloatValue);
			pNode->mMatrix.Set(pFloatValue);
		}
		else IS_Element("instance_geometry")
		{
			if (pNode->pInstanceMesh != nullptr)
			{
				getLog()->LogLine(log_allert, "dae file find multy instancemesh child under node!");
				delete pNode->pInstanceMesh;
			}
			pNode->pInstanceMesh = parse_instance_geometry(pXmlChildNode);
		}
		else IS_Element("instance_controller")
		{
			if (pNode->pInstanceMesh != nullptr)
			{
				getLog()->LogLine(log_allert, "dae file find multy instancemesh child under node!");
				delete pNode->pInstanceMesh;
			}
			pNode->pInstanceMesh = parse_instance_controller(pXmlChildNode);
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
			else if (strcmp(pXmlAttribute->Value(), "JOINT") == 0)
				input.semantic = ES_JOINT;
			else if (strcmp(pXmlAttribute->Value(), "WEIGHT") == 0)
				input.semantic = ES_WEIGHT;
			else if (strcmp(pXmlAttribute->Value(), "INV_BIND_MATRIX") == 0)
				input.semantic = ES_INV_BIND_MATRIX;
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

void DexModelDaeLoader::str_to_float_array(const char* str, float32** value)
{
	const char* pt = str;
	char tempValue[32];
	uint8 tempValueIndex = 0; //for tempValue
	uint32 valueIndex = 0; //for value
	DVector<float32> testVector;
	while (pt != nullptr && *pt != '\0')
	{
		tempValue[tempValueIndex] = *pt;
		if (*(++pt) == ' ')
		{
			++tempValueIndex;
			tempValue[tempValueIndex] = '\0';
			testVector.push_back(atof(tempValue));
			(*value)[valueIndex] = atof(tempValue);
			tempValueIndex = 0;
			++valueIndex;
			++pt;
		}
		else
			++tempValueIndex;
	}
	tempValue[tempValueIndex] = '\0';
	testVector.push_back(atof(tempValue));
	(*value)[valueIndex] = atof(tempValue);
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
DexModelDaeLoader::DaeSource* DexModelDaeLoader::find_source(DVector<DaeSource*>& vData, DString sId)
{
	for (size_t i = 0; i < vData.size(); ++i)
		if (vData[i]->sId == sId)
			return vData[i];
	return nullptr;
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
		DexSkinMesh::stMeshVertex newVertex;
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
						pDexMesh->m_iMaterialId = i;
						pDexMesh->m_iTextureId = i;
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
							newVertex.pos.Set(&pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3]);//从该索引处取3个浮点数放进pos中
							newVertex.normal.Set(&pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3]);
							TransVector3ByAxis(newVertex.pos);
							TransVector3ByAxis(newVertex.normal);
							//newVertex.uv.x = 1.0f - newVertex.uv.x;
							//newVertex.uv.y = 1.0f - newVertex.uv.y;

							newVertex.uv.Set(&pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2]);
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
						pDexMesh->m_iMaterialId = i;
						pDexMesh->m_iTextureId = i;
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
								newVertex.pos.Set(&pSourcePos->pFloatArrayValues[index.m_iPosIndex * 3]);//从该索引处取3个浮点数放进pos中
								newVertex.normal.Set(&pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3]);
								TransVector3ByAxis(newVertex.pos);
								TransVector3ByAxis(newVertex.normal);

								newVertex.uv.Set(&pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2]);
								newVertex.uv.x = 1.0f - newVertex.uv.x;
								newVertex.uv.y = 1.0f - newVertex.uv.y;
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
	DexSkinMesh* pDexSkinMesh = new DexSkinMesh();
	DexSkinMesh::DexMesh* pDexMesh = NULL;
	DaeInstanceController* pInstanceController = (DaeInstanceController*)pAniMeshNode->pInstanceMesh;
	//DaeInstanceController中指明的materials就是这个skinmesh需要用到的所有material
	//处理materials
	DVector<DString> vMaterials;
	DVector<DString> vImages;
	deal_with_material_texture(pInstanceController->vecMaterials, pDexSkinMesh, vMaterials, vImages);


	return nullptr;
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
					dexMaterial.emissive = pPhong->colorEmission;
					dexMaterial.diffuse = pPhong->pDiffuse->color;
					dexMaterial.ambient = pPhong->colorAmbient;
					dexMaterial.specular = pPhong->colorSpecular;
					dexMaterial.power = pPhong->fShininess;
					DaeImage* pImage = find_image(pEffectProfile, pPhong->pDiffuse->sTexture);
					vec2.push_back(vec[i].sSymbol);
					vec3.push_back(pPhong->pDiffuse->sTexcoord);
					pSkinMesh->AddMaterial(dexMaterial);
					pSkinMesh->AddTexture(pImage->init_from.c_str());
				}
			}
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

DexModelBase* DexModelDaeLoader::LoadModel(const char* filename)
{
	//一个JOINT类型的node是一个骨骼系统，一个NODE类型的node是一个模型
	//静态模型下面有instance_geometry,动态模型下面有instance_controller
	//无论instance_geometry还是instantce_controller都会引用一个geometry,一个geometry就是一个skinmesh的mesh数据
	//geometry里面有多组triangles或者polylist等，每一组都对应一个材质，也就是DexMesh
	getLog()->LogLine(log_ok, "load dae model %s...\n", filename);
	int64 Time = getTime()->GetTotalMillSeconds();
	TiXmlDocument Xml(filename);
	Xml.LoadFile();
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
		m_pCollada = parse_COLLADA(pXmlNode);


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
					pInstanceGeometry = (DaeInstanceGeometry*) pNode->pInstanceMesh;
					pSkinMesh = create_SkinMeshStatic(pNode);
				}
			}
		}


		delete m_pCollada;
		DexCheckMemLeak::getDexMemoryLeakCheck()->EndCheck();
	}
	Time = getTime()->GetTotalMillSeconds() - Time;
	getLog()->LogLine(log_ok, "load dae model %s ok, use time %d ms\n", filename, Time);
	return pSkinMesh;
}
//#undef IS_NODE
