#include "Globals.h"
#include <stdlib.h>

const char* windowName = "Basic Movement";

int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;

// Haptic Globals
cGenericHapticDevice* hapticDevice;
double MAX_HAPTIC_FORCE = 0.0;	// Should be set in Main.InitHapticDevice()
double HAPTIC_RADIUS = 0.0;		// Should be set in Main.InitHapticDevice()

void ExitProgram(int status) {
	if (hapticDevice) {
		hapticDevice->close();
	}

	exit(status);
}