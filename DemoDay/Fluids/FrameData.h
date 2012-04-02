#pragma once

#include <map>
#include <set>
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

	// Finds all IDs that have a sort value between startVal and endVal
	std::set<int> GetIDsInRange(const GEOParticleSortData* list, const double startVal, const double endVal);

public:
	FrameData(int totalPartCount, int livePartCount, map<int, GEOParticle*> masterList);
	// THIS SHOULD ONLY BE USED TO ALLOCATE ARRAYS/VECTORS
	FrameData(void);
	virtual ~FrameData(void);

	// Returns a list of particle IDs for particles that are in the specified neighborhood
	std::set<int> GetIDsInNeighborhood(const double neighborhoodSize, const cVector3d& center);
	// Returns the particle with the given ID
	GEOParticle* GetParticleByID(int partId);
	// Returns the total number of particles that are alive
	int GetTotalActiveParticles(void);
};

