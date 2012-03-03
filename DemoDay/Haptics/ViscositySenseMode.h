#pragma once

#include "IHapticMode.h"

class ViscositySenseMode : public IHapticMode {
private:
	double maxViscosity;
	double standardViscosity;
	double minViscosity;
public:
	virtual void Tick(void);
}