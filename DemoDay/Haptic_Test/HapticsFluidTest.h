#pragma once
#include <vector>
#include "chai3d.h"
#include "IFluid.h"
#include "IFluidParticle.h"
#include "HapticTestRunner.h"
#include "ConstRunner.h"
#include "OscillRunner.h"
#include "ITerrain.h"


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
	void GetAllPoints(std::vector<IFluidParticle*>&) {}
	double GetMaxParticleSpeed(void);
	virtual void AdvanceFrame(void) {};
	int GetPointCount(void) { return 0; };
private:
	HapticTestRunner * testRunner;
};