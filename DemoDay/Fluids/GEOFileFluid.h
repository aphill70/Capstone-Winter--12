#pragma once

#include <vector>
#include <string>

#include "chai3d.h"

#include "FrameData.h"
#include "SPHFluid.h"
#include "IFluidParticle.h"

#define NEIGHBORHOOD_SIZE 1.0

class GEOFileFluid : public SPHFluid {
protected:
	FrameData** frames;
	int frameCount;
	int currentFrame;
	int maxSimParticles;

public:
	// Creates a GEOFileFluid by loading in a set of .geo files with the 
	//  file in the format '[baseFileName][FRAME #].geo'.
	// Only files with a frame suffix in the range of [startFrame, endFrame]
	//  will be loaded.
	GEOFileFluid(const string& baseFileName, int startFrame, int endFrame);
	virtual ~GEOFileFluid(void);

	// Finds all the live points in the fluid simulation and copies them into the list 'destination'
	virtual void GetAllPoints(std::vector<IFluidParticle*>& destination);
	// Finds all the points in the fluid simulation and copies them into the list 'destination'
	// If the particle is dead, the vector will contain a null pointer.
	virtual void GetFullPointList(std::vector<IFluidParticle*>& destination);
	// Returns the number of particles in the simulation's current frame
	virtual int GetCurrentPointCount(void);
	// Finds the velocity of the fluid at the given point and copies it into 'velocity'
	virtual void GetVelocityAt(cVector3d& velocity, const cVector3d& location);
	// Returns the fastest possible speed of a particle
	virtual double GetMaxParticleSpeed(void);
	// Max number of particles being simulated at any given time
	virtual int GetMaxSimulatedParticles();
	// Advances the fluid simulation one frame
	virtual void AdvanceFrame(void);
};