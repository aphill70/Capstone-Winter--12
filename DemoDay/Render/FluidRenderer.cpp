#include "FluidRenderer.h"


FluidRenderer::FluidRenderer()
{

}

void FluidRenderer::InitFluids(cWorld* w)
{
	world = w;
	
	RenderSphere(new glm::vec4(0,0,3,0));
	std::cout << "init fluids" << std::endl;
}

void FluidRenderer::RenderSphere(glm::vec4 * sphere) {
	cShapeSphere * s = new cShapeSphere(.1);
	s->setPos(sphere->x, sphere->y, sphere->z);
	world->addChild(s);
}