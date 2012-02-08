#include <stdlib.h>
#include <math.h>

#include "Globals.h"
#include "KeyboardInput.h"

#ifndef _WIN32
using namespace std;
#endif

KeyboardInput::KeyboardInput(void) : 
		forwardPressed(false), backPressed(false), 
		leftPressed(false), rightPressed(false), 
		upPressed(false), downPressed(false),
		resetPressed(false) {
}

KeyboardInput::~KeyboardInput(void) {}



void KeyboardInput::ModifyTransformations(WorldTransformation& transform) {
	if (resetPressed) {
		resetPressed = false;
		transform.rotateX = 0;
		transform.rotateY = 0;
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
	case 'p':							// Debug breakpoint
		int a = 1;
		break;
	}
}

void KeyboardInput::MoveForward(WorldTransformation& transform) {
	double yComponent = sin(transform.rotateX * PI / 180.0) * MOVEMENT_DELTA;
	double xzComponent = sqrt( MOVEMENT_DELTA * MOVEMENT_DELTA - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos -= xComponent;
	transform.yPos += yComponent;
	transform.zPos += zComponent;
}

void KeyboardInput::MoveBack(WorldTransformation& transform) {
	double yComponent = sin(transform.rotateX * PI / 180.0) * MOVEMENT_DELTA;
	double xzComponent = sqrt( MOVEMENT_DELTA * MOVEMENT_DELTA - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos += xComponent;
	transform.yPos -= yComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::MoveLeft(WorldTransformation& transform) {
	double angle = transform.rotateY + 90;
	double xComponent = sin(angle * PI / 180.0) * MOVEMENT_DELTA;
	double zComponent = cos(angle * PI / 180.0) * MOVEMENT_DELTA;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::MoveRight(WorldTransformation& transform) {
	double angle = transform.rotateY - 90;
	double xComponent = sin(angle * PI / 180.0) * MOVEMENT_DELTA;
	double zComponent = cos(angle * PI / 180.0) * MOVEMENT_DELTA;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void KeyboardInput::MoveUp(WorldTransformation& transform) {
	double angle = transform.rotateX - 90;
	double yComponent = sin(angle * PI / 180.0) * MOVEMENT_DELTA;
	double xzComponent = sqrt( MOVEMENT_DELTA * MOVEMENT_DELTA - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos -= xComponent;
	transform.yPos += yComponent;
	transform.zPos += zComponent;
}

void KeyboardInput::MoveDown(WorldTransformation& transform) {
	double angle = transform.rotateX - 90;
	double yComponent = sin(angle * PI / 180.0) * MOVEMENT_DELTA;
	double xzComponent = sqrt( MOVEMENT_DELTA * MOVEMENT_DELTA - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos += xComponent;
	transform.yPos -= yComponent;
	transform.zPos -= zComponent;
}
