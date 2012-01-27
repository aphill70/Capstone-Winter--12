#pragma once

#include "WorldTransformation.h"
#include "IControlInput.h"
#include "MouseInput.h"
#include "KeyboardInput.h"

#define MOUSE_INPUT_INDEX 0
#define KEYBOARD_INPUT_INDEX 1
#define INPUT_COUNT 2

class ControlManager {
private:
	WorldTransformation transforms;						// Conceptually like a camera
	IControlInput* inputs[INPUT_COUNT];
	MouseInput mouseInput;
	KeyboardInput keyboardInput;
public:
	ControlManager(void);
	~ControlManager(void);

	void HandleKeyDown(unsigned char key, int x, int y);
	void HandleKeyUp(unsigned char key, int x, int y);
	void HandleMouseMotion(int x, int y);

	void UpdateWorldTransformations(void);
};