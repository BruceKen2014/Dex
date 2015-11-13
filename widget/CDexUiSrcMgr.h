

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
	CDexUiTexFactory* getUiTexFactory();  //根据当前不同的游戏状态，返回不同的资源工厂

	//传入一个游戏状态和该状态所对应的UI资源配置文件，新建一个资源工厂
	//生成的工厂并未实际载入资源，只有在调用setCurrState时才会载入对应状态的资源，并释放其他状态的资源
	CDexUiTexFactory* createUiTexFactory(int state, const char* filename); 

	void              setCurrState(int state);


};
 extern CDexUiSrcMgr*getUiSrcMgrSingleton();
