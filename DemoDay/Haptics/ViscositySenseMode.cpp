#include "chai3d.h"
#include "ViscositySenseMode.h"

IHapticMode* ViscositySenseMode::singleton = 0;

ViscositySenseMode::ViscositySenseMode(void) : maxViscosity(8), standardViscosity(4.5), minViscosity(1) {}

ViscositySenseMode::~ViscositySenseMode(void) {}

void ViscositySenseMode::Tick(void) {
	cVector3d destination;
	device->GetCursorVelocity(destination);
	destination.mul(-1);
	GenericDevice* genDevice = dynamic_cast<GenericDevice*>(device);
	cGenericHapticDevice* haptic = genDevice->chaiDevice;
	haptic->setForce(destination);
}

IHapticMode* ViscositySenseMode::GetSingleton(void){
	if (! singleton) {
		singleton = new ViscositySenseMode();
	}
	return singleton;
}

void ViscositySenseMode::SetDevice(IHapticDevice* newDevice) {
	device = newDevice;
}