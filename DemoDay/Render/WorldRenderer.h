#pragma once

#include "chai3d.h"
#include "WorldTransformation.h"

#define XAXIS		0
#define YAXIS		1
#define ZAXIS		2
#define AXIS_LEN	10
#define WORLD_BOUND	50

class WorldRenderer
{
private:
	cWorld* world;
	cCamera* camera;
	cLight* light;

	cShapeLine* axes[3];	//As is the plural of "axis"
	cMesh* skybox[5];		//Render a skybox
	cMesh* ground;			//Render the Ground

	void RenderAxes(int);
	void RenderSky(int);
	void RenderGround(int);

	void InitCamera(cMatrix3d);
	void InitLight(void);

public:
	WorldRenderer(void);
	~WorldRenderer(void);
	
	void InitWorld(cWorld*, cMatrix3d);
	void SetCamera(cMatrix3d);
	void RenderCamera(int, int);
};