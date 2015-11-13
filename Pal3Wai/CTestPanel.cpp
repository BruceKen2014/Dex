

#include "../DexBase/CDexDraw2D.h"
#include "../widget/DexUiInclude.h"
#include "CTestPanel.h"





CTestPanel::CTestPanel()
{
	//m_scrollV = NULL;
	m_backImage = NULL;
	m_image1 = NULL;
	m_image2 = NULL;
	m_btn1 = NULL;
	m_imagesequence = NULL;
	m_checkBtn1 = NULL;
	m_editBox = NULL;
	m_focusWidget = NULL;
	m_label = NULL;
	g_iMouseLX = 0;
	g_iMouseLY = 0;
	g_bInTriangle = false;
}

CTestPanel::~CTestPanel()
{
	ShutDown();
}


bool CTestPanel::Init()
{
	//return 1;
	getWidgetFactory()->loadWidgetsFromXml("ini\\ui.xml");
	m_backImage = (CDexWidgetImage* )getWidgetFactory()->findWidget("background");
	m_image1 = (CDexWidgetImage* )getWidgetFactory()->findWidget("background.image1");
	m_image2 = (CDexWidgetImage* )getWidgetFactory()->findWidget("background.image2");

	CDexGuiActionRotate *rotate = new CDexGuiActionRotate;
	m_image2->AddAction(rotate);
	rotate->SetName("旋转action");
	rotate->SetCycles(20);
	rotate->SetCyclesPerSecond(1/5.0f);
	rotate->BeginAction();

	m_btn1 = (CDexWidgetButton* )getWidgetFactory()->findWidget("background.newGame");
	m_btn1->m_MouseLUp += UI_DELEGATE(this, CTestPanel, OnClickBtn);

	m_btn2 = (CDexWidgetButton* )getWidgetFactory()->createWidget(widget_button, "独立按钮控件");
	m_btn2->SetStateTex(btn_normal, "about.png");
	m_btn2->SetStateTexRect(btn_normal, DexRect(0,0,115,240));
	m_btn2->SetStateTex(btn_mouse_on, "about.png");
	m_btn2->SetStateTexRect(btn_mouse_on, DexRect(0,240,115,480));
	m_btn2->SetStateTex(btn_mouse_down, "about.png");
	m_btn2->SetStateTexRect(btn_mouse_down, DexRect(0,480,115,720));
	m_btn2->SetStateTex(btn_useless, "about.png");
	m_btn2->SetStateTexRect(btn_useless, DexRect(0,0,115,240));
	m_btn2->SetPos(DexPoint(600, 10));
	m_btn2->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
	m_btn2->Resize(DexSize(150,60));
	m_btn2->m_MouseLUp += UI_DELEGATE(this, CTestPanel, OnClickBtn2);

	m_imagesequence = (CDexWidgetImageSequence*) getWidgetFactory()->findWidget("background.lovelygirl");
	m_checkBtn1 = (CDexWidgetCheckBtn*) getWidgetFactory()->findWidget("background.check"); 

	m_editBox = (CDexWidgetEditBox*) getWidgetFactory()->createWidget(widget_editbox, "editbox"); 
	m_editBox->SetBackImage("22y.png", DexRect(345, 666, 650, 695));
	m_editBox->SetStartOffset(20, 5);
	m_editBox->Resize(DexSize(300, 30));
	m_editBox->SetPos(DexPoint(270, 520));
	m_editBox->SetFocus();
	m_editBox->m_DisTextChange += UI_DELEGATE(this, CTestPanel, OnEditTextChange);
	m_editBox->m_DisTextUpdate += UI_DELEGATE(this, CTestPanel, OnEditTextUpdate);

	m_label = (CDexWidgetLabel*)getWidgetFactory()->createWidget(widget_label, "label");
	m_backImage->GetChildrenContainer()->AddWidgetToBack(m_label);
	m_label->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
	m_label->Resize(DexSize(100, 20));
	m_label->setFatherOffset(100,100);
	m_label->SetTextSize(20);
	m_label->SetTextColor(0xffff0000);
	m_label->SetText("这是一个label");

	m_label2 = (CDexWidgetLabel*)getWidgetFactory()->createWidget(widget_label, "label2");
	m_backImage->GetChildrenContainer()->AddWidgetToBack(m_label2);
	m_label2->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
	m_label2->Resize(DexSize(120, 20));
	m_label2->setFatherOffset(100,120);
	m_label2->SetTextSize(20);
	m_label2->SetTextColor(0xf000ff00);
	m_label2->SetText("label2");
	//m_scrollV = (CDexWidgetScrollBarV*) getWidgetFactory()->createWidget(widget_scrollbar_v, "滚动框"); 
	//m_scrollV->SetPos(DexPoint(300, 200));
	//m_scrollV->Resize(DexSize(20, 300));
	//m_scrollV->SetTotalValue(0.5f);
	//m_scrollV->Create();
	//m_scrollV->GetTopBtn()->SetStateTex(btn_normal, "ui11.png");
	//m_scrollV->GetTopBtn()->SetStateTexRect(btn_normal, DexRect(490, 275, 527, 319));
	//m_scrollV->GetBottomBtn()->SetStateTex(btn_normal, "ui11.png");
	//m_scrollV->GetBottomBtn()->SetStateTexRect(btn_normal, DexRect(490, 275, 527, 319));
	//m_scrollV->GetHandleBtn()->SetStateTex(btn_normal, "ui11.png");
	//m_scrollV->GetHandleBtn()->SetStateTexRect(btn_normal, DexRect(490, 275, 527, 319));
	//m_scrollV->SetBackImage("22y.png", DexRect(345, 666, 650, 695));


	//m_backImage->GetChildrenContainer()->AddWidgetToBack(m_image1);
	//m_backImage->GetChildrenContainer()->AddWidgetToBack(m_image2);
	//m_backImage->GetChildrenContainer()->AddWidgetToBack(m_btn1);
	//m_backImage->GetChildrenContainer()->AddWidgetToBack(m_imagesequence);
	//m_backImage->GetChildrenContainer()->AddWidgetToBack(m_editBox);
	//m_backImage->GetChildrenContainer()->AddWidgetToBack(m_scrollV);
	return true;
}

