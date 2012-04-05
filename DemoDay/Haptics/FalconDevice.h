#pragma once

#include "IHapticDevice.h"
#include "GenericDevice.h"
#include "IHapticMode.h"
#include "ITerrain.h"
#include "IFluid.h"

#define FALCON_MIN_CURSOR_SPEED 0.01

class FalconDevice : public GenericDevice {
private:
	cVector3d cursorPosition;
	cVector3d cursorVelocity;
public:
	FalconDevice(void);
	~FalconDevice(void);

	// Overridden from IHapticDevice
	virtual void ConvertToDeviceAxes(cVector3d& vector);
	virtual void ConvertFromDeviceAxes(cVector3d& vector);
};