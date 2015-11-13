

#include "../DexBase/typedefine.h"
#include "DexScriptMgr.h"
#include "DexExpression.h"



CDexScriptMgr::CDexScriptMgr()
{
	m_iWaitTime = 0;
	m_iScriptIndex = 0;
	m_bGoing = true;
}

CDexScriptMgr::~CDexScriptMgr()
{
}

bool CDexScriptMgr::LoadScriptFile(const char* filename)
{
	return true;
}

void CDexScriptMgr::Run(int delta)
{
	DEX_ENSURE(m_bGoing);
	if(m_iWaitTime != 0)
	{
		m_iWaitTime -= delta;
		m_iWaitTime = m_iWaitTime < 0 ? 0 : m_iWaitTime;
	}
	else
	{
		//执行一行脚本
		DEX_ENSURE(m_iScriptIndex < m_scriptVec.size());
		ExcuteExp(m_scriptVec[m_iScriptIndex].c_str());
		m_iScriptIndex++;
	}
}

void CDexScriptMgr::AddCommand(string str)
{
	m_scriptVec.push_back(str);
}

void CDexScriptMgr::ExeCommand(string command)
{
	 ExcuteExp(command.c_str());
}


void CDexScriptMgr::setWaitTime(int wait_time)
{
	m_iWaitTime = wait_time;
}

int CDexScriptMgr::getWaitTime()
{
	return m_iWaitTime;
}

void CDexScriptMgr::Pause()
{
	m_bGoing = false;
}

void CDexScriptMgr::KeepGoing()
{
	m_bGoing = true;
}