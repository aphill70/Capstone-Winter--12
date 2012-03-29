#include "IFluid.h"

class TestOneDirectionFluid : public IFluid {
public:
	void GetAllPoints(std::vector<cVector3d>& destination);
	void GetVelocityAt(cVector3d& velocity, const cVector3d& location);
};