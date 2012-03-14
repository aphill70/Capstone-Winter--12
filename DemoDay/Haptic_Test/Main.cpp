#include "chai3d.h"

#include "FalconDevice.h"
#include "HapticsFluidTest.h"
#include "IHapticMode.h"
#include "ViscositySenseMode.h"

int bradsMain (void) {
	FalconDevice falcon;
	falcon.Init();

	IHapticMode* mode = ViscositySenseMode::GetSingleton();
	falcon.SetMode(mode);
	while (true) {
		mode->Tick();
	}

	return 0;
}

int nathanMain(void) {
	cVector3d initial;
	IFluid * driver = new HapticsFluidTest(HapticsFluidTest::CONSTANT_VELOCITY, initial);
	return 0;
}

int	main(void) {
	if(true) {
		return bradsMain();
	} else {
		return	nathanMain();
	}
}