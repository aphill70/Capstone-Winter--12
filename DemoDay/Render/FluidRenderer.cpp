#include "FluidRenderer.h"

//IFluid h

FluidRenderer::FluidRenderer()
{

}

void FluidRenderer::InitFluids(cWorld* w)
{
	world = w;
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
	if(world != NULL)
		RenderSphere(new glm::vec4(0,0,3,0));
	else
		std::cout << " world null" << std::endl;


}

vec4 * FluidRenderer::glmAdd(vec4* sphere, vec4* velocity) {
	return new glm::vec4(sphere->x + velocity->x, sphere->y + velocity->y, sphere->z + velocity->z, 0);
}