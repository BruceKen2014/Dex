
#include "DexBase/typedefine.h"
#include "DexBase/CInput.h"
#include "state/DexGameEngine.h"
#include "DexBase/CDexDraw2D.h"
#include "Source/CModelXAni.h"
#include "Pal2StateBattle.h"
#include "Pal2Global.h"



DexGameStateBattle::DexGameStateBattle(int id)
{
	m_id = id;
}
DexGameStateBattle::~DexGameStateBattle()
{

}
bool DexGameStateBattle::ApplyRes() 
{
	D3DXVECTOR3 cameraPos(0, 100.0f,-200.0f);
	D3DXVECTOR3 lookAtPos(0, 0, 0);	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);	
	// Build view matrix.
	DexGameEngine::getEngine()->LookAtLH(&cameraPos, &lookAtPos, &upDir);
	m_bApply = true;
	return true;
}
void DexGameStateBattle::ReleaseRes()
{
	m_bApply = false;
}

void DexGameStateBattle::Update(int delta)
{
	getGlobal()->g_pJingtian->Update();
}
void DexGameStateBattle::BeginRender()
{
	DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
	DexGameEngine::getEngine()->GetDevice()->BeginScene();

}
void DexGameStateBattle::Render()
{
	DexGameEngine::getEngine()->RenderCoorLines();
	getGlobal()->g_pJingtian->Render();
	get2DDrawer()->SetTextSize(15);
	get2DDrawer()->DrawString(0,0,"fps:%d", DexGameEngine::getEngine()->getFps());
	D3DXVECTOR3 camera_pos = DexGameEngine::getEngine()->getCamera()->GetPosition();
	get2DDrawer()->DrawString(0,15,"camera:X:%d Y:%d Z:%d", (int)camera_pos.x, (int)camera_pos.y, (int)camera_pos.z);
	
}
void DexGameStateBattle::EndRender()
{
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	DexGameEngine::getEngine()->GetDevice()->Present(NULL, NULL, NULL, NULL);
}
void DexGameStateBattle::MouseWheelFront()
{
	DexGameEngine::getEngine()->RotateCameraByFocus(0.2f);
}
void DexGameStateBattle::MouseWheelBack()
{
	DexGameEngine::getEngine()->RotateCameraByFocus(-0.2f);
}
void DexGameStateBattle::MouseMove(int xPos, int yPos)
{

}
void DexGameStateBattle::MouseLDown(int xPos, int yPos)
{

}
void DexGameStateBattle::MouseLUp(int xPos, int yPos)
{

}
void DexGameStateBattle::MouseRUp(int xPos, int yPos)
{

}
void DexGameStateBattle::KeyUp()
{

}
void DexGameStateBattle::KeyDown()
{
	float vel = 4.0f;	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_S))
	{	
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, vel);
	}	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_W))
	{	
		DexGameEngine::getEngine()->MoveCamera(CCamera::DIR_VIEW, -vel);
	}
}