#pragma once

#include "chai3d.h"
#include "IHapticMode.h"

class IHapticDevice {
private:
	// Mode determines how the haptic device will react
	IHapticMode* mode;
public:
	// Takes the current position of the haptic cursor and copies it into destination
	virtual void GetCursorPosition(cVector3d& destination) = 0;
	
	// Takes the current linear velocity of the haptic cursor and copies it into destination
	virtual void GetCursorVelocity(cVector3d& destination) = 0;

	// Initializes the haptic device. The device is ready to use once this function returns.
	//TODO: decide if SetTerrain and SetFluid must be called before this function is called.
	virtual void Init(void) = 0;

	// Changes the haptic mode of the device
	virtual void SetMode(IHapticMode* newMode) {
		mode = newMode;
	}
};