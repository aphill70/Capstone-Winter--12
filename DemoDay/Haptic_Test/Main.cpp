#include "FalconDevice.h"
#include "HapticsFluidTest.h"
#include "chai3d.h"
#include "IHapticMode.h"
#include "ViscositySenseMode.h"

// function prototypes for main
int bradsMain(void);
int nathanMain(void);

int	main(void) {
	if(true) {
		return bradsMain();
	} else {
		return nathanMain();
	}
}

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
	FalconDevice falcon;
	falcon.Init();

	IHapticMode* mode = ViscositySenseMode::GetSingleton();
	falcon.SetMode(mode);

	cVector3d initial(0.5, 0.1, 0.2);
	IFluid * driver = new HapticsFluidTest(HapticsFluidTest::CONSTANT_VELOCITY, initial);
	
	falcon.SetFluid(driver);

	while (true) {
		mode->Tick();
	}
	return 0;
}