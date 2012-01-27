#pragma once

#include "IControlInput.h"

// Smallest unit of movement
#define MOVEMENT_DELTA 0.3
#define FORWARD_BUTTON 'w'
#define BACK_BUTTON 's'
#define LEFT_BUTTON 'a'
#define RIGHT_BUTTON 'd'
#define UP_BUTTON 'r'
#define DOWN_BUTTON 'f'
#define RESET_BUTTON '1'
#define QUIT_BUTTON 27		// ESC key

class KeyboardInput : public IControlInput {
private:
	bool forwardPressed, backPressed, leftPressed, rightPressed, upPressed, downPressed;
	bool resetPressed;

	void MoveForward(WorldTransformation& transform);
	void MoveBack(WorldTransformation& transform);
	void MoveLeft(WorldTransformation& transform);
	void MoveRight(WorldTransformation& transform);
	void MoveUp(WorldTransformation& transform);
	void MoveDown(WorldTransformation& transform);
public:
	KeyboardInput(void);
	~KeyboardInput(void);

	virtual void ModifyTransformations(WorldTransformation& transform);
	void HandleKeyPress(unsigned char key);
	void HandleKeyRelease(unsigned char key);
};