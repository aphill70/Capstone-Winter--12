#pragma once

#include <vector>
#include <string>

#include "chai3d.h"

#include "FrameData.h"
#include "SPHFluid.h"
#include "IFluidParticle.h"

class GEOFileFluid : public SPHFluid {
protected:
	vector<FrameData> frames;
	int frameCount;
	int currentFrame;

public:
	// Creates a GEOFileFluid by loading in a set of .geo files with the 
	//  file in the format '[baseFileName][FRAME #].geo'.
	// Only files with a frame suffix in the range of [startFrame, endFrame]
	//  will be loaded.
	GEOFileFluid(const string& baseFileName, int startFrame, int endFrame);
	virtual ~GEOFileFluid(void);

	// Finds all the points in the fluid simulation and copies them into the list 'destination'
	virtual void GetAllPoints(std::vector<IFluidParticle*>& destination);
	// Returns the number of particles in the simulation
	virtual int GetPointCount(void);
	// Finds the velocity of the fluid at the given point and copies it into 'velocity'
	virtual void GetVelocityAt(cVector3d& velocity, const cVector3d& location);
	// Returns the fastest possible speed of a particle
	virtual double GetMaxParticleSpeed(void);

	// Advances the fluid simulation one frame
	virtual void AdvanceFrame(void);
};