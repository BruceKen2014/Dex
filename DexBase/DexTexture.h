//
//
//
#ifndef _DEX_TEXTURE_H
#define _DEX_TEXTURE_H

#include <map>
#include <list>
#include "typedefine.h"
#include "CReference.h"
#include "DexColor.h"
#include "DexString.h"


class DexTexture:public CRefCount
{
protected:
	DInt8*	m_pData;
	DString name;
	int m_width; 
	int m_height;

public:
	DexTexture();
	virtual ~DexTexture();
public:
	virtual bool LoadTexture(DString filname, const DexColor& transcolor = DEXCOLOR_BLACK) = 0;
	int  GetWidth() const; 
	int  GetHeight() const;
	DInt8* GetData();
	DString Name()  const; //包括路径的纹理名称
	DString FileName();//仅仅取纹理的文件名 不包括路径
};

#endif