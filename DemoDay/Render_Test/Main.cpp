#include "RenderManager.h"

int main(void)
{
	RenderManager::getInstance().Initialize();
	RenderManager::getInstance().RunSimulation();
	RenderManager::getInstance().EndSimulation();
}