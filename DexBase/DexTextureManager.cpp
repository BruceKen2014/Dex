
#include "DexTexture.h"
#include "DexTextureManager.h"
#include "DexType.h"
#include "..\state\DexGameEngine.h"
#include "DexDeviceDirectx9.h"
#include "DexTextureDx9.h"
#include "DexTextureNull.h"

DexTextureManager* DexTextureManager::g_pManager = NULL;

DexTextureManager::DexTextureManager()
{

}
DexTextureManager::~DexTextureManager()
{//�@�rһ������������ֻ������Y���ĕr���N�����@���r�����е��YԴ����ԓ������Ո�N���ˣ�����ԓ߀�����YԴ���@�e
}

DexTexture* DexTextureManager::CreateTexture(DString name, const DexColor& transcolor /* = DEXCOLOR_BLACK */)
{
	DexTexture* ret = DexNull;
	if (DexGameEngine::getEngine()->GetDDevice() == DexNull)
		ret = new DexTextureNull;
	else if (DexGameEngine::getEngine()->GetDDevice()->getType() == DexDeviceDirectx9::getClassType())
	{
		ret = new DexTextureDx9();
	}
	ret->LoadTexture(name, transcolor);
	m_TexMap[name] = ret;
	return ret;
}

DexTexture* DexTextureManager::FindTexure(DString name)
{
	TTexMap::iterator ite;
	for (ite = m_TexMap.begin(); ite != m_TexMap.end(); ++ite)
	{
		if (ite->first == name)
			return ite->second;
	}
	return DexNull;
}

void DexTextureManager::ReleaseTexture(DexTexture* texture)
{
	DEX_ENSURE(texture != DexNull);
	TTexMap::iterator ite;
	for (ite = m_TexMap.begin(); ite != m_TexMap.end(); ++ite)
	{
		if (ite->first == texture->Name())
		{
			texture->ReduceRef();
			if (texture->GetRefCount() == 0)
			{
				_SafeDelete(texture);
				m_TexMap.erase(ite);
			}
			break;
		}
	}
}
