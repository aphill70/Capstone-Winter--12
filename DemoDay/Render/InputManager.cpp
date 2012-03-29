#include "InputManager.h"

InputManager::InputManager()
{
	mouseInput.SetFlag(&mode);
}

void InputManager::InitTransforms(double heading, double elevation, cVector3d pos)
{
	transforms.xPos = pos.x;
	transforms.yPos = pos.y;
	transforms.zPos = pos.z;

	transforms.heading = heading;
	transforms.elevation = elevation;
}

void InputManager::UpdateTransforms()
{
	mouseInput.ModifyTransformations(transforms);
	keyboardInput.ModifyTransformations(transforms);
	//cout << transforms.heading << " " << transforms.elevation << endl;
}

void InputManager::SetWindowDimensions(int width, int height)
{
	mouseInput.SetWindowDimensions(width, height);
}

void InputManager::MouseMotion(int x, int y)
{
	mouseInput.HandleMouseInput(x, y);
}

void InputManager::MouseClick(int button, int state, int x, int y)
{
	mouseInput.HandleMouseClick(button, state, x, y);
}

void InputManager::KeyUp(unsigned char key, int x, int y)
{
	keyboardInput.HandleKeyRelease(key);
}

void InputManager::KeyDown(unsigned char key, int x, int y)
{
	keyboardInput.HandleKeyPress(key);
}

cMatrix3d InputManager::GetCameraTransformations()
{
	cMatrix3d cam;
	if(mode.GetMode() != EXPLORE)
	{
		BuildCamToWorld();
	}

	//Col0 has Position
	cam.setCol0(cVector3d(transforms.xPos, transforms.yPos, transforms.zPos));
	
	//Col1 has Gaze
	dvec4 gazeCamSpace = dvec4( 0, 0, 1, 1);
	dvec4 gazeWorldSpace = camToWorld * gazeCamSpace;
	cam.setCol1(cVector3d(gazeWorldSpace.x, gazeWorldSpace.y, gazeWorldSpace.z));

	//Col2 has Up
	cVector3d up;
	if(transforms.elevation == 0)
	{
		//If Looking straight down, change the up vector to the heading
		up = cVector3d(cos(d2r(transforms.heading)), 0, sin(d2r(transforms.heading)));
	}
	else if(transforms.elevation == 180)
	{
		//If looking straight up, change the up vector to the negative heading
		up = cVector3d(0 - cos(d2r(transforms.heading)), 0, 0 - sin(d2r(transforms.heading)));
	}
	else
	{
		up = cVector3d(0,1,0);
	}
	cam.setCol2(up);

	return cam;
}

void InputManager::BuildCamToWorld()
{
	dvec3 gaze, up, v, u, w;
	gaze.x = sin(d2r(transforms.elevation)) * cos(d2r(transforms.heading));
	gaze.y = 0 - cos(d2r(transforms.elevation));
	gaze.z = sin(d2r(transforms.elevation)) * sin(d2r(transforms.heading));
	
	if(transforms.elevation == 0)
	{
		//If Looking straight down, change the up vector to the heading
		up = dvec3(cos(d2r(transforms.heading)), 0, sin(d2r(transforms.heading)));
	}
	else if(transforms.elevation == 180)
	{
		//If looking straight up, change the up vector to the negative heading
		up = dvec3(0 - cos(d2r(transforms.heading)), 0, 0 - sin(d2r(transforms.heading)));
	}
	else
	{
		up = dvec3(0,1,0);
	}

	w.x = 0 - gaze.x;
	w.y = 0 - gaze.y;
	w.z = 0 - gaze.z;
	
	//Shouldn't be necessary, but just in case...
	w = normalize(w);

	u = cross(up, w);
	u = normalize(u);

	v = cross(w, u);
	v = normalize(v);

	dmat4x4 changeBase = dmat4x4(	u.x, u.y, u.z, 0,
									v.x, v.y, v.z, 0,
									gaze.x, gaze.y, gaze.z, 0,
									0  , 0  , 0  , 1);
	changeBase = transpose(changeBase);

	dmat4x4 eye2origin = dmat4x4(	1, 0, 0, 0 - transforms.xPos,
									0, 1, 0, 0 - transforms.yPos,
									0, 0, 1, 0 - transforms.zPos,
									0, 0, 0, 1);
	
	eye2origin = transpose(eye2origin);

	dmat4x4 changeBaseINV = inverse(changeBase);
	dmat4x4 eye2originINV = inverse(eye2origin);
	
	camToWorld = eye2originINV * changeBaseINV;
}