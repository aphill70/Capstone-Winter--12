#pragma once

#include "chai3d.h"

class HapticRenderer
{
private:
	cWorld* world;
	cShapeSphere* cursor;

public:
	HapticRenderer();
	void InitCursor(cWorld* w);
	cGenericObject* GetCursor();

};