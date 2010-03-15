#include "Application.h"

using namespace BitwiseEngine;
using namespace nvmath;
using namespace nvsg;
using namespace nvui;
using namespace nvutil;

Application::Application(HINSTANCE hInstance)
:NVGLWindow(hInstance)
{

}

bool
Application::OnInitialize()
{
	m_trackballManip = new TrackballCameraManipulator;
	m_trackballManip->setRenderArea(this);

	//scene
	m_scene = createScene();

	//viewState and Camera
	ViewStateHandle *vsHdl = CreateHandle(ViewStateHandle);
	WritableObject<ViewState>(vsHdl)->setCamera(createCamera());

	//tell renderArea the scene and view State
	this->setSceneData(m_scene,vsHdl);
	return true;
}

void
Application::OnDisplay() 
{
	this->renderScene();
}

bool 
Application::OnKeyDown(int key, int x, int y)
{
	switch(key)
	{
	case WindowApp::KEY_SHIFT:
		m_trackballManip->keyDown(nvui::NVSG_SHIFT);
		break;
	case WindowApp::KEY_ALT:
		m_trackballManip->keyDown(nvui::NVSG_ALT);
		break;
	case WindowApp::KEY_CONTROL:
		m_trackballManip->keyDown(nvui::NVSG_CONTROL);
		break;
	case WindowApp::KEY_F1:
		m_trackballManip->keyDown(nvui::NVSG_HOTSPOT);
		break;
	case WindowApp::KEY_F2:
		m_trackballManip->keyDown(nvui::NVSG_CONTINUE);
		break;
	default:
		return false;
	}

	return true;
}

bool 
Application::OnKeyUp(int key, int x, int y)
{
	switch(key)
	{
	case WindowApp::KEY_SHIFT:
		m_trackballManip->keyUp(nvui::NVSG_SHIFT);
		break;
	case WindowApp::KEY_ALT:
		m_trackballManip->keyUp(nvui::NVSG_ALT);
		break;
	case WindowApp::KEY_CONTROL:
		m_trackballManip->keyUp(nvui::NVSG_CONTROL);
		break;
	case WindowApp::KEY_F1:
		m_trackballManip->keyUp(nvui::NVSG_HOTSPOT);
		break;
	case WindowApp::KEY_F2:
		m_trackballManip->keyUp(nvui::NVSG_CONTINUE);
		break;
	default:
		return false;
	}

	return true;
}

bool 
Application::OnMouseUp(int button, int x, int y, unsigned int modifiers)
{
	switch(button)
	{
	case WindowApp::MOUSE_LEFT_BUTTON:
		{
			m_trackballManip->mouseButtonUp(NVSG_LEFT,x ,y);
			break;
		}
	case WindowApp::MOUSE_MIDDLE_BUTTON:
		{
			m_trackballManip->mouseButtonUp(NVSG_MIDDLE,x ,y);
			break;
		}
	default: return false;
	}

	return true;
}

bool 
Application::OnMouseDown(int button, int x, int y, unsigned int modifiers)
{
	switch(button)
	{
	case WindowApp::MOUSE_LEFT_BUTTON:
		{
			m_trackballManip->mouseButtonDown(NVSG_LEFT,x ,y);
			break;
		}
	case WindowApp::MOUSE_RIGHT_BUTTON:
		{			
			//zoom all
			if(this && ReadableObject<ViewState>(this->getViewState())->getCamera())
			{
				WritableObject<Camera> camera(ReadableObject<ViewState>(this->getViewState())->getCamera() );
				NodeHandle * rootHdl = ReadableObject<Scene>(m_scene)->getRootNode();
				if (rootHdl && camera)
				{
					// zoom(pNode) takes the boundingsphere of pNode into consideration.
					camera->zoom( ReadableObject<Node>(rootHdl)->getBoundingSphere() );
				}
			}
			break;
		}
	case WindowApp::MOUSE_MIDDLE_BUTTON:
		{
			m_trackballManip->mouseButtonDown(NVSG_MIDDLE,x ,y);
			break;
		}
	default: return false;
	}

	return true;
}

bool 
Application::OnMotion(int button, int x, int y, unsigned int modifiers)
{
	m_trackballManip->mouseMotion(x, y);
	if(m_trackballManip->updateFrame())
	{
		this->triggerRedraw();
	}  
	return true;  
}

