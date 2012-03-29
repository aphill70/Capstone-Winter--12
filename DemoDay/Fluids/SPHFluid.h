#pragma once

#include "IFluid.h"

// Abstract class that represents a fluid that is simulated with an
//  SPH simulation.
class SPHFluid : public IFluid {
protected:
	// The size of the particle 'neighborhood' to sample when computing
	//  fluid velocity at a given position. Defaults to 1.0
	double _neighborhoodRadius;
public:
	SPHFluid(void);
	SPHFluid(double neighborhood);
	virtual ~SPHFluid(void);

	virtual double GetNeighborhoodRadius(void);
	virtual void SetNeighborhoodRadius(double neighborhood);
};