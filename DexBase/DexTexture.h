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
//{//��DX texture�Ļ����ϼ����߳�Ա
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
	void AddFilePath(std::string path); //�������·��
	bool LoadTexResource(std::string file); 
	void ReleaseSourse();
	CDexTex* AddUiTex(std::string name, const DexColor& transcolor = DEXCOLOR_BLACK); //�����ļ����ִ���һ��CDexTex,�ɹ����ظ�ָ�룬ʧ�ܷ���NULL
	CDexTex* FindTex(std::string  name);

};

#endif