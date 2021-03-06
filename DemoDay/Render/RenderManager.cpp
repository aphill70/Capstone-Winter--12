#include "RenderManager.h"
#include "TestOneDirectionFluid.h"

RenderManager::RenderManager()
{
	world = NULL;
	displayW = 0;
	displayH = 0;
	simRunning = false;
	simEnded = false;
}

RenderManager::~RenderManager()
{
	//delete world;
}

void RenderManager::Initialize(IFluid * fluid)
{
	fluidModel = fluid;
	world = new cWorld();
	world->setBackgroundColor( 0, 0, 0);

	inputManager.InitTransforms(INIT_HEADING, INIT_ELEVATION,
								INIT_POS);
	cMatrix3d cam = inputManager.GetCameraTransformations();
	inputManager.SetWindowDimensions(WINDOW_SIZE_W, WINDOW_SIZE_H);
	
	worldRenderer.InitWorld(world, cam);
	
	hapticRenderer.InitCursor(world);
	cGenericObject* cursor = hapticRenderer.GetCursor();
	inputManager.SetHapticCursor(cursor);

	fluidRenderer.InitFluids(world, fluidModel);

	InitializeGlut();

}

void RenderManager::InitializeGlut()
{
	int argc = 1;
	char *argv[] = {"Capstone", NULL};
	glutInit(&argc, argv);

	// retrieve the resolution of the computer display and estimate the position
    // of the GLUT window so that it is located at the center of the screen
    int screenW = glutGet(GLUT_SCREEN_WIDTH);
    int screenH = glutGet(GLUT_SCREEN_HEIGHT);

    int windowPosX = (screenW - WINDOW_SIZE_W) / 2;
    int windowPosY = (screenH - WINDOW_SIZE_H) / 2;

	// initialize the OpenGL GLUT window
    glutInitWindowPosition(windowPosX, windowPosY);
    glutInitWindowSize(WINDOW_SIZE_W, WINDOW_SIZE_H);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow(argv[0]);
	glutDisplayFunc(RenderManager::GLUpdateGraphics);	
	glutPassiveMotionFunc(RenderManager::GLMouseMotion);
	glutMotionFunc(RenderManager::GLMouseMotion);
	glutMouseFunc(RenderManager::GLMouseFunc);
	glutKeyboardFunc(RenderManager::GLKeyDown);
	glutKeyboardUpFunc(RenderManager::GLKeyUp);
	glutSpecialFunc(RenderManager::GLSpecialKeyDown);
	glutSpecialUpFunc(RenderManager::GLSpecialKeyUp);
    glutReshapeFunc(RenderManager::GLResizeWindow);
    glutSetWindowTitle("Haptic Fluid Render");

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(WINDOW_SIZE_W / 2, WINDOW_SIZE_H / 2);
}

void RenderManager::InitModels(void) {
	fluidModel = new TestOneDirectionFluid();
}

void RenderManager::ResizeWindow(int width, int height)
{
	displayW = width;
	displayH = height;
	inputManager.SetWindowDimensions(displayW, displayH);
	glViewport(0, 0, displayW, displayH);	
}

void RenderManager::UpdateGraphics()
{

	inputManager.UpdateTransforms();
	cMatrix3d cam = inputManager.GetCameraTransformations();
	worldRenderer.SetCamera(cam);

	fluidRenderer.UpdateFluid();

	//make sure to step other renderers and simulation
	worldRenderer.RenderCamera(displayW, displayH);
	
	glutSwapBuffers();

	GLenum err;
    err = glGetError();
    if (err != GL_NO_ERROR) printf("Error:  %s\n", gluErrorString(err));

	if(simRunning)
		glutPostRedisplay();
	else
		simEnded = true;
}

void RenderManager::GetHapticCursorPosition(cVector3d& c)
{
	inputManager.GetHapticCursorPosition(c);
}

cVector3d RenderManager::RotateVector(const cVector3d& vec)
{
	return inputManager.RotateVector(vec);
}

void RenderManager::MouseMotion(int x, int y)
{
	inputManager.MouseMotion(x, y);
}

void RenderManager::MouseClick(int button, int state, int x, int y)
{
	inputManager.MouseClick(button, state, x, y);
}

void RenderManager::KeyDown(unsigned char key, int x, int y)
{
	inputManager.KeyDown(key, x, y);
}

void RenderManager::KeyUp(unsigned char key, int x, int y)
{
	inputManager.KeyUp(key, x, y);
}


void RenderManager::RunSimulation()
{
	simRunning = true;
	glutMainLoop();
}

void RenderManager::EndSimulation()
{
	simRunning = false;
	while (!simEnded) { cSleepMs(100); }
	exit(0);
}