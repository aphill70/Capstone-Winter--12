#include "chai3d.h"
#include "FalconDevice.h"
#include "ViscositySenseMode.h"

IHapticMode* ViscositySenseMode::singleton = 0;

ViscositySenseMode::ViscositySenseMode(void) : maxViscosity(8), standardViscosity(4.5), minViscosity(1) {}

ViscositySenseMode::~ViscositySenseMode(void) {}

void ViscositySenseMode::Tick(void) {
	cVector3d linearVelocity;
	device->GetCursorVelocity(linearVelocity);
	double forcePercent = linearVelocity.length() / MAX_SPEED;
	linearVelocity.mul(-forcePercent * device->GetMaxForce());

	GenericDevice* genDevice = dynamic_cast<GenericDevice*>(device);
	cGenericHapticDevice* haptic = genDevice->chaiDevice;

	FalconDevice::ConvertToDeviceAxes(linearVelocity);
	haptic->setForce(linearVelocity);
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