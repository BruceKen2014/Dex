
#ifndef _PAL_FIGHT_HEADMANAGER_H
#define _PAL_FIGHT_HEADMANAGER_H

#include "DexBase/CDexObject.h"
#include "widget/DexGuiStruct.h"

#include "PalTypeDefine.h"
class PalPlayer;
class CDexWidgetImage;
class PalFightHeadManager: public CDexObject
{
	friend class PalGameStateBattleMain;
private:
	TlistFlighHead  m_listHead;
	DInt16   m_xPos;
	DInt16   m_yPos;
	DInt16    m_iStartOffsetX;
	DInt16    m_iStartOffsetY;
	DInt16    m_iEndOffsetX;
	DInt16    m_iEndOffsetY;
	CDexWidgetImage* m_pBackImage;
private:
	PalFightHeadManager();
	~PalFightHeadManager();

private:
	bool Init(); 
public:
	//static PalFightHeadManager* getManager();
public:
	void ReleaseFightHead();
	bool AddFightHead(PalPlayer* player); //所有战斗头像资源都由内部创建，内部销毁

public:
	DexPointF getPosition(float value); //根据传入的头像进度，返回应该所在的位置
	bool Update();
	bool Render();
};

#endif