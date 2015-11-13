

#include "CDexUiSrcMgr.h"


CDexUiSrcMgr::CDexUiSrcMgr()
{
	m_currState = ui_src_state_none;
}

CDexUiSrcMgr::~CDexUiSrcMgr()
{
}

CDexUiTexFactory* CDexUiSrcMgr::getUiTexFactory()
{
	tTexFactoryMap::iterator it = m_SrcMap.find(m_currState);
	if(it == m_SrcMap.end())
	{//未找到对应状态的资源工厂
		return NULL;	
	}
	return it->second;
}

CDexUiTexFactory* CDexUiSrcMgr::createUiTexFactory(int state, const char* filename)
{
	tTexFactoryMap::iterator it = m_SrcMap.find(m_currState);
	if(it != m_SrcMap.end())
	{//该状态已经存在
		return NULL;
	}
	CDexUiTexFactory* factory = new CDexUiTexFactory(state, filename);
	m_SrcMap[state] = factory;
	return factory;
}

void CDexUiSrcMgr::setCurrState(int state)
{
	m_currState = state;
	tTexFactoryMap::iterator it = m_SrcMap.begin();
	for(; it != m_SrcMap.end(); it++)
	{
		if(it->second->getState() == m_currState)
		{
			it->second->LoadTexResource();
		}
		else
		{
			it->second->ReleaseSourse();
		}
	}
}

CDexUiSrcMgr* getUiSrcMgrSingleton()
{
	static CDexUiSrcMgr* g_uiSrcMgr = new CDexUiSrcMgr();
	return g_uiSrcMgr;
}