#pragma once

#include "IHapticDevice.h"
#include "GenericDevice.h"
#include "IHapticMode.h"
#include "ITerrain.h"
#include "IFluid.h"

class FalconDevice : public GenericDevice {
private:
	cVector3d cursorPosition;
	cVector3d cursorVelocity;

	ITerrain* terrain;
	IFluid* fluid;
public:
	FalconDevice(void);
	~FalconDevice(void);

	virtual void SetMode(IHapticMode* newMode);

	void SetTerrain(ITerrain* newTerrain);
	void SetFluid(IFluid* newFluid);

	virtual void GetCursorPosition(cVector3d& destination);
	virtual void GetCursorVelocity(cVector3d& destination);

	static void ConvertToDeviceAxes(cVector3d& vector);
	static void ConvertFromDeviceAxes(cVector3d& vector);
};