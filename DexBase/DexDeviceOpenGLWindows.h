
#include "DexCompiler.h"

#ifdef _DEX_USE_DEVICE_OPENGL
#ifndef _DEX_DEVICE_OPENGL_WINDOWS_H
#define _DEX_DEVICE_OPENGL_WINDOWS_H

#include <windows.h>
#pragma comment(lib, "GlU32.Lib")
#pragma comment(lib, "OpenGL32.Lib")
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\GLAux.h>
#include "IDexDeviceWindows.h"


class DexDeviceOpenGLWindows : public IDexDeviceWindows
{
	Dex_DeclareClass(DexDeviceOpenGLWindows, 0)
public:
	DexDeviceOpenGLWindows();
	virtual ~DexDeviceOpenGLWindows();

public:
	virtual bool InitDevice(bool bFullScreen, int16 iWindowWidth, int16 iWindowHeight, void* param1, void* param2);
	virtual bool SetTransform(DEXTRANSFORMSTATETYPE transformStateType, const DexMatrix4x4* matrix);
	virtual bool SetRenderState(eDEXRENDERSTATE eRenderState, int64 value);
	virtual bool SetFVF(int64 iFVF);
	virtual bool DrawPrimitiveUp(DexPrimitivetType eType, uint32 iPrimitiveCount, const void* pVertexStreamData, uint32 iVertexStreamStride);
	virtual bool Clear(int64 flags, const DexColor& color, float32 fZ, int64 sStencil);
	virtual bool BeginScene() ;
	virtual bool EndScene();
protected:
	virtual void LookAtLH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up);
	virtual void LookAtRH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up);
protected:
	HGLRC  g_hrc;
};
#endif//_DEX_DEVICE_OPENGL_H
#endif//_DEX_USE_DEVICE_OPENGL