#pragma once

#include "BeApp.h"
#include "WindowApp.h"

#include <windows.h>
namespace BitwiseEngine
{

class BE_APPLICATION_ITEM GLWindowApp : public WindowApp
{
public:	
	GLWindowApp(const char* windowTitle, int x, int y, int width, int height);
	GLWindowApp(const char* windowTitle, int x, int y, int width, int height, int majorVersion, int minorVersion);
	virtual ~GLWindowApp();

	int Main(int argc, char** argv);

	// member access
	const char* GetWindowTitle() const;
	int GetXPosition() const;
	int GetYPosition() const;
	int GetWidth() const;
	int GetHeight() const;

	void SetWindowID(int windowID);
	int GetWindowID() const;

	virtual bool UserWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

	// window initialization callbacks
	virtual bool PreRegisterClass(WNDCLASSEX& wndClass);
	virtual bool PreChoosePixelFormat(PIXELFORMATDESCRIPTOR& pfd);

	// event callbacks
	virtual bool OnPrecreate();
	virtual bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnMove(int x, int y);
	virtual void OnResize(int width, int height);
	virtual void OnDisplay();
	virtual void OnIdle();
	virtual bool OnKeyDown(int key, int x, int y);
	virtual bool OnKeyUp(int key, int x, int y);

	virtual bool OnMouseDown(int button, int x, int y, unsigned int modifiers);
	virtual bool OnMouseUp(int button, int x, int y, unsigned int modifiers);
	virtual bool OnMouseDoubleClick(int button, int x, int y, unsigned int modifiers);

	virtual bool OnMotion(int button, int x, int y, unsigned int modifiers);
	virtual bool OnMouseWheel(int scrollDelta, int x, int y, unsigned int modifiers);

	// mouse position
	void SetMousePosition(int x, int y);
	void GetMousePosition(int& x, int& y) const;

	void TerminateApplication();

	// show console window
	void ShowConsole(bool show);

	// Font information.  These are platform-specific, so classes that
	// implement the WindowApplication interfaces must implement these
	// functions.  They are not defined by WindowApplication.
	int GetStringWidth(const char* text) const;
	int GetCharacterWidth(const char c) const;
	int GetFontHeight() const;

protected:
	// constructor inputs
	const char* m_acWindowTitle;
	int m_xPosition, m_yPosition, m_width, m_height;
	int m_majorVersion, m_minorVersion;

	// An identifier for the window (representation is platform-specific).
	int m_ID;

	// DC handle
	HDC m_hDC;

	// static WndProc
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool DoRegisterClass(char* windowClass);
	virtual bool DoCreateWindow(char* windowClass);
	virtual bool DoCreatePixelFormat();
};

#include "GLWindowApp.inl"

}