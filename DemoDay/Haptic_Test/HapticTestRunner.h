#pragma once
#include "chai3d.h"

class HapticTestRunner {
public:
	//
	//
	//
	virtual void ComputeVelocity(cVector3d& velocity, const cVector3d& location) = 0;
};