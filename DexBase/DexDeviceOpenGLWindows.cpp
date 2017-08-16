

#include "DexDeviceOpenGLWindows.h"

#ifdef _DEX_USE_DEVICE_OPENGL

DexDeviceOpenGLWindows::DexDeviceOpenGLWindows()
{
	m_eViewMatrixType = VIEWMATRIXTYPE_RIGHTHAND;
}

DexDeviceOpenGLWindows::~DexDeviceOpenGLWindows()
{
	if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
	{
		MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
	}

	if (!wglDeleteContext(g_hrc))						// Are We Able To Delete The RC?
	{
		MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
	}
	g_hrc = NULL;

	if (g_hdc && !ReleaseDC(g_hwnd, g_hdc))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		g_hdc = NULL;										// Set DC To NULL
	}

	if (g_hwnd && !DestroyWindow(g_hwnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		g_hwnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", g_hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		g_hInstance = NULL;									// Set hInstance To NULL
	}
}

bool DexDeviceOpenGLWindows::InitDevice(bool bFullScreen, DInt16 iWindowWidth, DInt16 iWindowHeight, void* param1, void* param2)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)100;			// Set Left Value To 0
	WindowRect.right = (long)iWindowWidth;		// Set Right Value To Requested Width
	WindowRect.top = (long)100;				// Set Top Value To 0
	WindowRect.bottom = (long)iWindowHeight;		// Set Bottom Value To Requested Height

	//fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	g_hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)param1;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = g_hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	if (!(g_hwnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		"OpenGLText",								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		g_hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		return false;
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	g_hdc = GetDC(g_hwnd);
	if (g_hdc == 0)
		return false;
	if (!(PixelFormat = ChoosePixelFormat(g_hdc, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		return false;
	}
	if (!SetPixelFormat(g_hdc, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		return false;// Return FALSE
	}

	g_hrc = wglCreateContext(g_hdc);
	if (g_hrc == 0)
		return false;
	if (!wglMakeCurrent(g_hdc, g_hrc))
	{
		return false;
	}
	ShowWindow(g_hwnd, SW_SHOWDEFAULT);
	SetForegroundWindow(g_hwnd);						// Slightly Higher Priority
	SetFocus(g_hwnd);									// Sets Keyboard Focus To The Window

	glViewport(0, 0, iWindowWidth, iWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)iWindowWidth / (GLfloat)iWindowHeight, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
	glPushMatrix();//先push一个临时矩阵

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
				
	
	return true;
}

bool DexDeviceOpenGLWindows::SetTransform(DEXTRANSFORMSTATETYPE transformStateType, const DexMatrix4x4* matrix)
{
	switch (transformStateType)
	{
	case DEXTRANSFORMSTATETYPE_VIEW:
	{
	}
		break;
	case DEXTRANSFORMSTATETYPE_PROJECTION:
		break;
	case DEXTRANSFORMSTATETYPE_WORLD:
	{//如果采用引擎的这个接口，那么对于几何体的矩阵操作只能通过DexMatrix来操作，而不要通过glPushMatrix glPopMatrix glMulMatrix
		//因为这些函数会改变堆栈里面的矩阵数据
		//要求渲染几何体之前必须设置transform word
		glPopMatrix();//先弹出旧矩阵
		glPushMatrix();
		glMultMatrixf(matrix->m);
	}
		break;
	case DEXTRANSFORMSTATETYPE_WORLD1:
		break;
	case DEXTRANSFORMSTATETYPE_WORLD2:
		break;
	case DEXTRANSFORMSTATETYPE_WORLD3:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE0:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE1:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE2:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE3:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE4:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE5:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE6:
		break;
	case DEXTRANSFORMSTATETYPE_TEXTURE7:
		break;
	default:
		return false;
	}
	return true;
}

bool DexDeviceOpenGLWindows::SetRenderState(eDEXRENDERSTATE eRenderState, DInt64 value)
{
	return true;
}

bool DexDeviceOpenGLWindows::SetFVF(DInt64 iFVF)
{
	return true;
}
bool DexDeviceOpenGLWindows::DrawPrimitiveUp(DexPrimitivetType eType, DUInt32 iPrimitiveCount, const void* pVertexStreamData, DUInt32 iVertexStreamStride)
{
	if (eType == DexPT_LINELIST)
	{

	}
	return true;
}

bool DexDeviceOpenGLWindows::Clear(DInt64 flags, const DexColor& color, DFloat32 fZ, DInt64 sStencil)
{
	GLbitfield glFlag = 0;
	if (flags & DEXCLEAR_STENCIL)
		glFlag |= GL_STENCIL_BUFFER_BIT;
	if (flags & DEXCLEAR_TARGET)
		glFlag |= GL_COLOR_BUFFER_BIT;
	if (flags & DEXCLEAR_ZBUFFER)
		glFlag |= GL_DEPTH_BUFFER_BIT;
	glClear(glFlag);
	glClearDepth(fZ);									// Depth Buffer Setup
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	return true;
}

bool DexDeviceOpenGLWindows::BeginScene()
{
	return true;
}

bool DexDeviceOpenGLWindows::EndScene()
{
	SwapBuffers(g_hdc);
	return true;
}

void DexDeviceOpenGLWindows::LookAtLH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up)
{

}

void DexDeviceOpenGLWindows::LookAtRH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();//先pop掉上次渲染循环，最后一个几何体的矩阵，矩阵堆栈只有一个视图矩阵了
	DexVector3 zAxis = lookat - eye;
	zAxis.Normalize();
	DexVector3 xAxis = up.Cross(zAxis);
	xAxis.Normalize();
	DexVector3 yAxis = zAxis.Cross(xAxis);
	yAxis.Normalize();
	DexMatrix4x4 matrix;
	memset(matrix.m, 0, sizeof(DexMatrix4x4));
	matrix.m[0] = -xAxis.x;
	matrix.m[4] = -xAxis.y;
	matrix.m[8] = -xAxis.z;
	matrix.m[1] = yAxis.x;
	matrix.m[5] = yAxis.y;
	matrix.m[9] = yAxis.z;
	matrix.m[2] = -zAxis.x;
	matrix.m[6] = -zAxis.y;
	matrix.m[10] = -zAxis.z;
	matrix.m[15] = 1;
	matrix.m[3] = -xAxis.Dot(eye);
	matrix.m[7] = yAxis.Dot(eye);
	matrix.m[11] = zAxis.Dot(eye);

	matrix.MakeTranspose();
	glLoadMatrixf(matrix.m);
	glPushMatrix();//先push一个临时矩阵，在渲染几何体时会先将这个矩阵pop掉
}

#endif //_DEX_USE_DEVICE_OPENGL