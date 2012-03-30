#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>

#include "FrameDataParser.h"

//FrameData ParseFrame(const string& filename) {
void ParseFrame(const std::string& filename) {
	std::ifstream fileReader(filename.c_str());
	const int NUM_STR_BUF_SIZE = 16;
	char numberStrBuffer[NUM_STR_BUF_SIZE];

	std::string frameText;
	std::string currentLine;

	//Header; not needed
	std::getline(fileReader, currentLine);
	// Parse point count
	std::getline(fileReader, currentLine);
	assert(currentLine.find("NPoints") == 0);
	size_t index1 = currentLine.find(" ", strlen("NPoints"));
	size_t index2 = currentLine.find(" ", index1 + 1);
	memset(numberStrBuffer, 0, NUM_STR_BUF_SIZE);
	currentLine.copy(numberStrBuffer, index2 - index1 - 1, index1 + 1);
	int frameParticleCount = atoi(numberStrBuffer);

	// Group data; not needed
	std::getline(fileReader, currentLine);
	assert(currentLine.find("NPointGroups") == 0);
	
	// Parse attribute count
	std::getline(fileReader, currentLine);
	assert(currentLine.find("NPointAttrib") == 0);
	index1 = currentLine.find(" ", strlen("NPointAttrib"));
	index2 = currentLine.find(" ", index1 + 1);	
	memset(numberStrBuffer, 0, NUM_STR_BUF_SIZE);
	currentLine.copy(numberStrBuffer, index2 - index1 - 1, index1 + 1);
	int attributeCount = atoi(numberStrBuffer);

	// Attributes; not needed
	std::getline(fileReader, currentLine);
	assert(currentLine.compare("PointAttrib") == 0);
	for(int i = 0; i < attributeCount; i++) {
		std::getline(fileReader, currentLine);
	}

	// Parse particles
	std::getline(fileReader, currentLine);
	while(currentLine.compare("PrimitiveAttrib") != 0) {
		ParseParticle(currentLine);

		std::getline(fileReader, currentLine);
	}

	// Ignore the rest of the file's contents
	fileReader.close();
}

// TODO: this can be made more generic if you care to parse the PointAttrib section
void ParseParticle(const std::string& particleStr) {
	// posx
	// posy
	// posz
	// posw
	// as listed in header
	//-3.4566803 3.05862832 -3.00498819 1 (-0.2045331 0.0930733681 3.32522583	-2.55458832 0.0991082191 -2.59312153	0 3	0	1	0)
}