
#include "DexBase/typedefine.h"
#include "DexBase/CInput.h"
#include "widget/DexWidgetFactory.h"
#include "widget/DexWidgetImage.h"
#include "state/DexGameEngine.h"
#include "DexBase/CDexDraw2D.h"
#include "DexModel/DexModelBase.h"
#include "DexModel/DexModelMs3d.h"
#include "DexModel/TestSkinMesh.h"
#include "DexMath/DexVector3.h"
#include "Source/CModelXAni.h"
#include "PalStateBattleMain.h"
#include "DexBase/DexCollideObject.h"
#include "PalBattleStateBase.h"
#include "PalBattleStateTransition.h"
#include "DexStateCommonEvent.h"
#include "DexBase/Dex3DScene.h"
#include "PalPlayer.h"
#include "PalPlayerAttackState.h"
#include "PalFightHeadManager.h"
#include "PalNumber.h"
#include "CDefineXAni.h"
#include "PalSkill.h"

#include "PalPanel_Order.h"
#include "PalPanel_SelectSkill.h"
//#include "Pal2Global.h"



PalGameStateBattleMain::PalGameStateBattleMain()
{
	m_pCurrOperationPlayer = NULL;
	m_pFightHeadManager = new PalFightHeadManager;
	m_pFightHeadManager->Init();
	m_pBattleSkillContent = new PalBatterContent;
	m_pNumberManager = new PalNumberManager();

	m_pBattleMainMachine = new PalBattleStateMachine;
	PalBattleStateMoveCamera* moveCamera = new PalBattleStateMoveCamera;
	PalBattleStateLabelMove* labelMove = new PalBattleStateLabelMove();
	PalBattleStateSelectSkill* selectSkill = new PalBattleStateSelectSkill();
	PalBattleStateSelectTarget* selectTarget = new PalBattleStateSelectTarget();
	labelMove->AddTranstion(new PalBattleStateTransitionPlayerMarchComplete(selectSkill));
	selectSkill->AddTranstion(new PalBattleStateTransitionSelectSkillOK(selectTarget));
	selectTarget->AddTranstion(new PalBattleStateTransitionSelectTargetOK(labelMove));
	selectTarget->AddTranstion(new PalBattleStateTransitionCancelSkill(selectSkill));
	m_pBattleMainMachine->AddState(moveCamera);
	m_pBattleMainMachine->AddState(labelMove);
	m_pBattleMainMachine->AddState(selectSkill);
	m_pBattleMainMachine->AddState(selectTarget);

	m_pBattleMainMachine->setCurrState(labelMove->getType());
}
PalGameStateBattleMain::~PalGameStateBattleMain()
{

}

void PalGameStateBattleMain::setCurrOperationPlayer(PalPlayer* player)
{
	m_pCurrOperationPlayer = player;
}

PalGameStateBattleMain* PalGameStateBattleMain::getBattleMain()
{
	return (PalGameStateBattleMain*) DexGameEngine::getEngine()->getGameState(getClassType());
}

TlistPlayer PalGameStateBattleMain::getPlayers(EPlayerType type)
{
	if(type == EPT_ENEMY)
	{
		return m_listEnemies;
	}
	return m_listPlayers;
}

void PalGameStateBattleMain::setVisible(string type, bool visible)
{
	if(m_mapBattlePanels.find(type) == m_mapBattlePanels.end())
		return;
	m_mapBattlePanels[type]->setVisible(visible);
}

bool PalGameStateBattleMain::getVisible(string type)
{
	if(m_mapBattlePanels.find(type) == m_mapBattlePanels.end())
		return false;
	bool ret = m_mapBattlePanels[type]->getVisible();
	return ret;
}

