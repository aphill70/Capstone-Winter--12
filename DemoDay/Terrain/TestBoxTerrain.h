#include "ITerrain.h"

class TestBoxTerrain {
public:
	virtual bool IsInGround(cVector3d& point);
	virtual bool IsInWorld(cVector3d& point);
};