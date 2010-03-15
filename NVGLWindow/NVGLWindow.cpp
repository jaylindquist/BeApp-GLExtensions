#include "NVGLWindow.h"
#include <iostream>
using namespace std;

using namespace BitwiseEngine;

NVGLWindow::NVGLWindow(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = 0;
}

NVGLWindow::~NVGLWindow(void)
{
	destroy();
}

bool 
NVGLWindow::DoRegisterClass(char* windowClass)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));	
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = NVGLWindow::StaticWndProc;
	wc.hInstance     = this->m_hInstance;
	wc.hIcon         = LoadIcon(this->m_hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = windowClass;

	ATOM result = RegisterClassEx(&wc);
	
	return result != 0;
}

bool 
NVGLWindow::DoCreateWindow(char* windowClass)
{
	bool succ = false;

	if(m_hWnd)
	{
		return false; // Only initialize once. Must create a new instance.
	}

	// Create a simple window for the OpenGL context.
	this->m_hWnd = CreateWindow(windowClass
						, "SceniX Renderer" //szTitle
						, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW
						, 100, 100
						, 250, 250
						, NULL
						, NULL
						, this->m_hInstance
						, NULL);

	if (this->m_hWnd && attachWindow(this->m_hWnd))
	{
		if(succ = init())
		{
			setViewportSize(100, 100);
		}
	}
	else
	{
		succ = false;
	}

	return succ;
}

LRESULT
NVGLWindow::StaticWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	NVGLWindow* pkTheApp = (NVGLWindow*)App::TheApplication;

	if(!pkTheApp || !pkTheApp->m_hWnd)
	{
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	if(pkTheApp->UserWndProc(hWnd, uiMsg, wParam, lParam))
	{
		return 0;
	}

	switch(uiMsg) 
	{
    case WM_PAINT:
		{
			pkTheApp->OnDisplay();
			ValidateRect(hWnd, NULL);
			break;
		}
	case WM_ERASEBKGND:
		{
			// This tells Windows not to erase the background (and that the
			// application is doing so).
			break;
		}
	case WM_MOVE:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMove(x,y);
			break;
		}
	case WM_SIZE:
		{
			int width = int(LOWORD(lParam));
			int height = int(HIWORD(lParam));
			pkTheApp->OnResize(width, height);
			break;
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
			break;
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
			break;
		}
	case WM_LBUTTONDOWN:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDown(WindowApp::MOUSE_LEFT_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_LBUTTONUP:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseUp(WindowApp::MOUSE_LEFT_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_LBUTTONDBLCLK:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDoubleClick(WindowApp::MOUSE_LEFT_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_MBUTTONDOWN:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDown(WindowApp::MOUSE_MIDDLE_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_MBUTTONUP:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseUp(WindowApp::MOUSE_MIDDLE_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_MBUTTONDBLCLK:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDoubleClick(WindowApp::MOUSE_MIDDLE_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_RBUTTONDOWN:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDown(WindowApp::MOUSE_RIGHT_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_RBUTTONUP:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseUp(WindowApp::MOUSE_RIGHT_BUTTON, x, y, PtrToUint(wParam));
			break;
		}
	case WM_RBUTTONDBLCLK:
		{
			int x = int(LOWORD(lParam));
			int y = int(HIWORD(lParam));
			pkTheApp->OnMouseDoubleClick(WindowApp::MOUSE_RIGHT_BUTTON, x, y, PtrToUint(wParam));
			break;
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
			break;
		}
	case WM_MOUSEWHEEL:
		{
			short sWParam = (short)(HIWORD(wParam));
			int iDelta = ((int)sWParam)/WHEEL_DELTA;
			int iXPos = int(LOWORD(lParam));
			int iYPos = int(HIWORD(lParam));
			unsigned int uiModifiers = (unsigned int)(LOWORD(wParam));
			pkTheApp->OnMouseWheel(iDelta,iXPos,iYPos,uiModifiers);
			break;
		}
	case WM_DESTROY:
		{
			pkTheApp->OnTerminate();
			PostQuitMessage(0);
			break;
		}
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
	
	return 0;
}


int 
NVGLWindow::Main(int argc, char** argv)
{
	static char windowClass[] = "Bitwise Engine";

	// one-time initialization of global data required by the SceniX API
	nvsg::nvsgInitialize();

	if(!DoRegisterClass(windowClass))
	{
		return -1;
	}

	if(!DoCreateWindow(windowClass))
	{
		return -1;
	}

	// Perform application initialization:
	if (!OnInitialize()) 
	{
		return FALSE;
	}
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

 	MSG msg;
	int ret;
	while((ret = GetMessage(&msg, m_hWnd, 0, 0)) != 0) 
	{
		if(ret == 1 && !TranslateAccelerator(msg.hwnd, 0, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else 
		{
			break;
		}
	}
  
	// free global data required by the nvsg API
	nvsg::nvsgTerminate();

	return (int) msg.wParam;
}