void 
Application::OnTerminate()
{
	if (m_scene)
	{
		m_scene->removeRef();
	}

	if (m_trackballManip)
	{
		delete m_trackballManip;
		m_trackballManip = NULL;
	}
}

void 
Application::OnResize(int width, int height)
{
	this->setViewportSize(width, height);
}

SceneHandle* 
Application::createScene()
{
	// Create the root node and apply a rotation.
	// Note: 
	// We do not increment the ref count here, 
	// because we throw the root into the scene 
	// via setRoot(), and setRoot() will increment 
	// it for us.

	// setup vertices and faces  
	Vec3f vertices[8] = { Vec3f(-0.2f, -0.2f,  0.2f)
		, Vec3f(-0.2f,  0.2f,  0.2f)
		, Vec3f( 0.2f,  0.2f,  0.2f)
		, Vec3f( 0.2f, -0.2f,  0.2f)
		, Vec3f( 0.2f, -0.2f, -0.2f)
		, Vec3f( 0.2f,  0.2f, -0.2f)
		, Vec3f(-0.2f,  0.2f, -0.2f)
		, Vec3f(-0.2f, -0.2f, -0.2f) };

	Face3 faces[12]     = { {0,2,1}, {2,0,3}
	, {3,5,2}, {5,3,4}
	, {4,6,5}, {6,4,7}
	, {7,1,6}, {1,7,0}
	, {0,4,3}, {4,0,7}
	, {1,5,6}, {5,1,2} };

	// calculating face normals
	Vec3f v[36];
	Vec3f n[36];
	Face3 f[12];
	Vec3f v0, v1, v2, d0, d1, fn;
	for ( int kf=0, kv=0; kf<12; kf++, kv+=3 )
	{
		v0 = vertices[faces[kf][0]];
		v1 = vertices[faces[kf][1]];
		v2 = vertices[faces[kf][2]];

		d0 = v1 - v0;
		d1 = v2 - v0;

		fn = d0^d1;
		fn.normalize();

		f[kf][0] = kv;
		f[kf][1] = kv+1;
		f[kf][2] = kv+2;

		v[kv]    = v0;
		v[kv+1]  = v1;
		v[kv+2]  = v2;

		n[kv]    = fn; 
		n[kv+1]  = fn; 
		n[kv+2]  = fn; 
	}

	// Create a VertexAttributeSet with vertices and normals
	VertexAttributeSetHandle * vasHdl = CreateHandle(VertexAttributeSetHandle);
	{
		WritableObject<VertexAttributeSet> vas(vasHdl);
		vas->setVertices( v, 36 );
		vas->setNormals( n, 36 );
	}

	// Create a PrimitiveSet, in this case triangles.
	TrianglesHandle * shapeHdl = CreateHandle(TrianglesHandle);
	{
		WritableObject<Triangles> triangles(shapeHdl);
		triangles->setVertexAttributeSet( vasHdl );
		triangles->setFaces(f, 12);
	}

	// Create a material.
	MaterialHandle * materialHdl = CreateHandle(MaterialHandle);
	{
		WritableObject<Material> material(materialHdl);
		material->setAmbientColor( Vec3f(0.f, 0.f, 1.f) );
		material->setDiffuseColor( Vec3f(0.4f, 0.4f, 0.4f) );
		material->setEmissiveColor( Vec3f(0.f, 0.f, 0.f) );
		material->setSpecularColor( Vec3f(1.f, 1.f, 1.f) );
		material->setSpecularExponent( 10.f );
		material->setOpacity( 1.0f );
	}

	StateSetHandle * stateSetHdl = CreateHandle(StateSetHandle);
	{
		WritableObject<StateSet> stateSet(stateSetHdl);
		stateSet->addAttribute(materialHdl);
	}

	// Setup the whole scene graph
	GeoNodeHandle * geoNodeHdl = CreateHandle(GeoNodeHandle);
	WritableObject<GeoNode>(geoNodeHdl)->addGeometry(shapeHdl,stateSetHdl);

	GeoNodeHandle * geoNodeHdl2 = geoNodeHdl->clone();

	// Note: 
	// AddChild() will increment the reference count 
	// of the added node.
	Trafo shapeTrafo2;
	shapeTrafo2.setOrientation( Quatf(Vec3f(0.0f, 1.0f, 0.0f ), float(PI_QUARTER)) );
	TransformHandle * pShapeTrafo2 = CreateHandle(TransformHandle);
	{
		WritableObject<Transform> wot(pShapeTrafo2);
		wot->setTrafo( shapeTrafo2 );
		wot->addChild( geoNodeHdl );
	}

	Trafo shapeTrafo;
	shapeTrafo.setOrientation( Quatf(Vec3f(1.0f, 0.0f, 0.0f ), float(PI_QUARTER)) );
	TransformHandle * pShapeTrafo = CreateHandle(TransformHandle);
	{
		WritableObject<Transform> wot(pShapeTrafo);
		wot->setTrafo(shapeTrafo);
		wot->addChild( pShapeTrafo2 );
	}

	Trafo shape2Trafo2;
	shape2Trafo2.setOrientation( Quatf(Vec3f(0.0f, 1.0f, 0.0f ), float(PI_QUARTER)) );
	TransformHandle * pShape2Trafo2 = CreateHandle(TransformHandle);
	{
		WritableObject<Transform> wot(pShape2Trafo2);
		wot->setTrafo(shape2Trafo2);
		wot->addChild(geoNodeHdl2);
	}

	Trafo shape2Trafo;
	shape2Trafo.setOrientation( Quatf(Vec3f(1.0f, 0.0f, 0.0f ), 0.0f) );
	shape2Trafo.setTranslation( Vec3f( -1.0f, 0.0f, 0.0f ) ); 
	TransformHandle * pShape2Trafo = CreateHandle(TransformHandle);
	{
		WritableObject<Transform> wot(pShape2Trafo);
		wot->setTrafo(shape2Trafo);
		wot->addChild(pShape2Trafo2);
	}

	Trafo shape3Trafo;
	shape3Trafo.setOrientation( Quatf(Vec3f(1.0f, 0.0f, 0.0f ), 0.0f) );
	shape3Trafo.setTranslation( Vec3f( 1.0f, 0.0f, 0.0f ) ); 
	TransformHandle * pShape3Trafo = CreateHandle(TransformHandle);
	{
		WritableObject<Transform> wot(pShape3Trafo);
		wot->setTrafo(shape3Trafo);
		wot->addChild( geoNodeHdl2 );
	}

	GroupHandle * rootHdl = CreateHandle(GroupHandle);
	{
		WritableObject<Group> root(rootHdl);
		root->addChild( pShapeTrafo );
		root->addChild( pShape2Trafo );
		root->addChild( pShape3Trafo );
	}

	SceneHandle * sceneHdl = CreateHandle(SceneHandle);
	sceneHdl->addRef();
	{
		WritableObject<Scene> scene(sceneHdl);
		scene->setRootNode(rootHdl);
		scene->setBackColor(Vec4f(1,0,0,1)); 
	}

	return(sceneHdl);
}

