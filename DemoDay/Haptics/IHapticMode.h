#pragma once

#include "IHapticDevice.h"

class IHapticDevice;

// All classes that implement this interface should be implemented as singletons.
class IHapticMode {
public:
	// Called every time the haptic device needs to render a force or update variables.
	virtual void Tick(void) = 0;
};