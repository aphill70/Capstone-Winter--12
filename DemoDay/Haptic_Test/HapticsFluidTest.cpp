#include "HapticsFluidTest.h"

//  -- HapticsFluidTest
//  @param mode the test type
//  @param initial the intitial vector value
//  This is essentially checking to see what you have inputted and will create a testrunner
//  for that mode.
HapticsFluidTest::HapticsFluidTest(const e_TestMode mode, const cVector3d initial) {
	switch(mode) {
		case CONSTANT_VELOCITY:
		default:
			testRunner = new ConstantHapticTestRunner(initial);
	}
}

HapticsFluidTest::~HapticsFluidTest() {}

// -- GetVelocityAt
// @param velocity the velocity returned
// @param location the current location of the cursor
// This test should be different based on what mode was selected during initialization.
void HapticsFluidTest::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	testRunner->ComputeVelocity(velocity, location);	
}

double HapticsFluidTest::GetMaxParticleSpeed(void) {
	return TEST_FLUID_MAX_SPEED;
}