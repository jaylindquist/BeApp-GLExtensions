#include "GLWindowApp.h"
#include <GL/gl.h>
#include <GL/wglext.h>

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif

using namespace BitwiseEngine;

GLWindowApp::GLWindowApp(const char* windowTitle, 
						 int x, int y, int width, int height):
WindowApp(),
m_acWindowTitle(windowTitle),
m_xPosition(x),
m_yPosition(y),
m_width(width),
m_height(height),
m_ID(0),
m_majorVersion(3),
m_minorVersion(2)
{
	ShowConsole(false);
}

GLWindowApp::GLWindowApp(const char* windowTitle, 
						 int x, int y, int width, int height,
						 int majorVersion, int minorVersion):
WindowApp(),
m_acWindowTitle(windowTitle),
m_xPosition(x),
m_yPosition(y),
m_width(width),
m_height(height),
m_ID(0),
m_majorVersion(majorVersion),
m_minorVersion(minorVersion)
{
	ShowConsole(false);
}


GLWindowApp::~GLWindowApp(void)
{
	TerminateApplication();
}


void 
GLWindowApp::OnMove(int x, int y)
{
	m_xPosition = x;
	m_yPosition = y;
}

void 
GLWindowApp::OnResize(int width, int height)
{
	if(width > 0 && height > 0)
	{
		m_width = width;
		m_height = height;
	}
}

void 
GLWindowApp::SetMousePosition(int x, int y)
{
	HWND hWnd = (HWND)IntToPtr(m_ID);
	POINT kPoint;
	kPoint.x = (LONG)x;
	kPoint.y = (LONG)y;
	ClientToScreen(hWnd,&kPoint);
	SetCursorPos(kPoint.x,kPoint.y);
}

void 
GLWindowApp::GetMousePosition(int& x, int& y) const
{
	HWND hWnd = (HWND)IntToPtr(m_ID);
	POINT kPoint;
	GetCursorPos(&kPoint);
	ScreenToClient(hWnd,&kPoint);
	x = (int)kPoint.x;
	y = (int)kPoint.y;
}

int 
GLWindowApp::GetStringWidth(const char* text) const
{
	if (!text || strlen(text) == 0)
	{
		return 0;
	}

	HWND hWnd = (HWND)IntToPtr(m_ID);
	HDC hDC = GetDC(hWnd);
	SIZE kSize;
	GetTextExtentPoint32(hDC, text, (int)strlen(text), &kSize);
	ReleaseDC(hWnd, hDC);

	return (int)kSize.cx;
}

int 
GLWindowApp::GetCharacterWidth(const char c) const
{
	HWND hWnd = (HWND)IntToPtr(m_ID);
	HDC hDC = GetDC(hWnd);
	SIZE kSize;
	GetTextExtentPoint32(hDC, &c, 1, &kSize);
	ReleaseDC(hWnd, hDC);

	return (int)kSize.cx;
}

int 
GLWindowApp::GetFontHeight() const
{
	HWND hWnd = (HWND)IntToPtr(m_ID);
	HDC hDC = GetDC(hWnd);
	TEXTMETRIC kMetric;
	GetTextMetrics(hDC, &kMetric);
	ReleaseDC(hWnd, hDC);

	return (int)kMetric.tmHeight;
}

bool
GLWindowApp::DoRegisterClass(char* windowClass)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));	
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = GLWindowApp::StaticWndProc;
	wc.hInstance     = 0;
	wc.hIcon         = LoadIcon(0,IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
	wc.lpszClassName = windowClass;


	// allow user modification to window class
	if(!PreRegisterClass(wc))
	{
		return false;
	}

	ATOM result = RegisterClassEx(&wc);
	
	return result != 0;
}

