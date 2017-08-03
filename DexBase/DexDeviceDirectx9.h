
#include "DexCompiler.h"

#ifdef _DEX_USE_DEVICE_DIRECTX9
#ifndef _DEX_DEVICE_DIRECTX9_H
#define _DEX_DEVICE_DIRECTX9_H

#include "IDexDeviceWindows.h"

struct IDirect3D9;
struct IDirect3DDevice9;
class DexDeviceDirectx9: public IDexDeviceWindows
{
	Dex_DeclareClass(DexDeviceDirectx9, IDexDeviceWindows, 0)
public:
	DexDeviceDirectx9();
	virtual ~DexDeviceDirectx9();

public:
	virtual bool InitDevice(bool bFullScreen, DInt16 iWindowWidth, DInt16 iWindowHeight, void* param1, void* param2);
	virtual bool SetTransform(DEXTRANSFORMSTATETYPE transformStateType, const DexMatrix4x4* matrix);
	virtual bool SetRenderState(eDEXRENDERSTATE eRenderState, DInt64 value);
	virtual bool SetFVF(DInt64 iFVF);
	virtual bool DrawPrimitiveUp(DexPrimitivetType eType, DUDInt32 iPrimitiveCount, const void* pVertexStreamData, DUDInt32 iVertexStreamStride);
	virtual bool Clear(DInt64 flags, const DexColor& color, DFloat32 fZ, DInt64 sStencil);
	virtual bool BeginScene();
	virtual bool EndScene();
public:
	IDirect3DDevice9 * GetD3D9Device();
protected:
	virtual void LookAtLH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up);
	virtual void LookAtRH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up);
protected:
	IDirect3D9*       g_D3D;				//D3D
	IDirect3DDevice9* g_D3DDevice;         //…Ë±∏

};
#endif
#endif