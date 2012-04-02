#include "GEOFileFluid.h"
#include "FrameDataParser.h"

GEOFileFluid::GEOFileFluid(const string& baseFileName, int startFrame, int endFrame) : 
	frameCount(endFrame - startFrame + 1), currentFrame(0), maxSimParticles(0)
{
	frames = new FrameData*[frameCount];
	for (int i = startFrame; i <= endFrame; i++) {
		char intConvert[21];
		sprintf(intConvert, "%03d", i);
		FrameData* currentFrame = FrameDataParse::ParseFrame(baseFileName + intConvert + ".geo");
		frames[i - startFrame] = currentFrame;
	
		int curActiveParticles = currentFrame->GetTotalActiveParticles();
		if (maxSimParticles < curActiveParticles) { 
			maxSimParticles = curActiveParticles; 
		}
	}

	
}
GEOFileFluid::~GEOFileFluid(void) {
	for (int i = 0; i < frameCount; i++) {
		delete frames[i];
	}

	delete[] frames;
}

void GEOFileFluid::GetAllPoints(std::vector<IFluidParticle*>& destination) {
	//frames[currentFrame]->;
}

int GEOFileFluid::GetPointCount(void) {
	return frames[currentFrame]->GetTotalActiveParticles();
}

void GEOFileFluid::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	FrameData* currentData = frames[currentFrame];
	std::set<int> ids = currentData->GetIDsInNeighborhood(NEIGHBORHOOD_SIZE, location);
	GEOParticle* currentParticle;
	double averageSum;
	for (std::set<int>::iterator partIter = ids.begin(); partIter != ids.end(); partIter++) {
		currentParticle = currentData->GetParticleByID(*partIter);
		cVector3d currentVelocity, currentPosition;
		currentParticle->GetPosition(currentPosition);
		currentPosition -= location;
		currentParticle->GetVelocity(currentVelocity);

		double weight = 1.0 / pow(currentPosition.length(), 3);

	}
}

double GEOFileFluid::GetMaxParticleSpeed(void) {
	//TODO: Decide if it's worth looking through the data and calculating the actual fastest speed that a particle achieves
	return 20;
}

int GEOFileFluid::GetMaxSimulatedParticles() {
	return maxSimParticles;
}

void GEOFileFluid::AdvanceFrame(void) { currentFrame++; }