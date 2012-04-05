#include "IFluidParticle.h"

class TestOneDirectionFuildParticle : IFluidParticle {
private:
	cVector3d * p;
	cVector3d * v;
public:
	TestOneDirectionFuildParticle() {
		p = new cVector3d(0,0,2);
		v = new cVector3d(0,0,1);
	}

	void GetPosition(cVector3d& position) {
		position = *p;
	}
	
	void GetVelocity(cVector3d& velocity) {
		velocity = *v;
	}

};