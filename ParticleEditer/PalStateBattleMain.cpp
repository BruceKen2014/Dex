
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

void PalGameStateBattleMain::InitVertexShader()
{
	ID3DXBuffer* shaderBuffer = NULL;
	ID3DXBuffer* errorBuffer = NULL;
	HRESULT hr = D3DXCompileShaderFromFile("VertexShader.fx", 0, 0, "SetColor", "vs_2_0", D3DXSHADER_DEBUG, &shaderBuffer, &errorBuffer, &BasicConstTable);
	if (errorBuffer)
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, (char*)(errorBuffer->GetBufferPointer()));
		getLog()->EndLog();
		errorBuffer->Release();
	}
	DexGameEngine::getEngine()->GetDevice()->CreateVertexShader((DWORD*)shaderBuffer->GetBufferPointer(), &BasicShader);
	shaderBuffer->Release();
	WVPMatrixHandle = BasicConstTable->GetConstantByName(0, "WVPMatrix");
	ColorHandle = BasicConstTable->GetConstantByName(0, "color");
}
void PalGameStateBattleMain::UpdateVertexShader(int32 delta)
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
		getLog()->BeginLog();
		getLog()->Log(log_error, "pixel shader version error!");
		getLog()->EndLog();
	}
	ID3DXBuffer* shaderBuffer = NULL;
	ID3DXBuffer* errorBuffer = NULL;
	//编译shader
	HRESULT hr = D3DXCompileShaderFromFile("tex2Shader.fx", 0, 0, "PS_Main", "ps_1_1", D3DXSHADER_DEBUG, &shaderBuffer, &errorBuffer, &pixelConstTable);
	if (errorBuffer)
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, (char*)(errorBuffer->GetBufferPointer()));
		getLog()->EndLog();
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

