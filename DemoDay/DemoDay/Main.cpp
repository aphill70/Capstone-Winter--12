#include "IHapticDevice.h"
#include "FalconDevice.h"

int main(void) {
	FalconDevice falconDevice;
	IHapticDevice* device = &falconDevice;
	device->Init();
	return 0;
}