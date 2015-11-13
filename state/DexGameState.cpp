


#include "DexGameState.h"
DexGameState::DexGameState():m_bApply(false)
{
	m_pScene = NULL;
}
DexGameState::~DexGameState()
{

}
void DexGameState::MouseWheelFront() 
{

}
void DexGameState::MouseWheelBack()
{

}
void DexGameState::MouseMove(int xPos, int yPos)
{

}

void DexGameState::MouseLDown(int xPos, int yPos)
{

}
void DexGameState::MouseLUp(int xPos, int yPos)
{

}
void DexGameState::MouseRDown(int xPos, int yPos)
{

}
void DexGameState::MouseRUp(int xPos, int yPos)
{

}
void DexGameState::KeyUp()
{

}
void DexGameState::KeyDown()
{

}
//==========================================================================
#include "DexGameEngine.h"
#include "../DexBase/CDexDraw2D.h"


DexGameStateLoading::DexGameStateLoading()
{

}
DexGameStateLoading::~DexGameStateLoading()
{

}
bool DexGameStateLoading::ApplyRes()
{
	return true;
}

void DexGameStateLoading::ReleaseRes()
{
}

bool DexGameStateLoading::Update(int delta)
{
	return true;
}
void DexGameStateLoading::BeginRender()
{

}
void DexGameStateLoading::Render()
{
	DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		0xff00ff00, 1.0f, 0);
	DexGameEngine::getEngine()->GetDevice()->BeginScene();
	get2DDrawer()->DrawString(100, 100, "loading...");
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	DexGameEngine::getEngine()->GetDevice()->Present(NULL, NULL, NULL, NULL);
}
void DexGameStateLoading::EndRender()
{

}