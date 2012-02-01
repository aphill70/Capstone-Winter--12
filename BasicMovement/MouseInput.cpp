#include "MouseInput.h"
#include "Globals.h"

#include <gl/glut.h>

#ifndef _WIN32
using namespace std;
#endif

MouseInput::MouseInput(void) : xOffset(0), yOffset(0) {}

MouseInput::~MouseInput(void) {}

void MouseInput::HandleMouseInput(int x, int y) {
	// I used += in case event is fired more than once per frame
	xOffset = x - (windowWidth / 2.0);
	yOffset = y - (windowHeight / 2.0);
}

void MouseInput::ModifyTransformations(WorldTransformation& transform) {
	
	double rotationX = yOffset * PIXEL_TO_ANGLE_FACTOR;
	transform.rotateX += rotationX;
	if (transform.rotateX > 90) {
		transform.rotateX = 90;
	}
	if (transform.rotateX < -90) {
		transform.rotateX = -90;
	}
	
	transform.rotateY += (double) xOffset * PIXEL_TO_ANGLE_FACTOR;
	if (transform.rotateY > 360) {
		transform.rotateY -= 360;
	}
	if (transform.rotateY < 0) {
		transform.rotateY += 360;
	}

	xOffset = windowWidth / 2.0;
	yOffset = windowHeight / 2.0;
	glutWarpPointer(xOffset, yOffset);
}
