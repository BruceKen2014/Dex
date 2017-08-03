

#include "DexDeviceDirectx9.h"
#ifdef _DEX_USE_DEVICE_DIRECTX9
#include <d3d9.h>
#include <d3d9types.h>
#include "DexType.h"
#include "DexLog.h"
#include "DexColor.h"
DexDeviceDirectx9::DexDeviceDirectx9() :IDexDeviceWindows()
{
	m_eViewMatrixType = VIEWMATRIXTYPE_LEFTHAND;
}

DexDeviceDirectx9::~DexDeviceDirectx9()
{
	g_D3DDevice->Release();
	g_D3D->Release();
}

bool DexDeviceDirectx9::InitDevice(bool bFullScreen, DInt16 iWindowWidth, DInt16 iWindowHeight, void* param1, void* param2)
{
	g_hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, (WNDPROC)param1, 0L, 0L,
		g_hInstance, NULL, NULL, NULL, NULL,
		"Pal", NULL };

	wc.hIcon = LoadIcon(NULL, IDC_ARROW);
	RegisterClassEx(&wc);

	// Create the application's window
	DWORD style;
	if (bFullScreen)
		style = WS_POPUP; //popup会创造一个无边框的窗口 在全屏要这样做 否则鼠标坐标会有变差
	else
		style = WS_OVERLAPPEDWINDOW;
	g_hwnd = CreateWindow("Pal", "windowname", style,
		100, 100, iWindowWidth, iWindowHeight, NULL,
		NULL, wc.hInstance, NULL);
	DragAcceptFiles(g_hwnd, true);//accept drag files
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == DexNull)
		return false;
	D3DDISPLAYMODE displayMode;
	// Get the desktop display mode.
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;

	if (g_D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, displayMode.Format, !bFullScreen,
		D3DMULTISAMPLE_8_SAMPLES,
		NULL) == D3D_OK)
		multiType = D3DMULTISAMPLE_8_SAMPLES;

	if (bFullScreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = iWindowWidth;
		d3dpp.BackBufferHeight = iWindowHeight;
		//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
		d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.MultiSampleType = multiType;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the D3DDevice
	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &g_D3DDevice)))
	{
		DexLog::getSingleton()->LogLine(log_ok, "D3D9 CreateDevice失败！");
		return false;
	}
	DexLog::getSingleton()->LogLine(log_ok, "D3D9 CreateDevice成功！");
	ShowWindow(g_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hwnd);
	return true;
}

IDirect3DDevice9 * DexDeviceDirectx9::GetD3D9Device()
{
	return g_D3DDevice;
}

bool DexDeviceDirectx9::SetTransform(DEXTRANSFORMSTATETYPE transformStateType, const DexMatrix4x4* matrix)
{
	D3DTRANSFORMSTATETYPE d3dTransformState;
	switch (transformStateType)
	{
	case DEXTRANSFORMSTATETYPE_VIEW:
		d3dTransformState = D3DTS_VIEW;
		break;
	case DEXTRANSFORMSTATETYPE_PROJECTION:
		d3dTransformState = D3DTS_PROJECTION;
		break;
	case DEXTRANSFORMSTATETYPE_WORLD:
		d3dTransformState = D3DTS_WORLD;
		break;
	case DEXTRANSFORMSTATETYPE_WORLD1:
		d3dTransformState = D3DTS_WORLD1;
		break;
	case DEXTRANSFORMSTATETYPE_WORLD2:
		d3dTransformState = D3DTS_WORLD2;
		break;
	case DEXTRANSFORMSTATETYPE_WORLD3:
		d3dTransformState = D3DTS_WORLD3;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE0:
		d3dTransformState = D3DTS_TEXTURE0;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE1:
		d3dTransformState = D3DTS_TEXTURE1;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE2:
		d3dTransformState = D3DTS_TEXTURE2;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE3:
		d3dTransformState = D3DTS_TEXTURE3;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE4:
		d3dTransformState = D3DTS_TEXTURE4;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE5:
		d3dTransformState = D3DTS_TEXTURE5;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE6:
		d3dTransformState = D3DTS_TEXTURE6;
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE7:
		d3dTransformState = D3DTS_TEXTURE7;
		break;
	default:
		return false;
	}
	D3DXMATRIX d3dMatrix;
	memcpy(&d3dMatrix, matrix, sizeof(D3DXMATRIX));
	HRESULT ret = g_D3DDevice->SetTransform(d3dTransformState, &d3dMatrix);
	return ret != 0;
}

