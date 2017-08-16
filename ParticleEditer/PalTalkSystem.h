


#ifndef _PAL_TALK_SYSTEM_H
#define _PAL_TALK_SYSTEM_H 

#include "DexBase/DexObject.h"
class DexWidgetImage;
class CDexWidgetLabel;
class CDexWidgetImageSequence;
#define MAX_WORDS_LENGTH 1024

extern int Lua_Talk_Wait(lua_State* L);
extern int Lua_Talk_Say(lua_State* L);
class CPalTalkSystem:DexObject
{
	Dex_DeclareClass(CPalTalkSystem, DexObject,1)
public:
	typedef enum EHead
	{
		EH_JINGTIAN,
		EH_JINGJING,
		EH_ZHULONG,
		EH_BEAR,
		EH_THINNER,
		EH_TOTAL
	};
private:
	static CPalTalkSystem*   g_pSystem;
	std::vector<std::string> m_luaCommands;  //当前脚本的命令行
	string  m_words;      //要显示的文字
	bool   m_bContinue;//剧情文字显示完成后且鼠标单击后才继续
	bool   m_bCurrScriptIsOver;
	short                    m_iLuaCurrCommandIndex;
	float m_iCurLength;  //当前已显示的长度
	short m_iWaitingTime; //脚本暂停的时间
	DInt64 m_iLastTime;
	bool			  m_bShowLeftHead;
	DexWidgetImage*  m_headArray[EH_TOTAL];
	DexWidgetImage*  m_pBackImage;//背景图
	CDexWidgetLabel*  m_pPilot;  //对话label
	CDexWidgetImageSequence* m_pClickToContinue;

private:
	CPalTalkSystem();
	~CPalTalkSystem();
	bool Initialize();
	
	bool _IsCompleteThisSentence(); //本句是否完成
public:
	static CPalTalkSystem* getTalkSystem();
	static void registerLuaFunction(lua_State* L);
	bool IsCompleteThisTalkScript();//本次对话脚本是否完成
	void setHeadLeft(EHead headIndex);
	void setHeadRight(EHead headIndex);
	bool loadLuaFile(const char* filename);
	bool Update(int delta);
	void Wait(int time);
	void Render();
	void OnMouseLClick(int xPos, int yPos);

	void SetPilot(char* str);
};
#endif