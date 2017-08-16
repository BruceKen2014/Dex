
#include "DexBase/typedefine.h"
#include "DexBase/CInput.h"
#include "DexBase/DexMemoryLeakCheck.h"
#include "widget/DexWidgetFactory.h"
#include "widget/DexWidgetImage.h"
#include "widget/CDexUiSrcMgr.h"
#include "state/DexGameEngine.h"
#include "DexBase/CDexDraw2D.h"
#include "DexModel/DexModelBase.h"
#include "DexModel/DexSkinMesh.h"
#include "DexMath/DexVector3.h"
#include "DexMath/DexQuaternion.h"
#include "Source/CModelXAni.h"
#include "PalStateBattleMain.h"
#include "DexBase/DexCollideObject.h"
#include "PalBattleStateBase.h"
#include "PalBattleStateTransition.h"
#include "DexStateCommonEvent.h"
#include "DexBase/Dex3DScene.h"
#include "DexBase/DexTextureManager.h"
#include "DexBase/DexIni.h"
#include "PalPlayer.h"
#include "PalPlayerAttackState.h"
#include "PalFightHeadManager.h"
#include "PalNumber.h"
#include "CDefineXAni.h"
#include "PalSkill.h"

#include "PalPanel_Order.h"
#include "PalPanel_SelectSkill.h"
#include "../DexModel/DexModelSkinMeshLoader.h"
#include "../widget/DexWidgetProgressBar.h"
#include "../widget/DexWidget.h"
#include "../widget/DexWidgetContainer.h"
#include <Psapi.h>
//#include "Pal2Global.h"


PalGameStateBattleMain* pTestGlobal = DexNull;
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
	pTestGlobal = this;
}
PalGameStateBattleMain::~PalGameStateBattleMain()
{

}

