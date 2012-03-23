#pragma once
#include "chai3d.h"
#include "IFluid.h"
#include "HapticTestRunner.h"
#include "ConstRunner.h"
#include "ITerrain.h"
#include <vector>

#define TEST_FLUID_MAX_SPEED 0.05

class HapticsFluidTest : public IFluid {
public:
	enum e_TestMode {
		CONSTANT_VELOCITY,
		X_AXIS_OSCILLATING,
		Y_AXIS_OSCILLATING,
		Z_AXIS_OSCILLATING,
		XY_PLANE_OSCILLATING,
		YZ_PLANE_OSCILLATING,
		XZ_PLANE_OSCILLATING,
	};
	HapticsFluidTest(const e_TestMode mode, const cVector3d initial);
	~HapticsFluidTest();
	void GetVelocityAt(cVector3d& velocity, const cVector3d& location);
	void GetAllPoints(std::vector<cVector3d>&) {}
	void SetTerrain(ITerrain*) {}
	double GetMaxParticleSpeed(void);
private:
	HapticTestRunner * testRunner;
};