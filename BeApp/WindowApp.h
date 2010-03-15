#pragma once

#include "App.h"
#include "BeApp.h"

#include <windows.h>

namespace BitwiseEngine
{

/***********************************************
*
* Window Application Interface
* All methods are virtual and none
* are defined
*
************************************************/

class BE_APPLICATION_ITEM WindowApp : public App
{
public:
	WindowApp(){}
	virtual ~WindowApp(){}

	virtual void SetWindowID(int windowID) = 0;
	virtual int GetWindowID() const = 0;

	// event callbacks
	virtual bool OnPrecreate() = 0;
	virtual bool OnInitialize() = 0;
	virtual void OnTerminate() = 0;

	virtual void OnMove(int x, int y) = 0;
	virtual void OnResize(int width, int height) = 0;

	virtual void OnDisplay() = 0;
	virtual void OnIdle() = 0;

	virtual bool OnKeyDown(int key, int x, int y) = 0;
	virtual bool OnKeyUp(int key, int x, int y) = 0;

	virtual bool OnMouseUp(int button, int x, int y, unsigned int modifiers) = 0;
	virtual bool OnMouseDown(int button, int x, int y, unsigned int modifiers) = 0;
	virtual bool OnMouseDoubleClick(int button, int x, int y, unsigned int modifiers) = 0;

	virtual bool OnMotion(int button, int x, int y, unsigned int modifiers) = 0;
	virtual bool OnMouseWheel(int scrollDelta, int x, int y, unsigned int modifiers) = 0;

	// mouse position
	virtual void SetMousePosition(int x, int y) = 0;
	virtual void GetMousePosition(int& x, int& y) const = 0;

	virtual void TerminateApplication() = 0;

	// Key identifiers.  These are platform-specific, so classes that
	// implement the WindowApplication interfaces must define these variables.
	// They are not defined by WindowApplication.
	static const int KEY_ESCAPE = VK_ESCAPE;
	static const int KEY_LEFT_ARROW = VK_LEFT;
	static const int KEY_RIGHT_ARROW = VK_RIGHT;
	static const int KEY_UP_ARROW = VK_UP;
	static const int KEY_DOWN_ARROW = VK_DOWN;
	static const int KEY_HOME = VK_HOME;
	static const int KEY_END = VK_END;
	static const int KEY_PAGE_UP = VK_PRIOR;
	static const int KEY_PAGE_DOWN = VK_NEXT;
	static const int KEY_INSERT = VK_INSERT;
	static const int KEY_DELETE = VK_DELETE;
	static const int KEY_F1 = VK_F1;
	static const int KEY_F2 = VK_F2;
	static const int KEY_F3 = VK_F3;
	static const int KEY_F4 = VK_F4;
	static const int KEY_F5 = VK_F5;
	static const int KEY_F6 = VK_F6;
	static const int KEY_F7 = VK_F7;
	static const int KEY_F8 = VK_F8;
	static const int KEY_F9 = VK_F9;
	static const int KEY_F10 = VK_F10;
	static const int KEY_F11 = VK_F11;
	static const int KEY_F12 = VK_F12;
	static const int KEY_BACKSPACE = VK_BACK;
	static const int KEY_TAB = VK_TAB;
	static const int KEY_ENTER = VK_RETURN;
	static const int KEY_RETURN = VK_RETURN;

	// keyboard modifiers
	static const int KEY_SHIFT = MK_SHIFT;
	static const int KEY_CONTROL = MK_CONTROL;
	static const int KEY_ALT = 0;      // not currently handled
	static const int KEY_COMMAND = 0;  // not currently handled

	// mouse buttons
	static const int MOUSE_LEFT_BUTTON = 0;
	static const int MOUSE_MIDDLE_BUTTON = 1;
	static const int MOUSE_RIGHT_BUTTON = 2;
	static const int MOUSE_NO_BUTTON = -1;

	// mouse state
	static const int MOUSE_UP = 0;
	static const int MOUSE_DOWN = 1;

	// mouse modifiers
	static const int MODIFIER_CONTROL = MK_CONTROL;
	static const int MODIFIER_LBUTTON = MK_LBUTTON;
	static const int MODIFIER_MBUTTON = MK_MBUTTON;
	static const int MODIFIER_RBUTTON = MK_RBUTTON;
	static const int MODIFIER_SHIFT = MK_SHIFT;
};

}