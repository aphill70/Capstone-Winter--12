#pragma once
#include "HapticTestRunner.h"

class OscillatingHapticTestRunner : public HapticTestRunner {
public:
	enum e_OscillationType {
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		XY_PLANE,
		YZ_PLANE,
		XZ_PLANE
	};
	OscillatingHapticTestRunner(const cVector3d& initialVector, enum e_OscillationType type);
	void RunnerLoop();
private:
	void SetOscillation();
	void SetPolarity();

	cVector3d _initial;
	bool polarity;
	float oscAmount;
	e_OscillationType _type;
	cPrecisionClock clock;

	static const double TIMEOUT_PERIOD;
	static const double INCREMENT_AMOUNT;
};