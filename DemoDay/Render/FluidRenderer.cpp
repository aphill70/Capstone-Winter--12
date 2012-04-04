#include "FluidRenderer.h"

//IFluid h

FluidRenderer::FluidRenderer() {}

void FluidRenderer::InitFluids(cWorld* w, IFluid * fluid)
{
	this->world = w;
	this->fluidModel = fluid;
	fluidModel->AdvanceFrame();
	diameter = 1;

	for(int i = 0; i < fluidModel->GetMaxSimulatedParticles(); i++ ) {
		std::cout << "size2: " << fluidModel->GetMaxSimulatedParticles() << std::endl;
		cShapeSphere * c = new cShapeSphere(diameter);
		c->setPos(0,0,0);
		world->addChild(c);
		particles.push_back(c);
		
	}
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
	fluidModel->AdvanceFrame();
	//std::cout << "size2: " << fluidModel->GetMaxSimulatedParticles() << std::endl;
	world = w;

	RenderSphere(new glm::vec4(0,0,3,0));

	vector<IFluidParticle*> v;
	fluidModel->GetAllPoints(v);
	//std::cout << "size3: " << v.size() << std::endl;

	



}

vec4 * FluidRenderer::glmAdd(vec4* sphere, vec4* velocity) {
	return new glm::vec4(sphere->x + velocity->x, sphere->y + velocity->y, sphere->z + velocity->z, 0);
}