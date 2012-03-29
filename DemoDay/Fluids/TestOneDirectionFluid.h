#include "IFluid.h"

class TestOneDirectionFluid : public IFluid {
public:
<<<<<<< HEAD
	void SetTerrain(ITerrain* newTerrain);

=======
>>>>>>> dc381a2d9dd27705f824071eb1dc0a07b25e145d
	void GetAllPoints(std::vector<cVector3d>& destination);
	
	void GetVelocityAt(cVector3d& velocity, const cVector3d& location);

	double GetMaxParticleSpeed(void);
};