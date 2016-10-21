
#ifndef _DEX_TEXTURE_MANAGER_H
#define _DEX_TEXTURE_MANAGER_H
#include "DexDMap.h"
#include "DexList.h"
#include "DexString.h"
#include "DexColor.h"

class DexTexture;
class DexTextureManager
{
private:
	typedef  DMap<std::string, DexTexture*>  TTexMap;
	DList<DString>  m_FilePath;
	TTexMap m_TexMap;

	static DexTextureManager* g_pManager;
private:
	DexTextureManager();
	~DexTextureManager();

public:
	static DexTextureManager* GetManager()
	{
		if (g_pManager == NULL)
			g_pManager = new DexTextureManager;
		return g_pManager;
	}
	DexTexture* CreateTexture(DString name, const DexColor& transcolor = DEXCOLOR_BLACK); //根据文件名字创建一个DexTexture,成功返回该指针，失败返回NULL
	DexTexture* FindTexure(DString  name);
	void		ReleaseTexture(DexTexture* texture);

};
#endif
