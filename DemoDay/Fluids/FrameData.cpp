#include <assert.h>
#include <math.h>

#include "FrameData.h"

int comparePartSortData(const void* first, const void* second) {
	GEOParticleSortData firstPart = *(GEOParticleSortData*) first;
	GEOParticleSortData secondPart = *(GEOParticleSortData*) second;
	return firstPart.sortValue < secondPart.sortValue;
}



FrameData::FrameData(int totalPartCount, int livePartCount, map<int, GEOParticle*> masterList) :
	_totalPartCount(totalPartCount), _livePartCount(livePartCount), 
	particleList(totalPartCount + 1, 0)	// IDs are 1-indexed, not 0-indexed
{
	// Create sorted lists
	xSortedPartIDs = new GEOParticleSortData[livePartCount];
	ySortedPartIDs = new GEOParticleSortData[livePartCount];
	zSortedPartIDs = new GEOParticleSortData[livePartCount];
	memset(xSortedPartIDs, 0, livePartCount * sizeof(GEOParticleSortData));
	memset(ySortedPartIDs, 0, livePartCount * sizeof(GEOParticleSortData));
	memset(zSortedPartIDs, 0, livePartCount * sizeof(GEOParticleSortData));

	int index = 0;
	int liveParticleIndex = 0;
	for ( map<int, GEOParticle*>::iterator it = masterList.begin(); it != masterList.end(); it++, liveParticleIndex++ ) {
		GEOParticle* curParticle = (*it).second;
		assert(curParticle);
		
		int curID = curParticle->GetID();
		while (index < curID) {	// Fill empty spots with null pointers
			index++;
		}

		particleList[curID] = curParticle;
		xSortedPartIDs[liveParticleIndex].id = curID;
		xSortedPartIDs[liveParticleIndex].sortValue = curParticle->GetPositionX();
		ySortedPartIDs[liveParticleIndex].id = curID;
		ySortedPartIDs[liveParticleIndex].sortValue = curParticle->GetPositionY();
		zSortedPartIDs[liveParticleIndex].id = curID;
		zSortedPartIDs[liveParticleIndex].sortValue = curParticle->GetPositionZ();
	}
	while (index < _totalPartCount) {	// Fill empty spots with null pointers
		particleList[index] = 0;
		index++;
	}

	qsort(xSortedPartIDs, livePartCount, sizeof(GEOParticleSortData), comparePartSortData);
	qsort(ySortedPartIDs, livePartCount, sizeof(GEOParticleSortData), comparePartSortData);
	qsort(zSortedPartIDs, livePartCount, sizeof(GEOParticleSortData), comparePartSortData);
}

FrameData::FrameData(void) : _totalPartCount(-1), _livePartCount(-1) {
	
}

FrameData::~FrameData(void) {
	delete[] xSortedPartIDs;
	delete[] ySortedPartIDs;
	delete[] zSortedPartIDs;

	for ( vector<GEOParticle*>::iterator it = particleList.begin(); it != particleList.end(); it++ ) {
		delete *it;
	}
}

std::set<int> FrameData::GetIDsInRange(const GEOParticleSortData* list, const double lowerBound, const double upperBound) {
	// Find the lower bound
	int start = 0; 
	int end = _livePartCount;

	while (end - start != 1) {
		int mid = (int)ceil(end - start / 2.0);
		if (list[mid].sortValue >= lowerBound) {
			start = mid;
			continue;
		} else if (list[mid].sortValue <= lowerBound) {
			end = mid;
			continue;
		}

		printf("Nothing happened\n");
	}

	int firstIDIndex = list[start].sortValue >= lowerBound ? start : end;

	// Find the upper bound
	start = 0; 
	end = _livePartCount;

	while (end - start != 1) {
		int mid = (int)ceil(end - start / 2.0);
		if (list[mid].sortValue >= upperBound) {
			end = mid;
			continue;
		} else if (list[mid].sortValue <= upperBound) {
			start = mid;
			continue;
		}

		printf("Nothing happened\n");
	}

	int lastIDIndex = list[end].sortValue <= upperBound ? end : start; 

	std::set<int> inRangeIds;
	for(int i = firstIDIndex; i <= lastIDIndex; i++) {
		inRangeIds.insert(list[i].id);
	}
	return  inRangeIds;
}

std::set<int> FrameData::GetIDsInNeighborhood(const double neighborhoodSize, const cVector3d& center) {
	std::set<int> validIds = GetIDsInRange(xSortedPartIDs, center.x - neighborhoodSize, center.x + neighborhoodSize);
	
	std::set<int> validIdCopy(validIds);
	std::set<int> yIDs = GetIDsInRange(ySortedPartIDs, center.y - neighborhoodSize, center.y + neighborhoodSize);
	for (std::set<int>::iterator itY = yIDs.begin(); itY != yIDs.end(); itY++) {
		for (std::set<int>::iterator itValid = validIdCopy.begin(); itValid != validIdCopy.end(); itValid++) {
			int currentId = (*itY);
			if ((*itValid) != currentId) {
				validIds.insert(currentId);
			}
		}
	}
	
	validIdCopy = std::set<int>(validIds);
	std::set<int> zIDs = GetIDsInRange(zSortedPartIDs, center.z - neighborhoodSize, center.z + neighborhoodSize);
	for (std::set<int>::iterator itZ = zIDs.begin(); itZ != zIDs.end(); itZ++) {
		for (std::set<int>::iterator itValid = validIdCopy.begin(); itValid != validIdCopy.end(); itValid++) {
			int currentId = (*itZ);
			if ((*itValid) != currentId) {
				validIds.insert(currentId);
			}
		}
	}

	return validIds;
}

GEOParticle* FrameData::GetParticleByID(int partId) {
	return particleList[partId];
}

