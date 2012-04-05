#pragma once
#include "chai3d.h"
#include "HapticTestRunner.h"

class ConstantHapticTestRunner : public HapticTestRunner {
public:
	ConstantHapticTestRunner(cVector3d velocity); 
	virtual void ComputeVelocity(cVector3d& velocity, const cVector3d& location);
	void RunnerLoop() {}
private:
	cVector3d _velocity;
};