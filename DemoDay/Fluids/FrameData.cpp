#include "FrameData.h"

int comparePartSortData(const void* first, const void* second) {
	GEOParticleSortData firstPart = *(GEOParticleSortData*) first;
	GEOParticleSortData secondPart = *(GEOParticleSortData*) second;
	return firstPart.sortValue < secondPart.sortValue;
}



FrameData::FrameData(int totalPartCount, int livePartCount, GEOParticle** masterList) :
	_totalPartCount(totalPartCount), _livePartCount(livePartCount), 
	particleList(masterList, masterList + sizeof(masterList) / sizeof(GEOParticle*))
{
	// Create sorted lists
	xSortedPartIDs = new GEOParticleSortData[livePartCount];
	ySortedPartIDs = new GEOParticleSortData[livePartCount];
	zSortedPartIDs = new GEOParticleSortData[livePartCount];

	int currentIndex = 0;
	
	for (int i = 0; i < totalPartCount; i++) {
		if (particleList[i]) {
			xSortedPartIDs[currentIndex].id = i;
			xSortedPartIDs[currentIndex].sortValue = particleList[i]->GetPositionX();
			ySortedPartIDs[currentIndex].id = i;
			ySortedPartIDs[currentIndex].sortValue = particleList[i]->GetPositionY();
			zSortedPartIDs[currentIndex].id = i;
			zSortedPartIDs[currentIndex].sortValue = particleList[i]->GetPositionZ();
			
			currentIndex++;
		}
	}

	qsort(xSortedPartIDs, livePartCount, sizeof(int), comparePartSortData);
	qsort(ySortedPartIDs, livePartCount, sizeof(int), comparePartSortData);
	qsort(zSortedPartIDs, livePartCount, sizeof(int), comparePartSortData);
}

FrameData::FrameData(void) {

}

FrameData::~FrameData(void) {}


//vector<int> FrameData::GetIDsInRange(const double neighborhoodSize, const cVector3d& center) {
//}

GEOParticle* FrameData::GetParticleByID(int partId) {
	return particleList[partId];
}