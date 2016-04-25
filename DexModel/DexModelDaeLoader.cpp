

#include "DexModelDaeLoader.h"
#include "../DexBase/DexDVector.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"



static const char* g_COLLADA = "COLLADA";
static const char* g_asset = "asset";
static const char* g_library_images = "library_images";
static const char* g_image = "image";
static const char* g_library_materials = "library_materials";
static const char* g_material = "material";
static const char* g_library_effects = "library_effects";
static const char* g_effect = "effect";
static const char* g_profile_COMMON = "profile_COMMON";
static const char* g_technique = "technique";
static const char* g_library_geometries = "library_geometries";
static const char* g_geometry = "geometry";
static const char* g_library_controllers = "library_controllers";
static const char* g_controller = "controller";
static const char* g_library_visual_scenes = "library_visual_scenes";
static const char* g_visual_scene = "visual_scene";
static const char* g_scene = "scene";

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
	return nullptr;
}
DexModelDaeLoader::DaeBase* DexModelDaeLoader::parse_geometry(TiXmlNode* pXmlNode, DaeLibraryGeometries* father)
{
	return nullptr;
	DEX_ENSURE_P(pXmlNode);
	TiXmlElement* pXmlElement = pXmlNode->ToElement();
	TiXmlAttribute* pXmlAtti = pXmlElement->FirstAttribute();
	const char* pGeometryId = nullptr;
	const char* pGeometryName = nullptr;
	while (pXmlAtti != nullptr)
	{
		if (strcmp(pXmlAtti->Name(), "id") == 0)
		{
			pGeometryId = pXmlAtti->Value();
		}
		else if (strcmp(pXmlAtti->Name(), "name") == 0)
		{
			pGeometryName = pXmlAtti->Value();
		}
		else
		{
			getLog()->BeginLog();
			getLog()->Log(log_allert, "dae file unknown attribute in geometry!");
			getLog()->EndLog();
		}
		pXmlAtti = pXmlAtti->Next();
	}
	if (pXmlNode->FirstChild() != nullptr)
	{
		parseMesh(pXmlNode->FirstChild());
	}
	if (pXmlNode->NextSibling() != nullptr)
	{
		parse_geometry(pXmlNode->NextSibling(), nullptr);
	}
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parseMesh(TiXmlNode* pXmlNode)
{
	DEX_ENSURE_P(pXmlNode);
}

DexModelDaeLoader::DaeBase* DexModelDaeLoader::parseSource(TiXmlNode* pXmlNode)
{
	return nullptr;
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
