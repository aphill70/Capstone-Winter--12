#pragma once

#include "chai3d.h"
#include "glm/glm.hpp"

class FluidRenderer
{

public:
	FluidRenderer();
	void InitFluids(cWorld*);

private:
	cWorld * world;
	void RenderSphere(glm::vec4*);
};