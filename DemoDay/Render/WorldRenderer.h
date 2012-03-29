#pragma once

#include "chai3d.h"
#include "WorldTransformation.h"

#define XAXIS		0
#define YAXIS		1
#define ZAXIS		2

class WorldRenderer
{
private:
	cWorld* world;
	cCamera* camera;
	cLight* light;

	cShapeLine* axes[3];	//As is the plural of "axis"

	void RenderAxes(int);
	void InitCamera(cMatrix3d);
	void InitLight(void);

public:
	WorldRenderer(void);
	~WorldRenderer(void);
	
	void InitWorld(cWorld*, cMatrix3d);
	void SetCamera(cMatrix3d);
	void RenderCamera(int, int);
};