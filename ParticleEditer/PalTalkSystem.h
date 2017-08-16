


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
	std::vector<std::string> m_luaCommands;  //��ǰ�ű���������
	string  m_words;      //Ҫ��ʾ������
	bool   m_bContinue;//����������ʾ��ɺ�����굥����ż���
	bool   m_bCurrScriptIsOver;
	short                    m_iLuaCurrCommandIndex;
	float m_iCurLength;  //��ǰ����ʾ�ĳ���
	short m_iWaitingTime; //�ű���ͣ��ʱ��
	DInt64 m_iLastTime;
	bool			  m_bShowLeftHead;
	DexWidgetImage*  m_headArray[EH_TOTAL];
	DexWidgetImage*  m_pBackImage;//����ͼ
	CDexWidgetLabel*  m_pPilot;  //�Ի�label
	CDexWidgetImageSequence* m_pClickToContinue;

private:
	CPalTalkSystem();
	~CPalTalkSystem();
	bool Initialize();
	
	bool _IsCompleteThisSentence(); //�����Ƿ����
public:
	static CPalTalkSystem* getTalkSystem();
	static void registerLuaFunction(lua_State* L);
	bool IsCompleteThisTalkScript();//���ζԻ��ű��Ƿ����
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