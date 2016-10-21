/*
DexEngine&Obj model loader
*/
#ifndef _DEX_MODEL_LOADER_OBJ_H
#define _DEX_MODEL_LOADER_OBJ_H
#include "DexModelLoader.h"
#include "../DexBase/DexDVector.h"
#include "../DexMath/DexVector3.h"


class DexModelObjLoader :public IDexModelLoader
{
public:
	struct ObjMaterial
	{
		int8	name[64];
		float32 m_ambient[3];
		float32 m_diffuse[3];
		float32 m_specular[3];
		float32 m_emissive[3];
		float   m_shininess;	
		int8	m_Illumination; //光照模型，DexMaterial暂不支持
		int8    m_texture[128];
		ObjMaterial()
		{
			memset(this, 0, sizeof(ObjMaterial));
		}
	};
public:
	DexModelObjLoader();
	virtual ~DexModelObjLoader();

public:
	virtual bool SupportType(const char* fileType);
	virtual DexModelBase* LoadModel(const char* filename, int32 flag);
	virtual bool SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int32 flag);
private:
	bool readMaterial(const char* filename, DVector<ObjMaterial>& vecMaterial);
	void getToken(int8*& pBuffer, int8* pOut, int8* pEnd);
	void moveNextLine(int8*& pBuffer, int8* pEnd);
	
};

#endif 