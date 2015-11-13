
#include "DexBase/typedefine.h"
#include "DexBase/CDexDraw2D.h"
#include "Source/CModelXAni.h"
#include "state/DexGameEngine.h"
#include "Pal2StateTown.h"


DexGameStateTown::DexGameStateTown(int id)
{
	m_id = id;
}
DexGameStateTown::~DexGameStateTown()
{

}
bool DexGameStateTown::ApplyRes() 
{
	m_bApply = true;
	return true;
}
void DexGameStateTown::ReleaseRes()
{
	m_bApply = false;
}

void DexGameStateTown::Update(int delta)
{
}
void DexGameStateTown::BeginRender()
{
	DexGameEngine::getEngine()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
	DexGameEngine::getEngine()->GetDevice()->BeginScene();
}
void DexGameStateTown::Render()
{
	get2DDrawer()->DrawString(0,0,"fps:%d", DexGameEngine::getEngine()->getFps());
}
void DexGameStateTown::EndRender()
{
	DexGameEngine::getEngine()->GetDevice()->EndScene();
	DexGameEngine::getEngine()->GetDevice()->Present(NULL, NULL, NULL, NULL);
}
void DexGameStateTown::MouseWheelFront()
{

}
void DexGameStateTown::MouseWheelBack()
{

}
void DexGameStateTown::MouseMove(int xPos, int yPos)
{

}
void DexGameStateTown::MouseLDown(int xPos, int yPos)
{

}
void DexGameStateTown::MouseLUp(int xPos, int yPos)
{

}
void DexGameStateTown::MouseRUp(int xPos, int yPos)
{

}
void DexGameStateTown::KeyUp()
{

}
void DexGameStateTown::KeyDown()
{

}