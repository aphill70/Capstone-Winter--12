#include "FluidRenderer.h"
#include "IFluidParticle.h"

//IFluid h

FluidRenderer::FluidRenderer() {
	fluidMaterial.m_ambient.set(0.0, 0.635, 0.910);
    fluidMaterial.m_diffuse.set(1.0, 0.5, 0.0);
    fluidMaterial.m_specular.set(1.0, 1.0, 1.0);
	fluidMaterial.setTransparencyLevel(0.1);
}

void FluidRenderer::InitFluids(cWorld* w, IFluid * fluid)
{
	this->world = w;
	this->fluidModel = fluid;
	fluidModel->AdvanceFrame();
	diameter = 0.2;
}

cShapeSphere* FluidRenderer::RenderSphere(glm::vec4 * sphere) {
	cShapeSphere * s = new cShapeSphere(diameter);
	s->setPos(sphere->x, sphere->y, sphere->z);
	s->m_material = fluidMaterial;
	world->addChild(s);
	return s;
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
	world = w;

	vector<IFluidParticle*> v;
	fluidModel->GetAllPoints(v);

	int sphereCount = particles.size();
	int particleCount = v.size();

	int currentIndex = 0;
	int smallerCount = sphereCount < particleCount ? sphereCount : particleCount;
	while(currentIndex < smallerCount) {
		cVector3d curPartPosition;
		v[currentIndex]->GetPosition(curPartPosition);
		particles[currentIndex]->setPos(curPartPosition);

		currentIndex++;
	}

	if (currentIndex == particleCount) {
		// Stop
	} else if (currentIndex < particleCount) {
		// Start adding spheres
		while (currentIndex < particleCount) {
			cVector3d curPartPosition;
			v[currentIndex]->GetPosition(curPartPosition);

			cShapeSphere* addSphere = RenderSphere(new glm::vec4(curPartPosition.x, curPartPosition.y, curPartPosition.z, 1));
			particles.push_back(addSphere);
			currentIndex++;
		}
	} else {
		// Start removing spheres
		while(particles.size() > particleCount) {
			cShapeSphere* extraSphere = particles[particles.size() - 1];
			world->removeChild(extraSphere);
			delete extraSphere;
			particles.pop_back();
		}
	}
}

vec4 * FluidRenderer::glmAdd(vec4* sphere, vec4* velocity) {
	return new glm::vec4(sphere->x + velocity->x, sphere->y + velocity->y, sphere->z + velocity->z, 0);
}