CameraHandle* 
Application::createCamera()
{
	NVSG_ASSERT(m_scene);

	// Create camera
	CameraHandle * cameraHdl = CreateHandle(PerspectiveCameraHandle);
	{
		WritableObject<Camera> camera(cameraHdl);

		// Make the (world space) window aspect ratio 
		// correspond to the (pixel space) viewport aspect ratio
		camera->setName( "ViewCamera" );
		camera->setAspectRatio( this->getAspectRatio() );

		// Setup the cameras' headlight
		PointLightHandle * pointLightHdl = CreateHandle(PointLightHandle);
		{
			WritableObject<PointLight> pointLight(pointLightHdl);
			pointLight->setAmbientColor( Vec3f(1.f, 1.f, 1.f) );
			pointLight->setDiffuseColor( Vec3f(1.0f, 1.0f, 1.0f) );
			pointLight->setSpecularColor( Vec3f(1.0f, 1.0f, 1.0f) );
			pointLight->setAttenuation( 1.0f, 0.0f, 0.0f );
			pointLight->setPosition( Vec3f( 5.0f, 5.0f, 0.0f ) );
		}
		camera->addHeadLight( pointLightHdl );

		// Make scene fit into the viewport.
		ReadableObject<Scene> scene(m_scene);
		camera->zoom( ReadableObject<Node>(scene->getRootNode())->getBoundingSphere()
			, float(nvmath::PI_QUARTER) );
	}

	return( cameraHdl );
}


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	Application app(hInstance);
	App::TheApplication = &app;
	return App::TheApplication->Run(NULL, 0);
}