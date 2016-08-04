

#include "IDexDeviceWindows.h"

IDexDeviceWindows::IDexDeviceWindows()
{
	g_hwnd = 0;
	g_hInstance = 0;
}

IDexDeviceWindows::~IDexDeviceWindows()
{

}

HWND IDexDeviceWindows::GetHwnd()
{
	return g_hwnd;
}

HINSTANCE IDexDeviceWindows::GetHInstance()
{
	return g_hInstance;
}
