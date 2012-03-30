#include "OscillRunner.h"

// constants
const double OscillatingHapticTestRunner::TIMEOUT_PERIOD = 5.0; // timeout for clock
const double OscillatingHapticTestRunner::INCREMENT_AMOUNT = 0.5; // increment for oscAmount

// -- OscillatingHapticTestRunner
// @param initialVector the initial vector
// @param type the type of oscillation to be performed
//	
// Oscillates with a constant, polarized, timed motion specified axis.
// Can be any of the following modes:
//      X_AXIS
//		Y_AXIS
//      Z_AXIS
//		XY_PLANE
//		YZ_PLANE
//		XZ_PLANE
OscillatingHapticTestRunner::OscillatingHapticTestRunner(const cVector3d& initialVector, 
														 e_OscillationType type) :	_initial(initialVector),
																					_type(type), polarity(true),
																					oscAmount(0.0), clock()		
{
	clock.setTimeoutPeriodSeconds(TIMEOUT_PERIOD);
	clock.start();
}

// -- ComputeVelocity
// @param velocity the destination velocity
// @param location the location of the haptic device (not used)
//
// Computes the oscillation amount and performs a scalar multiplation 
//   on the specified axes.
void OscillatingHapticTestRunner::ComputeVelocity(cVector3d& velocity, 
												  const cVector3d& location)  {
	
	SetOscillation();

	velocity = _initial;
	switch (_type) {
		case XY_PLANE:
			velocity.y *= oscAmount;
		case X_AXIS:
			velocity.x *= oscAmount;
			break;
		case YZ_PLANE:
			velocity.z *= oscAmount;
		case Y_AXIS:
			velocity.y *= oscAmount;
			break;
		case XZ_PLANE:
			velocity.x *= oscAmount;
		case Z_AXIS:
			velocity.z *= oscAmount;
			break;
		default:
			velocity.mul(oscAmount);
	}
}

// -- SetOscillation
// Checks for timeout and sets the oscillation amount 
// based on INCREMENT_AMOUNT and polarity. Sets polarity
// when finished.
void OscillatingHapticTestRunner::SetOscillation() {
	if (clock.timeoutOccurred()) {
		clock.reset();
		if (polarity == true) {
			oscAmount += INCREMENT_AMOUNT;
			printf("switch+\n");
		} else {
			oscAmount -= INCREMENT_AMOUNT;
			printf("switch-\n");
		}
		SetPolarity();
	}
}

// -- SetPolarity
// Sets the polarity flag if oscAmount reaches
// 1 or -1
void OscillatingHapticTestRunner::SetPolarity()
{
	if (oscAmount >= 1.0)
		polarity = false;
	else if (oscAmount <= 1.0)
		polarity = true;		
}

void OscillatingHapticTestRunner::RunnerLoop() {
}