bool DexDeviceDirectx9::SetRenderState(eDEXRENDERSTATE eRenderState, DInt64 value)
{
	D3DRENDERSTATETYPE d3dRenderStateType;
	switch (eRenderState)
	{
	case eDEXRENDERSTATE::DEXRENDERSTATE_LIGHTING:
		d3dRenderStateType = D3DRS_LIGHTING;
		break;
	case eDEXRENDERSTATE::DEXRENDERSTATE_ALPHATESTENABLE:
		d3dRenderStateType = D3DRS_ALPHABLENDENABLE;
		break;
	case eDEXRENDERSTATE::DEXRENDERSTATE_SRCBLEND:
		d3dRenderStateType = D3DRS_SRCBLEND;
		break;
	case eDEXRENDERSTATE::DEXRENDERSTATE_DESTBLEND:
		d3dRenderStateType = D3DRS_DESTBLEND;
		break;
	default:
		return false;
	}
	HRESULT ret = g_D3DDevice->SetRenderState(d3dRenderStateType, value);
	return ret != 0;
}

bool DexDeviceDirectx9::SetFVF(DInt64 iFVF)
{
	DWORD dxFvF = 0;
	if (iFVF & DEXFVF_POSITION)
		dxFvF |= D3DFVF_XYZ;
	if (iFVF & DEXFVF_NORMAL)
		dxFvF |= D3DFVF_NORMAL;
	if (iFVF & DEXFVF_TEXTURE0)
		dxFvF |= D3DFVF_TEX0;
	g_D3DDevice->SetFVF(dxFvF);
	return true;
}

bool DexDeviceDirectx9::DrawPrimitiveUp(DexPrimitivetType eType, DUDInt32 iPrimitiveCount, const void* pVertexStreamData, DUDInt32 iVertexStreamStride)
{
	D3DPRIMITIVETYPE d3d_primitive_type = D3DPT_LINELIST;
	switch (eType)
	{
	case DexPT_POINTLIST:
	{
		d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_POINTLIST; 
		break; 
	}
	case DexPT_LINELIST:
	{
		d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINELIST;
		break; 
	}
	case DexPT_LINESTRIP:
	{
		d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINESTRIP; 
		break; 
	}
	case DexPT_TRIANGLELIST:
	{
		d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST;
		break; 
	}
	case DexPT_TRIANGLESTRIP:
	{
		d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP; 
		break; 
	}
	case DexPT_TRIANGLEFAN:
	{
		d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLEFAN;
		break; 
	}
	default:
		return false;
	}
	HRESULT ret = g_D3DDevice->DrawPrimitiveUP(d3d_primitive_type, iPrimitiveCount, pVertexStreamData, iVertexStreamStride);
	return ret != 0;
}

bool DexDeviceDirectx9::Clear(DInt64 flags, const DexColor& color, DFloat32 fZ, DInt64 sStencil)
{
	DWORD dxFlag = 0;
	if (flags & DEXCLEAR_STENCIL)
		dxFlag |= D3DCLEAR_STENCIL;
	if (flags & DEXCLEAR_TARGET)
		dxFlag |= D3DCLEAR_STENCIL;
	if (flags & DEXCLEAR_ZBUFFER)
		dxFlag |= D3DCLEAR_STENCIL;
	g_D3DDevice->Clear(0, DexNull, dxFlag, getD3DColor(color), fZ, sStencil);
	return true;
}
bool DexDeviceDirectx9::BeginScene()
{
	return g_D3DDevice->BeginScene();
}

bool DexDeviceDirectx9::EndScene()
{
	g_D3DDevice->EndScene();
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
	return true;
}
void DexDeviceDirectx9::LookAtLH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up)
{
	DexVector3 zAxis = (lookat - eye).Normalize();
	DexVector3 xAxis = (up.Cross(zAxis)).Normalize();
	DexVector3 yAxis = zAxis.Cross(xAxis);
	DexMatrix4x4 matrix;
	matrix.m_m[0][0] = xAxis.x; matrix.m_m[0][1] = yAxis.x; matrix.m_m[0][2] = zAxis.x; matrix.m_m[0][3] = 0;
	matrix.m_m[1][0] = xAxis.y; matrix.m_m[1][1] = yAxis.y; matrix.m_m[1][2] = zAxis.y; matrix.m_m[1][3] = 0;
	matrix.m_m[2][0] = xAxis.z; matrix.m_m[2][1] = yAxis.z; matrix.m_m[2][2] = zAxis.z; matrix.m_m[2][3] = 0;
	matrix.m_m[3][0] = -xAxis.Dot(eye); matrix.m_m[3][1] = -yAxis.Dot(eye); matrix.m_m[3][2] = -zAxis.Dot(eye); matrix.m_m[3][3] = 1.0f;
	SetTransform(DEXTRANSFORMSTATETYPE_VIEW, &matrix);
}

void DexDeviceDirectx9::LookAtRH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up)
{

}
#endif