bool
GLWindowApp::DoCreateWindow(char* windowClass)
{
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;

	// Require the window to have the specified client area.
	RECT kRect = { 0, 0, GetWidth() - 1, GetHeight() - 1 };

	if(AdjustWindowRectEx(&kRect, windowStyle, FALSE, windowExtendedStyle) == FALSE)
	{
		return false;
	}

	// Create The OpenGL Window
	HWND hWnd = CreateWindowEx(	
		windowExtendedStyle,				// Extended Style
		windowClass,						// Class Name
		GetWindowTitle(),					// Window Title
		windowStyle,						// Window Style
		GetXPosition(),						// Window X,Y Position
		GetYPosition(),
		GetWidth(),							// Window Width
		GetHeight(),						// Window Height
		HWND_DESKTOP,						// Desktop Is Window's Parent
		0,									// No Menu
		0,									// Pass The Window Instance
		(LPVOID)this);						// Creation Parameters

	if(hWnd == 0)
	{
		return false;
	}

	SetWindowID(PtrToInt(hWnd));

	return true;
}

bool
GLWindowApp::DoCreatePixelFormat()
{
	HWND hWnd = (HWND)IntToPtr(GetWindowID());
	HDC hDC = GetDC(hWnd);
	if(hDC == 0)
	{
		DestroyWindow(hWnd);
		SetWindowID(0);
		return false;
	}

	m_hDC = hDC;

	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
					PFD_SUPPORT_OPENGL |
					PFD_SWAP_LAYER_BUFFERS |
					PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 0;
	pfd.cStencilBits = 0;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;

	if(!PreChoosePixelFormat(pfd))
	{
		return false;
	}

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if(pixelFormat == 0)
	{
		// Failed
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		SetWindowID(0);
		return false;
	}

	if(SetPixelFormat(hDC, pixelFormat, &pfd) == FALSE)
	{
		// Failed
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		SetWindowID(0);
		return false;
	}

	// need to create a temporary context to access wglCreateContextAttribsARB
	HGLRC tempContext = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC,tempContext);
	int attribs[] = {
		 WGL_CONTEXT_MAJOR_VERSION_ARB, m_majorVersion,
		 WGL_CONTEXT_MINOR_VERSION_ARB, m_minorVersion,
		 //and it shall be forward compatible so that we can only use up to date functionality
		 WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	0}; //zero indicates the end of the array

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL; //pointer to the method
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

	HGLRC hRC = NULL;
	if(wglCreateContextAttribsARB == NULL) //OpenGL 3.0 is not supported
	{
		// use temp context instead
		hRC = tempContext;
	}
	else 
	{
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglDeleteContext(tempContext);
	}

	if(hRC == 0)
	{
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		SetWindowID(0);
		return false;
	}

	// Make The Rendering Context Our Current Rendering Context
	if(wglMakeCurrent(hDC, hRC) == FALSE)
	{
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		SetWindowID(0);
		return false;
	}

	// set the actual major / minor version
	const GLubyte* version = glGetString(GL_VERSION); //major.minor
	m_majorVersion = version[0] - '0';
	m_minorVersion = version[2] - '0';

	return true;
}

void
GLWindowApp::TerminateApplication()
{
	PostQuitMessage(0);
}

