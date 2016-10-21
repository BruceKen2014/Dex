
#ifndef _DEX_MODEL_LOADER_H
#define _DEX_MODEL_LOADER_H

#include "../DexBase/DexType.h"
#include "../DexBase/DexString.h"
class DexModelBase;
class DexSkinMesh;
class IDexModelLoader
{
protected:
	struct stIndex
	{
		int32 m_iPosIndex;
		int32 m_iNormalIndex;
		int32 m_iUvIndex;
		int32 m_iColorIndex;
		stIndex(){ m_iPosIndex = m_iNormalIndex = m_iUvIndex = m_iColorIndex = 0; }
		stIndex(int32 i1, int32 i2, int i3) :m_iPosIndex(i1), m_iNormalIndex(i2), m_iUvIndex(i3), m_iColorIndex(0){}
		stIndex(int32 i1, int32 i2, int i3, int i4) :m_iPosIndex(i1), m_iNormalIndex(i2), m_iUvIndex(i3), m_iColorIndex(i4){}
		bool operator == (const stIndex& index)	const
		{
			return m_iPosIndex == index.m_iPosIndex &&
				m_iNormalIndex == index.m_iNormalIndex &&
				m_iUvIndex == index.m_iUvIndex &&
				m_iColorIndex == m_iColorIndex;
		}
		bool operator < (const stIndex& index)	const
		{
			if (m_iPosIndex < index.m_iPosIndex)
				return true;
			if (m_iPosIndex > index.m_iPosIndex)
				return false;
			if (m_iNormalIndex < index.m_iNormalIndex)
				return true;
			if (m_iNormalIndex > index.m_iNormalIndex)
				return false;
			if (m_iUvIndex < index.m_iUvIndex)
				return true;
			if (m_iUvIndex > index.m_iUvIndex)
				return false;
			if (m_iColorIndex < index.m_iColorIndex)
				return true;
			if (m_iColorIndex > index.m_iColorIndex)
				return false;
			return false;
		}
	};
protected:
	DString m_strFilePath;
public:
	IDexModelLoader(){};
	virtual ~IDexModelLoader(){};

public:
	virtual bool		  SupportType(const char* fileType) = 0;
	virtual DexModelBase* LoadModel(const char* filename, int32 flag) = 0;
	virtual bool		  SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int32 flag) = 0;
};
#endif