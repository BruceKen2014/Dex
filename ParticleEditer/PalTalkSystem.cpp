

#include "widget/DexWidgetImage.h"
#include "widget/DexWidgetLabel.h"
#include "widget/DexWidgetImageSequence.h"
#include "widget/DexWidgetFactory.h"

#include "state/DexGameEngine.h"
#include "Source/CCommandScript.h"
#include "PalTalkSystem.h"

CPalTalkSystem* CPalTalkSystem::g_pSystem = NULL;

int Lua_Talk_Wait(lua_State* L)
{
	CPalTalkSystem* pTalkSystem = CPalTalkSystem::getTalkSystem();
	if(pTalkSystem == NULL)		return 0;
	int time = lua_tointeger(L,1);
	pTalkSystem->Wait(time);
	return 0;
}

int Lua_Talk_Say(lua_State* L)
{
	CPalTalkSystem* pTalkSystem = CPalTalkSystem::getTalkSystem();
	if(pTalkSystem == NULL)		return 0;
	int dir = lua_tointeger(L, 1);
	int headId = lua_tointeger(L, 2);
	if(dir == 0)
		pTalkSystem->setHeadLeft((CPalTalkSystem::EHead)headId);
	else
		pTalkSystem->setHeadRight((CPalTalkSystem::EHead)headId);
	char pilot[1024];
	strcpy(pilot, lua_tostring(L,3));
	pTalkSystem->SetPilot(pilot);
	return 0;
}

CPalTalkSystem::CPalTalkSystem()
{
	m_bContinue = false;
	m_bCurrScriptIsOver = false;
	m_iCurLength = 0;
	m_iWaitingTime = 0;
	m_iLuaCurrCommandIndex = 0;
	for(int i = 0 ; i < EH_TOTAL; i++)
	{
		m_headArray[i] = NULL;
	}
}
CPalTalkSystem::~CPalTalkSystem()
{

}

bool CPalTalkSystem::Initialize()
{
	m_pBackImage = (DexWidgetImage* )getWidgetFactory()->findWidget("TalkBackground");
	m_headArray[EH_JINGTIAN] = (DexWidgetImage* )getWidgetFactory()->findWidget("TalkBackground.jingtian");
	m_headArray[EH_JINGJING] = (DexWidgetImage* )getWidgetFactory()->findWidget("TalkBackground.jingjing");
	m_headArray[EH_ZHULONG] = (DexWidgetImage* )getWidgetFactory()->findWidget("TalkBackground.zhulong");
	m_pPilot = (CDexWidgetLabel* )getWidgetFactory()->findWidget("TalkBackground.label");
	m_pClickToContinue = (CDexWidgetImageSequence* )getWidgetFactory()->findWidget("TalkBackground.continue");
	m_pPilot->SetText("");
	m_pClickToContinue->setVisible(false);
	m_words = "dfafffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
	setHeadRight(EH_JINGTIAN);
	return true;
}

bool CPalTalkSystem::_IsCompleteThisSentence()
{
	return m_iCurLength == m_words.length();
}

bool CPalTalkSystem::IsCompleteThisTalkScript()
{
	return m_bCurrScriptIsOver;
}

CPalTalkSystem* CPalTalkSystem::getTalkSystem()
{
	if(g_pSystem == NULL)
	{
		g_pSystem = new CPalTalkSystem;
		g_pSystem->Initialize();
		CPalTalkSystem::registerLuaFunction(DexGameEngine::getEngine()->GetLuaState());
	}
	return g_pSystem;
}

