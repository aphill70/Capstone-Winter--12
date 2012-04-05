#include "GEOFileFluid.h"
#include "FrameDataParser.h"

GEOFileFluid::GEOFileFluid(const string& baseFileName, int startFrame, int endFrame) : 
	frameCount(endFrame - startFrame + 1), currentFrame(0), maxSimParticles(0)
{
	printf("Loading .geo files\n");

	frames = new FrameData*[frameCount];
	double incrementPercent = 1.0 / frameCount;
	double percent = 0;
	for (int i = startFrame; i <= endFrame; i++) {
		char intConvert[21];
		sprintf(intConvert, "%03d", i);
		FrameData* currentFrame = FrameDataParse::ParseFrame(baseFileName + intConvert + ".geo");
		frames[i - startFrame] = currentFrame;
	
		int curActiveParticles = currentFrame->_livePartCount;
		if (maxSimParticles < curActiveParticles) { 
			maxSimParticles = curActiveParticles; 
		}

		// Just for display
		percent += incrementPercent;
		if (percent > 0.05) {
			printf(".");
			percent = 0;
		}
	}

	printf(".    Done!");

	
}
GEOFileFluid::~GEOFileFluid(void) {
	for (int i = 0; i < frameCount; i++) {
		delete frames[i];
	}

	delete[] frames;
}

void GEOFileFluid::GetFullPointList(std::vector<IFluidParticle*>& destination) {
	destination = std::vector<IFluidParticle*>(frames[currentFrame]->particleList.begin(), frames[currentFrame]->particleList.end());
}

void GEOFileFluid::GetAllPoints(std::vector<IFluidParticle*>& destination) {
	FrameData* frame = frames[currentFrame];
	GEOParticleSortData* liveParticleList = frame->xSortedPartIDs;
	destination = std::vector<IFluidParticle*>(frame->_livePartCount, 0);
	for (int i = 0; i < frame->_livePartCount; i++) {
		int index = liveParticleList[i].id;
		destination[i] = frame->particleList[index];
	}
}

int GEOFileFluid::GetCurrentPointCount(void) {
	return frames[currentFrame]->_livePartCount;
}

void GEOFileFluid::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	printf("asking for velocity\n");
	FrameData* currentData = frames[currentFrame];
	std::set<int> ids = currentData->GetIDsInNeighborhood(NEIGHBORHOOD_SIZE, location);
	GEOParticle* currentParticle;
	cVector3d averageSum;
	for (std::set<int>::iterator partIter = ids.begin(); partIter != ids.end(); partIter++) {
		currentParticle = currentData->GetParticleByID(*partIter);
		cVector3d currentVelocity, currentPosition;
		currentParticle->GetPosition(currentPosition);
		currentPosition -= location;
		currentParticle->GetVelocity(currentVelocity);

		double weight = 1.0 / pow(currentPosition.length(), 3);
		averageSum += weight * currentVelocity;
	}

	averageSum.mul(1.0 / ids.size());
	velocity.copyfrom(averageSum);
}

double GEOFileFluid::GetMaxParticleSpeed(void) {
	//TODO: Decide if it's worth looking through the data and calculating the actual fastest speed that a particle achieves
	return 20;
}

int GEOFileFluid::GetMaxSimulatedParticles() {
	return maxSimParticles;
}

void GEOFileFluid::AdvanceFrame(void) { 
	currentFrame++; 
	currentFrame %= frameCount;
}