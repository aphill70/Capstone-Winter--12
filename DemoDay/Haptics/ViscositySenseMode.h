#pragma once

#include "IHapticMode.h"

class ViscositySenseMode : public IHapticMode {
private:
	static IHapticMode* singleton;

	double maxViscosity;
	double standardViscosity;
	double minViscosity;
public:
	virtual void Tick(void);

	static IHapticMode* GetSingleton(void);
};

IHapticMode* ViscositySenseMode::singleton = 0;