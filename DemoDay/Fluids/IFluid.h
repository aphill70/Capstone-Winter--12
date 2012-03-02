#pragma once

#include <vector>
#include "ITerrain.h"
#include "chai3d.h"

class IFluid {
public:
	virtual void SetTerrain(ITerrain newTerrain) = 0;
	virtual void GetAllPoints(std::vector<cVector3d>& destination) = 0;
	virtual cVector3d GetVelocityAt(const cVector3d& location) = 0;
}