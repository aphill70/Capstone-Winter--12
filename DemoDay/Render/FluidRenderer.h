#pragma once

#include "chai3d.h"
#include "glm/glm.hpp"
#include <vector>
#include "IFluid.h"
#include "capSimpleTetra.h"

using namespace glm;

class FluidRenderer
{

public:
	FluidRenderer();
	void InitFluids(cWorld*,IFluid*);
	void UpdateFluid();

private:
	cWorld * world;
	IFluid * fluidModel;

	double diameter;
	int activeParticles;
	cMaterial fluidMaterial;
	
	void HandleParticleCountChange(int delta);

	vector<capSimpleTetra*> particles;
};