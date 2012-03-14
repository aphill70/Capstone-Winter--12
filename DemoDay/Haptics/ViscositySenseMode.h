#pragma once

#include "IHapticMode.h"
#include "GenericDevice.h"

#define MAX_SPEED 0.4

class ViscositySenseMode : public IHapticMode {
private:
	static IHapticMode* singleton;

	double maxViscosity;
	double standardViscosity;
	double minViscosity;
	
	ViscositySenseMode(void);
	~ViscositySenseMode(void);
public:
	virtual void Tick(void);
	void SetDevice(IHapticDevice* newDevice);

	static IHapticMode* GetSingleton(void);
};