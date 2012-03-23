#pragma once
#include "HapticTestRunner.h"
#include "Include/PerlinNoise.h"
#include "math/CMaths.h"
#include "OscillRunner.h"

#define NOISERUNNER_MAX_NOISE 3.0
#devine NOISERUNNER_TIME_INTERVAL 300.0

class NoiseHapticTestRunner : HapticTestRunner {
public:
	enum e_NoiseType {
		POS_X_AXIS,
		NEG_X_AXIS,
		POS_Y_AXIS,
		NEG_Y_AXIS,
		POS_Z_AXIS
	};
	NoiseHapticTestRunner(cVector3d& initialVelocity, e_NoiseType type);
	virtual void ComputeVelocity(cVector3d& velocity, const cVector3d& location);
private:
	void GenerateNoise(e_NoiseType &type);
	void Gen1dNoise(double &destination, const double origin, const double time_interval);
	cVector3d vector;
	cVector3d *noise;
};