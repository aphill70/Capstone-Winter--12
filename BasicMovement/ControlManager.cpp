#include "ControlManager.h"

#include <gl/glut.h>

using namespace std;

ControlManager::ControlManager(void) {
	transforms.xPos = 0;
	transforms.yPos = 0;
	transforms.zPos = -5;
	transforms.rotateX = 0; 
	transforms.rotateY = 0;

	inputs[MOUSE_INPUT_INDEX] = &mouseInput;
	inputs[KEYBOARD_INPUT_INDEX] = &keyboardInput;
}

ControlManager::~ControlManager(void) {}

void ControlManager::HandleKeyDown(unsigned char key, int x, int y) {
	keyboardInput.HandleKeyPress(key);
}

void ControlManager::HandleKeyUp(unsigned char key, int x, int y) {
	keyboardInput.HandleKeyRelease(key);
}

void ControlManager::HandleMouseMotion(int x, int y) {
	mouseInput.HandleMouseInput(x, y);
}


void ControlManager::UpdateWorldTransformations() {
	for (int i = 0; i < INPUT_COUNT; i++) {
		inputs[i]->ModifyTransformations(transforms);
	}

	glRotated(transforms.rotateX, 1, 0, 0);
	glRotated(transforms.rotateY, 0, 1, 0);
	glTranslated(transforms.xPos, transforms.yPos, transforms.zPos);
}
