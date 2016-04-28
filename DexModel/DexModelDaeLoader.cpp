

#include "DexModelDaeLoader.h"
#include "../DexBase/DexDVector.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"



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
static const char* g_profile_COMMON = "profile_COMMON";
static const char* g_polylist = "polylist";
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

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_COLLADA(TiXmlNode* pXmlNode)
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
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute in COLLADA!");
			getLog()->EndLog();
		}
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
				pNewDaeElement = parse_scene(pXmlChildNode, pCollada);
			else
			{
				getLog()->BeginLog();
				getLog()->Log(log_allert, "dae file unknown child %s in COLLADA!", pXmlElement->Value());
				getLog()->EndLog();
			}
			if (pOldDaeElement != nullptr)
			{
				pOldDaeElement->sibling = pNewDaeElement;
			}
			pOldDaeElement = pNewDaeElement;
			pXmlChildNode = pXmlChildNode->NextSibling();
		}
	}
	delete pCollada;
	return nullptr;
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
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child %s in asset!", pXmlElement->Value());
			getLog()->EndLog();
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child in library_images!");
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute:%s in image!", pXmlAttribute->Name());
			getLog()->EndLog();
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown element:%s in image!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	} 

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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in library_materials!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			pMaterial->id = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pMaterial->name = DString(pXmlAttribute->Value());
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute:%s in material!", pXmlAttribute->Name());
			getLog()->EndLog();
		}
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
			//跳过第一个井号'#'
			pMaterial->instance_effect_url = DString(&(pXmlAttribute->Value())[1]);
		}
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown element:%s under material!", pXmlElement->Value());
			getLog()->EndLog();
		}
		pXmlChildNode = pXmlChildNode->NextSibling();
	}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in library_effects!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute:%s in effect!", pXmlAttribute->Name());
			getLog()->EndLog();
		}
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
		{
			pNewDaeElement = parse_profile_Common(pXmlChildNode, pEffect);
		}
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown element:%s under effect!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}

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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in library_geometries!", pXmlElement->Value());
			getLog()->EndLog();
		}	
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			pGeometry->id = DString(pXmlAttribute->Value());
		else IS_Attribute("name")
			pGeometry->name = DString(pXmlAttribute->Value());
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute:%s in geometry!", pXmlAttribute->Name());
			getLog()->EndLog();
		}
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
		{
			pNewDaeElement = parse_mesh(pXmlChildNode, pGeometry);
		}
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown element:%s under effect!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}

	return pGeometry;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_source(TiXmlNode* pXmlNode, DaeMesh* father)
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
			pSource->id = DString(pXmlAttribute->Value());
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute:%s in source!", pXmlAttribute->Name());
			getLog()->EndLog();
		}
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
					pSource->floatArrarId = DString(pXmlAttribute->Value());
				else IS_Attribute("count")
					pSource->count = atoi(pXmlAttribute->Value());
				else
				{
					getLog()->BeginLog();
					getLog()->Log(log_allert, "dae file unknown attribute:%s in source float array!", pXmlAttribute->Name());
					getLog()->EndLog();
				}
				pXmlAttribute = pXmlAttribute->Next();
			}
			//读取float_array的value
			const char* strFloatValue = pXmlElement->GetText();
			pSource->floatValues = new float32[pSource->count];
			str_to_float_array(strFloatValue, &pSource->floatValues);
		}
		else IS_Element(g_technique_common)
		{//直接在此解析technique_common，不再递归到下一层级
			//technique_common本身没有属性，直接解析子节点<accessor>
			pXmlAccessorNode = pXmlChildNode->FirstChild();
			//目前只关系accessor中的stride属性
			pXmlElement = pXmlAccessorNode->ToElement();
			TiXmlAttribute* pXmlAttribute = pXmlElement->FirstAttribute();
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("source")
					int i = 0;
				else IS_Attribute("count")
					int i = 0;
				else IS_Attribute("stride")
				{
					pSource->stride = atoi(pXmlAttribute->Value());
				}
				pXmlAttribute = pXmlAttribute->Next();
			}
			//accessor下面还有param等，目前并不解析
		}
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown element:%s under source!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
		pOldDaeElement = pNewDaeElement;
		pXmlChildNode = pXmlChildNode->NextSibling();
	}

	return pSource;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_controllers(TiXmlNode* pXmlNode, DaeCollada* father)
{
	return nullptr;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_controller(TiXmlNode* pXmlNode, DaeLibraryControllers* father)
{
	return nullptr;
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_library_visual_scenes(TiXmlNode* pXmlNode, DaeCollada* father)
{
	return nullptr;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_visual_scene(TiXmlNode* pXmlNode, DaeLibraryVisualScenes* father)
{
	return nullptr;
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in profile_COMMON!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in phong!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute:%s in technique!", pXmlAttribute->Name());
			getLog()->EndLog();
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in technique!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in emission!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in ambient!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
				{
					pDiffuse->texture = pXmlAttribute->Value();
				}
				else IS_Attribute("texcoord")
				{
					pDiffuse->texture = pXmlAttribute->Value();
				}
				else
				{
					getLog()->BeginLog();
					getLog()->Log(log_allert, "dae file unknown attribute:%s in texture!", pXmlElement->Value());
					getLog()->EndLog();
				}
				pXmlAttribute = pXmlAttribute->Next();
			}
		}
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in diffuse!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in specular!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in shininess!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in reflective!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in reflectivity!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown child:%s in transparent!", pXmlElement->Value());
			getLog()->EndLog();
		}
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
		getLog()->BeginLog();
		getLog()->Log(log_allert, "dae file unknown child:%s in transparency!", pXmlElement->Value());
		getLog()->EndLog();
	}
	if (pOldDaeElement != nullptr)
	{
		pOldDaeElement->sibling = pNewDaeElement;
	}
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
		else
			getLog()->LogLine(log_allert, "dae file unknown child:%s in mesh!", pXmlElement->Value());
		if (pOldDaeElement != nullptr)
		{
			pOldDaeElement->sibling = pNewDaeElement;
		}
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
		{
			pVertices->id = pXmlAttribute->Value();
		}
		else
		{	
			getLog()->LogLine(log_allert, "dae file unknown attribute:%s in vertices!", pXmlElement->Value());
		}
		pXmlAttribute = pXmlAttribute->Next();
	}
	//解析Input
	while (pXmlChildNode != nullptr)
	{
		pXmlElement = pXmlChildNode->ToElement();
		IS_Element("input")
		{
			pXmlAttribute = pXmlElement->FirstAttribute();
			while (pXmlAttribute != nullptr)
			{
				IS_Attribute("semantic")
				{
					pVertices->semantic = pXmlAttribute->Value();
				}
				else IS_Attribute("source")
				{
					pVertices->source = &(pXmlAttribute->Value()[1]);//跳过#
				}
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in vertices input!", pXmlElement->Value());
				pXmlAttribute = pXmlAttribute->Next();
			}
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
		{
			pTriangle->count = atoi(pXmlAttribute->Value());
		}
		else IS_Attribute("material")
		{
			pTriangle->material = pXmlAttribute->Value();
		}
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
			DaeTriangle::stInput input;
			pXmlAttribute = pXmlElement->FirstAttribute();
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
					else
					{
						input.semantic = ES_UNKNOWN;
						getLog()->LogLine(log_allert, "dae file unknown semantic:%s in triangle input!", pXmlElement->Value());
					}
				}
				else IS_Attribute("source")
					input.source = &(pXmlAttribute->Value()[1]);//跳过#
				else IS_Attribute("offset")
					input.offset = atoi(pXmlAttribute->Value());
				else IS_Attribute("set")
					input.set =   atoi(pXmlAttribute->Value());
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in triangle input!", pXmlElement->Value());
				pXmlAttribute = pXmlAttribute->Next();
			}
			pTriangle->inputs.push_back(input);
			++input_count;
		}
		else IS_Element("p")
		{
			pTriangle->pData = new int32[input_count * pTriangle->count * 3];
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
		{
			pPolylist->count = atoi(pXmlAttribute->Value());
		}
		else IS_Attribute("material")
		{
			pPolylist->material = pXmlAttribute->Value();
		}
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
			DaeTriangle::stInput input;
			pXmlAttribute = pXmlElement->FirstAttribute();
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
					else
					{
						input.semantic = ES_UNKNOWN;
						getLog()->LogLine(log_allert, "dae file unknown semantic:%s in polygon input!", pXmlElement->Value());
					}
				}
				else IS_Attribute("source")
					input.source = &(pXmlAttribute->Value()[1]);//跳过#
				else IS_Attribute("offset")
					input.offset = atoi(pXmlAttribute->Value());
				else IS_Attribute("set")
					input.set = atoi(pXmlAttribute->Value());
				else
					getLog()->LogLine(log_allert, "dae file unknown attribute:%s in polygon input!", pXmlElement->Value());
				pXmlAttribute = pXmlAttribute->Next();
			}
			pPolylist->inputs.push_back(input);
			++p_input;
		}
		else IS_Element("vcount")
		{
			pPolylist->p_vcountData = new int32[pPolylist->count];
			//这里因为我们要记录总共的顶点数量所以就没有调用str_to_int32_array
			int32** value = &pPolylist->p_vcountData;
			const char* elementText = pXmlElement->GetText();
			const char* pt = elementText;
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
					p_pCount += (*value)[valueIndex] * p_input;//一个多边形有几个顶点，一个顶点占几个input
					if ((*value)[valueIndex] != 3 && (*value)[valueIndex] != 4)
						getLog()->LogLine(log_allert, "dae polylist 有不支持的%d边形！", (*value)[valueIndex]);
					tempValueIndex = 0;
					++valueIndex;
					++pt;
				}
				else
					++tempValueIndex;
			}
			tempValue[tempValueIndex] = '\0';
			(*value)[valueIndex] = atoi(tempValue);
			p_pCount += (*value)[valueIndex] * p_input;
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
	DVector<float32> testVector;
	while (pt != nullptr && *pt != '\0')
	{
		tempValue[tempValueIndex] = *pt;
		if (*(++pt) == ' ')
		{
			++tempValueIndex;
			tempValue[tempValueIndex] = '\0';
			testVector.push_back(atoi(tempValue));
			(*value)[valueIndex] = atoi(tempValue);
			tempValueIndex = 0;
			++valueIndex;
			++pt;
		}
		else
			++tempValueIndex;
	}
	tempValue[tempValueIndex] = '\0';
	testVector.push_back(atoi(tempValue));
	(*value)[valueIndex] = atoi(tempValue);
}
DexModelBase* DexModelDaeLoader::LoadModel(const char* filename)
{
	TiXmlDocument Xml(filename);
	Xml.LoadFile();
	TiXmlNode* pXmlNode = Xml.FirstChild();
	if (pXmlNode->Type() == TiXmlNode::TINYXML_DECLARATION)
	{
		pXmlNode = pXmlNode->NextSibling();
		if (pXmlNode->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			getLog()->BeginLog(); getLog()->Log(log_error, "dae file format error !"); getLog()->EndLog();
			return nullptr;
		}
		TiXmlElement* pXmlElement = pXmlNode->ToElement();
		if (strcmp(pXmlElement->Value(), g_COLLADA) != 0)
		{
			getLog()->BeginLog(); getLog()->Log(log_error, "dae file not find COLLADA element!"); getLog()->EndLog();
			return nullptr;
		}
		parse_COLLADA(pXmlNode);
	}
	getLog()->BeginLog(); getLog()->Log(log_error, "dae file format error !"); getLog()->EndLog();
	return nullptr;
}
//#undef IS_NODE
