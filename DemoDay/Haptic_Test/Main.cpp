#include "FalconDevice.h"
#include "IHapticMode.h"
#include "ViscositySenseMode.h"

int main (void) {
	FalconDevice falcon;
	falcon.Init();

	IHapticMode* mode = ViscositySenseMode::GetSingleton();
	falcon.SetMode(mode);
	while (true) {
		mode->Tick();
	}

	return 0;
}