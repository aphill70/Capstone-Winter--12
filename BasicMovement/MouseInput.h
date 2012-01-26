#pragma once

#include "IControlInput.h"

// Defines how pixel values are converted to rotation angles
#define PIXEL_TO_ANGLE_FACTOR 0.5

class MouseInput : public IControlInput {
private:
	int xOffset, yOffset;
public:
	MouseInput(void);
	~MouseInput(void);

	void HandleMouseInput(int x, int y);
	virtual void ModifyTransformations(WorldTransformation& transform);
};