void PalGameStateBattleMain::InitVertexShader()
{
	ID3DXBuffer* shaderBuffer = NULL;
	ID3DXBuffer* errorBuffer = NULL;
	HRESULT hr = D3DXCompileShaderFromFile("VertexShader.fx", 0, 0, "SetColor", "vs_2_0", D3DXSHADER_DEBUG, &shaderBuffer, &errorBuffer, &BasicConstTable);
	if (errorBuffer)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_error, (char*)(errorBuffer->GetBufferPointer()));
		DexLog::getSingleton()->EndLog();
		errorBuffer->Release();
	}
	DexGameEngine::getEngine()->GetDevice()->CreateVertexShader((DWORD*)shaderBuffer->GetBufferPointer(), &BasicShader);
	shaderBuffer->Release();
	WVPMatrixHandle = BasicConstTable->GetConstantByName(0, "WVPMatrix");
	ColorHandle = BasicConstTable->GetConstantByName(0, "color");
}
void PalGameStateBattleMain::UpdateVertexShader(DInt32 delta)
{
	D3DXMATRIX matWorld, matView, matProj; 
	DexMatrix4x4 worldMatrix;
	worldMatrix.Translate(100.0f, 0.0f ,0.0f);
	memcpy(&matWorld, &worldMatrix, sizeof(DexMatrix4x4));
	//DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_WORLD, &matWorld);
	DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMATRIX	matWVP = matWorld * matView * matProj;
	BasicConstTable->SetMatrix(DexGameEngine::getEngine()->GetDevice(), WVPMatrixHandle, &matWVP);
	D3DXVECTOR4 color(1.0f, 1.0f, 0.0f, 0.5f);
	BasicConstTable->SetVector(DexGameEngine::getEngine()->GetDevice(), ColorHandle, &color);

}
void PalGameStateBattleMain::RenderVertexShader()
{
	DexGameEngine::getEngine()->GetDevice()->SetVertexShader(BasicShader);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(-10.0f, 20.0f, 0.0f);
	g_pVertexList0[1].m_pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	g_pVertexList0[2].m_pos = D3DXVECTOR3(10.0f, 20.0f, 0.0f);
	g_pVertexList0[3].m_pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	
	
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_pVertexList0, sizeof(stVertex0));
	DexGameEngine::getEngine()->GetDevice()->SetVertexShader(NULL);
}
struct CUSTOMVERTEX
{
	DexVector3 pos;
	D3DCOLOR color;
	DexVector2 uv0;
	DexVector2 uv1;
};
CUSTOMVERTEX vertex[4];
void PalGameStateBattleMain::InitPixelShader()
{
	LPDIRECT3DDEVICE9 device = DexGameEngine::getEngine()->GetDevice();
	vertex[0].color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.5f)); 
	vertex[1].color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 0.5f));
	vertex[2].color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 1.0f));
	vertex[3].color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 1.0f));
	vertex[0].pos = DexVector3(-10.0f, 20.0f, 0.0f);  vertex[0].uv0 = DexVector2(0.0f, 0.0f); vertex[0].uv1 = DexVector2(0.0f, 0.0f);
	vertex[1].pos = DexVector3(-10.0f, 0.0f, 0.0f);   vertex[1].uv0 = DexVector2(0.0f, 1.0f); vertex[1].uv1 = DexVector2(0.0f, 1.0f);
	vertex[2].pos = DexVector3(10.0f, 20.0f, 0.0f);	  vertex[2].uv0 = DexVector2(1.0f, 0.0f); vertex[2].uv1 = DexVector2(1.0f, 0.0f);
	vertex[3].pos = DexVector3(10.0f, 0.0f, 0.0f);	  vertex[3].uv0 = DexVector2(1.0f, 1.0f); vertex[3].uv1 = DexVector2(1.0f, 1.0f);
	//vertex->pos = DexVector3(-10.0f);
	D3DCAPS9 caps;
	device->GetDeviceCaps(&caps);
	//测试pixelshader的支持
	if (caps.PixelShaderVersion < D3DPS_VERSION(1, 1))
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_error, "pixel shader version error!");
		DexLog::getSingleton()->EndLog();
	}
	ID3DXBuffer* shaderBuffer = NULL;
	ID3DXBuffer* errorBuffer = NULL;
	//编译shader
	HRESULT hr = D3DXCompileShaderFromFile("tex2Shader.fx", 0, 0, "PS_Main", "ps_1_1", D3DXSHADER_DEBUG, &shaderBuffer, &errorBuffer, &pixelConstTable);
	if (errorBuffer)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_error, (char*)(errorBuffer->GetBufferPointer()));
		DexLog::getSingleton()->EndLog();
		errorBuffer->Release();
	}
	device->CreatePixelShader((DWORD*)shaderBuffer->GetBufferPointer(), &pixelShader);
	shaderBuffer->Release();

	//得到常量句柄
	ScalarHandle = pixelConstTable->GetConstantByName(0, "Scalar");
	Samp0Handle = pixelConstTable->GetConstantByName(0, "Samp0");
	Samp1Handle = pixelConstTable->GetConstantByName(0, "Samp1");
	//得到对着色器变量Samp0 Samp1的描述
	UINT count;
	pixelConstTable->GetConstantDesc(Samp0Handle, &Samp0Desc, &count);
	pixelConstTable->GetConstantDesc(Samp1Handle, &Samp1Desc, &count);
	//设置各着色器变量为初始值
	pixelConstTable->SetDefaults(DexGameEngine::getEngine()->GetDevice());

	D3DXVECTOR4 scalar(0.5f, 0.5f, 0.5f, 0.5f);
	pixelConstTable->SetVector(device, ScalarHandle, &scalar);//为着色器全局变量Scalar赋值

}

void PalGameStateBattleMain::UpdatePixelShader(DInt32 delta)
{

}

void PalGameStateBattleMain::RenderPixelShader()
{
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ |D3DFVF_DIFFUSE |D3DFVF_TEX2)
	LPDIRECT3DDEVICE9 device = DexGameEngine::getEngine()->GetDevice();
	device->SetPixelShader(pixelShader);
	device->SetTexture(Samp0Desc.RegisterIndex, getUiSrcMgrSingleton()->getUiTexFactory()->FindTex("女孩0.png")->GetTexPt());
	device->SetTexture(Samp1Desc.RegisterIndex, getUiSrcMgrSingleton()->getUiTexFactory()->FindTex("b23.png")->GetTexPt());
	device->SetFVF(D3DFVF_CUSTOMVERTEX); //这里必须设置FVF，shader里面的input接受uv0 和uv1，如果不设置数据格式，设备将不知道数据流中那一块是uv
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(CUSTOMVERTEX));
	device->SetPixelShader(NULL);
