#pragma once

#include "chai3d.h"

/*
 * This class represents a particle in the fluid
 */
class IFluidParticle {

public:
	// Copies this particle's position into the vector 'position'
	virtual void GetPosition(cVector3d& position) = 0;
	// Copies this particle's velocity into the vector 'velocity'
	virtual void GetVelocity(cVector3d& velocity) = 0;
};