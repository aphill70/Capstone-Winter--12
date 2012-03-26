#include "WorldRenderer.h"

WorldRenderer::WorldRenderer()
{
	camera = NULL;
	light = NULL;

	for(int i = 0; i < 3; i++)
	{
		axes[i] = NULL;
	}
}

WorldRenderer::~WorldRenderer()
{
	delete light;
	delete camera;

	for(int i = 0; i < 3; i++)
	{
		delete axes[i];
	}
}

void WorldRenderer::InitWorld(cWorld * w)
{
	world = w;

	camera = new cCamera(world);
	InitCamera();

	light = new cLight(world);
	InitLight();

	RenderAxes(10);
}

void WorldRenderer::InitCamera()
{
	world->addChild(camera);

    camera->set(	cVector3d (0.0, 0.0, 3.0),    // camera position (eye)
					cVector3d (0.0, 0.0, 0.0),    // lookat position (target)
					cVector3d (0.0, 1.0, 0.0));   // direction of the "up" vector

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    camera->setClippingPlanes(0.01, 10.0);

    // enable higher quality rendering for transparent objects
    camera->enableMultipassTransparency(true);
}

void WorldRenderer::InitLight()
{
    camera->addChild(light);                   // attach light to camera
    light->setEnabled(true);                   // enable light source
    light->setPos(cVector3d( 2.0, 0.5, 1.0));  // position the light source
    light->setDir(cVector3d(-2.0, 0.5, 1.0));  // define the direction of the light beam
}

void WorldRenderer::RenderAxes(int magnitude)
{
	//Render X-Axis
	axes[XAXIS] = new cShapeLine(	cVector3d(0,0,0),
									cVector3d(magnitude, 0,0));

	axes[XAXIS]->m_ColorPointA = cColorf(0, 1, 0);	//Green

	//Render Y-Axis
	axes[YAXIS] = new cShapeLine(	cVector3d(0,0,0),
									cVector3d(0, magnitude, 0));

	axes[YAXIS]->m_ColorPointA = cColorf(0, 0, 1);	//Blue

	//Render Z-Axis
	axes[ZAXIS] = new cShapeLine(	cVector3d(0,0,0),
									cVector3d(0, 0, magnitude));

	axes[ZAXIS]->m_ColorPointA = cColorf(1, 0, 0);	//Red

	world->addChild(axes[XAXIS]);
	world->addChild(axes[YAXIS]);
	world->addChild(axes[ZAXIS]);
}