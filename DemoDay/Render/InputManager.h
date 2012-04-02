#pragma once

#include "IInput.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "HapticInput.h"
#include "chai3d.h"
#include "glm/glm.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#define d2r(x)		(x * M_PI / 180.0)

using namespace glm;
using namespace std;

class InputManager
{
private:
	MouseInput mouseInput;
	KeyboardInput keyboardInput;
	HapticInput hapticInput;
	ModeFlag mode;
	
	WorldTransformation transforms;
	
	dmat4x4 camToWorld;

	void BuildCamToWorld();

public:	
	InputManager();

	void InitTransforms(double, double, cVector3d);
	void SetWindowDimensions(int, int);
	void SetHapticCursor(cGenericObject* cursor);
	void GetHapticCursorPosition(cVector3d&);
	void UpdateTransforms(void);

	void MouseMotion(int, int);
	void MouseClick(int, int, int, int);

	void KeyDown(unsigned char, int, int);
	void KeyUp(unsigned char, int, int);

	cVector3d CamPointToWorld(cVector3d);
	cMatrix3d GetCameraTransformations();
};