#undef D3DFVF_CUSTOMVERTEX
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
void testfunctionAddress()
{

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

void PalGameStateBattleMain::SetBackImageVisible(DBool visible)
{
	g_pImageBackgroud->setVisible(visible);
}
bool PalGameStateBattleMain::ApplyRes()
{
	DexIni iniHandle;
	DEX_ENSURE_B(iniHandle.readIniFile("ini/data.ini"));
	int time = iniHandle.getInt("GameFailDelay", "time");
	DString texture = iniHandle.getString("TerrainData", "texture");
	int width = iniHandle.getInt("TerrainData", "cWidth");
	DFloat32 posX = iniHandle.getFloat("TerrainData", "posx");
	iniHandle.clearData();
	iniHandle.writeData("map", "weather", "good");
	iniHandle.writeData("map", "npcs", 20);
	iniHandle.writeData("map", "height", 20.5f);

	iniHandle.saveFile("ini/newFile.ini");

	DexVector3 cameraPos(-200, 100.0f, -5.0f);
	DexVector3 lookAtPos(0, 0, 0);
	DexVector3 upDir(0.0f, 1.0f, 0.0f);
	// Build view matrix.
	m_pScene = new CDexScene;
	DexGameEngine::getEngine()->LookAtLH(&cameraPos, &lookAtPos, &upDir);
	DexGameEngine::getEngine()->setRendeCollideMesh(true);

	DexPanelInterface* panel = new PalPanel_Order;
	panel->Initialize();
	panel->setVisible(false);
	m_mapBattlePanels[panel->getType()] = panel;
	panel = new PalPanel_SelectSkill;
	panel->Initialize();
	panel->setVisible(false);

	m_mapBattlePanels[panel->getType()] = panel;

	Test_AddJingtian();
	//Test_AddEnemy1();
	DexGameEngine::getEngine()->GetDevice()->CreateTexture(128, 128, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &m_testTexture, NULL);
	m_testTexture->GetSurfaceLevel(0, &m_testSurface);
	//D3DXMatrixPerspectiveFovLH(&m_testProjection,D3DX_PI/4.0f,1,1,100);
	//DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_PROJECTION, & m_testProjectionOld);
	DexGameEngine::getEngine()->GetDevice()->GetRenderTarget(0, &DexGameEngine::getEngine()->m_pBackSurface);
	g_pImageBackgroud = (DexWidgetImage*)getWidgetFactory()->createWidget(widget_image, "background111");

	g_pImageBackgroud->SetTexName("b23.png");
	g_pImageBackgroud->SetPos(200, 200);
	g_pImageBackgroud->Resize(DexSize(300, 300));
	g_pImageBackgroud->ModifyFlag(Add_Flag, DexGUI::mouse_move);
	//g_pImageBackgroud->ModifyFlag(Minus_Flag, catch_event);
	g_pImageBackgroud->setVisible(false);
	g_pProgressBar = (DexWidgetProgressBar*)getWidgetFactory()->createWidget(widget_progressBar, "progressBar");
	g_pProgressBar->SetProgressImage("image1.png", DexRectF(50, 26, 60, 29), "image1.png", DexRectF(50, 23, 60, 25));
	g_pProgressBar->SetPos(210, 210);
	g_pProgressBar->Resize(DexSize(200, 10));
	g_pProgressBar->ModifyFlag(Minus_Flag, catch_event);

	g_pImageBackgroud->GetChildrenContainer()->AddWidgetToBack(g_pProgressBar);

	mwsModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/mws/alc0101_sky.mws");//mws dexmodel
																								  //mwsModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/mws/alc1002_kabe.mws");
	mwsModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | DEXRENDER_LIGHT_POINT | DEXRENDER_ENABLE_FOG);
	mwsModel->SetRenderFlag(//SKINMESH_RENDER_JOINT | SKINMESH_RENDER_JOINT2JOINT | //SKINMESH_RENDER_VERTEX2JOINT|
		SKINMESH_RENDER_MESH);
	mwsModel->SetAmbientColor(DexColor(255, 255, 255));
	//mwsModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/c1004/mws/c1004.mws");
	daeWeaponModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/weapon/w0001.dexmodel", 1);//xml
	daeWeaponModel->SetRenderFlag(SKINMESH_RENDER_JOINT | SKINMESH_RENDER_JOINT2JOINT | //SKINMESH_RENDER_VERTEX2JOINT|
		SKINMESH_RENDER_MESH);
	daeWeaponModel->SetAmbientColor(DexColor(200, 200, 200));
	daeWeaponModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | DEXRENDER_LIGHT_POINT);
	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/Old Book/Book.dae");
	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/mickey/Mickey_Mouse.dae",0);
	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/ArmyPilot/ArmyPilot2.dae");
	/*
	daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/f0011/mws/hc1004_01.xml",1);
	DexGameEngine::getEngine()->ReadFFSkeletonInfo(daeModel, "model/dae/f0011/mws/f0011.mws");
	DexGameEngine::getEngine()->ReadActInfoFxii(daeModel, "model/dae/f0011/mws/f0011_a.act");
	*/

	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/c1004/c1004.dexmodel",1);//xml
	//DexGameEngine::getEngine()->ReadFFSkeletonInfo(daeModel, "model/dae/c1004/c1004.mws");
	//DexGameEngine::getEngine()->ReadModelAnimation(daeModel, "model/dae/c1004/act/mgc_4.act");


	daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/monster/m1019/m1019.dexmodel", 1);
	DexGameEngine::getEngine()->ReadFFSkeletonInfo(daeModel, "model/dae/monster/m1019/m1019.mws");
	DexGameEngine::getEngine()->ReadModelAnimation(daeModel, "model/dae/monster/m1019/act/atk_m_1.act");

	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/weapon/mws/w0002.xml", 1);
	daeModel->SetJointScale(0.01f);
	//daeModel->SetAnimateType(SkinMeshAnimateType_Loop_Back);
	daeModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | DEXRENDER_LIGHT_POINT);// | DEXRENDER_ENABLE_FOG);
	daeModel->SetRenderFlag(SKINMESH_RENDER_JOINT | SKINMESH_RENDER_JOINT2JOINT | //SKINMESH_RENDER_VERTEX2JOINT|
		SKINMESH_RENDER_MESH);
	daeModel->SetAmbientColor(DexColor(200, 200, 200));
	DInt32 order[24] = { 0, 1, 2, 9, 23, 12, 13, 17, 14, 21, 22, 20, 11, 15, 16, 19, 18, 3, 4, 5, 6, 7, 8, 10 };
	DVector<DInt32> tempOrder(order, order + sizeof(order) / sizeof(order[0]));
	//daeModel->SetOrderInfo(tempOrder);

	objModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/obj/RYU.dexmodel");//obj
	objModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | DEXRENDER_LIGHT_POINT | DEXRENDER_ENABLE_FOG);
	objModel->SetRenderFlag(SKINMESH_RENDER_MESH);
	ms3d = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/ms3d/Model.ms3d"); //ms3d
	ms3d->SetAnimateType(SkinMeshAnimateType_Loop);
	ms3d->SetAnimateRatio(1.0f);
	ms3d->SetRenderFlag(SKINMESH_RENDER_ALL_FLAGS);
	ms3d->SetLightFlag(DEXRENDER_LIGHT_ALL_ON | DEXRENDER_ENABLE_FOG);
	DexMatrix4x4 world_matrix;


	groundModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dexmodel/ground.dexmodeltext");//dexmodeltext

	testMesh = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dexmodel/cube.dexmodeltext");//dexmodeltext 
	DexGameEngine::getEngine()->ReadModelAnimation(testMesh, "model/dexmodel/cube.dexact");

	light.type = DexLight::DexLight_POINT;
	light.id = 1;
	light.attenuation0 = 1.0f;
	light.diffuse = DexColor(1.0f, 1.0f, 1.0f);
	light.SetPointLight(DexVector3(0.0f, 300.0f, 0.0f), 2000.0f);
	DexGameEngine::getEngine()->AddLight(light);
	DexGameEngine::getEngine()->SetWorldAmbient(DEXCOLOR_BLACK);
	DexGameEngine::getEngine()->SetLightIdEnable(1, true);
	DexGameEngine::getEngine()->SetLightEnable(1);
	//DexGameEngine::getEngine()->SetRenderMode(DexRenderMode_LINE);
	world_matrix.Identity();
	world_matrix.Scale(3.0f, 3.0f, 3.0f);
	//world_matrix.RotateY(_getRadian(-90.0f));
	DexVector3 skin_pos(100.0f, 100.0f, 100.0f);
	world_matrix.Translate(skin_pos);
	testMesh->SetSceneNodeMatrix(world_matrix);
	//ms3d->SetSceneNodeMatrix(world_matrix);

	world_matrix.Identity();
	world_matrix.Scale(1000.0f, 1000.0f, 1000.0f);
	objModel->SetSceneNodeMatrix(world_matrix);
	world_matrix.Identity();
	world_matrix.Scale(10.0f, 10.0f, 10.0f);
	world_matrix.Translate(-50.0f, 0.0f, 50.0f);

	//daeModel->SetSceneNodeMatrix(world_matrix);

	InitVertexShader();
	InitPixelShader();
	m_bApply = true;
	
	DexGameEngine::stDexModuleInfo* moduleInfo = DexGameEngine::getEngine()->GetModuleInfo();
	DexLog::getSingleton()->LogLine(log_error, " 模块地址 %X  程序入口地址:%X 大小%d 函数地址 %X", moduleInfo->iModuleAddress, moduleInfo->iStartFunctionAddress, moduleInfo->iModuleSize, testfunctionAddress);
	//DInt32 offset = (void*)&(PalGameStateBattleMain::ApplyRes) - (DInt32)moduleInfo.EntryPoint;
	//DexLog::getSingleton()->LogLine(log_error, "ApplyRes 地址%0X 偏移量:%0X", ApplyRes, 3);
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
const int testInt = 100;
bool PalGameStateBattleMain::Update(int delta)
{	
	//light.attenuation0 = 1.0f;
	DexGameEngine::getEngine()->GetLight(1)->SetPointLight(DexGameEngine::getEngine()->getCamera()->GetPosition(), 2000.0f);
	DexGameEngine::getEngine()->CalculateLightData();
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
	for(std::map<string, DexPanelInterface*>::iterator ite = m_mapBattlePanels.begin();
		 ite != m_mapBattlePanels.end(); ++ite)
	{
		if(ite->second != NULL)
		{
			ite->second->Update(delta);
		}
		
	}
	ms3d->Update(delta/2);
	objModel->Update(delta);
	mwsModel->Update(delta); 
	static int testDelta = 16;
	daeModel->Update(testDelta);
	static DInt32 jointIndex = 14;
	DexSkinMesh::Joint* pJoint = daeModel->FindJoint(jointIndex);
	if (pJoint != nullptr)
		daeWeaponModel->SetSceneNodeMatrix(pJoint->world_matrix);
	else 
		daeWeaponModel->SetSceneNodeMatrix(DexMatrix4x4());
	daeWeaponModel->Update(delta);
	UpdateVertexShader(delta);
	UpdatePixelShader(delta);
	groundModel->Update(delta);
	testMesh->Update(delta/4);
	static DFloat32 progressRadio = 0.0f;
	progressRadio += 0.002f;
	g_pProgressBar->SetProgress(progressRadio);
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
	//DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, DexGameEngine::getEngine()->m_pBackSurface);
	//DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, getD3DColor(m_pScene->getClearColor()), 1.0f, 0);

}
void PalGameStateBattleMain::Render()
{
	m_pScene->Render();
	m_pBattleMainMachine->Render();
	DexGameEngine::getEngine()->RenderCoorLines();
	stDexPointLight pointLight1(DexVector4(1.0f, 1.0f, 1.0f, 1.0f), DexVector4(DexGameEngine::getEngine()->getCamera()->GetPosition(), 1.0f),
		DexVector4(500.0f, 0.0f, 0.0f, 0.0f));
	stDexPointLight pointLight2(DexVector4(0.0f, 1.0f, 0.0f, 1.0f), DexVector4(0.0f, 0.0f, -200.0f, 1.0f),
		DexVector4(1000.0f, 0.0f, 0.0f, 0.0f));
	stDexDirectionLight dirLight(DexVector4(1.0f, 1.0f, 1.0f, 2.0f), DexVector4(0.0f, -1.0f, 0.0f, 1.0f));
	
	DexMatrix4x4 matrix;
	static float rotate = 0.0f;
	rotate += 0.01f;
	matrix.RotateY(rotate);
	matrix.Translate(-50.0f, 0.0f, 50.0f);
	ms3d->SetSceneNodeMatrix(matrix);
	ms3d->ClearPointLight();
	ms3d->SetDirectionLight(dirLight);
	ms3d->AddPointLight(pointLight1);
	ms3d->AddPointLight(pointLight2);
	ms3d->Render();

	
	matrix.Identity();
	matrix.Scale(100.0f, 100.0f, 100.0f);
	matrix.RotateY(rotate);
	matrix.Translate(200.0f, 0.0f, 200.0f); 
	
	objModel->SetSceneNodeMatrix(matrix);
	objModel->ClearPointLight();
	objModel->AddPointLight(pointLight1);
	objModel->AddPointLight(pointLight2);
	objModel->SetDirectionLight(dirLight);
	objModel->Render();
	//RenderVertexShader();
	//RenderPixelShader();
	testMesh->ClearPointLight();
	testMesh->SetDirectionLight(dirLight);
	testMesh->AddPointLight(pointLight1);
	testMesh->AddPointLight(pointLight2);
	testMesh->Render();
	groundModel->ClearPointLight();
	groundModel->SetDirectionLight(dirLight);
	groundModel->AddPointLight(pointLight1);
	groundModel->AddPointLight(pointLight2);
	groundModel->Render();
	
	matrix.Identity();
	matrix.Scale(10.0f, 10.0f, 10.0f);
	//matrix.RotateY(rotate/50);
	//matrix.Translate(1.0f, 0.0f, 0.0f);
	FILE* pFile = fopen("model/dae/test.txt", "rb");
	const DInt16  iMaxLineByte = 2048;
	char tempLineData[iMaxLineByte];
	fgets(tempLineData, iMaxLineByte, pFile);
	daeModel->iHideMeshIndex = atoi(tempLineData);
	fgets(tempLineData, iMaxLineByte, pFile);
	daeModel->iOnlyShowIndex = atoi(tempLineData);
	fclose(pFile);
	mwsModel->SetSceneNodeMatrix(matrix);
	mwsModel->ClearPointLight();
	mwsModel->AddPointLight(pointLight1);
	mwsModel->AddPointLight(pointLight2);
	mwsModel->SetDirectionLight(dirLight);
	mwsModel->Render();
	matrix.Identity();
	matrix.Scale(1.0f, 1.0f, 1.0f);
	//matrix.RotateY(rotate);
	matrix.Translate(20.0f, 0.0f, 0.0f);
	//daeWeaponModel->SetSceneNodeMatrix(matrix);
	daeWeaponModel->ClearPointLight();
	daeWeaponModel->SetDirectionLight(dirLight);
	pointLight1.rangeAtte.x = 100;
	daeWeaponModel->AddPointLight(pointLight1);
	daeWeaponModel->AddPointLight(pointLight2);
	//daeWeaponModel->Render();
	

	matrix.Identity();
	matrix.Scale(1.0f, 1.0f, 1.0f);
	//matrix.RotateY(rotate);
	matrix.Translate(10.0f, 0.0f, 0.0f);
	daeModel->SetSceneNodeMatrix(matrix);
	daeModel->ClearPointLight();
	daeModel->SetDirectionLight(dirLight);
	pointLight1.rangeAtte.x = 100;
	daeModel->AddPointLight(pointLight1);
	daeModel->AddPointLight(pointLight2);
	daeModel->Render();
	//DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, test_primitive_vertex, 8, test_primitive_indice, 12, sizeof(stVertex0));
	//getGlobal()->g_pJingtian->Render();
	get2DDrawer()->BeginDraw2D();
	for(std::map<string, DexPanelInterface*>::iterator ite = m_mapBattlePanels.begin();
		 ite != m_mapBattlePanels.end(); ++ite)
	{
		if(ite->second != NULL)
		{
			//ite->second->Render();
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

	//m_pFightHeadManager->Render();
	//m_pNumberManager->Render();
	get2DDrawer()->SetTextSize(15);
	get2DDrawer()->DrawString(0,0,"fps:%d", DexGameEngine::getEngine()->getFps());
	D3DXVECTOR3 pos;
	memcpy(&pos, &DexGameEngine::getEngine()->getCamera()->GetPosition(), sizeof(D3DXVECTOR3));
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
	g_pImageBackgroud->Render();
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
}
void PalGameStateBattleMain::MouseWheelFront()
{
	//camera update self
	//DexGameEngine::getEngine()->RotateCameraByFocus(0.2f);
}
void PalGameStateBattleMain::MouseWheelBack()
{
	//camera update self
	//DexGameEngine::getEngine()->RotateCameraByFocus(-0.2f);
}
void PalGameStateBattleMain::MouseMove(int xPos, int yPos)
{

}
void PalGameStateBattleMain::MouseLDown(int xPos, int yPos)
{
	m_pBattleMainMachine->MouseLDown(xPos, yPos);
	return;
	CPlane plane(DexVector3(0, 0, 0), DexVector3(50, 0, 0), DexVector3(0, 0, 50));
	stRay ray;
	DexGameEngine::getEngine()->GetRay(xPos, yPos, ray);
	D3DXVECTOR3 crossPoint;
	memcpy(&crossPoint, &plane.GetCrossPoint(ray), sizeof(D3DXVECTOR3)); 
	g_pJingtian->getAniModel()->SetAnimation(4);
	D3DXVECTOR3 collide_center; 
	panda->getCollideObject()->getCenter(collide_center);
	D3DXVECTOR3 set_vector = crossPoint - collide_center;
	set_vector.y = 0;
	panda->setDirection(set_vector);
	//DexGameEngine::getEngine()->MoveCamera(DexVector3(set_vector.x, set_vector.y, set_vector.z),4.0f, true);
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
	if (0)
	{//camera update self
		if (CInputSystem::GetInputSingleton().KeyDown(DIK_S))
		{
			DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, vel);
		}
		if (CInputSystem::GetInputSingleton().KeyDown(DIK_W))
		{
			DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, -vel);
		}
		if (CInputSystem::GetInputSingleton().KeyDown(DIK_SPACE))
		{
			DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_UP, 5);
		}
		if (CInputSystem::GetInputSingleton().KeyDown(DIK_LCONTROL))
		{
			DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_UP, -5);
		}

	}
	if (CInputSystem::GetInputSingleton().KeyUp(DIK_LCONTROL))
	{
		DexGameEngine::getEngine()->SetRenderMode(DexGameEngine::getEngine()->GetRenderMode()
			== DexRenderMode_LINE ? DexRenderMode_TRIANGLE : DexRenderMode_LINE);
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

void PalGameStateBattleMain::OnDragFiles(const DVector<DString>& FileNames)
{
	size_t len = FileNames[0].length();
	if (FileNames[0][len - 1] == 's')
	{//mws model
		delete mwsModel;
		mwsModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel(FileNames[0].c_str());//mws dexmodel
	}
	else if (FileNames[0][len - 1] == 'e')
	{//dae weapon
		delete daeWeaponModel;
		daeWeaponModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel(FileNames[0].c_str(), 1);//dae weapon dexmodel
		daeWeaponModel->SetRenderFlag(//SKINMESH_RENDER_VERTEX2JOINT|
			SKINMESH_RENDER_MESH);
		daeWeaponModel->SetAmbientColor(DexColor(200, 200, 200));
		daeWeaponModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | DEXRENDER_LIGHT_POINT);
	}
	if (FileNames[0][len - 1] == 't')
	{//act file
		DexGameEngine::getEngine()->ReadModelAnimation(daeModel, FileNames[0].c_str());
	}
}
