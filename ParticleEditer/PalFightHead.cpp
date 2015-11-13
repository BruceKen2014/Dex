
#include "widget/DexWidgetFactory.h"
#include "widget/DexWidgetImage.h"
#include "PalTypeDefine.h"
#include "PalFightHead.h"
#include "PalPlayer.h"
PalFightHead::PalFightHead(PalPlayer* player)
{
	m_pPlayer = player;
	m_value = 0.0f;
	player->setHeadFlag(this);
	const stPalPlayerData* data = m_pPlayer->getPlayerData();
	m_headImage = (CDexWidgetImage*)getWidgetFactory()->createWidget(DexGUI::widget_image, "");


	m_headImage->SetTexName(data->headImageName);
	DexGUI::DexRect rect= data->headImageRect;
	m_headImage->SetTexRes(rect);
	m_headImage->Resize(data->headSize);
}
PalFightHead::~PalFightHead()
{

}
PalPlayer* PalFightHead::getPlayer()
{
	return m_pPlayer;
}

bool PalFightHead::Update()
{
	int delta_time = getTime()->getDeltaTime();
	CDexObject::Update(delta_time);
	m_headImage->Update(delta_time);
	return true;
}

bool PalFightHead::Render()
{
	m_headImage->Render();
	return true;
}