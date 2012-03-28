#pragma once

#include "WorldRenderer.h"
#include "FluidRenderer.h"
#include "HapticRenderer.h"
#include "InputManager.h"

class RenderManager
{
private:
	cWorld* world;

	WorldRenderer worldRenderer;
	FluidRenderer fluidRenderer;
	HapticRenderer hapticRenderer;

	InputManager inputManager;

public:
	RenderManager();
	~RenderManager();

	void Initialize();
	void RunSimulation();
	
};