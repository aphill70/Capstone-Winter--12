#include "GEOFileFluid.h"
#include "FrameDataParser.h"

GEOFileFluid::GEOFileFluid(const string& baseFileName, int startFrame, int endFrame) : 
	frameCount(endFrame - startFrame), currentFrame(0)
{
	frames.resize(frameCount);
	for (int i = startFrame; i <= endFrame; i++) {
		char intConvert[21];
		sprintf(intConvert, "%03d", i);
		ParseFrame(baseFileName + intConvert + ".geo");
	}
}
GEOFileFluid::~GEOFileFluid(void) {}

void GEOFileFluid::GetAllPoints(std::vector<IFluidParticle*>& destination) {
}

int GEOFileFluid::GetPointCount(void) {
	//TODO: fix when FrameData is created
	return -1;
}

void GEOFileFluid::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	FrameData currentData = frames[currentFrame];
	//vector<int> ids = currentData.GetIDsInRange(_neighborhoodRadius, location);
	
}

double GEOFileFluid::GetMaxParticleSpeed(void) {
	//TODO: fix when FrameData is created
	return -1;
}

void GEOFileFluid::AdvanceFrame(void) { currentFrame++; }