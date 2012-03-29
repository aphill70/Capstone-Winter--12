#pragma once

#include "WorldRenderer.h"
#include "FluidRenderer.h"
#include "HapticRenderer.h"
#include "InputManager.h"
#include "chai3d.h"
#include <iostream>
#include "IFluid.h"


#define	WINDOW_SIZE_W	600
#define	WINDOW_SIZE_H	600

#define INIT_HEADING	225.0
#define INIT_ELEVATION	45.0
#define INIT_POS		cVector3d(10, 10, 10)

using namespace std;

//A Singleton object
class RenderManager
{
private:

	cWorld* world;

	// rendering classes
	WorldRenderer worldRenderer;
	FluidRenderer fluidRenderer;
	HapticRenderer hapticRenderer;

	// input classes
	InputManager inputManager;

	// model classes
	IFluid * fluidModel;


	int displayW, displayH;
	bool simRunning, simEnded;

	RenderManager();

	static void GLResizeWindow(int width, int height);
	static void GLUpdateGraphics(void);
	static void GLMouseMotion(int x, int y);
	static void GLKeyDown(unsigned char key, int x, int y);

	void InitializeGlut(void);
	void UpdateGraphics(void);
	void ResizeWindow(int, int);
	void MouseMotion(int x, int y);

	void InitModels(void);
public:
	
	~RenderManager(void);

	static RenderManager& getInstance()
	{
		static RenderManager instance;

		return instance;
	}

	RenderManager(RenderManager const&);
	void operator=(RenderManager const&);

	void Initialize();
	void RunSimulation();
	void EndSimulation();
};