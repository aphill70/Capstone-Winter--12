#include "FalconDevice.h"

/*
private:
	cFalconDevice device;
	cVector3d cursorPosition;
	cVector3d cursorVelocity;

	ITerrain* terrain;
	IFluid* fluid;
*/
	
FalconDevice::FalconDevice(void) {
}

FalconDevice::~FalconDevice(void) {
}
	
void FalconDevice::Init(void) {
	cHapticDeviceHandler handler;
	handler.getDevice(device, 0);
	device->open();
	device->initialize(false);

	cHapticDeviceInfo specs = device->getSpecifications();
	//TODO: decide where to put these
	//double MAX_HAPTIC_FORCE = specs.m_maxForce;
	//double HAPTIC_RADIUS = specs.m_workspaceRadius;

	CenterHapticDevice();

	//TODO: find out if this is working
	//cThread* hapticsThread = new cThread();
    //hapticsThread->set(UpdateHaptics, CHAI_THREAD_PRIORITY_HAPTICS);
}

void FalconDevice::SetTerrain(ITerrain* newTerrain) {
	terrain = newTerrain;
}

void FalconDevice::SetFluid(IFluid* newFluid) {
	fluid = newFluid;
}

void FalconDevice::GetCursorPosition(cVector3d& destination) {

}

void FalconDevice::GetCursorVelocity(cVector3d& destination) {
}

void FalconDevice::CenterHapticDevice(void) {
}