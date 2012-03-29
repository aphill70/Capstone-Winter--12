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