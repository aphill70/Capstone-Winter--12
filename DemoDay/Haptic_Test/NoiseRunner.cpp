#include "NoiseRunner.h"


NoiseHapticTestRunner::NoiseHapticTestRunner(cVector3d& initialVelocity, e_NoiseType type) {
		
}

void NoiseHapticTestRunner::GenerateNoise() {
	for (int i = 0; i < NOISERUNNER_INTERVAL; i++)
		noise[i] = 0;

}
