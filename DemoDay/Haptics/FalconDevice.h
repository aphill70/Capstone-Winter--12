#pragma once

#include "IHapticDevice.h"
#include "IHapticMode.h"
#include "ITerrain.h"
#include "IFluid.h"

class FalconDevice : public IHapticDevice {
private:
	cGenericHapticDevice* device;
	cVector3d cursorPosition;
	cVector3d cursorVelocity;

	ITerrain* terrain;
	IFluid* fluid;

	void CenterHapticDevice(void);
public:
	FalconDevice(void);
	~FalconDevice(void);
	virtual void Init(void);

	void SetTerrain(ITerrain* newTerrain);
	void SetFluid(IFluid* newFluid);

	void GetCursorPosition(cVector3d& destination);
	void GetCursorVelocity(cVector3d& destination);
};