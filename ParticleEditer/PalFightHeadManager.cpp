


#include "widget/DexWidgetFactory.h"
#include "widget/DexWidgetImage.h"

#include "PalPlayer.h"
#include "PalFightHeadManager.h"
#include "PalFightHead.h"

PalFightHeadManager::PalFightHeadManager()
{
	m_xPos = m_yPos = 0;
	m_iStartOffsetX = 30;
	m_iStartOffsetY = 20;
	m_iEndOffsetX = 340; 
	m_iEndOffsetY = 20; 
	m_pBackImage = NULL;
}
PalFightHeadManager::~PalFightHeadManager()
{

}

//PalFightHeadManager* PalFightHeadManager::getManager()
//{
//	static PalFightHeadManager* g_panel = NULL;
//	if(g_panel == NULL)
//	{
//	    g_panel = new PalFightHeadManager;
//		g_panel->Init();
//	}
//	return g_panel;
//}

bool PalFightHeadManager::Init()
{
	m_pBackImage = (DexWidgetImage*)getWidgetFactory()->createWidget(DexGUI::widget_image, "flag_back_image");
	
	
	m_pBackImage->SetTexName("image1.png");
	m_pBackImage->SetTexRes(DexGUI::DexRect(3,3,380,40));
	m_pBackImage->Resize(DexGUI::DexSize(373, 37));
	m_pBackImage->SetPos(400,10);
	return true;
}
void PalFightHeadManager::ReleaseFightHead()
{

}

bool PalFightHeadManager::AddFightHead(PalPlayer* player)
{
	//这里申请了资源 在对出战斗时要释放
	PalFightHead* head = new PalFightHead(player);
	head->setValue(0.0f);
	DexGUI::DexPointF pt = getPosition(head->getValue());
	head->getHeadImage()->SetPos(pt.x, pt.y); 
	head->getHeadImage()->setDrawFlag(DexGUI::DRAW_TOP_CENTER);
	m_listHead.push_back(head);
	return true;
}

bool PalFightHeadManager::Update()
{
	int delta = DexTime::getSingleton()->getDeltaTime();
	m_pBackImage->Update(delta);
	foreach(TlistFlighHead, ite, m_listHead)
	{
		(*ite)->Update();
		PalFightHead* head = *ite;
		DexGUI::DexPointF pt = getPosition(head->getValue());
		head->getHeadImage()->SetPos(pt.x, pt.y); 
		head->getHeadImage()->setDrawFlag(DexGUI::DRAW_TOP_CENTER);
	}
	return true;
}

DexPointF PalFightHeadManager::getPosition(float value)
{
	DexGUI::DexPointF pt = m_pBackImage->GetPos();
	DexPointF pnt;
	float v = value < 0.0f ? 0.0f : value;
	v = v > 1.0f ? 1.0f : v;
	float width = m_iEndOffsetX - m_iStartOffsetX;
	float height = m_iEndOffsetY - m_iStartOffsetY;
	pnt.x = pt.x + m_iStartOffsetX + width *v;
	pnt.y = pt.y + m_iStartOffsetY + height *v;
	return pnt;
}

bool PalFightHeadManager::Render()
{
	m_pBackImage->Render();
	foreach(TlistFlighHead, ite, m_listHead)
	{
		(*ite)->Render();
	}
	return true;
}