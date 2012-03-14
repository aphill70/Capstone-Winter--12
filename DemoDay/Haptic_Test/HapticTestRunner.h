#pragma once
#include "chai3d.h"

class HapticTestRunner {
public:
	// -- ComputeVelocity
	// Abstract function to be implemented by child tests
	virtual void ComputeVelocity(cVector3d& velocity, const cVector3d& location) = 0;
};