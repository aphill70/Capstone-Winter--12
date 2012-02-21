#pragma once
#include "chai3d.h"
#undef _ENABLE_CUSTOM_DEVICE_SUPPORT
#undef _ENABLE_DELTA_DEVICE_SUPPORT
#define _ENABLE_FALCON_DEVICE_SUPPORT
#undef _ENABLE_MPB_DEVICE_SUPPORT
#undef _ENABLE_PHANTOM_DEVICE_SUPPORT
#undef _ENABLE_SENSORAY626_DEVICE_SUPPORT
#undef _ENABLE_SERVOTOGO_DEVICE_SUPPORT
#undef _ENABLE_VIRTUAL_DEVICE_SUPPORT

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define PI 3.141592653
#define FRAME_INTERVAL (1000.0/30.0) // Millis per frame

extern const char* windowName;

extern int windowWidth;
extern int windowHeight;

// A pointer to the haptic device
extern cGenericHapticDevice* hapticDevice;
extern double MAX_HAPTIC_FORCE;
extern double HAPTIC_RADIUS;

// Provides a common exit point for the program.
// This will free resources (like the haptic device handle)
void ExitProgram(int);
