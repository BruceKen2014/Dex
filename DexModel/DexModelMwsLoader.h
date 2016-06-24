/*
DexEngine& mws model loader
*/
#ifndef _DEX_MODEL_LOADER_MWS_H
#define _DEX_MODEL_LOADER_MWS_H
#include "../DexBase/DexDVector.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexVector4.h"
#include "../DexBase/DexString.h"
#include "../DexBase/DexMaterial.h"

#include "DexModelLoader.h"




class DexModelMwsLoader :public IDexModelLoader
{
private:
	typedef struct _stMwsToken
	{
		_stMwsToken(){};
		virtual ~_stMwsToken(){};
	}stMwsToken;

	//vertex
	typedef struct _stMwsVertex :public _stMwsToken
	{
		DexVector3 pos;
		_stMwsVertex(){};
		virtual ~_stMwsVertex(){};
	}stMwsVertex;
	//normal
	typedef struct _stMwsNormal :public _stMwsToken
	{
		DexVector3 normal;
		_stMwsNormal(){};
		virtual ~_stMwsNormal(){};
	}stMwsNormal;
	//uv
	typedef struct _stMwsUv :public _stMwsToken
	{
		DexVector2 uv;
		_stMwsUv(){};
		virtual ~_stMwsUv(){};
	}stMwsUv;
	//color
	typedef struct _stMwsColor :public _stMwsToken
	{
		DexVector4 color;
		_stMwsColor(){};
		virtual ~_stMwsColor(){};
	}stMwsColor;
	//texture
	typedef struct _stMwsTexture :public _stMwsToken
	{
		DString sName;
		_stMwsTexture(){};
		virtual ~_stMwsTexture(){};
	}stMwsTexture;
	//material
	typedef struct _stMwsMaterial :public _stMwsToken
	{
		typedef struct _stMaterialLayerData
		{
			int textureIndex;
			bool wrap;
			bool wrapU;
			bool wrapV;
			bool culling;
			bool reflection;
			bool reflectionEye;
			bool reflectionUV;
			char reflectionTextureName[256];
			double rayDirection[3];
			bool fresnel;
			bool fresnelMode;
			double fresnelEffectColor[4];
			double fresnelBaseColor[4];
			double fresnelIntensity;
			bool alphaBlend;
			int alphaBlendMode[4];
			double alphaBlendFix;
			int paletteNo;
			bool zSort;
			bool specular;
			double specularColor[4];
			double specularIntensity;
			double color[4];
			DexMaterial& getDexMaterial(DexMaterial& material)
			{
				return material;
			}
		}stMaterialLayerData;
		DString sName;
		uint16  iNumLayers;
		stMaterialLayerData* pData;
		_stMwsMaterial() :pData(nullptr){};
		virtual ~_stMwsMaterial() { delete[] pData; };
	}stMwsMaterial;
	//face
	typedef struct _stMwsFace :public _stMwsToken
	{
		uint16 iTimes; //暂时只支持iTimes==1的情况
		uint16 iVertices;
		uint16* pVertexIndex;
		uint16* pNormalIndex;
		uint16* pUvIndex;
		uint16* pMaterialIndex;
		_stMwsFace(){ pVertexIndex = pNormalIndex = pUvIndex = pMaterialIndex = nullptr; }
		virtual ~_stMwsFace() { delete[] pVertexIndex; delete[] pNormalIndex; delete[] pUvIndex; delete[] pMaterialIndex; }
	}stMwsFace;
	//group
	typedef struct _stMwsGroup : public _stMwsToken
	{
		DString sName;
		DString sParentName;
		uint16  iFaceCount;
		uint16*  pFaceIndex;
		_stMwsGroup() :pFaceIndex(nullptr){}
		virtual ~_stMwsGroup() { delete[]pFaceIndex; }
	}stMwsGroup;
	//tree
	typedef struct _stMwsTree
	{
		enum TreeType{
			TYPE_LOCATOR,
			TYPE_JOINT,
			TYPE_EFFECTOR,
			TYPE_CHAIN,
			TYPE_IK,
			TYPE_GROUP,
			TYPE_TRANSFORM,
			TYPE_NULL_SKELETON,
			TYPE_LOD_GROUP,
			TYPE_BILLBOARD_Y,
			TYPE_BILLBOARD_XYZ,
			TYPE_BILLBOARD2_Y,
			TYPE_BILLBOARD2_XYZ,
			TYPE_MIME_VERTEX,
			TYPE_MIME_COLOR,
			TYPE_MIME_VERTEX_AND_COLOR,
			TYPE_PRIMITIVE_SPHERE,
			TYPE_PRIMITIVE_CYLINDER,
			TYPE_PRIMITIVE_CUBE,
			TYPE_UNKNOWN,
			NUM_TREETYPE
		};
		DString sName;
		int16   iParentIndex;
		DexVector3 translation;
		DexVector3 rotation;
		DexVector3 scale;
		TreeType eTreeType;
		_stMwsTree() {}
		virtual ~_stMwsTree() {}
	}stMwsTree;
protected:
	uint32 iMwsVersion;

	uint16 iVertexCount;
	uint16 iNormalCount;
	uint16 iUvCount;
	uint16 iColorCount;
	uint16 iTextureCount;
	uint16 iMaterialCount;
	uint16 iFaceCount;
	uint16 iGroupCount;
	uint16 iTreeCount;
	stMwsVertex*	pVertexData;
	stMwsNormal*	pNormalData;
	stMwsUv*		pUvData;
	stMwsColor*		pColorData;
	stMwsTexture*   pTextureData;
	stMwsMaterial*  pMaterialData;
	stMwsFace*		pFaceData;
	stMwsGroup*		pGroupData;
	stMwsTree*		pTreeData;

protected:
	void freeModelData();
	void ReadVertex(const char* data, stMwsVertex& vertex);
	void ReadNormal(const char* data, stMwsNormal& normal);
	void ReadUv(const char* data, stMwsUv& uv);
	void ReadColor(const char* data, stMwsColor& color);
	void ReadTexture(const char* data, stMwsTexture& texture);
	void ReadMaterial(const char* data, stMwsMaterial& material);
	void ReadFace(const char* data, stMwsFace& face);
	void ReadGroup(const char* data, stMwsGroup& group);
	void ReadTree(const char* data, stMwsTree& tree);
public:
	DexModelMwsLoader();
	virtual ~DexModelMwsLoader();

public:
	virtual DexModelBase* LoadModel(const char* filename, int32 flag);
};

#endif 