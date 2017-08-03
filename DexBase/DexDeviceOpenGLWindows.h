
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
	Dex_DeclareClass(DexDeviceOpenGLWindows, IDexDeviceWindows, 0)
public:
	DexDeviceOpenGLWindows();
	virtual ~DexDeviceOpenGLWindows();

public:
	virtual bool InitDevice(bool bFullScreen, DInt16 iWindowWidth, DInt16 iWindowHeight, void* param1, void* param2);
	virtual bool SetTransform(DEXTRANSFORMSTATETYPE transformStateType, const DexMatrix4x4* matrix);
	virtual bool SetRenderState(eDEXRENDERSTATE eRenderState, DInt64 value);
	virtual bool SetFVF(DInt64 iFVF);
	virtual bool DrawPrimitiveUp(DexPrimitivetType eType, DUDInt32 iPrimitiveCount, const void* pVertexStreamData, DUDInt32 iVertexStreamStride);
	virtual bool Clear(DInt64 flags, const DexColor& color, DFloat32 fZ, DInt64 sStencil);
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