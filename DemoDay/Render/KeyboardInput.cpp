#include "KeyboardInput.h"

KeyboardInput::KeyboardInput(void) : 
	forwardPressed(false), backPressed(false), 
	leftPressed(false), rightPressed(false), 
	upPressed(false), downPressed(false),
	resetPressed(false) { }

void KeyboardInput::SetFlag(ModeFlag * flag)
{
	modeFlag = flag;
	modeFlag->AddListener(this);
}

void KeyboardInput::MoveRight(WorldTransformation& transform) {
	double xzComponent = MOVEMENT_DELTA;
	double xComponent = sin(transform.heading * M_PI / 180.0) * xzComponent;
	double zComponent = cos(transform.heading * M_PI / 180.0) * xzComponent;

	transform.xPos -= xComponent;
	transform.zPos += zComponent;
}

void KeyboardInput::MoveLeft(WorldTransformation& transform) {
	double xzComponent = MOVEMENT_DELTA;
	double xComponent = sin(transform.heading * M_PI / 180.0) * xzComponent;
	double zComponent = cos(transform.heading * M_PI / 180.0) * xzComponent;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::MoveForward(WorldTransformation& transform) {
	double angle = transform.heading + 90;
	double xComponent = sin(angle * M_PI / 180.0) * MOVEMENT_DELTA;
	double zComponent = cos(angle * M_PI / 180.0) * MOVEMENT_DELTA;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::MoveBack(WorldTransformation& transform) {
	double angle = transform.heading - 90;
	double xComponent = sin(angle * M_PI / 180.0) * MOVEMENT_DELTA;
	double zComponent = cos(angle * M_PI / 180.0) * MOVEMENT_DELTA;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::MoveUp(WorldTransformation& transform) {
	double yComponent = /*sin(angle * M_PI / 180.0) */ MOVEMENT_DELTA;
	double xzComponent = sqrt( MOVEMENT_DELTA * MOVEMENT_DELTA - yComponent * yComponent);
	double xComponent = sin(transform.heading * M_PI / 180.0) * xzComponent;
	double zComponent = cos(transform.heading * M_PI / 180.0) * xzComponent;

	transform.xPos -= xComponent;
	transform.yPos += yComponent;
	transform.zPos += zComponent;
}

void KeyboardInput::MoveDown(WorldTransformation& transform) {
	double yComponent = /*sin(angle * M_PI / 180.0) */ MOVEMENT_DELTA;
	double xzComponent = sqrt( MOVEMENT_DELTA * MOVEMENT_DELTA - yComponent * yComponent);
	double xComponent = sin(transform.heading * M_PI / 180.0) * xzComponent;
	double zComponent = cos(transform.heading * M_PI / 180.0) * xzComponent;

	transform.xPos += xComponent;
	transform.yPos -= yComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::HandleKeyPress(unsigned char key) {
	switch (key) {
	case FORWARD_BUTTON:
		forwardPressed = true;
		break;
	case BACK_BUTTON:
		backPressed = true;
		break;
	case LEFT_BUTTON:
		leftPressed = true;
		break;
	case RIGHT_BUTTON:
		rightPressed = true;
		break;
	case UP_BUTTON:
		upPressed = true;
		break;
	case DOWN_BUTTON:
		downPressed = true;
		break;
	case RESET_BUTTON:
		resetPressed = true;
		break;
	case QUIT_BUTTON:						
		exit(0);
		break;
	}
}

void KeyboardInput::HandleKeyRelease(unsigned char key) {
	switch (key) {
	case FORWARD_BUTTON:
		forwardPressed = false;
		break;
	case BACK_BUTTON:
		backPressed = false;
		break;
	case LEFT_BUTTON:
		leftPressed = false;
		break;
	case RIGHT_BUTTON:
		rightPressed = false;
		break;
	case UP_BUTTON:
		upPressed = false;
		break;
	case DOWN_BUTTON:
		downPressed = false;
		break;
	case RESET_BUTTON:
		resetPressed = false;
		break;
	}
}

void KeyboardInput::ModifyTransformations(WorldTransformation& transform) {
	if (resetPressed) {
		resetPressed = false;
		transform.elevation = 0;
		transform.heading = 0;
		transform.xPos = 0;
		transform.yPos = 0;
		transform.zPos = 0;
		return;
	}

	if (forwardPressed) { MoveForward(transform); }
	if (backPressed) { MoveBack(transform); }
	if (leftPressed) { MoveLeft(transform); }
	if (rightPressed) { MoveRight(transform); }
	if (upPressed) { MoveUp(transform); }
	if (downPressed) { MoveDown(transform); }
}

void KeyboardInput::ModeChange()
{

}