

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
DexModelDaeLoader::DaeBase::DaeBase(EColladaElement _type)
{
	type = _type;
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
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		if (pXmlChildNode->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child in COLLADA!");
			getLog()->EndLog();
		}
		else
		{
			pXmlElement = pXmlChildNode->ToElement();
			IS_Element(g_asset)
				pNewDaeElement = parse_asset(pXmlChildNode, pCollada);
			else IS_Element(g_library_images)
				pNewDaeElement = parse_library_images(pXmlChildNode, pCollada);
			else IS_Element(g_library_materials)
				pNewDaeElement = parse_library_materials(pXmlChildNode, pCollada);
			else IS_Element(g_library_effects)
				pNewDaeElement = parse_library_effects(pXmlChildNode, pCollada);
			else IS_Element(g_library_geometries)
				pNewDaeElement = parse_library_geometries(pXmlChildNode, pCollada);
			else IS_Element(g_library_controllers)
				pNewDaeElement = parse_library_controllers(pXmlChildNode, pCollada);
			else IS_Element(g_library_visual_scenes)
				pNewDaeElement = parse_library_visual_scenes(pXmlChildNode, pCollada);
			else IS_Element(g_scene)
			{
				//pNewDaeElement = parse_scene(pXmlChildNode, pCollada);
				pXmlChildNode = pXmlChildNode->NextSibling();
				continue;
			}
			else
			{
				getLog()->LogLine(log_allert, "dae file unknown child %s in COLLADA!", pXmlElement->Value());
				pXmlChildNode = pXmlChildNode->NextSibling();
				continue;
			}
			if (pOldDaeElement != nullptr)
				pOldDaeElement->sibling = pNewDaeElement;
			pOldDaeElement = pNewDaeElement;
			pXmlChildNode = pXmlChildNode->NextSibling();
		}
	}
	return pCollada;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_asset(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeAsset* pAsset = new DaeAsset;
	pAsset->father = father;
	if (father->child == nullptr)
		father->child = pAsset;
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
			pAsset->up_axis = DString(pXmlElement->GetText());
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
	pLibraryImages->father = father;
	if (father->child == nullptr)
		father->child = pLibraryImages;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_image)
			pNewDaeElement = parse_image(pXmlChildNode, pLibraryImages);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child in library_images!");
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryImages;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_image(TiXmlNode* pXmlNode, DaeLibraryImage* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeImage* pImage = new DaeImage;
	pImage->father = father;
	if (father->child == nullptr)
		father->child = pImage;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析image的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pImage->id = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pImage->name = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in image!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一个节点<init_from>
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element("init_from")
			pImage->init_from = DString(pXmlChildNode->ToElement()->GetText());
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown element:%s in image!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	} 
	VEC_ADD(m_vecImages, pImage);
	return pImage;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_materials(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryMaterials* pLibraryMaterials = new DaeLibraryMaterials;
	pLibraryMaterials->father = father;
	if (father->child == nullptr)
		father->child = pLibraryMaterials;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_material)
			pNewDaeElement = parse_material(pXmlChildNode, pLibraryMaterials);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_materials!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryMaterials;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_material(TiXmlNode* pXmlNode, DaeLibraryMaterials* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeMaterial* pMaterial = new DaeMaterial;
	pMaterial->father = father;
	if (father->child == nullptr)
		father->child = pMaterial;
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
	VEC_ADD(m_vecMaterials, pMaterial);
	return pMaterial;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_effects(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryEffects* pLibraryEffects = new DaeLibraryEffects;
	pLibraryEffects->father = father;
	if (father->child == nullptr)
		father->child = pLibraryEffects;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_effect)
			pNewDaeElement = parse_effect(pXmlChildNode, pLibraryEffects);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_effects!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryEffects;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_effect(TiXmlNode* pXmlNode, DaeLibraryEffects* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeEffect* pEffect = new DaeEffect;
	pEffect->father = father;
	if (father->child == nullptr)
		father->child = pEffect;
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
	//先解析effect的attribute
	while (pXmlAttribute != nullptr)
	{
		IS_Attribute("id")
			pEffect->id = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pEffect->name = DString(pXmlAttribute->Value());
		else
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in effect!", pXmlAttribute->Name());
		pXmlAttribute = pXmlAttribute->Next();
	}
	//再解析下一个节点<profile_COMMON>
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	while (pXmlChildNode != nullptr)
	{
		TiXmlElement* pXmlElement = pXmlChildNode->ToElement();
		IS_Element("profile_COMMON")
			pNewDaeElement = parse_profile_Common(pXmlChildNode, pEffect);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown element:%s under effect!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	VEC_ADD(m_vecEffects, pEffect);
	return pEffect;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_geometries(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryGeometries* pLibraryGeometries = new DaeLibraryGeometries;
	pLibraryGeometries->father = father;
	if (father->child == nullptr)
		father->child = pLibraryGeometries;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_geometry)
			pNewDaeElement = parse_geometry(pXmlChildNode, pLibraryGeometries);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_geometries!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryGeometries;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_geometry(TiXmlNode* pXmlNode, DaeLibraryGeometries* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeGeometry* pGeometry = new DaeGeometry;
	pGeometry->father = father;
	if (father->child == nullptr)
		father->child = pGeometry;
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
	VEC_ADD(m_vecGeometries, pGeometry);
	return pGeometry;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_source(TiXmlNode* pXmlNode, DaeBase* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeSource* pSource = new DaeSource;
	pSource->father = father;
	if (father->child == nullptr)
		father->child = pSource;
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
	pLibraryControllers->father = father;
	if (father->child == nullptr)
		father->child = pLibraryControllers;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_controller)
			pNewDaeElement = parse_controller(pXmlChildNode, pLibraryControllers);
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_controllers!", pXmlElement->Value());
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryControllers;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_controller(TiXmlNode* pXmlNode, DaeLibraryControllers* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeController* pController = new DaeController;
	pController->father = father;
	if (father->child == nullptr)
		father->child = pController;
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
	VEC_ADD(m_vecControllers, pController);
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
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else IS_Element("source")
			pNewDaeElement = parse_source(pXmlChildNode, pSkin);
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
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
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
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in skin!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pSkin;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_visual_scenes(TiXmlNode* pXmlNode, DaeCollada* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeLibraryVisualScenes* pLibraryVisualScenes = new DaeLibraryVisualScenes;
	pLibraryVisualScenes->father = father;
	if (father->child == nullptr)
		father->child = pLibraryVisualScenes;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_visual_scene)
			pNewDaeElement = parse_visual_scene(pXmlChildNode, pLibraryVisualScenes);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in library_visual_scenes!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pLibraryVisualScenes;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_visual_scene(TiXmlNode* pXmlNode, DaeLibraryVisualScenes* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeVisualScene* pVisualScene = new DaeVisualScene;
	pVisualScene->father = father;
	if (father->child == nullptr)
		father->child = pVisualScene;
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
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("node")
			pNewDaeElement = parse_node(pXmlChildNode, pVisualScene);
		else IS_Element("extra")
		{
			getLog()->LogLine(log_ok, "dae file skip extra under visual_scene.", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in visual_scene!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
		DaeNode* pNode = ((DaeNode*)pNewDaeElement);
		if (pNode->eNodeType == ENT_JOINT)
			VEC_ADD(m_vecJointsSystem, pNode);
		else if (pNode->eNodeType == ENT_NODE)
			VEC_ADD(m_vecMeshes, pNode);
	}
	return pVisualScene;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_scene(TiXmlNode* pXmlNode, DaeCollada* father)
{
	return nullptr;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_profile_Common(TiXmlNode* pXmlNode, DaeEffect* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeProfileCommon* pProfileCommon = new DaeProfileCommon;
	pProfileCommon->father = father;
	if (father->child == nullptr)
		father->child = pProfileCommon;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("technique")
			pNewDaeElement = parse_technique(pXmlChildNode, pProfileCommon);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in profile_COMMON!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pProfileCommon;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_phong(TiXmlNode* pXmlNode, DaeTechnique* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaePhong* pPhong = new DaePhong;
	pPhong->father = father;
	if (father->child == nullptr)
		father->child = pPhong;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("emission")
			pNewDaeElement = parse_emission(pXmlChildNode, pPhong);
		else IS_Element("ambient")
			pNewDaeElement = parse_ambient(pXmlChildNode, pPhong);
		else IS_Element("diffuse")
			pNewDaeElement = parse_diffuse(pXmlChildNode, pPhong);
		else IS_Element("specular")
			pNewDaeElement = parse_specular(pXmlChildNode, pPhong);
		else IS_Element("shininess")
			pNewDaeElement = parse_shininess(pXmlChildNode, pPhong);
		else IS_Element("reflective")
			pNewDaeElement = parse_reflective(pXmlChildNode, pPhong);
		else IS_Element("reflectivity")
			pNewDaeElement = parse_reflectivity(pXmlChildNode, pPhong);
		else IS_Element("transparent")
			pNewDaeElement = parse_transparent(pXmlChildNode, pPhong);
		else IS_Element("transparency")
			pNewDaeElement = parse_transparency(pXmlChildNode, pPhong);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in phong!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pPhong;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_technique(TiXmlNode* pXmlNode, DaeProfileCommon* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeTechnique* pTechnique = new DaeTechnique;
	pTechnique->father = father;
	if (father->child == nullptr)
		father->child = pTechnique;
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
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("phong")
			pNewDaeElement = parse_phong(pXmlChildNode, pTechnique);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in technique!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pTechnique;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_emission(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeEmission* pEmission = new DaeEmission;
	pEmission->father = father;
	if (father->child == nullptr)
		father->child = pEmission;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("color")
		{
			const char* colorValue = pXmlElement->GetText();
			DVector<DString> out_str;
			SplitStr(colorValue, ' ', out_str);
			pEmission->color.m[0] = atof(out_str[0].c_str());
			pEmission->color.m[1] = atof(out_str[1].c_str());
			pEmission->color.m[2] = atof(out_str[2].c_str());
			pEmission->color.m[3] = atof(out_str[3].c_str());
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in emission!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pEmission;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_ambient(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeAmbient* pAmbient = new DaeAmbient;
	pAmbient->father = father;
	if (father->child == nullptr)
		father->child = pAmbient;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("color")
		{
			const char* colorValue = pXmlElement->GetText();
			DVector<DString> out_str;
			SplitStr(colorValue, ' ', out_str);
			pAmbient->color.m[0] = atof(out_str[0].c_str());
			pAmbient->color.m[1] = atof(out_str[1].c_str());
			pAmbient->color.m[2] = atof(out_str[2].c_str());
			pAmbient->color.m[3] = atof(out_str[3].c_str());
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in ambient!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pAmbient;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_diffuse(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeDiffuse* pDiffuse = new DaeDiffuse;
	pDiffuse->father = father;
	if (father->child == nullptr)
		father->child = pDiffuse;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("texture")
		{
			TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("texture")
					pDiffuse->texture = pXmlAttribute->Value();
				else IS_Attribute("texcoord")
					pDiffuse->texture = pXmlAttribute->Value();
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in texture!", pXmlElement->Value());
				pXmlAttribute = pXmlAttribute->Next();
			}
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in diffuse!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pDiffuse;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_specular(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeSpecular* pSpecular = new DaeSpecular;
	pSpecular->father = father;
	if (father->child == nullptr)
		father->child = pSpecular;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("color")
		{
			const char* colorValue = pXmlElement->GetText();
			DVector<DString> out_str;
			SplitStr(colorValue, ' ', out_str);
			pSpecular->color.m[0] = atof(out_str[0].c_str());
			pSpecular->color.m[1] = atof(out_str[1].c_str());
			pSpecular->color.m[2] = atof(out_str[2].c_str());
			pSpecular->color.m[3] = atof(out_str[3].c_str());
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in specular!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pSpecular;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_shininess(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeShininess* pShininess = new DaeShininess;
	pShininess->father = father;
	if (father->child == nullptr)
		father->child = pShininess;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("float")
		{
			const char* value = pXmlElement->GetText();
			pShininess->shininess = atof(value);
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in shininess!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pShininess;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_reflective(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeReflective* pReflective = new DaeReflective;
	pReflective->father = father;
	if (father->child == nullptr)
		father->child = pReflective;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("color")
		{
			const char* colorValue = pXmlElement->GetText();
			DVector<DString> out_str;
			SplitStr(colorValue, ' ', out_str);
			pReflective->color.m[0] = atof(out_str[0].c_str());
			pReflective->color.m[1] = atof(out_str[1].c_str());
			pReflective->color.m[2] = atof(out_str[2].c_str());
			pReflective->color.m[3] = atof(out_str[3].c_str());
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in reflective!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pReflective;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_reflectivity(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeReflectivity* pReflectivity = new DaeReflectivity;
	pReflectivity->father = father;
	if (father->child == nullptr)
		father->child = pReflectivity;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("float")
		{
			const char* value = pXmlElement->GetText();
			pReflectivity->reflectivity = atof(value);
		}
		else
		{
			getLog()->LogLine (log_allert, "dae file unknown child:%s in reflectivity!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pReflectivity;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_transparent(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeTransparent* pTransparent = new DaeTransparent;
	pTransparent->father = father;
	if (father->child == nullptr)
		father->child = pTransparent;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("color")
		{
			const char* colorValue = pXmlElement->GetText();
			DVector<DString> out_str;
			SplitStr(colorValue, ' ', out_str);
			pTransparent->color.m[0] = atof(out_str[0].c_str());
			pTransparent->color.m[1] = atof(out_str[1].c_str());
			pTransparent->color.m[2] = atof(out_str[2].c_str());
			pTransparent->color.m[3] = atof(out_str[3].c_str());
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in transparent!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pTransparent;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_transparency(TiXmlNode* pXmlNode, DaePhong* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeTransparency* pTransparency = new DaeTransparency;
	pTransparency->father = father;
	if (father->child == nullptr)
		father->child = pTransparency;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("float")
		{
			const char* value = pXmlElement->GetText();
			pTransparency->transparency = atof(value);
		}
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in transparency!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pTransparency;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_mesh(TiXmlNode* pXmlNode, DaeGeometry* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeMesh* pMesh = new DaeMesh;
	pMesh->father = father;
	if (father->child == nullptr)
		father->child = pMesh;
	TiXmlNode* pXmlChildNode = pXmlNode->FirstChild();
	TiXmlElement* pXmlElement = nullptr;
	DaeBase* pOldDaeElement = nullptr;
	DaeBase* pNewDaeElement = nullptr;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element(g_source)
			pNewDaeElement = parse_source(pXmlChildNode, pMesh);
		else IS_Element(g_vertices)
			pNewDaeElement = parse_vertices(pXmlChildNode, pMesh);
		else IS_Element(g_triangles)
			pNewDaeElement = parse_triangle(pXmlChildNode, pMesh);
		else IS_Element(g_polylist)
			pNewDaeElement = parse_polylist(pXmlChildNode, pMesh);
		else IS_Element(g_polygons)
			pNewDaeElement = parse_polygons(pXmlChildNode, pMesh);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in mesh!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	return pMesh;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_vertices(TiXmlNode* pXmlNode, DaeMesh* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeVertices* pVertices = new DaeVertices;
	pVertices->father = father;
	if (father->child == nullptr)
		father->child = pVertices;
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
	DaeTriangle* pTriangle = new DaeTriangle;
	pTriangle->father = father;
	if (father->child == nullptr)
		father->child = pTriangle;
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
	uint8 input_count = 0;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			stDaeInput input;
			parse_input(pXmlElement, input);
			if (input.semantic == ES_VERTEX)
			{//如果是一个VERTEX，则这个input不算，到vertices中去找具体的input
				DaeVertices* pVertices = find_verticles((DaeGeometry*)pTriangle->father->father, input.source); //->mesh->geometry
				int32 iVerticesInputSize = pVertices->vInputs.size();
				for (size_t i = 0; i < iVerticesInputSize; ++i)
					pTriangle->vInputs.push_back(pVertices->vInputs[i]);
				input_count += iVerticesInputSize;
			}
			else
			{
				pTriangle->vInputs.push_back(input);
				++input_count;
			}

		}
		else IS_Element("p")
		{
			pTriangle->pData = new int32[input_count * pTriangle->iCount * 3];
			const char* value = pXmlElement->GetText();
			str_to_int32_array(value, &pTriangle->pData);
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
	pPolylist->father = father;
	if (father->child == nullptr)
		father->child = pPolylist;
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
	uint8  p_input = 0;
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			stDaeInput input;
			parse_input(pXmlElement, input);
			pPolylist->inputs.push_back(input);
			++p_input;
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
					p_pCount += pPolylist->p_vcountData[valueIndex] * p_input;//一个多边形有几个顶点，一个顶点占几个input
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
			p_pCount += pPolylist->p_vcountData[valueIndex] * p_input;
		}
		else IS_Element("p")
		{
			pPolylist->pData = new int32[p_pCount];
			const char* value = pXmlElement->GetText();
			str_to_int32_array(value, &pPolylist->pData);
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
	pPolygons->father = father;
	if (father->child == nullptr)
		father->child = pPolygons;
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
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_node(TiXmlNode* pXmlNode, DaeBase* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeNode* pNode = new DaeNode;
	pNode->father = father;
	if (father->child == nullptr)
		father->child = pNode;
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
			pNewDaeElement = parse_node(pXmlChildNode, pNode);
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
			pNewDaeElement = parse_instance_geometry(pXmlChildNode, pNode);
		else IS_Element("instance_controller")
			pNewDaeElement = parse_instance_controller(pXmlChildNode, pNode);
		else
		{
			getLog()->LogLine(log_allert, "dae file unknown child:%s in node!", pXmlElement->Value());
			pXmlChildNode = pXmlChildNode->NextSibling();
			continue;
		}
		if (pOldDaeElement != nullptr)
			pOldDaeElement->sibling = pNewDaeElement;
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
	delete[]pFloatValue;
	return pNode;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_instance_geometry(TiXmlNode* pXmlNode, DaeNode* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeInstanceGeometry* pInstanceGeometry = new DaeInstanceGeometry;
	pInstanceGeometry->father = father;
	if (father->child == nullptr)
		father->child = pInstanceGeometry;
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
				TiXmlNode* pXmlInstanceMaterialChildNode = pXmlChildNode->FirstChild();//应该是instance_material
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

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_instance_controller(TiXmlNode* pXmlNode, DaeNode* father)
{
	DEX_ENSURE_P(pXmlNode);
	DaeInstanceController* pInstanceController = new DaeInstanceController;
	pInstanceController->father = father;
	if (father->child == nullptr)
		father->child = pInstanceController;
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
void DexModelDaeLoader::str_to_int32_array(const char* str, int32** value)
{
	const char* pt = str;
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
			(*value)[valueIndex] = atoi(tempValue);
			tempValueIndex = 0;
			++valueIndex;
			++pt;
		}
		else
			++tempValueIndex;
	}
	tempValue[tempValueIndex] = '\0';
	(*value)[valueIndex] = atoi(tempValue);
}

void DexModelDaeLoader::clear_vecInfo()
{
	m_vecImages.swap(DVector<DaeImage*>());
	m_vecMaterials.swap(DVector<DaeMaterial*>());
	m_vecEffects.swap(DVector<DaeEffect*>());
	m_vecGeometries.swap(DVector<DaeGeometry*>());
	m_vecControllers.swap(DVector<DaeController*>());
	m_vecJointsSystem.swap(DVector<DaeNode*>());
	m_vecMeshes.swap(DVector<DaeNode*>());
}

DexModelDaeLoader::DaeGeometry* DexModelDaeLoader::find_geometry(DString sGeometryId)
{
	for (size_t i = 0; i < m_vecGeometries.size(); ++i)
		if (m_vecGeometries[i]->sId == sGeometryId)
			return m_vecGeometries[i];
	return nullptr;
}
DexModelDaeLoader::DaeMaterial* DexModelDaeLoader::find_material(DString sMaterialId)
{
	for (size_t i = 0; i < m_vecMaterials.size(); ++i)
		if (m_vecMaterials[i]->sId == sMaterialId)
			return m_vecMaterials[i];
	return nullptr;
}
DexModelDaeLoader::DaeSource* DexModelDaeLoader::find_source(DaeGeometry* pGeometry, DString sId)
{//目前默认geometry下面只有一个mesh，所有具体数据都在这个mesh里面
	DaeBase* pMesh = pGeometry->child;
	DaeBase* pRet = pMesh->child;
	while (pRet != nullptr)
	{
		if (pRet->type == ECE_source)
		{
			DaeSource* pSource = (DaeSource*)pRet;
			if (pSource->sId == sId)
				return pSource;
		}
		pRet = pRet->sibling;
	}
	return nullptr;
}
DexModelDaeLoader::DaeVertices* DexModelDaeLoader::find_verticles(DaeGeometry* pGeometry, DString sId)
{
	DaeBase* pMesh = pGeometry->child;
	DaeBase* pRet = pMesh->child;
	while (pRet != nullptr)
	{
		if (pRet->type == ECE_vertices)
		{//目前认定mesh下面只有一个vertices，所以不用比较sId，直接返回
			return (DaeVertices*)pRet;
		}
		pRet = pRet->sibling;
	}
	return nullptr;
}
DexSkinMesh* DexModelDaeLoader::create_SkinMeshStatic(DaeNode* pStaticMeshNode)
{
	DexSkinMesh* pDexSkinMesh = new DexSkinMesh();
	DexSkinMesh::DexMesh* pDexMesh = NULL;
	DaeInstanceGeometry* pInstanceGeometry = (DaeInstanceGeometry*)pStaticMeshNode->child;
	//DaeInstanceGeometry中指明的materials就是这个skinmesh需要用到的所有material
	//处理materials
	DexMaterial dexMaterial;
	for (size_t i = 0; i < pInstanceGeometry->vecMaterials.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterials.size(); ++j)
		{
			if (m_vecMaterials[j]->sId == pInstanceGeometry->vecMaterials[i].sSymbol)
			{
				//寻找meterial对应的instance_effect
				for (size_t k = 0; k < m_vecEffects.size(); ++k)
				{
					if (m_vecEffects[i]->id == m_vecMaterials[j]->instance_effect_url)
					{//找到url对应的instance_effect

					}
				}
			}
		}
	}
	DaeGeometry* pGeometry = find_geometry(pInstanceGeometry->sUrl);
	if (pGeometry != nullptr)
	{
		stIndex index;
		DexSkinMesh::stMeshVertex newVertex;
		DaeMaterial* pMaterial;
		DaeSource* pSourcePos = nullptr;
		DaeSource* pSourceNormal = nullptr;
		DaeSource* pSourceUv = nullptr;
		DaeSource* pSourceTangent = nullptr; //暂时不支持
		DaeSource* pSourceBinormal = nullptr; //暂时不支持
		DaeSource* pSourceColor = nullptr; 

		DaeBase* pDaeElement = pGeometry->child;
		while (pDaeElement != nullptr)
		{
			switch (pDaeElement->type)
			{
			case ECE_triangle:
			{
				DMap<stIndex, int32> mapVertexInfo;
				DaeTriangle* pTriangle = (DaeTriangle*)pDaeElement;
				int32 iInputSize = pTriangle->vInputs.size();
				pDexMesh = pDexSkinMesh->AddMesh(pTriangle->material.c_str());//暂时用meterial的名字来命名
				//处理material
				pMaterial = find_material(pTriangle->material);

				//对应pos normal uv 等source
				for (size_t i = 0; i < pTriangle->vInputs.size(); ++i)
				{
					stDaeInput* pInput = &pTriangle->vInputs[i];
					switch (pInput->semantic)
					{
					case ES_POSITION:
						pSourcePos = find_source(pGeometry, pInput->source);
						break;
					case ES_NORMAL:
						pSourceNormal = find_source(pGeometry, pInput->source);
						break;
					case ES_TEXCOORD:
						pSourceUv = find_source(pGeometry, pInput->source);
						break;
					case ES_TEXTANGENT:
						pSourceTangent = find_source(pGeometry, pInput->source);
						break;
					case ES_TEXBINORMAL:
						pSourceBinormal = find_source(pGeometry, pInput->source);
						break;
					case ES_COLOR:
						pSourceColor = find_source(pGeometry, pInput->source);
						break;
					default:
						break;
					}
				}
				for (int32 i = 0; i < pTriangle->iCount;)
				{//一次性读取iInputSize个数据
					//默认input顺序是VERTEX NORMAL TEXCOORD TEXTANGENT TEXBINORMAL COLOR
					if (iInputSize > 0 )
						index.m_iPosIndex = pTriangle->pData[i++];
					if (iInputSize > 1)
						index.m_iNormalIndex = pTriangle->pData[i++];
					if (iInputSize > 2)
						index.m_iUvIndex = pTriangle->pData[i++];
					if (iInputSize > 3)
						i++;//tangent
					if (iInputSize > 4)
						i++;//binormal
					if (iInputSize > 5)
						index.m_iColorIndex = pTriangle->pData[i++];
					map<stIndex, int32>::iterator ite = mapVertexInfo.find(index);
					if (ite != mapVertexInfo.end())
						pDexMesh->AddVertexIndice(ite->second);
					else
					{
						newVertex.pos.Set(&pSourcePos->pFloatArrayValues[index.m_iPosIndex*3]);//从该索引处取3个浮点数放进pos中
						newVertex.normal.Set(&pSourceNormal->pFloatArrayValues[index.m_iNormalIndex * 3]);
						newVertex.uv.Set(&pSourceUv->pFloatArrayValues[index.m_iUvIndex * 2]);
						if (iInputSize > 5)
						{//有color
							uint8 r = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 0] * 255;
							uint8 g = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 1] * 255;
							uint8 b = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 2] * 255;
							uint8 a = pSourceColor->pFloatArrayValues[index.m_iColorIndex * 4 + 3] * 255;
							newVertex.color.Set(r,g,b,a);
						}
						mapVertexInfo.insert(std::make_pair(index, pDexMesh->m_vecVertexsBuffer.size()));
						pDexMesh->AddVertexIndice(pDexMesh->m_vecVertexsBuffer.size());
						pDexMesh->m_vecVertexsBuffer.push_back(newVertex);
					}
				}

			}
			case ECE_polygons:
			case ECE_polylist:
				break;
			default:
				break;
			}
			pDaeElement = pDaeElement->sibling;
		}
	}
	else
		getLog()->LogLine(log_allert, "can not find instance_geometry url %s", pInstanceGeometry->sUrl.c_str());
	return pDexSkinMesh;
}
DexModelBase* DexModelDaeLoader::LoadModel(const char* filename)
{
	//一个JOINT类型的node是一个骨骼系统，一个NODE类型的node是一个模型
	//静态模型下面有instance_geometry,动态模型下面有instance_controller
	//无论instance_geometry还是instantce_controller都会引用一个geometry,一个geometry就是一个skinmesh的mesh数据
	//geometry里面有多组triangles或者polylist等，每一组都对应一个材质，也就是DexMesh
	TiXmlDocument Xml(filename);
	Xml.LoadFile();
	
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
		DaeCollada* pCollada = parse_COLLADA(pXmlNode);


		DaeNode* pNode = nullptr;
		DaeInstanceGeometry* pInstanceGeometry = nullptr;
		DaeInstanceController* pInstanceController = nullptr;
		for (size_t i = 0; i < m_vecMeshes.size(); ++i)
		{
			pNode = m_vecMeshes[i];
			if (pNode->child != nullptr)
			{
				if (pNode->child->type == ECE_instance_geometry)
				{//静态模型
					pInstanceGeometry = (DaeInstanceGeometry*) pNode->child;
				}
			}
		}


		delete pCollada;
		clear_vecInfo();
		DexCheckMemLeak::getDexMemoryLeakCheck()->EndCheck();
	}
	return nullptr;
}
//#undef IS_NODE
