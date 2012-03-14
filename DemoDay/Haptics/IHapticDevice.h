#pragma once

// Idea: create handlers for button presses

#include "chai3d.h"
#include "IHapticMode.h"

class IHapticMode;

class IHapticDevice {
protected:
	double maxForce;
	double hapticRadius;
	
	// Mode determines how the haptic device will react
	IHapticMode* mode;
	
public:
	virtual ~IHapticDevice(void) {}

	// Takes the current position of the haptic cursor and copies it into destination
	virtual void GetCursorPosition(cVector3d& destination) = 0;
	
	// Takes the current linear velocity of the haptic cursor and copies it into destination
	virtual void GetCursorVelocity(cVector3d& destination) = 0;

	// Initializes the haptic device. The device is ready to use once this function returns.
	//TODO: decide if SetTerrain and SetFluid must be called before this function is called.
	//   If not, move this code to the constructor
	virtual void Init(void) = 0;

	// Returns the maximum force that this device can render (in Newtons)
	virtual double GetMaxForce(void) = 0;
	// Returns the recommended workspace radius for this device
	virtual double GetHapticRadius(void) = 0;

	// Changes the haptic mode of the device
	virtual void SetMode(IHapticMode* newMode) {
		mode = newMode;
	}
};