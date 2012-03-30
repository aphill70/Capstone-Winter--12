#include "RenderManager.h"
#include "chai3d.h"
#include "VirtualHapticDevice.h"
#include "FalconDevice.h"

cHapticDeviceHandler handler;
IHapticDevice* hapticDevice;

int main(void)
{

	//VirtualHapticDevice device;
	FalconDevice device;
	device.Init();
	hapticDevice = &device;

	RenderManager::getInstance().Initialize();
	RenderManager::getInstance().RunSimulation();
	RenderManager::getInstance().EndSimulation();
}