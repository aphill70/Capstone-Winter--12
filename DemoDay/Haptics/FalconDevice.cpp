#include "FalconDevice.h"
#include "UniformViscositySenseMode.h"

/*
private:
	cFalconDevice device;
	cVector3d cursorPosition;
	cVector3d cursorVelocity;
*/
	
FalconDevice::FalconDevice(void) {
	mode = 0;
	maxForce = 0;
	hapticRadius = 0;
}

FalconDevice::~FalconDevice(void) {
	
}

// The Falcon's position components are mapped to the wrong fields; this
//  just remaps them to the correct fields
void FalconDevice::ConvertToDeviceAxes(cVector3d& vector) {
	double x = vector.x;
	double y = vector.y;
	double z = vector.z;

	vector.x = z;
	vector.y = x;
	vector.z = y;
}

// The Falcon's position components are mapped to the wrong fields; this
//  just remaps them to the correct fields
void FalconDevice::ConvertFromDeviceAxes(cVector3d& vector) {
	double x = vector.x;
	double y = vector.y;
	double z = vector.z;

	vector.x = y;
	vector.y = z;
	vector.z = x;
}