bool PalGameStateBattleMain::ApplyRes() 
{
	
	D3DXVECTOR3 cameraPos(-200, 100.0f,-5.0f);
	D3DXVECTOR3 lookAtPos(0, 0, 0);	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);	
	// Build view matrix.
	m_pScene = new CDexScene;
	DexGameEngine::getEngine()->LookAtLH(&cameraPos, &lookAtPos, &upDir);
	DexGameEngine::getEngine()->setRendeCollideMesh(true);
	
	PalPanelInterface* panel = new PalPanel_Order;
	panel->Initialize();
	panel->setVisible(false);
	m_mapBattlePanels[panel->getType()] = panel;
	panel = new PalPanel_SelectSkill;
	panel->Initialize();
	panel->setVisible(false);
	m_mapBattlePanels[panel->getType()] = panel;
	Test_AddJingtian();
	Test_AddEnemy1();
	DexGameEngine::getEngine()->GetDevice()->CreateTexture(128,128,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, & m_testTexture, NULL);
	m_testTexture->GetSurfaceLevel(0, &m_testSurface);
	//D3DXMatrixPerspectiveFovLH(&m_testProjection,D3DX_PI/4.0f,1,1,100);
	//DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_PROJECTION, & m_testProjectionOld);
	DexGameEngine::getEngine()->GetDevice()->GetRenderTarget(0, & DexGameEngine::getEngine()->m_pBackSurface);
	g_pImageBackgroud = (CDexWidgetImage*) getWidgetFactory()->createWidget(widget_image, "background111"); 

	g_pImageBackgroud->SetTexName("b23.png");
	g_pImageBackgroud->SetPos(200,200);
	g_pImageBackgroud->Resize(DexSize(300, 300));
	g_pImageBackgroud->ModifyFlag(Minus_Flag, catch_event);
	ms3d = new DexModelMs3d();
	ms3d->LoadModel("ms3d/Model.ms3d");
	testMesh = new TestSkinMesh(2000);
	DexMatrix4x4 temp_matrix;	temp_matrix.Identity();
	temp_matrix.SetPosition(0, 0, 0); testMesh->AddJoint(1, -1, temp_matrix);
	temp_matrix.Identity(); testMesh->AddJointKeyFrame(1, 0, temp_matrix);
	temp_matrix.Identity(); temp_matrix.RotateZ(_getRadian(90.0f)); testMesh->AddJointKeyFrame(1, 1000, temp_matrix);
	temp_matrix.Identity(); testMesh->AddJointKeyFrame(1, 2000, temp_matrix);

	temp_matrix.SetPosition(100, 0, 0); testMesh->AddJoint(2, 1, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(100, 0, 0); testMesh->AddJointKeyFrame(2, 0, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(100, 0, 0); testMesh->AddJointKeyFrame(2, 1000, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(100, 0, 0); testMesh->AddJointKeyFrame(2, 2000, temp_matrix);

	temp_matrix.SetPosition(0, 0, 20); testMesh->AddJoint(3, 2, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(0, 0, 20);  testMesh->AddJointKeyFrame(3, 0, temp_matrix);
	temp_matrix.Identity(); temp_matrix.RotateX(_getRadian(0.0f)); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(0, 0, 40);  testMesh->AddJointKeyFrame(3, 1000, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(0, 0, 20); testMesh->AddJointKeyFrame(3, 2000, temp_matrix);

	temp_matrix.SetPosition(20, 0, 0); testMesh->AddJoint(4, 2, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(20, 0, 0);  testMesh->AddJointKeyFrame(4, 0, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(80, 0, 0);  testMesh->AddJointKeyFrame(4, 1000, temp_matrix);
	temp_matrix.Identity(); temp_matrix.Scale(1, 1, 1); temp_matrix.Translate(20, 0, 0); testMesh->AddJointKeyFrame(4, 2000, temp_matrix);

	stVertex0	vertexs[8];
	vertexs[0].m_pos = D3DXVECTOR3(30.0f, 0, -30.0f); vertexs[0].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	vertexs[1].m_pos = D3DXVECTOR3(30.0f, 0, 30.0f); vertexs[1].m_color = getD3DColor(DexColor(1.0f, 0.0f, 1.0f, 0.1f));
	vertexs[2].m_pos = D3DXVECTOR3(-30.0f, 0, 30.0f); vertexs[2].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	vertexs[3].m_pos = D3DXVECTOR3(-30.0f, 0, -30.0f); vertexs[3].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	vertexs[4].m_pos = D3DXVECTOR3(30.0f, 60, -30.0f); vertexs[4].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	vertexs[5].m_pos = D3DXVECTOR3(30.0f, 60, 30.0f); vertexs[5].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	vertexs[6].m_pos = D3DXVECTOR3(-30.0f, 60, 30.0f); vertexs[6].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	vertexs[7].m_pos = D3DXVECTOR3(-30.0f, 60, -30.0f); vertexs[7].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.1f));
	test_primitive_vertex = (void*)malloc(sizeof(stVertex0) * 8);
	memcpy(test_primitive_vertex, vertexs, sizeof(stVertex0)*8);
	int32 indices[] = {0,1,4,1,5,4,2,3,6,6,3,7,0,4,3,3,4,7,1,2,5,2,6,5,1,0,3,1,3,2,4,5,7,7,5,6};
	test_primitive_indice = (void*)malloc(sizeof(indices));
	memcpy(test_primitive_indice, indices, sizeof(indices));

	testMesh->AddVertex(DexVector3(30.0f, 0, -30.0f), 2, 1.0f);
	testMesh->AddVertex(DexVector3(30.0f, 0, 30.0f), 2, 1.0f);
	testMesh->AddVertex(DexVector3(-30.0f, 0, 30.0f), 4, 0.5f, 3, 0.5f);
	testMesh->AddVertex(DexVector3(-30.0f, 0, -30.0f), 4, 0.5f, 3, 0.5f);
	testMesh->AddVertex(DexVector3(30.0f, 60, -30.0f), 4, 1.0f);
	testMesh->AddVertex(DexVector3(30.0f, 60, 30.0f), 4, 1.0f);
	testMesh->AddVertex(DexVector3(-30.0f, 60, 30.0f), 3, 1.0f);
	testMesh->AddVertex(DexVector3(-30.0f, 60, -30.0f), 3, 1.0f);

	testMesh->CalculateVertex();
	testMesh->SetIndices(indices, 12 * 3);

	m_bApply = true;
	return true;
}
bool PalGameStateBattleMain::Test_AddEnemy1()
{
	D3DXMATRIX localTransMatrix;
	 panda =  new PalPlayer();
	DEX_ENSURE_B(panda && panda->LoadModelFile("model/panda.X", true));
	DexCollideObjectSphere* collide_ball = Dex_QueryObject(DexCollideObjectSphere);
	collide_ball->Initialize(D3DXVECTOR3(0,100,0), 10.0f);
	D3DXMatrixIdentity(&localTransMatrix);
	D3DXMatrixTranslation(&localTransMatrix, 0, 20, 0);
	panda->setCollideObjectMatrix(localTransMatrix);
	panda->setCollideObject(collide_ball);
	//g_pJingtian->SetRenderSphere(false);
	//g_pJingtian->IniDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	panda->setLocalMatrix(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0.3f,0.3f,0.3f), CDexSceneObject::AXIS_Y, _getRadian(90));
	panda->setPosition(D3DXVECTOR3(80,0,80));
	//g_pJingtian->setDirection(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
	panda->getAniModel()->SetAniSpeed(20);
	panda->getAniModel()->SetAnimation(0);
	panda->getPlayerData()->headImageName = "image1.png";
	panda->getPlayerData()->headImageRect = DexGUI::DexRect(416,0,440,38);
	panda->getPlayerData()->headSize = DexGUI::DexSize(25,38);
	panda->getPlayerData()->speed = 0.005f;
	panda->setStandOriginiPos(panda->getPosition());
	PalSkill* slash = new PalSkill;
	slash->getSkillData()->skillId = 001;
	slash->getSkillData()->attackType = space_skill::PSAAT_TARGET;
	slash->AddDamageKey(100,100);
	slash->AddDamageKey(500,500);
	slash->AddDamageKey(800,300);
	panda->getSkillContainer()->AddSkill(space_skill::EPASA_NONE, slash);
	D3DXVECTOR3 jpos = g_pJingtian->getPosition();
	panda->FaceTo(D3DXVECTOR3(0,0,0));
	CPalPeople::SetIndex(panda, ID_PANDA);
	
	//panda->FaceTo(D3DXVECTOR3(100,0,0));
	AddEnemy(panda);
	panda->getAIMachine()->setCurrState(PalPlayerAttackStateMarching::getClassType());
	m_pScene->registerObject(panda);
	return true;
}
bool PalGameStateBattleMain::Test_AddJingtian()
{
	D3DXMATRIX localTransMatrix;
	PalPlayer* jingtian =  new PalPlayer();
	DEX_ENSURE_B(jingtian && jingtian->LoadModelFile("model/jingtian.X", true));
	DexCollideObjectSphere* jing_collide_ball = Dex_QueryObject(DexCollideObjectSphere);
	jing_collide_ball->Initialize(D3DXVECTOR3(0,60,0), 10.0f);
	D3DXMatrixIdentity(&localTransMatrix);
	D3DXMatrixTranslation(&localTransMatrix, 0, 20, 0);
	jingtian->setCollideObjectMatrix(localTransMatrix);
	jingtian->setCollideObject(jing_collide_ball);
	//g_pJingtian->SetRenderSphere(false);
	//g_pJingtian->IniDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	jingtian->setLocalMatrix(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0.5f,0.5f,0.5f));
	jingtian->setPosition(D3DXVECTOR3(-80,0,-80));
	//g_pJingtian->setDirection(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
	jingtian->getAniModel()->SetAniSpeed(25);
	jingtian->getAniModel()->SetAnimation(0);
	jingtian->getPlayerData()->m_PlayerType = EPT_ROLE;
	jingtian->getPlayerData()->headImageName = "image1.png";
	jingtian->getPlayerData()->headImageRect = DexGUI::DexRect(385,0,410,38);
	jingtian->getPlayerData()->headSize = DexGUI::DexSize(25,38);
	jingtian->setStandOriginiPos(jingtian->getPosition());
	jingtian->getPlayerData()->speed = 0.006f;
	jingtian->FaceTo(D3DXVECTOR3(0,0,0));
	jingtian->setAuto(false);
	CPalPeople::SetIndex(jingtian, ID_JINGTIAN);
	PalSkill* slash = new PalSkill;
	slash->LoadEffect("effect/emitterInstance3.txt");
	slash->getSkillData()->skillId = 001;
	slash->getSkillData()->m_good = true;
	slash->getSkillData()->time = 2500;
	slash->getSkillData()->attackType = space_skill::PSAAT_TARGET;
	slash->AddDamageKey(300,200);
	slash->AddDamageKey(600,200);
	slash->AddDamageKey(900,200);
	jingtian->getSkillContainer()->AddSkill(space_skill::EPASA_NONE, slash);
	//jingtian->setHeadName("image1.png", DexGUI::DexRect(385,0,410,38), DexGUI::DexSize(25,38));

	g_pJingtian = jingtian;
	AddPlayers(jingtian);
	jingtian->getAIMachine()->setCurrState(PalPlayerAttackStateMarching::getClassType());
	m_pScene->registerObject(jingtian);
	return true;
}
void PalGameStateBattleMain::ReleaseRes()
{
	m_bApply = false;
}
bool PalGameStateBattleMain::Update(int delta)
{	
	m_pScene->Update(delta);
	{
		foreach(TlistPlayer, ite, m_listPlayers)
		{
			(*ite)->UpdatePlayerData(delta);
		}
	}
	{
		foreach(TlistPlayer, ite, m_listEnemies)
		{
			(*ite)->UpdatePlayerData(delta);
		}
	}
	m_pBattleMainMachine->Update();
	m_pFightHeadManager->Update();
	m_pNumberManager->Update(delta);
	g_pImageBackgroud->Update(delta);
	m_pBattleSkillContent->Update();
	{

		//		foreach((std::map<string, PalPanelInterface*>), ite, m_mapBattlePanels)
		//{
		//	
		//}
	}
	for(std::map<string, PalPanelInterface*>::iterator ite = m_mapBattlePanels.begin();
		 ite != m_mapBattlePanels.end(); ++ite)
	{
		if(ite->second != NULL)
		{
			ite->second->Update(delta);
		}
		
	}
	testMesh->Update(delta/4);
	return true;
	//getGlobal()->g_pJingtian->Update();
}
void PalGameStateBattleMain::BeginRender()
{
	//DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, m_testSurface);
	//DexGameEngine::getEngine()->GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(255, 255, 255, 255),1.0f,0); 
	//DexGameEngine::getEngine()->GetDevice()->BeginScene();
	//get2DDrawer()->DrawString(0,0,"fps:%d", DexGameEngine::getEngine()->getFps());
	//
	//DexGameEngine::getEngine()->GetDevice()->EndScene();
	//D3DXSaveTextureToFile("11.jpg",D3DXIFF_JPG,m_testTexture,NULL);
	DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, DexGameEngine::getEngine()->m_pBackSurface);
	DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, getD3DColor(m_pScene->getClearColor()), 1.0f, 0);
	DexGameEngine::getEngine()->GetDevice()->BeginScene();

}
void PalGameStateBattleMain::Render()
{
	m_pScene->Render();
	m_pBattleMainMachine->Render();
	DexGameEngine::getEngine()->RenderCoorLines();
	//ms3d->Render();
	testMesh->Render();
	//DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, test_primitive_vertex, 8, test_primitive_indice, 12, sizeof(stVertex0));
	//getGlobal()->g_pJingtian->Render();
	get2DDrawer()->BeginDraw2D();
	for(std::map<string, PalPanelInterface*>::iterator ite = m_mapBattlePanels.begin();
		 ite != m_mapBattlePanels.end(); ++ite)
	{
		if(ite->second != NULL)
		{
			ite->second->Render();
		}
		
	}
	DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, m_testSurface);
	DexGameEngine::getEngine()->GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(100, 255, 255, 255),1.0f,0); 
	//DexGameEngine::getEngine()->GetDevice()->BeginScene();
	get2DDrawer()->DrawString(0,0,"二次渲染");
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	//D3DXSaveTextureToFile("11.jpg",D3DXIFF_JPG,m_testTexture,NULL);
	DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, DexGameEngine::getEngine()->m_pBackSurface);
	get2DDrawer()->ResetMatrix();	
	get2DDrawer()->SetUV(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	get2DDrawer()->SetAlpha(-1);
	get2DDrawer()->SetScale(128, 128);
	get2DDrawer()->SetRotate(0);
	get2DDrawer()->SetTrans(100,200);
	get2DDrawer()->Draw(m_testTexture);

	m_pFightHeadManager->Render();
	m_pNumberManager->Render();
	get2DDrawer()->SetTextSize(15);
	get2DDrawer()->DrawString(0,0,"fps:%d", DexGameEngine::getEngine()->getFps());
	D3DXVECTOR3 pos = DexGameEngine::getEngine()->getCamera()->GetPosition();
	get2DDrawer()->DrawString(0,15,"camera:X:%d Y:%d Z:%d", (int)pos.x, (int)pos.y, (int)pos.z);
	pos = g_pJingtian->getPosition();
	get2DDrawer()->DrawString(0,30,"jingtian:X:%d Y:%d Z:%d", (int)pos.x, (int)pos.y, (int)pos.z);
	int iXMouse = (int)CInputSystem::GetInputSingleton().GetMouseXPos();
	int iYMouse = (int)CInputSystem::GetInputSingleton().GetMouseYPos(); 
	get2DDrawer()->DrawString(0,45,"mouse:X:%d Y:%d ", iXMouse, iYMouse);	
	DrawPlayerBlood();


	//DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, m_testBufferSurface);
	//get2DDrawer()->ResetMatrix();	
	//get2DDrawer()->SetUV(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	//get2DDrawer()->SetAlpha(-1);
	//get2DDrawer()->SetScale(128, 128);
	//get2DDrawer()->SetRotate(0);
	//get2DDrawer()->SetTrans(300,0);
	//get2DDrawer()->Draw(m_testTexture);

	//g_pImageBackgroud->getText()->SetTex(m_testTexture);
	//g_pImageBackgroud->Render();
	get2DDrawer()->EndDraw2D();
}
void PalGameStateBattleMain::DrawPlayerBlood()
{
	D3DXVECTOR3 _3Dpos;
	DexGUI::DexPoint _2Dpnt;
	get2DDrawer()->SetTextSize(30);
	PalPlayer* player;
	{
		foreach(TlistPlayer, ite, m_listPlayers)
		{
			player = (*ite);
			_3Dpos = player->getTopPosition();
			_2Dpnt = DexGameEngine::getEngine()->GetXY(_3Dpos);
			get2DDrawer()->DrawString(_2Dpnt.x, _2Dpnt.y, (char*)CDexFun::IntToStr(player->getPlayerData()->hp).c_str());
		}
	}
	{
		foreach(TlistPlayer, ite, m_listEnemies)
		{
			player = (*ite);
			_3Dpos = player->getTopPosition();
			_2Dpnt = DexGameEngine::getEngine()->GetXY(_3Dpos);
			get2DDrawer()->DrawString(_2Dpnt.x, _2Dpnt.y, (char*)CDexFun::IntToStr(player->getPlayerData()->hp).c_str());
		}
	}
}
void PalGameStateBattleMain::EndRender()
{
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	DexGameEngine::getEngine()->GetDevice()->Present(NULL, NULL, NULL, NULL);
}
void PalGameStateBattleMain::MouseWheelFront()
{
	DexGameEngine::getEngine()->RotateCameraByFocus(0.2f);
}
void PalGameStateBattleMain::MouseWheelBack()
{
	DexGameEngine::getEngine()->RotateCameraByFocus(-0.2f);
}
void PalGameStateBattleMain::MouseMove(int xPos, int yPos)
{

}
void PalGameStateBattleMain::MouseLDown(int xPos, int yPos)
{
	m_pBattleMainMachine->MouseLDown(xPos, yPos);
	return;
	CPlane plane(D3DXVECTOR3(0,0,0), D3DXVECTOR3(50,0,0), D3DXVECTOR3(0,0,50));
	stRay ray;
	DexGameEngine::getEngine()->GetRay(xPos, yPos, ray);
	D3DXVECTOR3 crossPoint = plane.GetCrossPoint(ray);
	g_pJingtian->getAniModel()->SetAnimation(4);
	D3DXVECTOR3 collide_center; 
	panda->getCollideObject()->getCenter(collide_center);
	D3DXVECTOR3 set_vector = crossPoint - collide_center;
	set_vector.y = 0;
	panda->setDirection(set_vector);
	DexGameEngine::getEngine()->MoveCamera(set_vector,4.0f, true);
	panda->Move(set_vector, 4.0f);
}
void PalGameStateBattleMain::MouseLUp(int xPos, int yPos)
{
	m_pBattleMainMachine->MouseLUp(xPos, yPos);
	//g_pJingtian->getAniModel()->SetAnimation(0);
}
void PalGameStateBattleMain::MouseRUp(int xPos, int yPos)
{
	m_pBattleMainMachine->MouseRUp(xPos, yPos);
}
void PalGameStateBattleMain::KeyUp()
{

}
void PalGameStateBattleMain::KeyDown()
{
	m_pBattleMainMachine->KeyDown();
	float vel = 4.0f;	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_S))
	{	
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, vel);
	}	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_W))
	{	
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, -vel);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_SPACE))
	{
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_UP,5);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_LCONTROL))
	{
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_UP,-5);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}
}

bool PalGameStateBattleMain::AddPlayers(PalPlayer* player)
{//暂时先不添加重复等合法性判断
	m_listPlayers.push_back(player);
	m_pFightHeadManager->AddFightHead(player);
	return true;
}


bool PalGameStateBattleMain::AddEnemy(PalPlayer* player)
{//暂时先不添加重复等合法性判断
	m_listEnemies.push_back(player);
	m_pFightHeadManager->AddFightHead(player);
	return true;
}