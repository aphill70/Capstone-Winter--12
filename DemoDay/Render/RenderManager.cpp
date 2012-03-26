#include "RenderManager.h"

RenderManager::RenderManager()
{
	world = NULL;
}

RenderManager::~RenderManager()
{
	delete world;
}

void RenderManager::Initialize()
{
	world = new cWorld();
	world->setBackgroundColor( 0.0, 1.0, 1.0);

	worldRenderer.InitWorld(world);
}
