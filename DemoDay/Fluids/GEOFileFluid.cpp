#include "GEOFileFluid.h"

GEOFileFluid::GEOFileFluid(const string& baseFileName, int startFrame, int endFrame) : 
	frameCount(endFrame - startFrame), currentFrame(0)
{
}
GEOFileFluid::~GEOFileFluid(void) {
	// delete[] frames;
}

void GEOFileFluid::GetAllPoints(std::vector<IFluidParticle*>& destination) {
}

int GEOFileFluid::GetPointCount(void) {
	//TODO: fix when FrameData is created
	return -1;
}

void GEOFileFluid::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
}

double GEOFileFluid::GetMaxParticleSpeed(void) {
	//TODO: fix when FrameData is created
	return -1;
}

void GEOFileFluid::AdvanceFrame(void) { currentFrame++; }