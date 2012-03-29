#include "FluidRenderer.h"

//IFluid h

FluidRenderer::FluidRenderer()
{
	diameter = .1;

	//test model;
	velocity = new vec4(0,.01,0,0);
	ballLoc = new glm::vec4(0,0,3,0);
	std::cout << "consts fluids" << std::endl;
}

void FluidRenderer::InitFluids(cWorld* w)
{
	world = w;
	
	
	//RenderSphere(new glm::vec4(0,0,3,0));
	//RenderSphere(new glm::vec4(0,0,4,0), new glm::vec4(0,1,0,0));
	ballLoc = glmAdd(ballLoc, velocity);

	std::cout << "init fluids" << std::endl;
}

void FluidRenderer::RenderSphere(glm::vec4 * sphere) {
	cShapeSphere * s = new cShapeSphere(diameter);
	s->setPos(sphere->x, sphere->y, sphere->z);
	world->addChild(s);
}

void FluidRenderer::RenderSphere(glm::vec4* sphere, glm::vec4* velocity) {
	RenderSphere(sphere);
	RenderLine(sphere, new glm::vec4(sphere->x + velocity->x, sphere->y + velocity->y, sphere->z + velocity->z, 0));

	
}

void FluidRenderer::RenderLine(glm::vec4* pt1, glm::vec4* pt2) {
	cVector3d d1(pt1->x, pt1->y, pt1->z);
	cVector3d d2(pt2->x, pt2->y, pt2->z);
	cShapeLine * l = new cShapeLine(d1, d2);
	world->addChild(l);
}

void FluidRenderer::UpdateFluid(cWorld * w) {
	world = w;
	std::cout << " updating fluids" << std::endl;
	if(world != NULL)
		RenderSphere(new glm::vec4(0,0,3,0));
	else
		std::cout << " world null" << std::endl;


	
	ballLoc = glmAdd(ballLoc, velocity);
	RenderSphere(ballLoc, velocity);
}

vec4 * FluidRenderer::glmAdd(vec4* sphere, vec4* velocity) {
	return new glm::vec4(sphere->x + velocity->x, sphere->y + velocity->y, sphere->z + velocity->z, 0);
}