#include "IFluid.h"
#include "TestOneDirectionFuildParticle.h"

class TestOneDirectionFluid : public IFluid {
private:


public:

	// Finds all the points in the fluid simulation and copies them into the list 'allParticles'
	void GetAllPoints(vector<IFluidParticle*>& allParticles) {
		//vector<TestOneDirectionFuildParticle*> allParticlesO();
		//TestOneDirectionFuildParticle * t = new TestOneDirectionFuildParticle();
		//allParticlesO.push_back(t);
		//allParticles = allParticlesO;
		
	}
	// Returns the number of particles in the simulation
	int GetPointCount(void) {
		return 1;
	}
	// Finds the velocity of the fluid at the given point and copies it into 'velocity'
	void GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	}
	// Returns the fastest possible speed of a particle
	double GetMaxParticleSpeed(void) {
		return 2.0;
	}
	// Advances the fluid simulation one frame
	void AdvanceFrame(void) {
	}

	int GetMaxSimulatedParticles() {
		return 0;
	}
	
};