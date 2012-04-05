#include "TestBoxTerrain.h"

bool TestBoxTerrain::IsInGround(cVector3d& point) {
	return point.y >= 0;
}

bool TestBoxTerrain::IsInWorld(cVector3d& point) {
	return point.x >= -0.25 && point.x <= 0.25 &&
		point.y >= 0 && point.y <= 0.5 &&
		point.z >= -0.25 && point.z <= 0.25;
}