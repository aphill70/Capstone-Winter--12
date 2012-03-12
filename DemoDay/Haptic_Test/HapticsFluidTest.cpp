#include "HapticsFluidTest.h"


HapticsFluidTest::HapticsFluidTest(const e_TestMode mode, const cVector3d initial) {
	switch(mode) {
		case CONSTANT_VELOCITY:
		default:
			testRunner = new ConstantHapticTestRunner(initial);
	}
}

void HapticsFluidTest::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	testRunner->ComputeVelocity(velocity, location);	
}