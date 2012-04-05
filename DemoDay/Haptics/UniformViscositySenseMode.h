#pragma once

#include "IHapticMode.h"

#define UNIF_VISC_MAX_SPEED 0.05

/*
 * This class renders a uniform viscous force oppposite the user's force.
 * This viscosity is rendered without respect to any fluid model.
 */
class UniformViscositySenseMode : public IHapticMode {
private:
	static IHapticMode* singleton;

	cVector3d lastPosition;

	UniformViscositySenseMode(void);
	~UniformViscositySenseMode(void);
public:
	virtual void Tick(void);

	static IHapticMode* GetSingleton(void);
};