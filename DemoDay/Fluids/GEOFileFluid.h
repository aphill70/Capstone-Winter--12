#pragma once

#include <vector>

#include "chai3d.h"

#include "IFluid.h"
#include "IFluidParticle.h"

class GEOFileFluid : public IFluid {
public:
	// Finds all the points in the fluid simulation and copies them into the list 'destination'
	virtual void GetAllPoints(std::vector<IFluidParticle*>& destination) = 0;
	// Returns the number of particles in the simulation
	virtual int GetPointCount(void) = 0;
	// Finds the velocity of the fluid at the given point and copies it into 'velocity'
	virtual void GetVelocityAt(cVector3d& velocity, const cVector3d& location) = 0;
	// Returns the fastest possible speed of a particle
	virtual double GetMaxParticleSpeed(void) = 0;
};