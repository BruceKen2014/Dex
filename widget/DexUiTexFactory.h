

#pragma once

#include "../DexBase/typedefine.h"
#include <map>
#include <list>
#include <string>
//图片工厂

enum
{
	ui_src_state_none,
	ui_src_state_interface,  //界面UI资源
	ui_src_state_town,       //主游戏UI资源
};
using namespace std;
class CDexTex;
class CDexUiTexFactory
{
protected:
	static std::list<string>  m_FilePath;   //路径名称(为所有texfactory公用)
	typedef std::map<std::string, CDexTex*> tTexMap;
	tTexMap m_TexMap;  //记录创建的指针
	int     m_state;
	string  m_filename; //该资源工厂对应的配置文件
public:
	CDexUiTexFactory(int state, const char* filename);
	~CDexUiTexFactory();

public:
	static void AddFilePath(string path); //添加纹理路径
	int getState() {return m_state;};
	bool LoadTexResource(); //文件m_filename中包含了该状态需要加载的资源文件的名称
	void ReleaseSourse();
	CDexTex* AddUiTex(string name, const DexColor& transcolor = NULL); //根据文件名字创建一个CDexTex,成功返回该指针，失败返回NULL
	CDexTex* FindTex(string  name);
};