LRESULT CALLBACK 
GLWindowApp::StaticWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	GLWindowApp* pkTheApp = (GLWindowApp*)App::TheApplication;

	if(!pkTheApp || !pkTheApp->GetWindowID())
	{
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	if(pkTheApp->UserWndProc(hWnd, uiMsg, wParam, lParam))
	{
		return 0;
	}

	switch(uiMsg) 
	{
	case WM_ERASEBKGND:
		{
			// This tells Windows not to erase the background (and that the
			// application is doing so).
			return 1;
		}
	case WM_MOVE:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMove(x,y);
			return 0;
		}
	case WM_SIZE:
		{
			int width = int(LOWORD(lParam));
			int height = int(HIWORD(lParam));
			pkTheApp->OnResize(width, height);
			return 0;
		}
	case WM_KEYDOWN:
		{
			int iVirtKey = int(wParam);

			// get cursor position client coordinates
			POINT kPoint;
			GetCursorPos(&kPoint);
			ScreenToClient(hWnd, &kPoint);
			int x = (int)kPoint.x;
			int y = (int)kPoint.y;

			pkTheApp->OnKeyDown(iVirtKey, x, y);

			return 0;
		}
	case WM_KEYUP:
		{
			int iVirtKey = int(wParam);

			// get the cursor position in client coordinates
			POINT kPoint;
			GetCursorPos(&kPoint);
			ScreenToClient(hWnd,&kPoint);
			int x = (int)kPoint.x;
			int y = (int)kPoint.y;

			pkTheApp->OnKeyUp(iVirtKey, x, y);
			
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDown(WindowApp::MOUSE_LEFT_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_LBUTTONUP:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseUp(WindowApp::MOUSE_LEFT_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_LBUTTONDBLCLK:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDoubleClick(WindowApp::MOUSE_LEFT_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_MBUTTONDOWN:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDown(WindowApp::MOUSE_MIDDLE_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_MBUTTONUP:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseUp(WindowApp::MOUSE_MIDDLE_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_MBUTTONDBLCLK:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDoubleClick(WindowApp::MOUSE_MIDDLE_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_RBUTTONDOWN:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDown(WindowApp::MOUSE_RIGHT_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_RBUTTONUP:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseUp(WindowApp::MOUSE_RIGHT_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_RBUTTONDBLCLK:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDoubleClick(WindowApp::MOUSE_RIGHT_BUTTON, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_MOUSEMOVE:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));

			int iButton = WindowApp::MOUSE_NO_BUTTON;
			if(wParam & MK_LBUTTON)
			{
				iButton = WindowApp::MOUSE_LEFT_BUTTON;
			}
			else if(wParam & MK_MBUTTON)
			{
				iButton = WindowApp::MOUSE_MIDDLE_BUTTON;
			}
			else if(wParam & MK_RBUTTON)
			{
				iButton = WindowApp::MOUSE_RIGHT_BUTTON;
			}

			pkTheApp->OnMotion(iButton, x, y, PtrToUint(wParam));

			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			short sWParam = (short)(HIWORD(wParam));
			int iDelta = ((int)sWParam)/WHEEL_DELTA;
			int iXPos = int(LOWORD(lParam));
			int iYPos = int(HIWORD(lParam));
			unsigned int uiModifiers = (unsigned int)(LOWORD(wParam));
			pkTheApp->OnMouseWheel(iDelta,iXPos,iYPos,uiModifiers);
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd,uiMsg,wParam,lParam);
}

int 
GLWindowApp::Main(int argc, char** argv)
{
	static char windowClass[] = "Bitwise Engine";

	// Allow work to be done before the window is created.
	if(!OnPrecreate())
	{
		return -1;
	}

	// Register the window class.
	if(!DoRegisterClass(windowClass))
	{
		return -1;
	}

	// create the window
	if(!DoCreateWindow(windowClass))
	{
		return -1;
	}

	// create the pixel format
	if(!DoCreatePixelFormat())
	{
		return -1;
	}

	if(OnInitialize())
	{
		HWND hWnd = (HWND)IntToPtr(GetWindowID());

		// display the window
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		// start the message pump
		bool bApplicationRunning = true;
		while(bApplicationRunning)
		{
			MSG kMsg;
			if(PeekMessage(&kMsg, 0, 0, 0, PM_REMOVE))
			{
				if(kMsg.message == WM_QUIT)
				{
					bApplicationRunning = false;
					continue;
				}

				HACCEL hAccel = 0;
				if(!TranslateAccelerator(hWnd, hAccel, &kMsg))
				{
					TranslateMessage(&kMsg);
					DispatchMessage(&kMsg);
				}
			}
			else
			{
				OnIdle();
				OnDisplay();
				SwapBuffers(m_hDC);
			}
		}
	}

	UnregisterClass(windowClass, 0);

	OnTerminate();
	
	return 0;
}

