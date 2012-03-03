#pragma once

class IHapticMode {
public:
	/*
	 * Called every time the haptic device needs to render a force or update variables.
	 */
	virtual void Tick(void) = 0;
}