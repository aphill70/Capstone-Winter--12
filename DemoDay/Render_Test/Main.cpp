#include "RenderManager.h"
#include "chai3d.h"
//#include "FalconDevice.h"

//cHapticDeviceHandler handler;
//IHapticDevice* hapticDevice;

int main(void)
{

	//FalconDevice falcon;
	//falcon.Init();
	//hapticDevice = &falcon;

	RenderManager::getInstance().Initialize();
	RenderManager::getInstance().RunSimulation();
	RenderManager::getInstance().EndSimulation();
}