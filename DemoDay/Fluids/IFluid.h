#pragma once

#include <vector>
#include "chai3d.h"

#include "ITerrain.h"


class IFluid {
protected:
	ITerrain* terrain;

public:
	virtual void SetTerrain(ITerrain* newTerrain) = 0;
	// Finds all the points in the fluid simulation and copies them into the list 'destination'
	virtual void GetAllPoints(std::vector<cVector3d>& destination) = 0;
	// Finds the velocity of the fluid at the given point and copies it into 'velocity'
	virtual void GetVelocityAt(cVector3d& velocity, const cVector3d& location) = 0;
};