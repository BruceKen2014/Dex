

#include "../Source/CTexture.h"
#include "../Source/CCommandScript.h"
#include "../DexBase/DexLog.h"
#include "DexUiTexFactory.h"

std::list<string> CDexUiTexFactory::m_FilePath;
CDexUiTexFactory::CDexUiTexFactory(int state, const char* filename)
{
	m_state = state;
	m_filename = filename;
	m_TexMap.clear();
}

CDexUiTexFactory::~CDexUiTexFactory()
{
	ReleaseSourse();
}

void CDexUiTexFactory::AddFilePath(string path)
{
	m_FilePath.push_back(path);
}

CDexTex* CDexUiTexFactory::AddUiTex(string  name, const DexColor& transcolor)
{
	tTexMap::iterator it = m_TexMap.find(name);
	if(it != m_TexMap.end())  //该资源已经存在
		return it->second;
	CDexTex* tex = new CDexTex();
	DexLog::getSingleton()->BeginLog();
	string true_name;
	for(std::list<string>::iterator it = m_FilePath.begin(); it != m_FilePath.end(); it ++)
	{
		true_name.clear();
		true_name = (*it);
		true_name += name;
		if(tex->LoadTex(true_name.c_str(), transcolor))
		{
			DexLog::getSingleton()->Log(log_ok, "CDexUiTexFactory::AddUiTex(%s)成功！", true_name.c_str());
			DexLog::getSingleton()->EndLog();
			m_TexMap[name] = tex;
			return tex;
		}
	}
	DexLog::getSingleton()->Log(log_allert, "CDexUiTexFactory::AddUiTex(%s)失败！", true_name.c_str());
	DexLog::getSingleton()->EndLog();
	delete tex;
	return NULL;
}

CDexTex* CDexUiTexFactory::FindTex(string  name)
{
	tTexMap::iterator it = m_TexMap.find(name);
	if(it != m_TexMap.end()) 
		return it->second;
	return NULL;
}

bool CDexUiTexFactory::LoadTexResource()
{
	//加载资源
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(m_filename.c_str()))
	{
		pScript->CloseScript();
		return false;
	}
	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		CurrChar = pScript->GetCurrChar();
		CurrLine = pScript->GetCurrLine(); 
		if(strlen(Script[CurrLine]) == 1 || 
			(Script[CurrLine][CurrChar] == '-' 
			&& Script[CurrLine][CurrChar+1] == '-'))  	//略过注释或空白行
			continue;
		//取出图片字符串
		char pic[MAX_PARAM_SIZE];
		pScript->GetStringParam(pic);

		AddUiTex((const char*)pic);
	}
	pScript->CloseScript();
	return true;
}
void CDexUiTexFactory::ReleaseSourse()
{
	for(tTexMap::iterator it = m_TexMap.begin(); it != m_TexMap.end(); it++)
	{
		_REDUCEREF(it->second);
		if(it->second == NULL)
			it = m_TexMap.begin();
	}
	m_TexMap.clear();
}