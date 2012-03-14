#include "FalconDevice.h"
#include "ViscositySenseMode.h"

/*
private:
	cFalconDevice device;
	cVector3d cursorPosition;
	cVector3d cursorVelocity;

	ITerrain* terrain;
	IFluid* fluid;
*/
	
FalconDevice::FalconDevice(void) {
	mode = 0;
	maxForce = 0;
	hapticRadius = 0;
}

FalconDevice::~FalconDevice(void) {
	
}

void FalconDevice::SetMode(IHapticMode* newMode) {
	IHapticDevice::SetMode(newMode);
	if (ViscositySenseMode* senseMode = dynamic_cast<ViscositySenseMode*>(newMode)) {
		senseMode->SetDevice(this);
	}
}

void FalconDevice::SetTerrain(ITerrain* newTerrain) {
	terrain = newTerrain;
}

void FalconDevice::SetFluid(IFluid* newFluid) {
	fluid = newFluid;
}

void FalconDevice::GetCursorPosition(cVector3d& destination) {
	chaiDevice->getPosition(destination);

	// The Falcon's position components are mapped to the wrong fields; this
	//  just remaps them to the correct fields
	ConvertFromDeviceAxes(destination);
	//printf("Lin. velocity: (%f, %f, %f)\n", destination.x, destination.y, destination.z);
}

void FalconDevice::GetCursorVelocity(cVector3d& destination) {
	chaiDevice->getPosition(destination);		// This must be called to get a non-zero linear velocity
	chaiDevice->getLinearVelocity(destination);

	// The Falcon's position components are mapped to the wrong fields; this
	//  just remaps them to the correct fields
	ConvertFromDeviceAxes(destination);
}

void FalconDevice::ConvertToDeviceAxes(cVector3d& vector) {
	double x = vector.x;
	double y = vector.y;
	double z = vector.z;

	vector.x = z;
	vector.y = x;
	vector.z = y;
}

void FalconDevice::ConvertFromDeviceAxes(cVector3d& vector) {
	double x = vector.x;
	double y = vector.y;
	double z = vector.z;

	vector.x = y;
	vector.y = z;
	vector.z = x;
}
