#include "FluidRenderer.h"
#include "IFluidParticle.h"
//IFluid h

FluidRenderer::FluidRenderer() {
	fluidMaterial.m_ambient.set(.5,.5,.5);
    fluidMaterial.m_diffuse.set(1,1,1);
    fluidMaterial.m_specular.set(1.0, 1.0, 1.0);
	/*fluidMaterial.m_ambient.set(0.0, 0.635, 0.910);
    fluidMaterial.m_diffuse.set(1.0, 0.5, 0.0);
    fluidMaterial.m_specular.set(1.0, 1.0, 1.0);*/
	fluidMaterial.setTransparencyLevel(0.1);
}

void FluidRenderer::InitFluids(cWorld* w, IFluid * fluid)
{
	this->world = w;
	this->fluidModel = fluid;
	diameter = 0.03;
}

void FluidRenderer::UpdateFluid()
{
	fluidModel->AdvanceFrame();
	
	vector<IFluidParticle*> v;
	fluidModel->GetAllPoints(v);

	int deltaActivity = v.size() - activeParticles;
	HandleParticleCountChange(deltaActivity);

	activeParticles = v.size();

	cVector3d pos;
	for(int i = 0; i < activeParticles; i++)
	{
		v[i]->GetPosition(pos);
		particles[i]->setPos(pos);
	}
}

void FluidRenderer::HandleParticleCountChange(int delta)
{
	//if delta is zero then we have the same number of particles
	if(delta == 0)
		return;
	//if delta is positive, then there are new particles
	else if(delta > 0)
	{
		//check if the vector must be expanded
		if(activeParticles + delta > particles.size())
		{
			//if so, expand it, create new particles and show them
			for(int i = 0; i < delta; i++)
			{
				capSimpleTetra* particle = new capSimpleTetra(world, diameter);
				particle->m_material = fluidMaterial;
				world->addChild(particle);
				particle->setShowEnabled(true);
				particles.push_back(particle);
			}
		}
		else
		{
			//if not, just show the idle ones
			for(int i = 0; i < delta; i++)
			{
				particles[activeParticles + i]->setShowEnabled(true);
			}
		}
	}
	//if delta is negative, then there are fewer particles this frame
	else //if(delta < 0)
	{
		//Hide the particles that don't need to be rendered
		for(int i = delta; i < 0; i++)
		{
			particles[activeParticles + i]->setShowEnabled(false);
		}
	}
}