void CTestPanel::Render()
{
	//return;
	DEX_ENSURE(m_backImage);
	m_backImage->Render();

	DEX_ENSURE(m_btn2);
	m_btn2->Render();

	DEX_ENSURE(m_editBox);
	m_editBox->Render();
	
	if(getDesktop()->getFocusWgt() != NULL)
	{
		if(getDesktop()->getFocusWgt() != m_editBox)
		{
			m_focusWidget = getDesktop()->getFocusWgt();

		}	
	}	
	if(m_focusWidget != NULL)
	{
		get2DDrawer()->SetTextSize(20);
		get2DDrawer()->DrawString(0, 500, "name:%s", (char*)m_focusWidget->GetName().c_str());
		get2DDrawer()->DrawString(0, 520, "size:width %d, height:%d", 
			(int)m_focusWidget->GetWidth(),(int)m_focusWidget->GetHeight());
		get2DDrawer()->DrawString(0, 540, "pos:x %d, y:%d", 
			(int)m_focusWidget->GetPos().x,(int)m_focusWidget->GetPos().y);
		get2DDrawer()->DrawString(0, 560, "offset:x %d, y:%d", 
			(int)m_focusWidget->getFatherOffset().x,(int)m_focusWidget->getFatherOffset().y);
	}
	get2DDrawer()->DrawRectLine(DexRectF(g_PntA.x, g_PntA.y, g_PntB.x, g_PntB.y), 0xff00ff00);
	get2DDrawer()->DrawRectLine(DexRectF(g_PntA.x + 50 , g_PntA.y + 50 , g_PntB.x+50, g_PntB.y+50), 0xff00ff00);

}
void CTestPanel::Update(int delta)
{
	DEX_ENSURE(m_backImage);
	m_backImage->Update(delta);

	DEX_ENSURE(m_btn2);
	m_btn2->Update(delta);

	DEX_ENSURE(m_editBox);
	m_editBox->Update(delta);

	
}


void CTestPanel::ShutDown()
{
	UI_DROP_REF(m_backImage);
	UI_DROP_REF(m_image1);
	UI_DROP_REF(m_image2);
	UI_DROP_REF(m_btn1);
	UI_DROP_REF(m_imagesequence);
	//UI_DROP_REF(m_scrollV);
	UI_DROP_REF(m_editBox);
}

void CTestPanel::OnMouseLDown(int mouse_x, int mouse_y)
{
}

