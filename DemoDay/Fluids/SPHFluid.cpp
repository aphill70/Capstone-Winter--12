#include "SPHFluid.h"

SPHFluid::SPHFluid(void) : _neighborhoodRadius(1.0) {}
SPHFluid::SPHFluid(double neighborhood) : _neighborhoodRadius(neighborhood) {}
SPHFluid::~SPHFluid(void) {}

double SPHFluid::GetNeighborhoodRadius(void) { return _neighborhoodRadius; }
void SPHFluid::SetNeighborhoodRadius(double neighborhood) {
	_neighborhoodRadius = neighborhood;
}