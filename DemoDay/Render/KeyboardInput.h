#pragma once

#include "IInput.h"
#include "ModeFlag.h"

#define _USE_MATH_DEFINES
#include <math.h>

// Smallest unit of movement
#define MOVEMENT_DELTA 0.3
#define FORWARD_BUTTON 'w'
#define BACK_BUTTON 's'
#define LEFT_BUTTON 'a'
#define RIGHT_BUTTON 'd'
#define UP_BUTTON 'v'
#define DOWN_BUTTON ' '
#define RESET_BUTTON '1'
#define QUIT_BUTTON 27		// ESC key

class KeyboardInput : public IInput
{
private:
	bool forwardPressed, backPressed;
	bool leftPressed, rightPressed;
	bool upPressed, downPressed;
	bool resetPressed;
	
	void MoveForward(WorldTransformation& transform);
	void MoveBack(WorldTransformation& transform);
	void MoveLeft(WorldTransformation& transform);
	void MoveRight(WorldTransformation& transform);
	void MoveUp(WorldTransformation& transform);
	void MoveDown(WorldTransformation& transform);
	
	void DepressAll();

public:
	KeyboardInput(void);
	
	ModeFlag* flag;

	void HandleKeyPress(unsigned char key);
	void HandleKeyRelease(unsigned char key);
	virtual void ModeChange();
	virtual void SetFlag(ModeFlag*);

	void ModifyTransformations(WorldTransformation& transform); 
};