void CTestPanel::OnClickBtn2(CEventHandler* sys,CDexObject* object, stEvent event)
{	
	bool visible = ! m_backImage->GetVisible();
	m_backImage->setVisible(visible);
	//CDexGuiActionMove* move5 = new CDexGuiActionMove;
	//move5->SetName("匀减速移动action SAT");
	//m_backImage->AddAction(move5);
	//move5->SetMoveType(GuiMove_VACCE);
	//move5->SetMoveParamType(GuiMove_S_A_T);
	//move5->SetParam_S_A_T(220.0f, 200.0f, 80.0f, 1000);
	//move5->BeginAction();

	//CDexGuiActionMove* move = new CDexGuiActionMove;
	//move->SetName("匀减速移动action");
	//m_backImage->AddAction(move);
	//move->SetMoveType(GuiMove_VACCE);
	//move->SetMoveParamType(GuiMove_V_A);
	//move->SetParam_V_A(-320.0f, 0.0f, 80.0f, 0.0f);
	//move->BeginAction();

	//CDexGuiActionMove* move2 = new CDexGuiActionMove;
	//move2->SetName("匀速移动action");
	//m_backImage->AddAction(move2);
	//move2->SetMoveType(GuiMove_NORMAL);
	//move2->SetMoveParamType(GuiMove_V_T);
	//move2->SetParam_V_T(0.0f, 200.0f, 2000);
	//move2->BeginAction();

	//CDexGuiActionMove* move3 = new CDexGuiActionMove;
	//move3->SetName("匀速移动action ST");
	//m_backImage->AddAction(move3);
	//move3->SetMoveType(GuiMove_NORMAL);
	//move3->SetMoveParamType(GuiMove_S_T);
	//move3->SetParam_S_T(200.0f, 200.0f, 500);
	//move3->BeginAction();


	//CDexGuiActionMove* move = new CDexGuiActionMove;
	//move->SetName("移动action");
	//m_backImage->AddAction(move);
	//static bool i = true; 
	//if(i)
	//{

	//	move->SetDestPos(350, 530);
	//	move->SetSpeed(25);
	//	move->SetAccelerate(3);
	//	move->SetMoveType(GuiMove_NORMAL);

	//	CDexGuiActionSizeChange* change = new CDexGuiActionSizeChange;
	//	change->SetName("尺寸改变aciton");
	//	m_backImage->AddAction(change);
	//	change->SetDestSize(50, 50);
	//	change->SetSizeMSeconds(move->GetTime());
	//	change->SetChangeChildren(true);
	//	change->BeginAction();
	//	move->BeginAction();

	//	CDexGuiActionAlphaChange* alpha = new CDexGuiActionAlphaChange;
	//	alpha->SetName("alpha改变action");
	//	m_backImage->AddAction(alpha);
	//	alpha->SetAlphaChangeType(EATA_LOW_STOP);
	//	alpha->SetAlphaLow(0);
	//	alpha->SetAlphaChangeTime(move->GetTime());
	//	alpha->BeginAction();


	//	i = false;
	//}
	//else
	//{
	//	move->SetDestPos(150, 0);
	//	move->SetSpeed(25);
	//	move->SetAccelerate(3);
	//	move->SetMoveType(GuiMove_NORMAL);

	//	CDexGuiActionSizeChange* change = new CDexGuiActionSizeChange;
	//	change->SetName("尺寸改变aciton");
	//	m_backImage->AddAction(change);
	//	change->SetDestSize(500, 200);
	//	change->SetSizeMSeconds(move->GetTime());
	//	change->SetChangeChildren(true);
	//	change->BeginAction();
	//	move->BeginAction();

	//	CDexGuiActionAlphaChange* alpha = new CDexGuiActionAlphaChange;
	//	alpha->SetName("alpha改变action");
	//	m_backImage->AddAction(alpha);
	//	alpha->SetAlphaChangeType(EATA_TOP_STOP);
	//	alpha->SetAlphaTop(m_backImage->GetInitAlpha());
	//	alpha->SetAlphaChangeTime(move->GetTime());
	//	//m_backImage->Show();
	//	alpha->BeginAction();

	//	i = true;
	//}
}
void CTestPanel::OnClickBtn(CEventHandler* sys,CDexObject* object, stEvent event)
{
}

void CTestPanel::OnEditTextChange(CEventHandler* sys,CDexObject* object, stEvent event)
{
	//_Message("editbox chars change!");
}
void CTestPanel::OnEditTextUpdate(CEventHandler* sys,CDexObject* object, stEvent event)
{
	vector<string> coord;
	SplitStr(m_editBox->GetEditText(), ' ', coord);
	DEX_ENSURE(coord.size() == 2);
	//g_PntC.x = atoi(coord[4].c_str());
	//g_PntC.y = atoi(coord[5].c_str());
	if(m_focusWidget != NULL)
	{
		m_focusWidget->Resize(DexSize(CDexFun::str_to_int(coord[0]),CDexFun::str_to_int(coord[1])));
	}
	m_editBox->ClearText();
}