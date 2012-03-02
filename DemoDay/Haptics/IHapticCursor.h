#pragma once
#include "chai3d.h"

class IHapticCursor {
public:
	/*
	 * Takes the current position of the haptic cursor and copies it into destination
	 */
	virtual void GetPosition(cVector3d& destination) = 0;
	
	/*
	 * Takes the current linear velocity of the haptic cursor and copies it into destination
	 */
	virtual void GetVelocity(cVector3d& destination) = 0;
}