


#include "DexTexture.h"
#include "DexLog.h"
#include "..\state\DexGameEngine.h"
//
//
//CDexTex::CDexTex()
//{
//	m_tex = NULL;
//	m_width = m_height = 0;
//}
//CDexTex::~CDexTex()
//{
//	_SafeRelease(m_tex);
//}
//
//bool CDexTex::LoadTex(string filename, const DexColor& transcolor)
//{
//	name = filename;
//	D3DXIMAGE_INFO info;
//	if(D3DXCreateTextureFromFileEx(DexGameEngine::getEngine()->GetDevice(), filename.c_str() , D3DX_DEFAULT_NONPOW2/*ʹ�ô˲����Ų���Ŵ�ʹ��0����D3DX_DEFAULTͼƬ�ᱻ�Ŵ�һ��*/
//		, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
//		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
//		getD3DColor(transcolor), &info, NULL, &m_tex) != D3D_OK)
//	{
//		//_Message("��������%sʧ��!",filename);
//		return false;
//	}
//	m_width = info.Width;
//	m_height = info.Height;
//
//	return true;
//}
//
DexTextureManager* DexTextureManager::g_pManager = NULL;

DexTextureManager::DexTextureManager()
{

}
DexTextureManager::~DexTextureManager()
{
	ReleaseSourse();
}

void DexTextureManager::AddFilePath(string path)
{
	m_FilePath.push_back(path);
}

CDexTex* DexTextureManager::AddUiTex(std::string  name, const DexColor& transcolor)
{
	//TTexMap::iterator it = m_TexMap.find(name);
	//if(it != m_TexMap.end())  //����Դ�Ѿ�����
	//	return it->second;
	//CDexTex* tex = new CDexTex();
	//getLog()->BeginLog();
	//string true_name;
	//for(std::list<string>::iterator it = m_FilePath.begin(); it != m_FilePath.end(); it ++)
	//{
	//	true_name.clear();
	//	true_name = (*it);
	//	true_name += name;
	//	if(tex->LoadTex(true_name.c_str(), transcolor))
	//	{
	//		getLog()->Log(log_ok, "DexTextureManager::AddUiTex(%s)�ɹ���", true_name.c_str());
	//		getLog()->EndLog();
	//		m_TexMap[name] = tex;
	//		return tex;
	//	}
	//}
	//getLog()->Log(log_allert, "DexTextureManager::AddUiTex(%s)ʧ�ܣ�", true_name.c_str());
	//getLog()->EndLog();
	//delete tex;
	return NULL;
}

CDexTex* DexTextureManager::FindTex(std::string  name)
{
	TTexMap::iterator it = m_TexMap.find(name);
	if(it != m_TexMap.end()) 
		return it->second;
	return NULL;
}

bool DexTextureManager::LoadTexResource(std::string file)
{
	////������Դ
	//CommandScript* pScript = getComandScript();
	//if(!pScript->OpenScript((char*)m_filename.c_str()))
	//	return false;
	//char ** Script = pScript->GetScript();
	//int  CurrChar = 0;
	//int  CurrLine = 0;
	//for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	//{	
	//	pScript->SetCurrChar() ;
	//	CurrChar = pScript->GetCurrChar();
	//	CurrLine = pScript->GetCurrLine(); 
	//	if(strlen(Script[CurrLine]) == 1 || 
	//		(Script[CurrLine][CurrChar] == '-' 
	//		&& Script[CurrLine][CurrChar+1] == '-'))  	//�Թ�ע�ͻ�հ���
	//		continue;
	//	//ȡ��ͼƬ�ַ���
	//	char pic[MAX_PARAM_SIZE];
	//	pScript->GetStringParam(pic);

	//	AddUiTex((const char*)pic);
	//}
	//pScript->CloseScript();
	return true;
}
void DexTextureManager::ReleaseSourse()
{
	//for(TTexMap::iterator it = m_TexMap.begin(); it != m_TexMap.end(); it++)
	//{
	//	_REDUCEREF(it->second);
	//	if(it->second == NULL)
	//		it = m_TexMap.begin();
	//}
	//m_TexMap.clear();
}