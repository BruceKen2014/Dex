

#pragma once

#include "../DexBase/typedefine.h"
#include <map>
#include <list>
#include <string>
//ͼƬ����

enum
{
	ui_src_state_none,
	ui_src_state_interface,  //����UI��Դ
	ui_src_state_town,       //����ϷUI��Դ
};
using namespace std;
class CDexTex;
class CDexUiTexFactory
{
protected:
	static std::list<string>  m_FilePath;   //·������(Ϊ����texfactory����)
	typedef std::map<std::string, CDexTex*> tTexMap;
	tTexMap m_TexMap;  //��¼������ָ��
	int     m_state;
	string  m_filename; //����Դ������Ӧ�������ļ�
public:
	CDexUiTexFactory(int state, const char* filename);
	~CDexUiTexFactory();

public:
	static void AddFilePath(string path); //�������·��
	int getState() {return m_state;};
	bool LoadTexResource(); //�ļ�m_filename�а����˸�״̬��Ҫ���ص���Դ�ļ�������
	void ReleaseSourse();
	CDexTex* AddUiTex(string name, const DexColor& transcolor = NULL); //�����ļ����ִ���һ��CDexTex,�ɹ����ظ�ָ�룬ʧ�ܷ���NULL
	CDexTex* FindTex(string  name);
};