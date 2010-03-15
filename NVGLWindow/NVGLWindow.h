#pragma once

#include <nvui/GLWinRenderArea.h>
#include <WindowApp.h>

namespace BitwiseEngine 
{


class NVGLWindow : public WindowApp, public nvui::GLWinRenderArea
{
public:
	NVGLWindow(HINSTANCE hInstance);
	~NVGLWindow(void);

	int Main(int argc, char** argv);
	virtual bool UserWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) { return false; }

	virtual void SetWindowID(int windowID) {}
	virtual int GetWindowID() const { return 0; }

	// event callbacks
	virtual bool OnPrecreate() { return true; }
	virtual bool OnInitialize() { return true; }
	virtual void OnTerminate() { }

	virtual void OnMove(int x, int y) { }
	virtual void OnResize(int width, int height) { }

	virtual void OnDisplay() { }
	virtual void OnIdle() { }

	virtual bool OnKeyDown(int key, int x, int y) { return true; }
	virtual bool OnKeyUp(int key, int x, int y) { return true; }

	virtual bool OnMouseUp(int button, int x, int y, unsigned int modifiers) { return true; }
	virtual bool OnMouseDown(int button, int x, int y, unsigned int modifiers) { return true; }
	virtual bool OnMouseDoubleClick(int button, int x, int y, unsigned int modifiers) { return true; }

	virtual bool OnMotion(int button, int x, int y, unsigned int modifiers) { return true; }
	virtual bool OnMouseWheel(int scrollDelta, int x, int y, unsigned int modifiers) { return true; }

	// mouse position
	virtual void SetMousePosition(int x, int y) { }
	virtual void GetMousePosition(int& x, int& y) const { }

	virtual void TerminateApplication() { }
protected:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	virtual bool DoRegisterClass(char* windowClass);
	virtual bool DoCreateWindow(char* windowClass);
	
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


}