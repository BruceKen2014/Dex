
#include "DexBase/typedefine.h"
#include "DexBase/CInput.h"
#include "state/DexGameEngine.h"
#include "DexBase/CDexDraw2D.h"
#include "DexBase/CDexPartical.h"
#include "DexBase/DexCollideObject.h"
#include "DexBase/Dex3DScene.h"
#include "DexBase/DexSceneObjectFlower.h"
#include "DexBase/DexObjectFactory.h"
#include "widget/CDexUiSrcMgr.h"
#include "widget/DexWidgetEditBox.h"
#include "widget/DexWidgetImage.h"
#include "widget/DexWidgetImageSequence.h"
#include "widget/DexWidgetFactory.h"
#include "DexBase/DexSceneObjectModel.h"
#include "ParticleEditState.h"
#include "PalTalkSystem.h"
#include "Geometry/CPlane.h"
#include "CDefineXAni.h"
#include "resource.h"



CLoadingState::CLoadingState()
{
	g_pImageBackgroud = (CDexWidgetImage*) getWidgetFactory()->createWidget(widget_image, "background"); 
	g_pImageBackgroud->Resize(DexSize(800, 600));
	g_pImageBackgroud->SetPos(0,0);
	g_pImageBackgroud->SetTexName("b23.png");
	g_pImageBackgroud->ModifyFlag(Minus_Flag, catch_event);

	g_pImage = (CDexWidgetImageSequence*) getWidgetFactory()->createWidget(widget_image_sequence, "lovelygirl");
	g_pImage->AddImage("女孩0.png");
	g_pImage->AddImage("女孩1.png");
	g_pImage->AddImage("女孩2.png");
	g_pImage->AddImage("女孩3.png");
	g_pImage->AddImage("女孩4.png");
	g_pImage->AddImage("女孩5.png");
	g_pImage->AddImage("女孩6.png");
	g_pImage->AddImage("女孩7.png");
	g_pImage->AddImage("女孩8.png");
	g_pImage->AddImage("女孩9.png");
	g_pImage->AddImage("女孩10.png");
	g_pImage->SetPos(50, 50);
	g_pImage->SetInterval(100);
	g_pImage->Resize(DexSize(71, 103));

}
CLoadingState::~CLoadingState()
{
}

bool CLoadingState::ApplyRes()
{
	return true;
}

void CLoadingState::ReleaseRes()
{

}

bool CLoadingState::Update(int delta)
{
	g_pImage->Update(delta);
	return true;
}

void CLoadingState::BeginRender()
{

}

void CLoadingState::Render()
{
	DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		0xff00ff00, 1.0f, 0);
	DexGameEngine::getEngine()->GetDevice()->BeginScene();
	get2DDrawer()->BeginDraw2D();
	g_pImageBackgroud->Render();
	g_pImage->Render();
	get2DDrawer()->EndDraw2D();
	get2DDrawer()->DrawString(100, 100, "loading...");
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	DexGameEngine::getEngine()->GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CLoadingState::EndRender()
{

}

