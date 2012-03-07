#pragma once
#include "chai3d.h"
#include "IFluid.h"

class HapticsFluidTest : public IFluid {
public:
	
	enum e_TestMode = {
		CONSTANT_VELOCITY
	}

	HapticFluidTest();
	HapticFluidTest(e_TestMode mode, cVector3d initial);
	void GetAllPoints(std::vector<cVector3d>& destination);
	void GetVelocityAt(cVector3d& velocity, const cVector3d& location);
private:
	void ProcessRequest(cVector3d& velocity, const cVector3d& location);
	HapticTestRunner testRunner;
}

class ConstantHapticTestRunner : public HapticTestRunner {
public:
	ConstantHapticTestRunner(cVector3d velocity); 
	void ComputeVelocity(cVector3d& velocity, const cVector3d& location);
private:
	cVector3d velocity;
}