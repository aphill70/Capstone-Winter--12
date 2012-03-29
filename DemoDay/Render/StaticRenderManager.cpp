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

void RenderManager::GLMouseFunc(int button, int state, int x, int y)
{
	RenderManager::getInstance().MouseClick(button, state, x, y);
}

void RenderManager::GLKeyDown(unsigned char key, int x, int y)
{
	RenderManager::getInstance().KeyDown(key, x, y);
}

void RenderManager::GLKeyUp(unsigned char key, int x, int y)
{
	RenderManager::getInstance().KeyUp(key, x, y);
}