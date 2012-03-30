#pragma once

#include <vector>

#include "chai3d.h"

#include "IFluidParticle.h"

#define MAX_NUMBER_OF_PARTICLES 1000

class IFluid {

public:
	virtual ~IFluid(void) {}

	// Finds all the points in the fluid simulation and copies them into the list 'allParticles'
	virtual void GetAllPoints(vector<IFluidParticle*>& allParticles) = 0;
	// Returns the number of particles in the simulation
	virtual int GetPointCount(void) = 0;
	// Finds the velocity of the fluid at the given point and copies it into 'velocity'
	virtual void GetVelocityAt(cVector3d& velocity, const cVector3d& location) = 0;
	// Returns the fastest possible speed of a particle
	virtual double GetMaxParticleSpeed(void) = 0;
	// Advances the fluid simulation one frame
	virtual void AdvanceFrame(void) = 0;
	// Max number of particles beign simulated at any given time
	virtual int GetMaxSimulatedParticles() = 0;
};