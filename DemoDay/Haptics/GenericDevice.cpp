#include "GenericDevice.h"

extern cHapticDeviceHandler handler;

void GenericDevice::Init(void) {
	handler.getDevice(chaiDevice, 0);
	chaiDevice->open();
	chaiDevice->initialize(false);

	cHapticDeviceInfo specs = chaiDevice->getSpecifications();
	maxForce = specs.m_maxForce;
	hapticRadius = specs.m_workspaceRadius;

	CenterHapticDevice();

	//TODO: find out if this is working
	//cThread* hapticsThread = new cThread();
    //hapticsThread->set(UpdateHaptics, CHAI_THREAD_PRIORITY_HAPTICS);
}

void GenericDevice::CenterHapticDevice(void) {
	cVector3d initPos(0, 0, 0);
	while (true) {
		cVector3d currPos;
		cVector3d linVeloc;
		chaiDevice->getPosition(currPos);
		chaiDevice->getLinearVelocity(linVeloc);

		double remainingDist = currPos.length();
		double currentSpeed = linVeloc.length();

		if (remainingDist < 0.013 && currentSpeed < 0.0001) {
			if (remainingDist < 0.006) {
				return;
			}

			// Render a force to overcome static friction
			currPos.negate();
			currPos.mul(1000);	// Needs a huge multiplier because distance is so small
		} else {
			// Render a rather large force
			currPos.negate();
			currPos.mul(80);

			// Add more speed if the cursor is stuck
			if (currentSpeed < 0.001) {
				currPos.mul(20);
			}
		}
		chaiDevice->setForce(currPos);
	}
}

double GenericDevice::GetMaxForce(void) { return maxForce; }

double GenericDevice::GetHapticRadius(void) { return hapticRadius; }

cGenericHapticDevice* GenericDevice::GetChaiDevice(void) { return chaiDevice; }


void GenericDevice::GetCursorPosition(cVector3d& destination) {
	chaiDevice->getPosition(destination);
	ConvertFromDeviceAxes(destination);
}

void GenericDevice::GetCursorVelocity(cVector3d& destination) {
	chaiDevice->getPosition(destination);		// This must be called to get a non-zero linear velocity
	chaiDevice->getLinearVelocity(destination);
	ConvertFromDeviceAxes(destination);
}