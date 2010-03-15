inline const char* 
GLWindowApp::GetWindowTitle() const
{
	return m_acWindowTitle;
}

inline int 
GLWindowApp::GetXPosition() const
{
	return m_xPosition;
}

inline int 
GLWindowApp::GetYPosition() const
{
	return m_yPosition;
}

inline int 
GLWindowApp::GetWidth() const
{
	return m_width;
}

inline int 
GLWindowApp::GetHeight() const
{
	return m_height;
}

inline void 
GLWindowApp::SetWindowID(int windowID)
{
	m_ID = windowID;
}

inline int 
GLWindowApp::GetWindowID() const
{
	return m_ID;
}

inline bool
GLWindowApp::PreRegisterClass(WNDCLASSEX& wndClass)
{
	return true;
}

inline bool 
GLWindowApp::PreChoosePixelFormat(PIXELFORMATDESCRIPTOR& pfd)
{
	return true;
}

inline bool 
GLWindowApp::UserWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

inline bool 
GLWindowApp::OnPrecreate()
{
    return true;
}

inline void 
GLWindowApp::OnDisplay()
{
}

inline void 
GLWindowApp::OnIdle()
{
}

inline bool 
GLWindowApp::OnKeyDown(int key, int x, int y)
{
    return false;
}

inline bool 
GLWindowApp::OnKeyUp(int key, int x, int y)
{
    return false;
}

inline bool 
GLWindowApp::OnMouseDown(int button, int x, int y, unsigned int modifiers)
{
    return false;
}

inline bool 
GLWindowApp::OnMouseUp(int button, int x, int y, unsigned int modifiers)
{
    return false;
}

inline bool 
GLWindowApp::OnMouseDoubleClick(int button, int x, int y, unsigned int modifiers)
{
    return false;
}

inline bool 
GLWindowApp::OnMotion(int button, int x, int y, unsigned int modifiers)
{
    return false;
}

inline bool 
GLWindowApp::OnMouseWheel(int scrollDelta, int x, int y, unsigned int modifiers)
{
    return false;
}

inline bool 
GLWindowApp::OnInitialize ()
{
	return true;
}

inline void 
GLWindowApp::OnTerminate ()
{
}

inline void
GLWindowApp::ShowConsole(bool show) 
{
	ShowWindow(GetConsoleWindow(), show? SW_SHOW : SW_HIDE);
}