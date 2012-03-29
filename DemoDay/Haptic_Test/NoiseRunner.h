#pragma once
#include "HapticTestRunner.h"
#include "Include/PerlinNoise.h"
#include "math/CMaths.h"
#include "OscillRunner.h"

#define NOISERUNNER_MAX_NOISE 3.0
#define NOISERUNNER_INTERVAL 30000

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
	void GenerateNoise();
	cVector3d vector;
	double [ NOISERUNNER_INTERVAL ]noise;
};