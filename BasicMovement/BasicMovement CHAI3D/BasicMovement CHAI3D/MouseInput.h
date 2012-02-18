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
	void HandleMouseClick(int button, int state, int x, int y);
	void RecenterMouse();
	void ShowCursor(bool);

	virtual void ModifyTransformations(WorldTransformation& transform);
};