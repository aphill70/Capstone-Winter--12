#pragma once

#include <vector>

#include "GEOParticle.h"

class FrameData {
private:
	// The number of particles that are active.
	int _livePartCount;
	// A master list of all of the particles that appear in this frame.
	// The array index is the same as the particle ID.
	GEOParticle** particleList;

	// These arrays hold the IDs of the particles, sorted according to position.
	int* xSortedPartIDs;
	int* ySortedPartIDs;
	int* zSortedPartIDs;

	//---- These comparator functions are used to sort the particle lists ----
	// Puts live particles in front of dead ones
	int compareLiveParticles(const void* first, const void* second);
	// Compares position coordinates
	int compareXPosition(const void* first, const void* second);
	int compareYPosition(const void* first, const void* second);
	int compareZPosition(const void* first, const void* second);
public:
	FrameData(int totalPartCount, int livePartCount, GEOParticle** masterList);
	virtual ~FrameData(void);

	// Returns a list of particle IDs for particles that are in the specified range
//	vector<int> GetIDsInRange(const double neighborhoodSize, const cVector3d& center);
	// Returns the particle with the given ID
	GEOParticle* GetParticleByID(int partId);
};