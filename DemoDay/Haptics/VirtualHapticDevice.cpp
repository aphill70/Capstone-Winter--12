#include "VirtualHapticDevice.h"

VirtualHapticDevice::VirtualHapticDevice(void) {
	clock.start(true);
	lastClock = 0;
}

VirtualHapticDevice::~VirtualHapticDevice(void) {
	clock.stop();
}

void VirtualHapticDevice::CenterHapticDevice(void) {
	// Nothing needs to happen
}

void VirtualHapticDevice::GetCursorVelocity(cVector3d& velocity) {
	double currentTime = clock.getCurrentTimeSeconds();
	double ellapsedTime = currentTime - lastClock;
	lastClock = currentTime;

	cVector3d currentPos;
	chaiDevice->getPosition(currentPos);
	velocity = currentPos - lastPosition;
	velocity.mul(1.0 / ellapsedTime );
	lastPosition.copyfrom(currentPos);
}

// The Falcon's position components are mapped to the wrong fields; this
//  just remaps them to the correct fields
void VirtualHapticDevice::ConvertToDeviceAxes(cVector3d& vector) {
	double x = vector.x;
	double y = vector.y;
	double z = vector.z;

	vector.x = z;
	vector.y = x;
	vector.z = y;
}

// The Falcon's position components are mapped to the wrong fields; this
//  just remaps them to the correct fields
void VirtualHapticDevice::ConvertFromDeviceAxes(cVector3d& vector) {
	double x = vector.x;
	double y = vector.y;
	double z = vector.z;

	vector.x = y;
	vector.y = z;
	vector.z = 0 - x;
}