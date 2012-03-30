#pragma once

#include <string>

/*
 * This is a collection of functions used to parse our .geo files into FrameData objects
 */

//FrameData ParseFrame(const string& filename);
void ParseFrame(const std::string& filename);

//GEOParticle* ParseParticle(const string& particleStr);
void ParseParticle(const std::string& particleStr);