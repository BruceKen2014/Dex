

#include "Pal2Global.h"
#include "Source/CModelXAni.h"
#include "state/DexGameEngine.h"
DexGlobalPal2::DexGlobalPal2()
{

}
DexGlobalPal2::~DexGlobalPal2()
{

}
bool DexGlobalPal2::Init()
{
	g_pJingtian = new CModelXAni(DexGameEngine::getEngine()->GetDevice()); 
	if(FAILED(g_pJingtian->LoadXFile("model/jingtian.x")))
	{
		return false;
	}
	g_pJingtian->SetPosition(0,0,0);
	g_pJingtian->SetRenderSphere(false);
	g_pJingtian->IniDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	g_pJingtian->SetDirection(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
	//g_Jintian->SetVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	g_pJingtian->SetAniSpeed(25);
	//g_Jintian->SetScale(2.0f);
	g_pJingtian->SetAnimation(0);
	g_pJingtian->SetScale(0.5);
	g_pJingtian->SetBallScale(0.5f);
	g_pJingtian->SetVel(0.0f);
	g_pJingtian->SetMoving(true);
	return true;
}

DexGlobalPal2* getGlobal()
{
	DexGlobalPal2* temp = (DexGlobalPal2*) DexGameEngine::getEngine()->GetGlobalVariable();
	return temp;
	//return (DexGlobalPal2*) DexGameEngine::getEngine()->GetGlobalVariable();
}
