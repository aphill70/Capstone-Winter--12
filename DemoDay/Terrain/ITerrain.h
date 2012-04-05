#pragma once

#include "chai3d.h"

class ITerrain {
public:
	virtual bool IsInGround(cVector3d& point) = 0;
	virtual bool IsInWorld(cVector3d& point) = 0;
};