#pragma once

#include "IInput.h"
#include "ModeFlag.h"
#include <gl/glut.h>

#define PIXEL_TO_ANGLE_FACTOR 0.25

class MouseInput : public IInput
{
private:
	int xOffset, yOffset;
	int windowWidth, windowHeight;
	void RecenterMouse(void);
	void ShowCursor(bool);

public:
	MouseInput(void);
	void HandleMouseInput(int x, int y);
	void HandleMouseClick(int button, int state, int x, int y);
	void SetWindowDimensions(int w, int h);
	virtual void ModeChange(void);
	virtual void SetFlag(ModeFlag*);
	void ModifyTransformations(WorldTransformation& transform); 
};