void CPalTalkSystem::registerLuaFunction(lua_State* L)
{
	lua_register(L, "Wait", Lua_Talk_Wait);
	lua_register(L, "Say", Lua_Talk_Say);
}
bool CPalTalkSystem::Update(int delta)
{
	DEX_ENSURE_B(DexObject::Update(delta));
	m_pBackImage->Update(delta);
	if(!_IsCompleteThisSentence())
	{
		int length = m_words.length();
		DInt64 thisTime = DexTime::getSingleton()->GetTotalMillSeconds() ;
		int wordPerSecond = 25;
		if(thisTime- m_iLastTime > 1000/wordPerSecond)
		{//Ã¿Ãë25¸öºº×Ö
			m_iLastTime = thisTime;
			m_iCurLength += 2;		
			string str = m_words.substr(0, m_iCurLength);
			if(m_iCurLength >= length)
			{
				m_iCurLength = length;
				str = m_words;
				m_pClickToContinue->setVisible(true);
			}
			m_pPilot->SetText(str);
		}
	}
	DEX_ENSURE_B(_IsCompleteThisSentence() && m_bContinue);
	if(!IsCompleteThisTalkScript())
	{
		m_iWaitingTime -= delta;
		if(m_iWaitingTime <= 0) 
		{
			if(m_iLuaCurrCommandIndex < m_luaCommands.size())
			{
				DexGameEngine::getEngine()->DoLuaString(m_luaCommands[m_iLuaCurrCommandIndex].c_str());
				m_iLuaCurrCommandIndex++;
			}
			else
			{
				m_luaCommands.clear();
				m_bCurrScriptIsOver = true;
				
				m_pClickToContinue->setVisible(false);
				m_pPilot->SetText("");
				for(int i = 0 ; i < EH_TOTAL; i++)
				{
					if(m_headArray[i] == NULL)
						continue;
					m_headArray[i]->setVisible(false);
				}

			}
		}
	}

	return true;
}

void CPalTalkSystem::Render()
{
	DEX_ENSURE(!IsCompleteThisTalkScript());
	m_pBackImage->Render();
}

void CPalTalkSystem::OnMouseLClick(int xPos, int yPos)
{
	CEventSystem::getEventSystem()->sendEvent(120);
	if(_IsCompleteThisSentence())
	{
		m_bContinue = true;
	}
	else
	{
		m_iCurLength = m_words.length();
		m_pPilot->SetText(m_words);
		m_pClickToContinue->setVisible(true);
	}
}

void CPalTalkSystem::SetPilot(char* str)
{
	m_words = str;
	m_bContinue = false;
	m_iCurLength = 0;
	m_pClickToContinue->setVisible(false);
}

void CPalTalkSystem::Wait(int time)
{
	m_iWaitingTime = time;
}

void CPalTalkSystem::setHeadLeft(EHead headIndex)
{
	m_headArray[headIndex]->SetMirro(true);
	for(int i = 0 ; i < EH_TOTAL; i++)
	{
		if(m_headArray[i] == NULL)
			continue;
		if( (EHead)i == headIndex)
		{
			//set position
			m_headArray[i]->SetPos(0,300);
			m_pPilot->SetPos(250, 420);
			m_pClickToContinue->SetPos(700, 510);
			m_headArray[i]->setVisible(true);
		}
		else
		{
			m_headArray[i]->setVisible(false);
		}
	}
}

void CPalTalkSystem::setHeadRight(EHead headIndex)
{
	m_headArray[headIndex]->SetMirro(false);
	for(int i = 0 ; i < EH_TOTAL; i++)
	{
		if(m_headArray[i] == NULL)
			continue;
		if( (EHead)i == headIndex)
		{
			//set position
			m_headArray[i]->SetPos(555,300);
			m_pPilot->SetPos(25, 420);
			m_pClickToContinue->SetPos(475, 510);
			m_headArray[i]->setVisible(true);
		}
		else
		{
			m_headArray[i]->setVisible(false);
		}
	}
}

bool CPalTalkSystem::loadLuaFile(const char* filename)
{
	m_luaCommands.clear();
	m_iLuaCurrCommandIndex = 0;
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		pScript->CloseScript();
		return false;
	}
	char content[256];
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{
		pScript->GetTotalLine(content);
		m_luaCommands.push_back(content);
	}
	pScript->CloseScript();
	m_bCurrScriptIsOver = false;
	return true;
}
