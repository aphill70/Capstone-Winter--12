#include "MouseInput.h"

MouseInput::MouseInput(void)
{
	xOffset = 0;
	yOffset = 0;
}

void MouseInput::SetFlag(ModeFlag * flag)
{
	modeFlag = flag;
	modeFlag->AddListener(this);
}

void MouseInput::SetWindowDimensions(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}

void MouseInput::HandleMouseInput(int x, int y) 
{
	//If not in free mode, don't track anything
	if(modeFlag->GetMode() != FREE)
		return;

	// I used += in case event is fired more than once per frame
	xOffset = x - (windowWidth / 2.0);
	yOffset = y - (windowHeight / 2.0);
}

void MouseInput::HandleMouseClick(int button, int state, int x, int y) 
{
	if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	Mode curMode = modeFlag->GetMode();

	if(curMode == FREE)
	{
		modeFlag->SetMode(EXPLORE);
	}
	else if(curMode == EXPLORE)
	{
		modeFlag->SetMode(FREE);
	}

}

void MouseInput::ModifyTransformations(WorldTransformation& transform) 
{
	
	if(modeFlag->GetMode() != FREE)
		return;

	double rotationX = yOffset * PIXEL_TO_ANGLE_FACTOR;
	transform.elevation -= rotationX;
	if (transform.elevation > 180) {
		transform.elevation = 180;
	}
	if (transform.elevation < 0) {
		transform.elevation = 0;
	}
	
	transform.heading += (double) xOffset * PIXEL_TO_ANGLE_FACTOR;
	if (transform.heading > 360) {
		transform.heading -= 360;
	}
	if (transform.heading < 0) {
		transform.heading += 360;
	}

	RecenterMouse();

}

void MouseInput::RecenterMouse() {
	double xOffset = windowWidth / 2.0;
	double yOffset = windowHeight / 2.0;
	glutWarpPointer(xOffset, yOffset);
}

void MouseInput::ShowCursor(bool toggle) {
	glutSetCursor((toggle ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE));
}

void MouseInput::ModeChange()
{
	if(modeFlag->GetMode() == FREE)
	{
		ShowCursor(false);
		RecenterMouse();
	} 
	else if(modeFlag->GetMode() == EXPLORE)
	{
		RecenterMouse();
		ShowCursor(true);
	}
	
}