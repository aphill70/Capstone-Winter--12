#include "chai3d.h"
#include "FalconDevice.h"
#include "DirectionalViscositySenseMode.h"

//
void printHapticLevel(double percent) {
	printf("Rendered force amount: ");
	if (percent > 0.05)
		printf("X");
	if (percent > 0.15)
		printf("X");
	if (percent > 0.25)
		printf("X");
	if (percent > 0.35)
		printf("X");
	if (percent > 0.45)
		printf("X");
	if (percent > 0.55)
		printf("X");
	if (percent > 0.65)
		printf("X");
	if (percent > 0.75)
		printf("X");
	if (percent > 0.85)
		printf("X");
	if (percent > 0.95)
		printf("X");

	printf("\n");
}
//

IHapticMode* DirectionalViscositySenseMode::singleton = 0;

DirectionalViscositySenseMode::DirectionalViscositySenseMode(void) : maxViscosity(1.0), minViscosity(0) {}	//standardViscosity(4.0), 

DirectionalViscositySenseMode::~DirectionalViscositySenseMode(void) {}

void DirectionalViscositySenseMode::Tick(void) {
	extern IHapticDevice* hapticDevice;
	extern IFluid* fluid;
	GenericDevice* genDevice = dynamic_cast<GenericDevice*>(hapticDevice);
	cGenericHapticDevice* haptic = genDevice->GetChaiDevice();

	cVector3d cursorPosition;
	hapticDevice->GetCursorPosition(cursorPosition);
	cVector3d linearVelocity;
	hapticDevice->GetCursorVelocity(linearVelocity);
	// Don't do anything if cursor is moving too slowly
	if (linearVelocity.length() < FALCON_MIN_CURSOR_SPEED) {
		// The device continuously renders the last set force; this will clear that
		cVector3d emptyForce(0, 0, 0);
		haptic->setForce(emptyForce);
		return;
	}

	cVector3d normLinVelocity(linearVelocity);
	normLinVelocity.normalize();
	
	cVector3d fluidVelocity;
	fluid->GetVelocityAt(fluidVelocity, cursorPosition);
	cVector3d normFluidVelocity(fluidVelocity);
	normFluidVelocity.normalize();

	double dotProduct = normLinVelocity.dot(normFluidVelocity);
	if (dotProduct < 0) {dotProduct = 0;}
	//dotProduct = (dotProduct + 1) / 2.0;	// Make the range [0, 1] instead of [-1, 1]

	double fluidForcePercent = fluidVelocity.length() / fluid->GetMaxParticleSpeed();
	if (fluidForcePercent > 1.0) { fluidForcePercent = 1.0; }
	double pushForcePercent = linearVelocity.length() / DIR_VISC_MAX_SPEED;
	if (pushForcePercent > 1.0) { pushForcePercent = 1.0; }

	double hapticForcePercent = -1 * fluidForcePercent * pushForcePercent * dotProduct;
	normLinVelocity.mul(hapticForcePercent * hapticDevice->GetMaxForce());

	FalconDevice::ConvertToDeviceAxes(normLinVelocity);
	haptic->setForce(normLinVelocity);
}

IHapticMode* DirectionalViscositySenseMode::GetSingleton(void){
	if (! singleton) {
		singleton = new DirectionalViscositySenseMode();
	}
	return singleton;
}