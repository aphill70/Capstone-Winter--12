#include "chai3d.h"

#include "FalconDevice.h"
#include "HapticsFluidTest.h"
#include "IHapticMode.h"
#include "UniformViscositySenseMode.h"
#include "DirectionalViscositySenseMode.h"
#include "VirtualHapticDevice.h"

cHapticDeviceHandler handler;

// Global functions
// TODO: See if you can wrap this in a class or namespace
IFluid* fluid = 0;
ITerrain* terrain = 0;
IHapticDevice* hapticDevice = 0;

// function prototypes for main
int uniformViscosity (void);
int directionalViscosity(void);
int nathanMain(void);
int virtualTest(void);

int	main(void) {
	int fncToRun = 3;

	switch (fncToRun) {
		case 0:
			return uniformViscosity();
		case 1:
			return directionalViscosity();
		case 2:
			return nathanMain();
		case 3:
			return virtualTest();
	}
}

int uniformViscosity (void) {
	FalconDevice falcon;
	falcon.Init();
	hapticDevice = &falcon;

	IHapticMode* mode = UniformViscositySenseMode::GetSingleton();
	falcon.SetMode(mode);

	while (true) {
		mode->Tick();
	}

	return 0;
}

int directionalViscosity (void) {
	FalconDevice falcon;
	falcon.Init();
	hapticDevice = &falcon;

	IHapticMode* mode = DirectionalViscositySenseMode::GetSingleton();
	falcon.SetMode(mode);

	cVector3d initial(1.0, 0, 0);
	HapticsFluidTest oneDirecionFluid(HapticsFluidTest::CONSTANT_VELOCITY, initial);
	fluid = &oneDirecionFluid;

	while (true) {
		mode->Tick();
	}

	return 0;
}

int nathanMain(void) {
	FalconDevice falcon;
	falcon.Init();

	IHapticMode* mode = UniformViscositySenseMode::GetSingleton();
	falcon.SetMode(mode);

	cVector3d initial(0.5, 0.1, 0.2);
	fluid = new HapticsFluidTest(HapticsFluidTest::CONSTANT_VELOCITY, initial);
	

	while (true) {
		mode->Tick();
	}
	return 0;
}

int virtualTest(void) {
	VirtualHapticDevice virtDevice;
	virtDevice.Init();
	hapticDevice = &virtDevice;
	
	IHapticMode* mode = DirectionalViscositySenseMode::GetSingleton();
	virtDevice.SetMode(mode);

	cVector3d initial(1.0, 0, 0);
	HapticsFluidTest oneDirecionFluid(HapticsFluidTest::CONSTANT_VELOCITY, initial);
	fluid = &oneDirecionFluid;

	while (true) {
		mode->Tick();
	}

	return 0;
}