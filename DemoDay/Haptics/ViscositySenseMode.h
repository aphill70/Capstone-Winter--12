#pragma once

#include "IHapticMode.h"
#include "GenericDevice.h"

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