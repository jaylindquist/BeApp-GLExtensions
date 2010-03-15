#pragma once

#include <GLWindowApp.h>
#include "VertexBufferObject.h"


class OGLWindow : public BitwiseEngine::GLWindowApp
{
public:	
	OGLWindow(const char* windowTitle, int x, int y, int width, int height);
	virtual ~OGLWindow(void) {};

	virtual bool OnKeyDown(int key, int x, int y);
	virtual void OnDisplay();
	virtual bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnResize(int width, int height);
protected:
	VertexBufferObject *m_vbo;
};
