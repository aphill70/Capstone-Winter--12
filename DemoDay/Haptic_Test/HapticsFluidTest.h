#pragma once
#include "chai3d.h"
#include "IFluid.h"
#include "HapticTestRunner.h"
#include "ConstRunner.h"
#include "ITerrain.h"
#include <vector>

class HapticsFluidTest : public IFluid {
public:
	enum e_TestMode {
		CONSTANT_VELOCITY
	};
	HapticsFluidTest(const e_TestMode mode, const cVector3d initial);
	void GetVelocityAt(cVector3d& velocity, const cVector3d& location);
	void GetAllPoints(std::vector<cVector3d>&) {}
	void SetTerrain(ITerrain*) {}
private:
	HapticTestRunner * testRunner;
};