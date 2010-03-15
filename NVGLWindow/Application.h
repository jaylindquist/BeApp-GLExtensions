#pragma once

#include "NVGLWindow.h"
#include "nvsg/nvsg.h"
#include "nvsg/Camera.h"
#include "nvsg/Material.h"
#include "nvsg/PointLight.h"
#include "nvsg/DirectedLight.h"
#include "nvsg/PerspectiveCamera.h"
#include "nvsg/Transform.h"
#include "nvsg/Node.h"
#include "nvsg/Triangles.h"
#include "nvsg/GeoNode.h"
#include "nvsg/FaceAttribute.h"
#include "nvmath/nvmath.h"
#include "nvui/TrackballCameraManipulator.h"


class Application : public BitwiseEngine::NVGLWindow
{
public:	
	Application(HINSTANCE hInstance);
	virtual ~Application(void) {};

	virtual bool OnInitialize();
	virtual void OnDisplay();

	virtual bool OnMouseUp(int button, int x, int y, unsigned int modifiers);
	virtual bool OnMouseDown(int button, int x, int y, unsigned int modifiers);
	virtual bool OnMotion(int button, int x, int y, unsigned int modifiers);

	virtual bool OnKeyDown(int key, int x, int y);
	virtual bool OnKeyUp(int key, int x, int y);
	virtual void OnTerminate();
	virtual void OnResize(int width, int height);
	
protected:
	nvui::TrackballCameraManipulator *m_trackballManip;
	nvsg::SceneHandle* m_scene;
	
	nvsg::CameraHandle* createCamera();
	nvsg::SceneHandle* createScene();
};
