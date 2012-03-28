#include "chai3d.h"
#include "FalconDevice.h"
#include "UniformViscositySenseMode.h"

IHapticMode* UniformViscositySenseMode::singleton = 0;

UniformViscositySenseMode::UniformViscositySenseMode(void) : lastPosition(0, 0, 0) {}

UniformViscositySenseMode::~UniformViscositySenseMode(void) {}

void UniformViscositySenseMode::Tick(void) {
	extern IHapticDevice* hapticDevice;
	GenericDevice* genDevice = dynamic_cast<GenericDevice*>(hapticDevice);
	cGenericHapticDevice* haptic = genDevice->GetChaiDevice();

	cVector3d linearVelocity;
	hapticDevice->GetCursorVelocity(linearVelocity);

	double forcePercent = linearVelocity.length() / UNIF_VISC_MAX_SPEED;
	linearVelocity.mul(-forcePercent * hapticDevice->GetMaxForce());

	hapticDevice->ConvertToDeviceAxes(linearVelocity);
	haptic->setForce(linearVelocity);
}

IHapticMode* UniformViscositySenseMode::GetSingleton(void){
	if (! singleton) {
		singleton = new UniformViscositySenseMode();
	}
	return singleton;
}