#include "TestOneDirectionFluid.h"

void TestOneDirectionFluid::SetTerrain(ITerrain* newTerrain) {
}

void TestOneDirectionFluid::GetAllPoints(std::vector<cVector3d>& destination) {
}
	
void TestOneDirectionFluid::GetVelocityAt(cVector3d& velocity, const cVector3d& location) {
}

double TestOneDirectionFluid::GetMaxParticleSpeed(void) {
	return 1.0;
}