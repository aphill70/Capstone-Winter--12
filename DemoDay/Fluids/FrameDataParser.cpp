#include <iostream>
#include <fstream>

#include "FrameDataParser.h"

//FrameData ParseFrame(const string& filename) {
void ParseFrame(const std::string& filename) {
	std::ifstream fileReader(filename.c_str());
	
	std::string frameText;
	std::string currentLine;

	//if (!fileReader.is_open()) {
	//}

	while(fileReader.good()) {
		getline(fileReader, currentLine);
		frameText += currentLine + "\n";

		/*
		size_t position = currentLine.find("parent");
		size_t headerEndPos = currentLine
		*/
	}
}