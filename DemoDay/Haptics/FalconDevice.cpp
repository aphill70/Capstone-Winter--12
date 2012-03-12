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
	cVector3d temp;
	chaiDevice->getPosition(temp);

	// The Falcon's position components are mapped to the wrong fields; this
	//  just remaps them to the correct fields
	destination.x = temp.y;
	destination.y = temp.z;
	destination.z = temp.x;
}

void FalconDevice::GetCursorVelocity(cVector3d& destination) {
	cVector3d temp;
	chaiDevice->getLinearVelocity(temp);

	// The Falcon's position components are mapped to the wrong fields; this
	//  just remaps them to the correct fields
	destination.x = temp.y;
	destination.y = temp.z;
	destination.z = temp.x;
}