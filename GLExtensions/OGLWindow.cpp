#include "OGLWindow.h"

#include <iostream>
#include <glee/glee.h>
#include <GL/glu.h>

using namespace std;
using namespace BitwiseEngine;


struct Vertex
{
	float vx, vy, vz;
	float cx, cy, cz;
};

OGLWindow::OGLWindow(const char* windowTitle, int x, int y, int width, int height) :
GLWindowApp(windowTitle, x, y, width, height, 3, 2)
{

}

bool 
OGLWindow::OnInitialize()
{
	cout << "Using OpenGL Version " << this->m_majorVersion << "." << this->m_minorVersion << endl;
	cout << "Using GLSL Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


	Vertex* vertices = new Vertex[3];
	vertices[0].vx = -1.414f;
	vertices[0].vy = -1.414f;
	vertices[0].vz = 0.0f;
	vertices[0].cx = 1.0f;
	vertices[0].cy = 0.0f;
	vertices[0].cz = 1.0f;
	
	vertices[1].vx = 1.414f;
	vertices[1].vy = -1.414f;
	vertices[1].vz = 0.0f;
	vertices[1].cx = 0.0f;
	vertices[1].cy = 1.0f;
	vertices[1].cz = 1.0f;
	
	vertices[2].vx = 0.0f;
	vertices[2].vy = 1.0f;
	vertices[2].vz = 0.0f;
	vertices[2].cx = 1.0f;
	vertices[2].cy = 1.0f;
	vertices[2].cz = 1.0f;

	m_vbo = new VertexBufferObject();
	m_vbo->Bind(VertexBufferObject::ARRAY_BUFFER);
	m_vbo->SetData(VertexBufferObject::ARRAY_BUFFER, sizeof(Vertex) * 3, vertices, VertexBufferObject::STATIC_READ);

	delete[] vertices;

	return true;
}

void
OGLWindow::OnTerminate()
{
	delete m_vbo;
}

bool 
OGLWindow::OnKeyDown(int key, int x, int y)
{
	static bool show = true;
	
	switch(key)
	{
	case 'Q':
		TerminateApplication();
		break;

	case 'R':
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;

	case 'W':
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case 'C':
		ShowConsole(show);
		show = !show;
		break;

	default: break;
	}

	return 0;
}

void
OGLWindow::OnResize(int width, int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, static_cast<double>(width)/static_cast<double>(height), 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void 
OGLWindow::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	static float r = 0.0f;
	glRotatef(r, 0.0f, 0.0f, 1.0f);
	r += 1.0f;

	gluLookAt(0.0, 0.0, -8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	m_vbo->Bind(VertexBufferObject::ARRAY_BUFFER);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glColorPointer(3, GL_FLOAT, sizeof(float)*6, (float*)NULL + 3);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();
		glTranslatef(-2.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(2.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, -2.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, 2.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	m_vbo->Unbind(VertexBufferObject::ARRAY_BUFFER);
}

int main(int argc, char** argv)
{
	App::TheApplication = new OGLWindow("Hello World", 100, 100, 500, 500);
	return App::TheApplication->Run(argc, argv);
}