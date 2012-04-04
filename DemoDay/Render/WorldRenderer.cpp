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
	/*delete light;
	delete camera;

	for(int i = 0; i < 3; i++)
	{
		delete axes[i];
	}*/
}

void WorldRenderer::InitWorld(cWorld * w, cMatrix3d cam)
{
	world = w;

	camera = new cCamera(world);
	InitCamera(cam);

	light = new cLight(world);
	InitLight();

	RenderAxes(AXIS_LEN);
	RenderSky(WORLD_BOUND);
	RenderGround(WORLD_BOUND);
}

void WorldRenderer::InitCamera(cMatrix3d cam)
{
	world->addChild(camera);

	SetCamera(cam);

    // set the near and far clipping planes of the camera
    // anything in front/behind these clipping planes will not be rendered
    
	//camera->setClippingPlanes(0.01, 10.0);

    // enable higher quality rendering for transparent objects
    
	//camera->enableMultipassTransparency(true);
}

void WorldRenderer::InitLight()
{
    world->addChild(light);                   // attach light to camera
    light->setEnabled(true);                   // enable light source
    light->setPos(cVector3d( WORLD_BOUND / 2, WORLD_BOUND, WORLD_BOUND / 2));  // position the light source
    light->setDir(cVector3d(-1, -1, -1));  // define the direction of the light beam
}

void WorldRenderer::SetCamera(cMatrix3d cam)
{
	camera->set(cam.getCol0(),    // camera position (eye)
				cam.getCol1(),    // lookat position (target)
				cam.getCol2());   // direction of the "up" vector
}

void WorldRenderer::RenderCamera(int width, int height)
{
	camera->renderView(width, height);
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

void WorldRenderer::RenderSky(int length)
{
	cColorb lowersky, uppersky;
	lowersky.set(165,207,255);
	uppersky.set(132,187,254);

	cTriangle *lower, *upper;

	//define values for creating coordinates
	double posX = length / 2.0;
	double negX = length / -2.0;
	double posY = length;
	double negY = 0;
	double posZ = length / 2.0;
	double negZ = length / -2.0;

	//Create +X Side
	skybox[0] = new cMesh(world);
	world->addChild(skybox[0]);
	skybox[0]->setPos(0, 0, 0);
	//Renders on side counterclockwise
	skybox[0]->newTriangle(	cVector3d(posX, negY, posZ), 
							cVector3d(posX, posY, negZ),
							cVector3d(posX, negY, negZ));
	lower = skybox[0]->getTriangle(0);
	skybox[0]->newTriangle( cVector3d(posX, negY, posZ),
							cVector3d(posX, posY, posZ),
							cVector3d(posX, posY, negZ));

	//Create -X Side
	skybox[1] = new cMesh(world);
	world->addChild(skybox[1]);
	skybox[1]->setPos(0, 0, 0);
	//Renders on side counterclockwise
	skybox[1]->newTriangle(	cVector3d(negX, negY, posZ), 
							cVector3d(negX, negY, negZ),
							cVector3d(negX, posY, negZ));
	skybox[1]->newTriangle( cVector3d(negX, negY, posZ),
							cVector3d(negX, posY, negZ),
							cVector3d(negX, posY, posZ));
	
	//Create +Z side
	skybox[2] = new cMesh(world);
	world->addChild(skybox[2]);
	skybox[2]->setPos(0, 0, 0);
	//Renders on side counterclockwise
	skybox[2]->newTriangle(	cVector3d(negX, negY, posZ), 
							cVector3d(posX, posY, posZ),
							cVector3d(posX, negY, posZ));
	skybox[2]->newTriangle( cVector3d(negX, negY, posZ),
							cVector3d(negX, posY, posZ),
							cVector3d(posX, posY, posZ));

	//Create -Z side
	skybox[3] = new cMesh(world);
	world->addChild(skybox[3]);
	skybox[3]->setPos(0, 0, 0);
	//Renders on side counterclockwise
	skybox[3]->newTriangle(	cVector3d(negX, negY, negZ), 
							cVector3d(posX, negY, negZ),
							cVector3d(posX, posY, negZ));
	skybox[3]->newTriangle( cVector3d(negX, negY, negZ),
							cVector3d(posX, posY, negZ),
							cVector3d(negX, posY, negZ));

	//Create +Y side
	skybox[4] = new cMesh(world);
	world->addChild(skybox[4]);
	skybox[4]->setPos(0, 0, 0);
	//Renders on side counterclockwise
	skybox[4]->newTriangle(	cVector3d(negX, posY, negZ), 
							cVector3d(posX, posY, negZ),
							cVector3d(posX, posY, posZ));
	skybox[4]->newTriangle( cVector3d(negX, posY, negZ),
							cVector3d(posX, posY, posZ),
							cVector3d(negX, posY, posZ));

	//Color
	cVertex * curVertex;
	for(int i = 0; i < 5; i++)
	{
		skybox[i]->setUseVertexColors(true);
		for(int j = 0; j < skybox[i]->getNumVertices(); j++)
		{
			curVertex = skybox[i]->getVertex(j);
			if(curVertex->getPos().y == posY)
				curVertex->setColor(uppersky);
			else
				curVertex->setColor(lowersky);
		}
	}
}

void WorldRenderer::RenderGround(int length)
{
	ground = new cMesh(world);
	world->addChild(ground);
}