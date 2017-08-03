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
		DInt8	name[64];
		DFloat32 m_ambient[3];
		DFloat32 m_diffuse[3];
		DFloat32 m_specular[3];
		DFloat32 m_emissive[3];
		float   m_shininess;	
		DInt8	m_Illumination; //光照模型，DexMaterial暂不支持
		DInt8    m_texture[128];
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
	virtual DexModelBase* LoadModel(const char* filename, DInt32 flag);
	virtual bool SaveModel(DexSkinMesh* pSkinMesh, const char* filename, DInt32 flag);
private:
	bool readMaterial(const char* filename, DVector<ObjMaterial>& vecMaterial);
	void getToken(DInt8*& pBuffer, DInt8* pOut, DInt8* pEnd);
	void moveNextLine(DInt8*& pBuffer, DInt8* pEnd);
	
};

#endif 