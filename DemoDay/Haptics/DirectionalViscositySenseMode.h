#pragma once

#include "IHapticMode.h"

#define DIR_VISC_MAX_SPEED 0.05

/*
 * This class renders a viscosity force depending on the user's input force and 
 * the fluid's velocity. If the user pushes against the fluid flow, a strong viscous 
 * force is rendered. If the user pushes with the fluid flow, a weak (or no) force is
 * rendered. If the user pushes perpendicular to the fluid flow, a constant, standard
 * force is rendered.
 */
class DirectionalViscositySenseMode : public IHapticMode {
private:
	static IHapticMode* singleton;

	double maxViscosity;
	double minViscosity;
	
	DirectionalViscositySenseMode(void);
	~DirectionalViscositySenseMode(void);
public:
	virtual void Tick(void);

	static IHapticMode* GetSingleton(void);
};