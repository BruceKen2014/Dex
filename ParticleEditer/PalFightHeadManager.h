
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
	bool AddFightHead(PalPlayer* player); //����ս��ͷ����Դ�����ڲ��������ڲ�����

public:
	DexPointF getPosition(float value); //���ݴ����ͷ����ȣ�����Ӧ�����ڵ�λ��
	bool Update();
	bool Render();
};

#endif