LRESULT CALLBACK PropDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG: 
		{
			int i = 1;
			break;
		}
	case WM_COMMAND:
		{
			int t = 1;
			break;
		}
	}
	return 0;
}
ParticalEditState::ParticalEditState()
{
	g_Object = NULL;

	m_hWndPropDlg = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG1), DexGameEngine::getEngine()->GetHwnd(), 
		(DLGPROC)PropDlgProc); 

	//ShowWindow(m_hWndPropDlg, SW_SHOW);
}
ParticalEditState::~ParticalEditState()
{

}
bool ParticalEditState::ApplyRes() 
{
	CDexModelFactory::getModelFactory()->LoadResource("model/model.ini");
	//CDelegateM* dlg = new CDelegateM(this, (CDelegateM::EventCallBack)(&ParticalEditState::OnEventClickTalkSystem));
	SYSTEM_EVENT_REGISTER_M(120, this, ParticalEditState, OnEventClickTalkSystem);
	m_pScene = new CDexScene;
	m_pScene->LoadCollideInit("scene/scene1.txt");
	m_pScene->SetFogMode(DEMOEX_FOG_EXP);
	D3DXMATRIX localTransMatrix;
	D3DXMATRIX localMatrix;
	D3DXMATRIX localRotateMatrix;
	D3DXMATRIX localScaleMatrix;
	DexGameEngine::getEngine()->setRenderPieceEffectRoute(true);
	m_editBox = (CDexWidgetEditBox*) getWidgetFactory()->createWidget(widget_editbox, "editbox"); 
	m_editBox->m_DisTextUpdate += UI_DELEGATE(this, ParticalEditState, OnEditTextUpdate);
	m_editBox->SetBackImage("22y.png", DexRect(345, 666, 650, 695));
	m_editBox->SetStartOffset(20, 5);
	m_editBox->Resize(DexSize(300, 30));
	m_editBox->SetPos(DexPoint(550, 140));
	m_editBox->SetFocus();

	CPalTalkSystem* sys = CPalTalkSystem::getTalkSystem();
	sys->loadLuaFile("script\\npc1.lua");
	
	CPalNpc* npc = new CPalNpc;
	CPalPeople::SetIndex(npc, ID_JINGTIAN);
	npc->LoadModelFile("model/jingtian.X", true);
	npc->AddNode(D3DXVECTOR3(100,0,200), 1000);
	npc->AddNode(D3DXVECTOR3(300,0,300), 1000);
	CDexSceneObjectModel *model = npc;
	
	DexCollideObjectSphere* collide_ball = Dex_QueryObject(DexCollideObjectSphere);
	collide_ball->Initialize(D3DXVECTOR3(0,50,0), 10.0f);
	
    
	D3DXMatrixIdentity(&localTransMatrix);
	D3DXMatrixTranslation(&localTransMatrix, 0, 20, 0);
	model->setCollideObjectMatrix(localTransMatrix);
	model->setCollideObject(collide_ball);
	model->setLocalMatrix(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0.5f,0.5f,0.5f));
	model->setPosition(D3DXVECTOR3(100,300,200));
	AddNpc(npc);

	g_pJingtian = Dex_QueryObject(CDexSceneObjectModel);
	DEX_ENSURE_B(g_pJingtian && g_pJingtian->LoadModelFile("model/jingtian.X", true));
	DexCollideObjectSphere* jing_collide_ball = Dex_QueryObject(DexCollideObjectSphere);
	jing_collide_ball->Initialize(D3DXVECTOR3(0,50,0), 10.0f);
	D3DXMatrixIdentity(&localTransMatrix);
	D3DXMatrixTranslation(&localTransMatrix, 0, 20, 0);
	g_pJingtian->setCollideObjectMatrix(localTransMatrix);
	g_pJingtian->setCollideObject(jing_collide_ball);
	DexGameEngine::getEngine()->setRendeCollideMesh(true);
	//g_pJingtian->SetRenderSphere(false);
	//g_pJingtian->IniDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	g_pJingtian->setLocalMatrix(D3DXVECTOR3(0,0,0),D3DXVECTOR3(0.5f,0.5f,0.5f));
	//g_pJingtian->setDirection(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
	
	g_pJingtian->getAniModel()->SetAniSpeed(25);
	g_pJingtian->getAniModel()->SetAnimation(0);
	//g_pJingtian->SetScale(0.5);


	DexCollideObjectBox* box = Dex_QueryObject(DexCollideObjectBox);
	box->Initialize(D3DXVECTOR3(0,0,0), D3DXVECTOR3(5,5,5));
	DexCollideObjectSphere* ball  =  Dex_QueryObject(DexCollideObjectSphere) ;
	ball->Initialize(D3DXVECTOR3(0,0,0), 3.0f);
	//box->SetPosition(D3DXVECTOR3(0,0,0));
	string type = CDexTerrain::getClassType();
	//g_pTerrain = new CDexTerrain;
	//g_pTerrain = (CDexTerrain*)CDexObjectFactroy::getObjectFactory()->queryObject(type);
	g_pTerrain =  Dex_QueryObject(CDexTerrain);
	g_pTerrain->CreateTerrain();
	
	g_pTerrain->InitFromFile("terrain.txt");
	g_pTerrain->SetTexture("grass.jpg");
	g_pTerrain->SetRender(false);
	
	g_pEmittInstance = Dex_QueryObject(CDexEffectEmitInstance);
	g_pEmittInstance->LoadInitFile("effect/emitterInstance1.txt");
	//g_pEmittInstance->setCollideObject(ball);

	float circle = 50;
	D3DXVECTOR3 pnt;
	pnt.x = circle;
	pnt.y = circle;
	pnt.z = 0;
	action = Dex_QueryObject(DexSceneObjectActionCatmull);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	m_points.push_back(pnt);
	pnt.x = -circle;
	pnt.y = circle;
	pnt.z = 0;
	m_points.push_back(pnt);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	pnt.x = -circle;
	pnt.y = -circle;
	pnt.z = 0;
	m_points.push_back(pnt);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	pnt.x = circle;
	pnt.y = -circle;
	pnt.z = 0;
	m_points.push_back(pnt);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	pnt.x = circle;
	pnt.y = circle;
	pnt.z = 0;
	m_points.push_back(pnt);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	pnt.x = -circle;
	pnt.y = circle;
	pnt.z = 0;
	m_points.push_back(pnt);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	pnt.x = -circle;
	pnt.y = -circle;
	pnt.z = 0;
	m_points.push_back(pnt);
	D3DXVec3TransformCoord(&pnt, &pnt, &localMatrix);
	action->addCatmullPoint(pnt);
	factor = 0.0f;
	curr_delta = 0.0f;

	action->setCatmullFactor(1.0f);
	action->setCatmullSpeed(10.0f);
	//g_pEmittInstance->addAction(action);
	g_pEmittInstance->setLocalMatrix(D3DXVECTOR3(0,0,0));

	DexSceneObjectActionCatmull* action2 =  Dex_QueryObject(DexSceneObjectActionCatmull);
	float circle2 = 100.0f;
	float height2 = 25.0f;
	DexCollideObjectSphere* ball2 = Dex_QueryObject(DexCollideObjectSphere);
	ball2->Initialize(D3DXVECTOR3(0,0,0), 3.0f);
	DexCollideObjectBox* box2 = Dex_QueryObject(DexCollideObjectBox);
	box2->Initialize(D3DXVECTOR3(0,0,0), D3DXVECTOR3(5,5,5));
	//particalMgr->setCollideObject(box2);
	//DexGameEngine::getEngine()->getCamera()->addAction(action3);

	D3DXVECTOR3 cameraPos(0, 300.0f,-200.0f);
	D3DXVECTOR3 lookAtPos(0, 170, 0);	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);	
	// Build view matrix.
	DexGameEngine::getEngine()->LookAtLH(&cameraPos, &lookAtPos, &upDir);
	m_bApply = true;
	//m_pScene->registerObject(pieceEffect);
	//m_pScene->registerObject(g_pEmittInstance);
	m_pScene->registerObject(g_pJingtian);

	DexGameEngine::getEngine()->DoLuaFile("script\\scene1.lua");

	getLog()->BeginLog();
	getLog()->Log(log_ok, "particle editer init ok!\n");
	getLog()->EndLog();
	D3DXVECTOR3 jingtian_pos =  g_pJingtian->getPosition();
	float jingtian_y = g_pTerrain->GetHeight(jingtian_pos.x, jingtian_pos.z);
	g_pJingtian->setPosition(CDexSceneObject::AXIS_Y, jingtian_y);
	return true;
}
void ParticalEditState::ReleaseRes()
{
	m_bApply = false;
}
bool ParticalEditState::AddNpc(CPalNpc* npc)
{
	DEX_ENSURE_B(npc != NULL);
	m_listSceneNpc.push_back(npc);
	CDexSceneObjectModel* model = npc;
	if(model != NULL && m_pScene != NULL)
	{
		m_pScene->registerObject(model);
	}
	return true;
}
CPalNpc* ParticalEditState::getClickNpc(const stRay& ray)
{
	{
		foreach(TlistNpc, ite, m_listSceneNpc)
		{
			if((*ite) != NULL)
			{
				CDexSceneObjectModel* model = (*ite);
				if(model == NULL)
					continue;
				if(model->checkPick(ray))
				{
					return *ite;
				}
			}
		}
	}
	return NULL;
}
bool ParticalEditState::Update(int delta)
{
	static float rotatex = 0.0f;
	rotatex += 10.0f;
	//pieceEffect->rotate(CDexSceneObject::AXIS_Y, rotatex);
	float second = 2; //2秒绕完
	float vel = 1.0f/(1000.0f*second);
	curr_delta += delta*vel;
	if(curr_delta >= 1.0f)
		curr_delta = 0.0f;
	//g_pEmittInstance->Update(delta);
	//particalMgr->Update(delta);
	//pieceEffect->Update(delta);

	foreach(TlistNpc, ite, m_listSceneNpc)
	{
		if((*ite) != NULL)
		{
			(*ite)->Update(delta);
			CDexSceneObjectModel* model = (*ite);
			if(model == NULL)
				continue;
			float new_y =  g_pTerrain->GetHeight(model->getPosition().x, model->getPosition().z);
			model->setPosition(CDexSceneObject::AXIS_Y, new_y);
		}
	}

	m_pScene->Update(delta);
	action->setCatmullFactor(factor);
	m_editBox->Update(delta);
	CPalTalkSystem::getTalkSystem()->Update(delta);
	return true;
}
void ParticalEditState::BeginRender()
{
	DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		getD3DColor(m_pScene->getClearColor()), 1.0f, 0);
	DexGameEngine::getEngine()->GetDevice()->BeginScene();

}
void ParticalEditState::Render()
{
	//g_pTerrain->SetRenderStyle(TERRIAN_LINE);
	g_pTerrain->Render();
	DexGameEngine::getEngine()->RenderCoorLines();
	D3DXMATRIX localMatrix;
	D3DXMATRIX fatherMatrix;
	D3DXMATRIX finalMatrix;

	D3DXMATRIX localScaleMatrix;
	D3DXMATRIX localTransMatrix;
	D3DXMATRIX localRotateMatrix;

	D3DXMATRIX fatherScaleMatrix;
	D3DXMATRIX fatherTransMatrix;
	D3DXMATRIX fatherRotateMatrix;
	
	float pi = 3.1416f;
	{//local matrix
		D3DXMatrixIdentity(&localMatrix);
		{//scale
			D3DXMatrixIdentity(&localScaleMatrix);
			D3DXMatrixScaling(&localScaleMatrix, 1,1,1);
		}
		D3DXMatrixIdentity(&localRotateMatrix);
		{//rotate
			D3DXMatrixIdentity(&localRotateMatrix);
			D3DXMatrixRotationY(&localRotateMatrix, 0 * pi/8);
		}
		{//traslation
			D3DXMatrixIdentity(&localTransMatrix);
			D3DXMatrixTranslation(&localTransMatrix, 50,0,0);
		}
	}
	{//father matrix
		D3DXMatrixIdentity(&fatherMatrix);
		{//scale
			D3DXMatrixIdentity(&fatherScaleMatrix);
			D3DXMatrixScaling(&fatherScaleMatrix, 1,1,1);
		}
		D3DXMatrixIdentity(&fatherRotateMatrix);
		{//rotate
			D3DXMatrixIdentity(&fatherRotateMatrix);
			//D3DXMatrixRotationY(&fatherRotateMatrix, 1 * pi/8);
			D3DXMatrixRotationZ(&fatherRotateMatrix, 1 * pi/8);
		}
		{//traslation
			D3DXMatrixIdentity(&fatherTransMatrix);
			D3DXMatrixTranslation(&fatherTransMatrix, 10,0,0);
		}
	}
	//final matrix
	D3DXMatrixIdentity(&finalMatrix);
	//D3DXMatrixMultiply(&finalMatrix, &finalMatrix, &localScaleMatrix);
	//D3DXMatrixMultiply(&localTransMatrix, &localTransMatrix, &fatherScaleMatrix);
	//D3DXMatrixMultiply(&finalMatrix, &finalMatrix, &localTransMatrix);
	//D3DXMatrixMultiply(&finalMatrix, &finalMatrix, &fatherTransMatrix);

	localMatrix = localScaleMatrix * localRotateMatrix * localTransMatrix;
	fatherMatrix = fatherScaleMatrix * fatherRotateMatrix * fatherTransMatrix;
	finalMatrix =   localMatrix  * fatherMatrix;

	DexGameEngine::getEngine()->SetMeshColor(DEXCOLOR_RED);
	DexGameEngine::getEngine()->RenderCube(fatherMatrix);

	DexGameEngine::getEngine()->SetMeshColor(DEXCOLOR_BLUE);
	DexGameEngine::getEngine()->RenderCube(finalMatrix);

	D3DXVECTOR3 dir(1.0f,0.0f, 0.0f);
	D3DXVec3TransformNormal(&dir, &dir, &finalMatrix);
	D3DXVECTOR3 pos(finalMatrix._41, finalMatrix._42,finalMatrix._43);

	DexGameEngine::getEngine()->Render3DLine(pos, dir, DEXCOLOR_PINK, 100,DEXCOLOR_PINK);

	DexGameEngine::getEngine()->SetMeshColor(DEXCOLOR_RED);
	DexGameEngine::getEngine()->RenderCube(g_TerrainClickPoint, D3DXVECTOR3(2.0f, 2.0f ,2.0f));
	//DexGameEngine::getEngine()->RenderCube(*matrix);
	
	//g_pEmittInstance->Render();
	//pieceEffect->Render();
	m_pScene->Render();
	//particalMgr->Render();
	get2DDrawer()->BeginDraw2D();
	
	CPalTalkSystem::getTalkSystem()->Render();

	get2DDrawer()->SetTextSize(15);
	get2DDrawer()->DrawString(0,0,"fps:%d", DexGameEngine::getEngine()->getFps());
	D3DXVECTOR3 camera_pos = DexGameEngine::getEngine()->getCamera()->GetPosition();
	get2DDrawer()->DrawString(0,15,"camera:X:%d Y:%d Z:%d", (int)camera_pos.x, (int)camera_pos.y, (int)camera_pos.z);
	int iXMouse = (int)CInputSystem::GetInputSingleton().GetMouseXPos();
	int iYMouse = (int)CInputSystem::GetInputSingleton().GetMouseYPos(); 
	get2DDrawer()->DrawString(0,30,"mouse:X:%d Y:%d ", iXMouse, iYMouse);	
	get2DDrawer()->DrawString(0,45,"景天X:%d Y:%d Z:%d ", (int)g_pJingtian->getPosition().x, (int)g_pJingtian->getPosition().y,(int)g_pJingtian->getPosition().z);	
	
	if(m_pScene->m_bAddObject != NULL)
	{
		D3DXVECTOR3 pos = m_pScene->m_bAddObject->getPosition();
		get2DDrawer()->DrawString(0,60,"object:X:%d Y:%d Z:%d ", (int)pos.x, (int)pos.y, (int)pos.z);	
	}
	m_editBox->Render();
	get2DDrawer()->EndDraw2D();
}
void ParticalEditState::EndRender()
{
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	DexGameEngine::getEngine()->GetDevice()->Present(NULL, NULL, NULL, NULL);
}
void ParticalEditState::MouseWheelFront()
{
	DexGameEngine::getEngine()->RotateCameraByFocus(0.2f);
	//factor += 0.01f;
}
void ParticalEditState::MouseWheelBack()
{
	DexGameEngine::getEngine()->RotateCameraByFocus(-0.2f);
	//factor -= 0.01f;
}
void ParticalEditState::MouseMove(int xPos, int yPos)
{

}
void ParticalEditState::MouseLDown(int xPos, int yPos)
{
	//factor -= 0.01f;
	string str = m_editBox->GetEditText();
	int height = CDexFun::str_to_int(str);
	//g_pTerrain->SetHeight(g_currCol, g_currRow, height);

	D3DXVECTOR3 jingtian_pos = g_pJingtian->getPosition();
	g_TerrainClickPoint = g_pJingtian->getPosition();
	g_TerrainClickPoint.y = 0;
	//DEX_ENSURE(g_TerrainClickPoint.x > g_pTerrain->GetPosition().x && g_TerrainClickPoint.z > g_pTerrain->GetPosition().z);
	g_currRow = g_TerrainClickPoint.x - g_pTerrain->GetPosition().x;
	g_currRow = g_currRow/Terrain_cell_width;
	g_currCol = g_TerrainClickPoint.z - g_pTerrain->GetPosition().z;
	g_currCol = g_currCol/Terrain_cell_height;
	g_TerrainClickPoint = D3DXVECTOR3(g_currRow * Terrain_cell_width, g_pTerrain->GetHeight(g_currRow, g_currCol), g_currCol * Terrain_cell_height);
	float old_y = g_pJingtian->getPosition().y;
	float new_y =  g_pTerrain->GetHeight(jingtian_pos.x, jingtian_pos.z);
	g_pJingtian->setPosition(CDexSceneObject::AXIS_Y,new_y);
	float y_delta = new_y - old_y;

	CPlane plane(D3DXVECTOR3(0,new_y,0), D3DXVECTOR3(50,new_y,0), D3DXVECTOR3(0,new_y,50));
	stRay ray;
	DexGameEngine::getEngine()->GetRay(xPos, yPos, ray);
	D3DXVECTOR3 crossPoint = plane.GetCrossPoint(ray);
	D3DXVECTOR3 collide_center; 
	g_pJingtian->getCollideObject()->getCenter(collide_center);
	D3DXVECTOR3 set_vector = crossPoint - collide_center;
	set_vector.y = 0;
	g_pJingtian->setDirection(set_vector);
	//g_pJingtian->SetVel(0.0f);
	g_pJingtian->getAniModel()->SetAnimation(4);
	//g_pJingtian->SetMoving(true);
	DexGameEngine::getEngine()->MoveCamera(set_vector,4.0f, true);
	DexGameEngine::getEngine()->MoveCamera(D3DXVECTOR3(0, 1.0f, 0), y_delta, true);
	g_pJingtian->Move(set_vector, 4.0f);
}
void ParticalEditState::MouseLUp(int xPos, int yPos)
{
	//g_pJingtian->SetMoving(false);
	if(CPalTalkSystem::getTalkSystem()->IsCompleteThisTalkScript())
	{
		g_pJingtian->getAniModel()->SetAnimation(0);
		CPlane plane(D3DXVECTOR3(0,0,0), D3DXVECTOR3(50,0,0), D3DXVECTOR3(0,0,50));
		stRay ray;
		DexGameEngine::getEngine()->GetRay(xPos, yPos, ray);
		CPalNpc* clickNpc = getClickNpc(ray);
		if(clickNpc != NULL)
		{
			CPalTalkSystem::getTalkSystem()->loadLuaFile("script/npc2.lua");
		}
	}
	else
	{
		CPalTalkSystem::getTalkSystem()->OnMouseLClick(xPos, yPos);
	}

	
}
void ParticalEditState::MouseRDown(int xPos, int yPos)
{
	//factor += 0.01f;
	D3DXVECTOR3 jingtian_pos = g_pJingtian->getPosition();
	g_TerrainClickPoint = g_pJingtian->getPosition();
	g_TerrainClickPoint.y = 0;
	//DEX_ENSURE(g_TerrainClickPoint.x > g_pTerrain->GetPosition().x && g_TerrainClickPoint.z > g_pTerrain->GetPosition().z);
	g_currRow = g_TerrainClickPoint.x - g_pTerrain->GetPosition().x;
	g_currRow = g_currRow/Terrain_cell_width;
	g_currCol = g_TerrainClickPoint.z - g_pTerrain->GetPosition().z;
	g_currCol = g_currCol/Terrain_cell_height;
	g_TerrainClickPoint = D3DXVECTOR3(g_currRow * Terrain_cell_width, g_pTerrain->GetHeight(g_currRow, g_currCol), g_currCol * Terrain_cell_height);
	float old_y = g_pJingtian->getPosition().y;
	float jingtian_y = g_pTerrain->GetHeight(jingtian_pos.x, jingtian_pos.z);
	g_pJingtian->setPosition(CDexSceneObject::AXIS_Y, jingtian_y);
	float y_delta = jingtian_y- old_y;
	CPlane plane(D3DXVECTOR3(0,jingtian_y,0), D3DXVECTOR3(50,jingtian_y,0), D3DXVECTOR3(0,jingtian_y,50));
	stRay ray;
	DexGameEngine::getEngine()->GetRay(xPos, yPos, ray);
	D3DXVECTOR3 crossPoint = plane.GetCrossPoint(ray);
	D3DXVECTOR3 collide_center; 
	g_pJingtian->getCollideObject()->getCenter(collide_center);
	D3DXVECTOR3 set_vector = crossPoint - collide_center;
	set_vector.y = 0;
	g_pJingtian->setDirection(set_vector);
	//g_pJingtian->SetVel(0.0f);
	g_pJingtian->getAniModel()->SetAnimation(4);
	//g_pJingtian->SetMoving(true);
	DexGameEngine::getEngine()->MoveCamera(set_vector,4.0f, true);
	DexGameEngine::getEngine()->MoveCamera(D3DXVECTOR3(0, 1.0f, 0), y_delta, true);
	g_pJingtian->Move(set_vector, 4.0f);
}
void ParticalEditState::MouseRUp(int xPos, int yPos)
{
	g_pJingtian->getAniModel()->SetAnimation(0);
	//m_editBox->ClearText();
}
void ParticalEditState::KeyUp()
{
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_T))
	{	
		static int i = 0;
		if(i == 1)
		{
			g_pTerrain->SetRenderStyle(CDexTerrain::TERRIAN_TRI);
			i = 0;
		}
		else
		{
			g_pTerrain->SetRenderStyle(CDexTerrain::TERRIAN_LINE);
			i = 1;
		}
	}	
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_M))
	{
		g_pTerrain->SaveToFile("terrain.txt");
	}
}
void ParticalEditState::KeyDown()
{
	float vel = 4.0f;	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_S))
	{	
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, vel);
	}	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_R))
	{	
		g_pTerrain->SetRender(g_pTerrain->getRender() ? false: true);
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
	CDexSceneObject* object = m_pScene->m_bAddObject;
	DEX_ENSURE(object);
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_LEFT))
	{
		object->Move(DexGameEngine::getEngine()->getCamera()->GetRight(),-1);

		D3DXVECTOR3 pos = get2DDrawer()->GetCameraPos();
		pos.x -= 1;
		get2DDrawer()->SetCameraPos(pos.x, pos.y);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_RIGHT))
	{
		object->Move(DexGameEngine::getEngine()->getCamera()->GetRight(),1);

		D3DXVECTOR3 pos = get2DDrawer()->GetCameraPos();
		pos.x += 1;
		get2DDrawer()->SetCameraPos(pos.x, pos.y);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_UP))
	{
		D3DXVECTOR3 dir = DexGameEngine::getEngine()->getCamera()->GetView();
		dir.y = 0;
		object->Move(dir,1);

		D3DXVECTOR3 pos = get2DDrawer()->GetCameraPos();
		pos.y += 1;
		get2DDrawer()->SetCameraPos(pos.x, pos.y);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_DOWN))
	{
		D3DXVECTOR3 dir = DexGameEngine::getEngine()->getCamera()->GetView();
		dir.y = 0;
		object->Move(dir,-1);

		D3DXVECTOR3 pos = get2DDrawer()->GetCameraPos();
		pos.y -= 1;
		get2DDrawer()->SetCameraPos(pos.x, pos.y);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	
}

void ParticalEditState::OnEditTextUpdate(CEventHandler* sys, CDexObject* object,stEvent event)
{
	ParticalEditState* system = (ParticalEditState*)sys;
	string str = system->m_editBox->GetEditText();
	DEX_ENSURE(str != "");
	int height = CDexFun::str_to_int(str);
	DexGameEngine::getEngine()->DoLuaString(str.c_str());
}

void ParticalEditState::OnEventClickTalkSystem(CEventHandler* sys, CDexObject* object,stEvent event)
{
	getLog()->BeginLog();
	getLog()->Log(log_ok, "click");
	getLog()->EndLog();
}

TestPanel::TestPanel()
{

}
TestPanel::~TestPanel()
{

}

bool TestPanel::Init()
{
	
	return true;
}

void TestPanel::Update(int delta)
{
	if(m_editBox != NULL)
		m_editBox->Update(delta);
}
void TestPanel::Render()
{
	if(m_editBox != NULL)
		m_editBox->Render();
}

void TestPanel::OnEditTextUpdate(CDexObject* object,stEvent event)
{
}