#pragma once 

#include "chai3d.h"
#include "GenericDevice.h"

class VirtualHapticDevice : public GenericDevice {
protected:
	cVector3d lastPosition;
	cPrecisionClock clock;
	double lastClock;

	virtual void CenterHapticDevice(void);
public:
	VirtualHapticDevice(void);
	~VirtualHapticDevice(void);

	// Overridden from GenericDevice; the chai virtual device doesn't implement
	//  getLinearVelocity(cVector3d), so this is a software implementation of
	//  linear velocity
	virtual void GetCursorVelocity(cVector3d& velocity);
};