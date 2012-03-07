#include "ControlManager.h"

#include <gl/glut.h>

#if !defined (_MSC_VER)
using namespace std;
#endif

ControlManager::ControlManager(void) {
	frozen = false;
	
	transforms.xPos = 0;
	transforms.yPos = 0;
	transforms.zPos = -5;
	transforms.rotateX = 0; 
	transforms.rotateY = 0;

	inputs[MOUSE_INPUT_INDEX] = &mouseInput;
	inputs[KEYBOARD_INPUT_INDEX] = &keyboardInput;
	inputs[HAPTIC_INPUT_INDEX] = &hapticInput;
}

ControlManager::~ControlManager(void) {}

void ControlManager::HandleKeyDown(unsigned char key, int x, int y) {
	keyboardInput.HandleKeyPress(key);
}

void ControlManager::HandleKeyUp(unsigned char key, int x, int y) {
	keyboardInput.HandleKeyRelease(key);
}

void ControlManager::HandleMouseMotion(int x, int y) {
	if(!frozen)
		mouseInput.HandleMouseInput(x, y);
}

void ControlManager::HandleMouseClick(int button, int state, int x, int y) {
	//mouseInput.HandleMouseClick(button, state, x, y);
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		frozen = !frozen;
		mouseInput.RecenterMouse();
		mouseInput.ShowCursor(frozen);
		//audioManager.PlayZoom(frozen);
	}
		
}

HapticInput& ControlManager::GetHapticInput(void) {
	return hapticInput;
}

void ControlManager::UpdateWorldTransformations() {

	if(!frozen)
		for (int i = 0; i < INPUT_COUNT; i++) {
			inputs[i]->ModifyTransformations(transforms);
		}

	glRotated(transforms.rotateX, 1, 0, 0);
	glRotated(transforms.rotateY, 0, 1, 0);
	glTranslated(transforms.xPos, transforms.yPos, transforms.zPos);
}


