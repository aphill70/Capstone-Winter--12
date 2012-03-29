#include "FrameData.h"

FrameData::FrameData(int totalPartCount, int livePartCount, GEOParticle** masterList) : _livePartCount(livePartCount) {
	particleList = new GEOParticle*[totalPartCount];
	memcpy(particleList, masterList, totalPartCount);
	
	// Create sorted lists
	int* livePartIds = new int[livePartCount];
}

FrameData::~FrameData(void) {
	delete[] particleList;
}

int FrameData::compareLiveParticles(const void* first, const void* second) {
	return first < second;
}

int FrameData::compareXPosition(const void* first, const void* second) {
	GEOParticle* firstPart = (GEOParticle*)first;
	GEOParticle* secondPart = (GEOParticle*)second;
	return firstPart->GetPositionX() < secondPart->GetPositionX();
}

int FrameData::compareYPosition(const void* first, const void* second) {
	GEOParticle* firstPart = (GEOParticle*)first;
	GEOParticle* secondPart = (GEOParticle*)second;
	return firstPart->GetPositionY() < secondPart->GetPositionY();
}

int FrameData::compareZPosition(const void* first, const void* second) {
	GEOParticle* firstPart = (GEOParticle*)first;
	GEOParticle* secondPart = (GEOParticle*)second;
	return firstPart->GetPositionZ() < secondPart->GetPositionZ();
}



//vector<int> FrameData::GetIDsInRange(const double neighborhoodSize, const cVector3d& center) {
//}

GEOParticle* FrameData::GetParticleByID(int partId) {
	return particleList[partId];
}