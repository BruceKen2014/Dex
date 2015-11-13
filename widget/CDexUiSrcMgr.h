

#pragma once

#include <map>

#include "../DexBase/typedefine.h"
#include "DexUiTexFactory.h"
#include "../DexBase/CReference.h"

using namespace std;

class CDexUiSrcMgr
{
public:
	CDexUiSrcMgr();
	~CDexUiSrcMgr();

protected:
	int  m_currState;
	typedef map<int, CDexUiTexFactory*> tTexFactoryMap;
	tTexFactoryMap m_SrcMap;
public:
	CDexUiTexFactory* getUiTexFactory();  //���ݵ�ǰ��ͬ����Ϸ״̬�����ز�ͬ����Դ����

	//����һ����Ϸ״̬�͸�״̬����Ӧ��UI��Դ�����ļ����½�һ����Դ����
	//���ɵĹ�����δʵ��������Դ��ֻ���ڵ���setCurrStateʱ�Ż������Ӧ״̬����Դ�����ͷ�����״̬����Դ
	CDexUiTexFactory* createUiTexFactory(int state, const char* filename); 

	void              setCurrState(int state);


};
 extern CDexUiSrcMgr*getUiSrcMgrSingleton();
