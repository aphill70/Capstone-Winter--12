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

void RenderManager::GLSpecialKeyDown(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		key = 'a';
		break;
	case GLUT_KEY_RIGHT:
		key = 'd';
		break;
	case GLUT_KEY_UP:
		key = 'w';
		break;
	case GLUT_KEY_DOWN:
		key = 's';
		break;
	case GLUT_KEY_PAGE_DOWN:
		key = ' ';
		break;
	case GLUT_KEY_PAGE_UP:
		key = 'v';
		break;
	default:
		return;
	}

	RenderManager::GLKeyDown(key, x, y);
}

void RenderManager::GLSpecialKeyUp(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		key = 'a';
		break;
	case GLUT_KEY_RIGHT:
		key = 'd';
		break;
	case GLUT_KEY_UP:
		key = 'w';
		break;
	case GLUT_KEY_DOWN:
		key = 's';
		break;
	case GLUT_KEY_PAGE_DOWN:
		key = ' ';
		break;
	case GLUT_KEY_PAGE_UP:
		key = 'v';
		break;
	default:
		return;
	}

	RenderManager::GLKeyUp(key, x, y);
}