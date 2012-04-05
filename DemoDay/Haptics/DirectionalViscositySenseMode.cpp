#include "chai3d.h"
#include "FalconDevice.h"
#include "DirectionalViscositySenseMode.h"

IHapticMode* DirectionalViscositySenseMode::singleton = 0;

DirectionalViscositySenseMode::DirectionalViscositySenseMode(void) : maxViscosity(1.0), minViscosity(0) {} 

DirectionalViscositySenseMode::~DirectionalViscositySenseMode(void) {}

void DirectionalViscositySenseMode::Tick(void) {
	extern IHapticDevice* hapticDevice;
	extern IFluid* fluid;
	cGenericHapticDevice* haptic = (dynamic_cast<GenericDevice*>(hapticDevice))->GetChaiDevice();

	cVector3d cursorPosition;
	hapticDevice->GetCursorPosition(cursorPosition);

	cVector3d linearVelocity;
	hapticDevice->GetCursorVelocity(linearVelocity);
	//
	printf("Speed: %4.4f\n", linearVelocity.length());
	//

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

	// If the direction of the cursor is perpendicular to or going with
	//  the fluid flow, render no force
	double dotProduct = normLinVelocity.dot(normFluidVelocity);
	if (dotProduct < 0) {
		dotProduct = 0;
	}

	double fluidForcePercent = fluidVelocity.length() / fluid->GetMaxParticleSpeed();
	if (fluidForcePercent > 1.0) { fluidForcePercent = 1.0; }
	double pushForcePercent = linearVelocity.length() / DIR_VISC_MAX_SPEED;
	if (pushForcePercent > 1.0) { pushForcePercent = 1.0; }

	double hapticForcePercent = -1 * fluidForcePercent * pushForcePercent * dotProduct;
	normLinVelocity.mul(hapticForcePercent * hapticDevice->GetMaxForce());

	hapticDevice->ConvertToDeviceAxes(normLinVelocity);
	haptic->setForce(normLinVelocity);
}

IHapticMode* DirectionalViscositySenseMode::GetSingleton(void){
	if (! singleton) {
		singleton = new DirectionalViscositySenseMode();
	}
	return singleton;
}