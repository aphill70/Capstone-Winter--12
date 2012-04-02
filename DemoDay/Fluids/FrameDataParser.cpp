#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "FrameDataParser.h"

//FrameData ParseFrame(const string& filename) {
FrameData* FrameDataParse::ParseFrame(const std::string& filename) {
	std::ifstream fileReader(filename.c_str());

	std::string frameText;
	std::string currentLine;

	//Header; not needed
	std::getline(fileReader, currentLine);
	// Parse point count
	std::getline(fileReader, currentLine);
	assert(currentLine.find("NPoints") == 0);
	size_t index1 = currentLine.find(" ", strlen("NPoints"));
	size_t index2 = currentLine.find(" ", index1 + 1);
	int frameParticleCount = ParseInt(currentLine, index1, index2);

	// Group data; not needed
	std::getline(fileReader, currentLine);
	assert(currentLine.find("NPointGroups") == 0);
	
	// Parse attribute count
	std::getline(fileReader, currentLine);
	assert(currentLine.find("NPointAttrib") == 0);
	index1 = currentLine.find(" ", strlen("NPointAttrib"));
	index2 = currentLine.find(" ", index1 + 1);	
	int attributeCount = ParseInt(currentLine, index1 + 1, index2);

	// Attributes; not needed
	std::getline(fileReader, currentLine);
	assert(currentLine.compare("PointAttrib") == 0);
	for(int i = 0; i < attributeCount; i++) {
		std::getline(fileReader, currentLine);
	}

	// Parse particles
	map<int, GEOParticle*> particles;
	std::getline(fileReader, currentLine);
	GEOParticle* newParticle;
	for (int i = 0; i < frameParticleCount; i++) {//while(currentLine.compare("PrimitiveAttrib") != 0) {
		newParticle = ParseParticle(currentLine);
		particles[newParticle->GetID()] = newParticle;

		std::getline(fileReader, currentLine);
	}
	int highestIDFound = newParticle->GetID();

	// Ignore the rest of the file's contents
	fileReader.close();

	FrameData* allData = new FrameData(highestIDFound, frameParticleCount, particles);
	return allData;
}

// TODO: this can be made more generic if you care to parse the PointAttrib section
GEOParticle* FrameDataParse::ParseParticle(const std::string& particleStr) {
	int startIndex, spaceIndex;

	// Parse position
	startIndex = 0;
	spaceIndex = particleStr.find(" ");
	double posX = ParseDouble(particleStr, startIndex, spaceIndex);

	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	double posY = ParseDouble(particleStr, startIndex, spaceIndex);

	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	double posZ = ParseDouble(particleStr, startIndex, spaceIndex);

	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	double w = ParseDouble(particleStr, startIndex, spaceIndex);
	posX /= w;
	posY /= w;
	posZ /= w;
	cVector3d position(posX, posY, posZ);

	// Parse velocity
	startIndex = particleStr.find("(", startIndex);
	startIndex++;
	spaceIndex = particleStr.find(" ", startIndex);
	double velocityX = ParseDouble(particleStr, startIndex, spaceIndex);

	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	double velocityY = ParseDouble(particleStr, startIndex, spaceIndex);

	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find("\t", startIndex);
	double velocityZ = ParseDouble(particleStr, startIndex, spaceIndex);
	cVector3d velocity(velocityX, velocityY, velocityZ);

	// Ignore acceleration
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find("\t", startIndex);
	// Ignore life
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find(" ", startIndex);
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find("\t", startIndex);
	// Ignore pstate
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find("\t", startIndex);
	
	// Parse ID
	startIndex = spaceIndex + 1;
	spaceIndex = particleStr.find("\t", startIndex);
	int id = ParseInt(particleStr, startIndex, spaceIndex);

	// Ignore parent

	return new GEOParticle(id, position, velocity);
}

const int NUMBER_STRING_BUFFER_SIZE = 32;
double FrameDataParse::ParseDouble(const std::string& source, int startIndex, int endIndex) {
	char numberBuffer[NUMBER_STRING_BUFFER_SIZE];
	memset(numberBuffer, 0, NUMBER_STRING_BUFFER_SIZE * sizeof(char));

	source.copy(numberBuffer, endIndex - startIndex, startIndex);
	char* endPtr;
	return strtod(numberBuffer, &endPtr);
}

int FrameDataParse::ParseInt(const std::string& source, int startIndex, int endIndex) {
	char numberBuffer[NUMBER_STRING_BUFFER_SIZE];
	memset(numberBuffer, 0, NUMBER_STRING_BUFFER_SIZE * sizeof(char));

	source.copy(numberBuffer, endIndex - startIndex, startIndex);
	return atoi(numberBuffer);
}