/*
DexEngine& mws model loader
*/
#ifndef _DEX_MODEL_LOADER_MWS_H
#define _DEX_MODEL_LOADER_MWS_H
#include "../DexBase/DexDVector.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexVector4.h"
#include "../DexMath/DexMatrix.h"
#include "../DexBase/DexString.h"
#include "../DexBase/DexMaterial.h"

#include "DexModelLoader.h"

class DexSkinMesh;
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
		DFloat64 pos[3];
		_stMwsVertex(){};
		virtual ~_stMwsVertex(){};
	}stMwsVertex;
	//normal
	typedef struct _stMwsNormal :public _stMwsToken
	{
		DFloat64 normal[3];
		_stMwsNormal(){};
		virtual ~_stMwsNormal(){};
	}stMwsNormal;
	//uv
	typedef struct _stMwsUv :public _stMwsToken
	{
		DFloat64 uv[2];
		_stMwsUv(){};
		virtual ~_stMwsUv(){};
	}stMwsUv;
	//color
	typedef struct _stMwsColor :public _stMwsToken
	{
		DFloat64 color[4];
		_stMwsColor(){};
		virtual ~_stMwsColor(){};
	}stMwsColor;
	//texture
	typedef struct _stMwsTexture :public _stMwsToken
	{
		char sName[64];
		_stMwsTexture(){ memset(sName, 0, sizeof(sName)); };
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
			DexMaterial& getDexMaterial(DexMaterial& material);
		}stMaterialLayerData;
		DString sName;
		DUInt16  iNumLayers;
		stMaterialLayerData* pData;
		_stMwsMaterial() :pData(nullptr){};
		virtual ~_stMwsMaterial() { delete[] pData; };
	}stMwsMaterial;
	//face
	typedef struct _stMwsFace :public _stMwsToken
	{
		DUInt16 iTimes; //暂时只支持iTimes==1的情况
		DUInt16 iVertices;
		DUInt16* pVertexIndex;
		DUInt16* pNormalIndex;
		DUInt16* pColorIndex;
		DUInt16* pUvIndex;
		DUInt16* pMaterialIndex;
		_stMwsFace(){ pVertexIndex = pNormalIndex = pColorIndex = pUvIndex = pMaterialIndex = nullptr; }
		virtual ~_stMwsFace() { delete[] pVertexIndex; delete[] pNormalIndex; delete[] pColorIndex; delete[] pUvIndex; delete[] pMaterialIndex; }
	}stMwsFace;
	//group
	typedef struct _stMwsGroup : public _stMwsToken
	{
		DString sName;
		DString sParentName;
		DUInt16  iFaceCount;
		DUInt16*  pFaceIndex;
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
		DInt16   iParentIndex;
		TreeType eTreeType;
		double translation[3];
		double rotation[3];
		double scale[3];

		DUInt16 iGroupIndex;
		DUInt16 iNumLods;
		double lodThresold[4];
		_stMwsTree* pFather;
		DVector<_stMwsTree*> children;
		DexMatrix4x4 matrix;
		_stMwsTree() { pFather = nullptr; iGroupIndex = 0; iNumLods = 0; }
		virtual ~_stMwsTree() { children.clear(); pFather = nullptr; }
		void buildMatrix();
		bool isShape();
	}stMwsTree;
protected:
	DUInt32 iMwsVersion;

	DUInt16 iVertexCount;
	DUInt16 iNormalCount;
	DUInt16 iUvCount;
	DUInt16 iColorCount;
	DUInt16 iTextureCount;
	DUInt16 iMaterialCount;
	DUInt16 iFaceCount;
	DUInt16 iGroupCount;
	DUInt16 iTreeCount;
	stMwsVertex*	pVertexData;
	stMwsNormal*	pNormalData;
	stMwsUv*		pUvData;
	stMwsColor*		pColorData;
	stMwsTexture*   pTextureData;
	stMwsMaterial*  pMaterialData;
	stMwsFace*		pFaceData;
	stMwsGroup*		pGroupData;
	stMwsTree*		pTreeData;
	stMwsTree*      pRootTree;
protected:
	int getNextInt(const char*& ptr, char splitChar[3]);
	bool getNextBool(const char*& ptr, char splitChar[3]);
	DFloat64 getNextDouble(const char*& ptr, char splitChar[3]);
	float getNextFloat(const char*& ptr, char splitChar[3]);
	void getNextString(const char*& ptr, DString& str);
	void getNextString(const char*& ptr, char* str);
protected:
	bool readModelData(const char* filename);
	void freeModelData();
	void buildTree();
	void AddMesh(stMwsTree* pTree, DexSkinMesh* pSkinMesh);

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
	virtual bool		  SupportType(const char* fileType);
	virtual DexModelBase* LoadModel(const char* filename, DInt32 flag);
	virtual bool		  SaveModel(DexSkinMesh* pSkinMesh, const char* filename, DInt32 flag);
};

#endif 