#include "RenderManager.h"

void RenderManager::GLUpdateGraphics() 
{
	RenderManager::getInstance().UpdateGraphics();
}

void RenderManager::GLResizeWindow(int width, int height)
{
	RenderManager::getInstance().ResizeWindow(width, height);
}

void RenderManager::GLMouseMotion(int x, int y)
{
	RenderManager::getInstance().MouseMotion(x, y);
}

void RenderManager::GLKeyDown(unsigned char key, int x, int y)
{
	if ((key == 27) || (key == 'x'))
    {
		exit(0);
    }
}