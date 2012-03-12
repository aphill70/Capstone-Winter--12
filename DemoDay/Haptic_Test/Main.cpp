#include "IHapticDevice.h"
#include "FalconDevice.h"
#include "HapticsFluidTest.h"
#include "chai3d.h"

int main (void) {
	cVector3d initial(1.0,1.0,1.0);
	IFluid * driver = new HapticsFluidTest(HapticsFluidTest::CONSTANT_VELOCITY, initial);
	//IHapticDevice* device = new FalconDevice();

	return 0;
}