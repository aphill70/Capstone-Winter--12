#pragma once

#include "IHapticDevice.h"
#include "IHapticMode.h"
#include "chai3d.h"

/*
 * Implements Init() with basic Chai setup code.
 */
class GenericDevice : public IHapticDevice {
protected:
	cGenericHapticDevice* chaiDevice;

	void CenterHapticDevice(void);
public:
	virtual void Init(void);

	virtual double GetMaxForce(void);
	virtual double GetHapticRadius(void);
	cGenericHapticDevice* GetChaiDevice(void);
};