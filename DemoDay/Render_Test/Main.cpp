#include "RenderManager.h"
#include "chai3d.h"
#include "VirtualHapticDevice.h"
#include "FalconDevice.h"
#include "IFluid.h"
#include "GEOFileFluid.h"

cHapticDeviceHandler handler;
IHapticDevice* hapticDevice;
IFluid * fluidModel;


int main(void)
{

	VirtualHapticDevice device;
	//FalconDevice device;

	GEOFileFluid fluid("../Fluids/fluidBake/demo_day_geometry", 200, 220);
	fluidModel = &fluid;

	device.Init();
	hapticDevice = &device;

	RenderManager::getInstance().Initialize(fluidModel);
	RenderManager::getInstance().RunSimulation();
	RenderManager::getInstance().EndSimulation();
}