void PalGameStateBattleMain::UpdatePixelShader(int32 delta)
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
	
	DexVector3 cameraPos(-200, 100.0f,-5.0f);
	DexVector3 lookAtPos(0, 0, 0);
	DexVector3 upDir(0.0f, 1.0f, 0.0f);
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
	
	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/Old Book/Book.dae");
	daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/mickey/Mickey_Mouse.dae",0);
	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/ArmyPilot/ArmyPilot2.dae");
	/*
	daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/c1004/mws/c1004.xml",1);
	DexGameEngine::getEngine()->ReadFFSkeletonInfo(daeModel, "model/dae/c1004/mws/c1004.mws");
	DexGameEngine::getEngine()->ReadActInfoFxii(daeModel, "model/dae/c1004/mws/run.act");
	*/
	/*
	daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/m1014/mws/m1014.dae", 1);
	DexGameEngine::getEngine()->ReadFFSkeletonInfo(daeModel, "model/dae/m1014/mws/m1014.mws");
	DexGameEngine::getEngine()->ReadActInfoFxii(daeModel, "model/dae/m1014/mws/idle0.act");
	*/
	//daeModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/dae/weapon/mws/w0002.xml", 1);
	daeModel->SetJointScale(0.01f);
	//daeModel->SetAnimateType(SkinMeshAnimateType_Loop_Back);
	daeModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT| DEXRENDER_LIGHT_POINT);
	daeModel->SetRenderFlag(SKINMESH_RENDER_JOINT |SKINMESH_RENDER_JOINT2JOINT | //SKINMESH_RENDER_VERTEX2JOINT|
		SKINMESH_RENDER_MESH );
	daeModel->SetAmbientColor(DexColor(200, 200, 200));
	
	mwsModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/mws/alc1002_door.mws");
	objModel = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/obj/RYU.obj");
	objModel->SetLightFlag(DEXRENDER_LIGHT_ENABLE | DEXRENDER_LIGHT_AMBIENT | DEXRENDER_LIGHT_POINT);
	//objModel->SetRenderFlag(SKINMESH_RENDER_ALL_FLAGS);
	ms3d = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel("model/ms3d/Model.ms3d");
	ms3d->SetAnimateType(SkinMeshAnimateType_Loop);
	ms3d->SetAnimateRatio(1.0f);
	ms3d->SetRenderFlag(SKINMESH_RENDER_ALL_FLAGS);
	ms3d->SetLightFlag(DEXRENDER_LIGHT_ALL_ON);
	DexMatrix4x4 world_matrix; 

	testMesh = new DexSkinMesh(2000); 
	testMesh->SetAnimateTime(0, 2000);
	testMesh->SetLightFlag(true);
	DexMatrix4x4 temp_matrix;	temp_matrix.Identity();
	temp_matrix.SetPosition(0, 0, 0); testMesh->AddJoint(1, -1, temp_matrix);
	testMesh->AddJointKeyFrameTrans(1, 0, DexVector3(0.0f, 0.0f, 0.0f));
	testMesh->AddJointKeyFrameRotation(1, 1000, DexVector3(0.0f, 0.0f, 1.0f), _getRadian(-90.0f));
	testMesh->AddJointKeyFrameTrans(1, 2000, DexVector3(0.0f, 0.0f, 0.0f));

	temp_matrix.SetPosition(100, 0, 0); testMesh->AddJoint(2, 1, temp_matrix);
	testMesh->AddJointKeyFrame(2, 0, DexVector3(100.0f, 0.0f, 0.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);
	testMesh->AddJointKeyFrame(2, 1000, DexVector3(100.0f, 0.0f, 0.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(1.0f, 0.0f, 0.0f), _getRadian(90.0f));
	testMesh->AddJointKeyFrame(2, 2000, DexVector3(100.0f, 0.0f, 0.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(1.0f, 0.0f, 0.0f), _getRadian(0.0f));

	temp_matrix.SetPosition(0, 0, 20); testMesh->AddJoint(3, 2, temp_matrix);
	testMesh->AddJointKeyFrame(3, 0, DexVector3(0.0f, 0.0f, 20.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);
	testMesh->AddJointKeyFrame(3, 1000, DexVector3(0.0f, 0.0f, 20.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);
	testMesh->AddJointKeyFrame(3, 2000, DexVector3(0.0f, 0.0f, 20.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);

	temp_matrix.SetPosition(20, 0, 0); testMesh->AddJoint(4, 2, temp_matrix);
	testMesh->AddJointKeyFrame(4, 0,    DexVector3(20.0f, 0.0f, 0.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);
	testMesh->AddJointKeyFrame(4, 1000, DexVector3(20.0f, 0.0f, 0.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);
	testMesh->AddJointKeyFrame(4, 2000, DexVector3(20.0f, 0.0f, 0.0f), DexVector3(1.0f, 1.0f, 1.0f), DexVector3(0.0f, 1.0f, 0.0f), 0.0f);
	testMesh->AddTexture(getUiSrcMgrSingleton()->getUiTexFactory()->FindTex("b23.png"));
	DexMaterial material1;
	material1.diffuse = DexColor(1.0f, 1.0f, 1.0f);
	material1.ambient = DexColor(0.0f, 0.0f, 0.0f);
	material1.emissive = DexColor(0.0f, 0.0f, 0.0f);
	material1.specular = DexColor(0.0f, 0.0f, 0.0f);
	testMesh->AddMaterial(material1);
	testMesh->AddTexture(getUiSrcMgrSingleton()->getUiTexFactory()->FindTex("女孩0.png"));
	DexMaterial material2;
	material2.diffuse = DexColor(1.0f, 1.0f, 1.0f);
	material2.ambient = DexColor(0.0f, 0.0f, 0.0f);
	material2.emissive = DexColor(0.0f, 0.0f, 0.0f);
	material2.specular = DexColor(0.0f, 0.0f, 0.0f);
	testMesh->AddMaterial(material2);
	DexSkinMesh::DexMesh* mesh1 = testMesh->AddMesh(1);
	DexSkinMesh::DexMesh* mesh2 = testMesh->AddMesh(2);
	int16 arrJointId[2] = {0,0};
	float arrJointWeight[2] = {0.0f, 0.0f};
	arrJointId[0] = 4; arrJointWeight[0] = 1.0f;
	testMesh->AddVertex(1, DexVector3(30.0f, 0, -30.0f), DexVector3(1.0f, -1.0f, -1.0f).Normalize(), DexVector2(0.33f, 1.0f), arrJointId, arrJointWeight, 1);
	arrJointId[0] = 4; arrJointWeight[0] = 1.0f;
	testMesh->AddVertex(1, DexVector3(30.0f, 0, 30.0f), DexVector3(1.0f, -1.0f, 1.0f).Normalize(), DexVector2(0.66f, 1.0f), arrJointId, arrJointWeight, 1);
	arrJointId[0] = 4; arrJointWeight[0] = 0.5f; arrJointId[1] = 3; arrJointWeight[1] = 0.5f;
	testMesh->AddVertex(1, DexVector3(-30.0f, 0, 30.0f), DexVector3(-1.0f, -1.0f, 1.0f).Normalize(), DexVector2(0.99f, 1.0f), arrJointId, arrJointWeight, 2);
	arrJointId[0] = 4; arrJointWeight[0] = 0.5f; arrJointId[1] = 3; arrJointWeight[1] = 0.5f;
	testMesh->AddVertex(1, DexVector3(-30.0f, 0, -30.0f), DexVector3(-1.0f, -1.0f, -1.0f).Normalize(), DexVector2(0.0f, 1.0f), arrJointId, arrJointWeight, 2);
	arrJointId[0] = 4; arrJointWeight[0] = 1.0f;
	testMesh->AddVertex(1, DexVector3(30.0f, 60, -30.0f), DexVector3(1.0f, 1.0f, -1.0f).Normalize(), DexVector2(0.33f, 0.0f), arrJointId, arrJointWeight, 1);
	arrJointId[0] = 4; arrJointWeight[0] = 1.0f;
	testMesh->AddVertex(1, DexVector3(30.0f, 60, 30.0f), DexVector3(1.0f, 1.0f, -1.0f).Normalize(), DexVector2(0.33f, 0.0f), arrJointId, arrJointWeight, 1);
	arrJointId[0] = 3; arrJointWeight[0] = 1.0f;
	testMesh->AddVertex(1, DexVector3(-30.0f, 60, 30.0f), DexVector3(-1.0f, 1.0f, 1.0f).Normalize(), DexVector2(0.99f, 0.0f), arrJointId, arrJointWeight, 1);
	arrJointId[0] = 3; arrJointWeight[0] = 1.0f;
	testMesh->AddVertex(1, DexVector3(-30.0f, 60, -30.0f), DexVector3(-1.0f, 1.0f, -1.0f).Normalize(), DexVector2(0.0f, 0.0f), arrJointId, arrJointWeight, 1);
	
	mesh1->m_iMaterialId = mesh1->m_iTextureId = 0;
	int32 indices1[] = { 0, 4, 1, 1, 4, 5, 2, 6, 3, 6, 7, 3, 0, 3, 4, 3, 7, 4, 1, 5, 2, 2, 5, 6 };
	testMesh->SetMeshIndices(1, indices1, sizeof(indices1) / sizeof(int32));
	//mesh2 vertex
	
	mesh2->m_iMaterialId = mesh2->m_iTextureId = 1;
	int32 indices2[] = { 0, 1, 3, 3, 1, 2, 4, 7, 5, 7, 6, 5 };
	testMesh->SetMeshIndices(2, indices2, sizeof(indices2) / sizeof(int32));
	
	testMesh->AddVertex(2, DexVector3(30.0f, 0, -30.0f), DexVector3(1.0f, -1.0f, -1.0f).Normalize(), DexVector2(1.0f, 0.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(30.0f, 0, 30.0f), DexVector3(1.0f, -1.0f, 1.0f).Normalize(), DexVector2(1.0f, 1.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(-30.0f, 0, 30.0f), DexVector3(-1.0f, -1.0f, 1.0f).Normalize(), DexVector2(0.0f, 1.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(-30.0f, 0, -30.0f), DexVector3(-1.0f, -1.0f, -1.0f).Normalize(), DexVector2(0.0f, 0.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(30.0f, 60, -30.0f), DexVector3(1.0f, 1.0f, -1.0f).Normalize(), DexVector2(1.0f, 0.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(30.0f, 60, 30.0f), DexVector3(1.0f, 1.0f, 1.0f).Normalize(), DexVector2(1.0f, 1.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(-30.0f, 60, 30.0f), DexVector3(-1.0f, 1.0f, 1.0f).Normalize(), DexVector2(0.0f, 1.0f), arrJointId, arrJointWeight, 1);
	testMesh->AddVertex(2, DexVector3(-30.0f, 60, -30.0f), DexVector3(-1.0f, 1.0f, -1.0f).Normalize(), DexVector2(0.0f, 0.0f), arrJointId, arrJointWeight, 1);

	testMesh->CalculateVertex();
	testMesh->SetRenderFlag(SKINMESH_RENDER_ALL_FLAGS);
	testMesh->SetLightFlag(DEXRENDER_LIGHT_ALL_ON);
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
	for(std::map<string, PalPanelInterface*>::iterator ite = m_mapBattlePanels.begin();
		 ite != m_mapBattlePanels.end(); ++ite)
	{
		if(ite->second != NULL)
		{
			ite->second->Update(delta);
		}
		
	}
	ms3d->Update(delta/2);
	objModel->Update(delta);
	daeModel->Update(delta); 
	UpdateVertexShader(delta);
	UpdatePixelShader(delta);
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
	matrix.Scale(30.0f, 30.0f, 30.0f);
	//matrix.RotateY(rotate);
	matrix.Translate(0.0f, 0.0f, 0.0f);
	daeModel->SetSceneNodeMatrix(matrix);
	daeModel->ClearPointLight();
	daeModel->SetDirectionLight(dirLight);
	daeModel->AddPointLight(pointLight1);
	daeModel->AddPointLight(pointLight2);
	daeModel->Render();

	
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