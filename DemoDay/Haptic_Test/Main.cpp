#include "IHapticDevice.h"
#include "FalconDevice.h"

int main (void) {

	IHapticDevice* device = new FalconDevice();

	return 0;
}