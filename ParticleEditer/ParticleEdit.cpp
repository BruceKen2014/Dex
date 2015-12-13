
#pragma comment(lib, "Dex.lib")

#include "state/DexGameEngine.h"
#include "widget/DexDesktop.h"
#include "DexBase/DexLog.h"
#include "DexBase/DexMemoryLeakCheck.h"
#include "widget/CDexUiSrcMgr.h"
#include "widget/DexWidgetFactory.h"

#include "resource.h"
#include "ParticleEditState.h"
#include "PalStateBattleMain.h"

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
	//DexCheckMemLeak::getDexMemoryLeakCheck()->SetBreakAlloc(8843);
	DexGameEngine::getEngine()->SetHInstance(hInst);
	DexGameEngine::getEngine()->SetIcon(IDI_ICON1);
	DexGameEngine::getEngine()->SetMsgPro(MsgProc);
	DexGameEngine::getEngine()->SetFullScreen(false);
	DexGameEngine::getEngine()->Initialize();
	CDexUiTexFactory::AddFilePath("res/");
	CDexUiTexFactory::AddFilePath("res/button/");
	getUiSrcMgrSingleton()->createUiTexFactory(ui_src_state_interface, "ini/ui_tex1.ini");
	getUiSrcMgrSingleton()->setCurrState(ui_src_state_interface);
	getWidgetFactory()->loadWidgetsFromXml("ini/ui.xml");
	DexGameEngine::getEngine()->SetLoadingState(new CLoadingState);


	//DEXENGINE_INITGLOBAL(DexGlobalPal2);
	ParticalEditState* pStateMain = new ParticalEditState();
	PalGameStateBattleMain* pStateBattle = new PalGameStateBattleMain;
	DexGameEngine::getEngine()->AddState(pStateMain);
	DexGameEngine::getEngine()->AddState(pStateBattle);

	//DexGameEngine::getEngine()->SetLoading(ParticalEditState::getClassType());
	DexGameEngine::getEngine()->SetLoading(PalGameStateBattleMain::getClassType());

	//DexGameEngine::getEngine()->SetCurrState(pStateBattle->GetStateId());
	DexGameEngine::getEngine()->Run();
	DexGameEngine::getEngine()->ShutDownEngine();
	DexCheckMemLeak::getDexMemoryLeakCheck()->CheckMemoryLeakEnd();
	return 0;
}