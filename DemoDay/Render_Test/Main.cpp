#include "RenderManager.h"
#include "chai3d.h"
#include "VirtualHapticDevice.h"
#include "FalconDevice.h"
#include "IFluid.h"
#include "GEOFileFluid.h"

#include <iostream>

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

	cout << endl << "Simulation Ready. Press enter to begin." << endl;
	cin.get();

	RenderManager::getInstance().Initialize(fluidModel);
	RenderManager::getInstance().RunSimulation();
	RenderManager::getInstance().EndSimulation();
}