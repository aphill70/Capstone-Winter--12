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

	virtual void CenterHapticDevice(void);
public:
	virtual void Init(void);

	virtual double GetMaxForce(void);
	virtual double GetHapticRadius(void);
	cGenericHapticDevice* GetChaiDevice(void);

	virtual void GetCursorPosition(cVector3d& destination);
	virtual void GetCursorVelocity(cVector3d& destination);
};