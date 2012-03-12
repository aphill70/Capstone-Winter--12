#pragma once
#include "chai3d.h"
#include "ConstantHapticTestRunner.h"

class ConstantHapticTestRunner : public HapticTestRunner {
public:
	ConstantHapticTestRunner(cVector3d velocity); 
	void ComputeVelocity(cVector3d& velocity, const cVector3d& location);
private:
	cVector3d velocity;
}