#pragma once

#include "chai3d.h"
#include "glm/glm.hpp"
#include <vector>
#include "IFluid.h"


using namespace glm;

class FluidRenderer
{

public:
	FluidRenderer();
	void InitFluids(cWorld*,IFluid*);
	void UpdateFluid(cWorld*);

private:
	cWorld * world;
	
	double diameter;
	cMaterial fluidMaterial;

	cShapeSphere* RenderSphere(glm::vec4*);
	void RenderSphere(glm::vec4*, glm::vec4* velocity);
	void RenderLine(glm::vec4*, glm::vec4*);

	vec4 * glmAdd(vec4* sphere, vec4* velocity);

	vector<cShapeSphere*> particles;

	IFluid * fluidModel;

};