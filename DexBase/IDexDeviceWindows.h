
#include "DexCompiler.h"

#ifdef _DEX_PLATFORM_WINDOWS
#ifndef _DEX_DEVICE_WINDOWS_H
#define _DEX_DEVICE_WINDOWS_H
#include <windows.h>
#include "IDexDevice.h"


class IDexDeviceWindows : public IDexDevice
{
	Dex_DeclareClass(IDexDeviceWindows, 0)
public:
	IDexDeviceWindows();
	virtual ~IDexDeviceWindows();
public:
	virtual HWND GetHwnd();
	virtual HINSTANCE GetHInstance();
protected:
	HWND			  g_hwnd;
	HDC				  g_hdc;
	HINSTANCE		  g_hInstance;
};
#endif
#endif