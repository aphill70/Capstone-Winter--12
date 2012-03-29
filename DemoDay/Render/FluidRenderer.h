#pragma once

#include "chai3d.h"
#include "glm/glm.hpp"

using namespace glm;

class FluidRenderer
{

public:
	FluidRenderer();
	void InitFluids(cWorld*);
	void UpdateFluid(cWorld*);

private:
	cWorld * world;
	
	double diameter;
	void RenderSphere(glm::vec4*);
	void RenderSphere(glm::vec4*, glm::vec4* velocity);
	void RenderLine(glm::vec4*, glm::vec4*);

	// test model
	vec4 * velocity;
	vec4 * ballLoc;
	vec4 * glmAdd(vec4*, vec4*);

};