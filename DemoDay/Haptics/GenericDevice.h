#pragma once

#include "IHapticDevice.h"
#include "IHapticMode.h"

/*
 * Implements Init() with basic Chai setup code.
 */
class GenericDevice : public IHapticDevice {
	friend class ViscositySenseMode;
protected:
	cHapticDeviceHandler handler;
	cGenericHapticDevice* chaiDevice;

	void CenterHapticDevice(void);
public:
	virtual void Init(void);
};