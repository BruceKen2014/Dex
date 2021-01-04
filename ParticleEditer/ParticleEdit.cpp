
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
#include "PalOpenglTestScene.h"
#include "../DexBase/DexStreamFile.h"
#include "PalCommand.h"

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
	case WM_KEYDOWN:
		{
			getDesktop()->OnKeyDown(char(wParam));
			break;
		}
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		DUInt32 nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		char strFileName[128];
		DVector<DString> FileNames;
		for (DUInt32 i = 0; i < nFileNum; ++i)
		{
			DragQueryFile(hDrop, i, strFileName, 128);
			FileNames.push_back(strFileName);
		}
		DragFinish(hDrop);
		DexGameEngine::getEngine()->OnDragFiles(FileNames);
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
	InitPalCommand();
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(memoryStatus);
	GlobalMemoryStatusEx(&memoryStatus);


	int testDevice = 0; 
	DexGameEngine::getEngine()->Initialize(testDevice);
	DexStreamFile::sGetStreamFile()->AddBigFile("data.pak");
	if (testDevice == 0)
	{//dx9
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
	}
	else
	{//opengl
		PalOpenGLTestScene* pOpenGLScene = new PalOpenGLTestScene;
		DexGameEngine::getEngine()->AddState(pOpenGLScene);
		DexGameEngine::getEngine()->SetCurrState(PalOpenGLTestScene::getClassType());
	}

	DexGameEngine::getEngine()->Run();
	DexGameEngine::getEngine()->ShutDownEngine();
	DexCheckMemLeak::getDexMemoryLeakCheck()->CheckMemoryLeakEnd();

	return 0;
}