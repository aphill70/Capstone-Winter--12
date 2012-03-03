#pragma once
#include "chai3d.h"

class IHapticDevice {
public:
	/*
	 * Takes the current position of the haptic cursor and copies it into destination
	 */
	virtual void GetCursorPosition(cVector3d& destination) = 0;
	
	/*
	 * Takes the current linear velocity of the haptic cursor and copies it into destination
	 */
	virtual void GetCursorVelocity(cVector3d& destination) = 0;
}