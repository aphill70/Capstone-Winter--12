#pragma once

#include <string>

#include "FrameData.h"
#include "GEOParticle.h"

/*
 * This is a collection of functions used to parse our .geo files into FrameData objects
 */

namespace FrameDataParse {
	FrameData* ParseFrame(const std::string& filename);
	GEOParticle* ParseParticle(const string& particleStr);
	double ParseDouble(const std::string& source, int startIndex, int endIndex);
	int ParseInt(const std::string& source, int startIndex, int endIndex);
}