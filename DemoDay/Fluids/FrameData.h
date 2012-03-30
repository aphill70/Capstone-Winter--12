#pragma once

#include <vector>

#include "GEOParticle.h"

struct GEOParticleSortData {
	int id;
	double sortValue;
};

// Compares position coordinates
int comparePartSortData(const void* first, const void* second);

class FrameData {
private:
	// The total number of particles in the simulation
	int _totalPartCount;
	// The number of particles that are active.
	int _livePartCount;
	// A master list of all of the particles that appear in this frame.
	// The array index is the same as the particle ID.
	vector<GEOParticle*> particleList;

	// These arrays hold the IDs of the particles, sorted according to position.
	GEOParticleSortData* xSortedPartIDs;
	GEOParticleSortData* ySortedPartIDs;
	GEOParticleSortData* zSortedPartIDs;

public:

	FrameData(int totalPartCount, int livePartCount, GEOParticle** masterList);
	// THIS SHOULD ONLY BE USED TO ALLOCATE ARRAYS/VECTORS
	FrameData(void);
	virtual ~FrameData(void);

	// Returns a list of particle IDs for particles that are in the specified range
//	vector<int> GetIDsInRange(const double neighborhoodSize, const cVector3d& center);
	// Returns the particle with the given ID
	GEOParticle* GetParticleByID(int partId);
};

