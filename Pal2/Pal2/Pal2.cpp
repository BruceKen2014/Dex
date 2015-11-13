
#pragma comment(lib, "Dex.lib")

#include "state/DexGameEngine.h"
#include "widget/DexDesktop.h"

#include "resource.h"
#include "Pal2StateTown.h"
#include "Pal2StateBattle.h"
#include "Pal2Global.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KEYUP:
		break;
	case WM_CHAR:
		{
			getDesktop()->OnKeyChar(char(wParam));
			break;
		}
	case WM_IME_SETCONTEXT:
		{
			int i = 0;
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevhInst, LPSTR cmdLine, int show)
{
	DexGameEngine::getEngine()->SetHInstance(hInst);
	DexGameEngine::getEngine()->SetIcon(IDI_ICON1);
	DexGameEngine::getEngine()->SetMsgPro(MsgProc);
	DexGameEngine::getEngine()->SetFullScreen(false);
	DexGameEngine::getEngine()->Initialize();
	//DEXENGINE_INITGLOBAL(DexGlobalPal2);
	DexGlobalPal2* gl = new DexGlobalPal2;
	gl->Init();
	DexGameEngine::getEngine()->SetGlobalVariable(gl);
	DexGameStateTown* pStateTown = new DexGameStateTown(10);
	DexGameStateBattle* pStateBattle = new DexGameStateBattle(11);
	DexGameEngine::getEngine()->AddState(pStateTown);
	DexGameEngine::getEngine()->AddState(pStateBattle);
	DexGameEngine::getEngine()->SetCurrState(pStateBattle->GetStateId());
	DexGameEngine::getEngine()->Run();
	DexGameEngine::getEngine()->ShutDownEngine();
	return 0;
}