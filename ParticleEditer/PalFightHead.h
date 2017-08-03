
#ifndef _PAL_FIGHTHEAD_H
#define _PAL_FIGHTHEAD_H

#include "DexBase/CDexObject.h"


class PalPlayer;
class CDexWidgetImage;
class PalFightHead:public CDexObject
{
	Dex_DeclareClass(PalFightHead, CDexObject,1)
private:
	CDexWidgetImage* m_headImage;
	float m_value; //��ǰͷ����� 0 ��ʾ�տ�ʼ 1��ʾ�Ѿ�����ս��
	PalPlayer* m_pPlayer;
public:
	PalFightHead(PalPlayer* player);
	virtual ~PalFightHead();
public:
	CDexWidgetImage* getHeadImage(){ return m_headImage;}
	PalPlayer* getPlayer();

public:
	void setValue(float v) { m_value = v;}
	float getValue() { return m_value;}
	virtual bool Update();
	virtual bool Render();
};

#endif