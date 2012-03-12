#include "ConstRunner.h"
ConstantHapticTestRunner::ConstantHapticTestRunner(cVector3d velocity) : _velocity(velocity) {}

void ConstantHapticTestRunner::ComputeVelocity(cVector3d& velocity, const cVector3d& location) {
	velocity = _velocity;
}