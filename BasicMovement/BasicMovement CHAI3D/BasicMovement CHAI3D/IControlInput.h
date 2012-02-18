#pragma once

#include "WorldTransformation.h"

class IControlInput {
public:
	virtual void ModifyTransformations(WorldTransformation& transform) = 0;
};