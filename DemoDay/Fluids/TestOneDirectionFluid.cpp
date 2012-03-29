#include "TestOneDirectionFluid.h"

void TestOneDirectionFluid::GetAllPoints(std::vector<cVector3d>& destination) {}

void TestOneDirectionFluid::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
	velocity.x = 0;
}