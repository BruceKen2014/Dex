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
//
//
//class CDexTex:public CRefCount
//{//在DX texture的基础上加入宽高成员
//private:	 
//	LPDIRECT3DTEXTURE9 m_tex;
//	string name;
//	int m_width; 
//	int m_height;
//
//public:
//	CDexTex();
//	virtual ~CDexTex();
//public:
//	bool LoadTex(string filname, const DexColor& transcolor = DEXCOLOR_BLACK);
//	int  GetWidth() { return m_width;};
//	int  GetHeight() { return m_height;};
//	LPDIRECT3DTEXTURE9 GetTexPt() { return m_tex;};
//	string Name()   { return name;};
//};
//
class CDexTex;
class DexTextureManager
{
private:
	typedef  map<std::string, CDexTex*>  TTexMap;
	std::list<string>  m_FilePath;
	TTexMap m_TexMap;

	static DexTextureManager* g_pManager;
private:
	DexTextureManager();
	~DexTextureManager();

public:
	static DexTextureManager* GetManager()
	{
		if(g_pManager == NULL)
			g_pManager = new DexTextureManager;
		return g_pManager;
	}
	void AddFilePath(std::string path); //添加纹理路径
	bool LoadTexResource(std::string file); 
	void ReleaseSourse();
	CDexTex* AddUiTex(std::string name, const DexColor& transcolor = DEXCOLOR_BLACK); //根据文件名字创建一个CDexTex,成功返回该指针，失败返回NULL
	CDexTex* FindTex(std::string  name);

};

#endif