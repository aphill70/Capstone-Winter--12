#include "ConstRunner.h"

// -- ConstantHapticTestRunner
// @param velocity the constant velocity
// ConstantHapticTestRunner will continously produce the same velocity that was given at
// the beginning.
ConstantHapticTestRunner::ConstantHapticTestRunner(cVector3d velocity) : _velocity(velocity) {}

// -- ComputeVelocity
// @param velocity the velocity to be computed
// @param location the pointer location. This isn't used for ConstantHapticTestRunner
// ConstantHapticTestRunner will set the value of velocity to what was given at the beginning
void ConstantHapticTestRunner::ComputeVelocity(cVector3d& velocity, const cVector3d& location) {
	velocity = _velocity;
}
	