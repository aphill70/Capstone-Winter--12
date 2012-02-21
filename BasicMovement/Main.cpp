// Main.cpp : The entry point for the console application.

#include <time.h>
#include <windows.h>
#include "chai3d.h"

//TODO: !! Figure out how to load the Falcon code !!
//#include "hdFalcon

#include "Globals.h"
#include "ControlManager.h"

#include <gl/glut.h>

#if !defined (_MSC_VER)
using namespace std;
#endif
// Defines

// Function prototypes
GLvoid InitGL(GLvoid);
GLvoid InitLighting(GLvoid);
GLvoid DrawGLScene(GLvoid);
GLvoid IdleGLScene(GLvoid);
GLvoid ReSizeGLScene(int width, int height);
GLvoid KeyDown(unsigned char key, int x, int y);
GLvoid KeyUp(unsigned char key, int x, int y);
GLvoid MouseMotion(int x, int y);
GLvoid MouseClick(int button, int state, int x, int y);
void InitHapticDevice(void);
void UpdateHaptics(void);

// Globals
clock_t lastTickTime;
ControlManager controlManager;

// Haptic globals
cHapticDeviceHandler* handler;

int main(int argc, char* argv[]){
	
	glutInit(&argc, argv);

	InitGL();
	InitHapticDevice();

	/* Gets the loop rolling */
	glutMainLoop();

	return 0;
}

// This function will not return until the device is centered
void CenterHapticDevice(void) {
	cVector3d initPos(0, 0, 0);
	while (true) {
		cVector3d currPos;
		cVector3d linVeloc;
		hapticDevice->getPosition(currPos);
		hapticDevice->getLinearVelocity(linVeloc);

		double remainingDist = currPos.length();
		double currentSpeed = linVeloc.length();

		if (remainingDist < 0.013 && currentSpeed < 0.0001) {
			if (remainingDist < 0.006) {
				return;
			}

			// Render a force to overcome static friction
			currPos.negate();
			currPos.mul(1000);	// Needs a huge multiplier because distance is so small
		} else {
			// Render a rather large force
			currPos.negate();
			currPos.mul(80);

			// Add more speed if the cursor is stuck
			if (currentSpeed < 0.001) {
				currPos.mul(20);
			}
		}
		hapticDevice->setForce(currPos);
	}
}

void InitHapticDevice(void) {
	handler = new cHapticDeviceHandler();
	handler->getDevice(hapticDevice, 0);
	hapticDevice->open();
	hapticDevice->initialize(false);

	cHapticDeviceInfo specs = hapticDevice->getSpecifications();
	MAX_HAPTIC_FORCE = specs.m_maxForce;
	HAPTIC_RADIUS = specs.m_workspaceRadius;

	CenterHapticDevice();

	cThread* hapticsThread = new cThread();
    hapticsThread->set(UpdateHaptics, CHAI_THREAD_PRIORITY_HAPTICS);
}

GLvoid InitGL(){
	// Setup for window
	/* setting up double buffering rbg and depth */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(700, 100);
	/* this name needs to match the name of the window in DrawGLScene */
	glutCreateWindow(windowName);

	// Setup shading variables/states
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0, 0, 0, 0);							// Sky Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);							// Enable antialiasing
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Setup lighting
	InitLighting();

	// Setup event handlers
	/* the draw function will draw once and no more */
	glutDisplayFunc(DrawGLScene);
	/* this is the idle function it gets called as much as possible */
	glutIdleFunc(IdleGLScene);
	/* This is called everytime the window is altered */
	glutReshapeFunc(ReSizeGLScene);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutPassiveMotionFunc(MouseMotion);
	glutMotionFunc(MouseMotion);
	glutMouseFunc(MouseClick);
	// Setup for animation
	lastTickTime = clock();

	// Put mouse in starting position
	glutWarpPointer(windowWidth / 2.0, windowHeight / 2.0);
	glutSetCursor(GLUT_CURSOR_NONE);
}


GLvoid InitLighting(GLvoid) {
	glEnable(GL_LIGHTING);
	
	// CraSun
	glEnable(GL_LIGHT0);

	GLfloat sunAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat sunDiffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat sunSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat sunPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
}


GLvoid DrawCube(int x, int y, int z) {
	glPushMatrix();
	glTranslatef(x, y, z);

	glBegin(GL_QUADS); 
	{
		// Draw a cube

		// Front
		glVertex3f(1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, -1, 1);
		glVertex3f(1, -1, 1);

		// Top
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, 1, 1);

		// Bottom
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, -1, -1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, -1, 1);
		
		// Left
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, -1, 1);

		// Right
		glVertex3f(1, -1, 1);
		glVertex3f(1, -1, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, 1, 1);

		// Back
		glVertex3f(1, -1, -1);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);
		glVertex3f(1, 1, -1);
	}
	glEnd();

	glPopMatrix();
}

GLvoid DrawGLScene(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
		
	/* This is your normal draw code. */
	glViewport(0, 0, windowWidth, windowHeight);
	
	controlManager.UpdateWorldTransformations();

	glColor3f(0.5, 0.5, 0.5);
	DrawCube(0, 0, 0);
	glColor3f(1.0, 0.3, 0.3);
	DrawCube(30, 0, 0);
	glColor3f(0.2, 0.05, 0.05);
	DrawCube(-30, 0, 0);
	glColor3f(0.3, 1.0, 0.3);
	DrawCube(0, 30, 0);
	glColor3f(0.05, 0.2, 0.05);
	DrawCube(0, -30, 0);
	glColor3f(0.3, 0.3, 1.0);
	DrawCube(0, 0, 30);
	glColor3f(0.05, 0.05, 0.2);
	DrawCube(0, 0, -30);

	glFlush();
	glutSwapBuffers();
}

/* checks for joystick input then draws */
GLvoid IdleGLScene(){
	UpdateHaptics();
	// Cap at 30 fps
	clock_t curTime = clock();
	double millis = curTime - lastTickTime;
#if CLOCKS_PER_SEC != 1000
	millis = millis / CLOCKS_PER_SEC * 1000.0;
#endif

	if ( millis  < FRAME_INTERVAL) {
		return;
	}
	lastTickTime = curTime;

	DrawGLScene();
}

/* this gets called everytime your window resizes or moves */
GLvoid ReSizeGLScene(int width, int height){
	if(height == 0)
		height = 1;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 2000.0f);

	windowWidth = width;
	windowHeight = height;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLvoid KeyDown(unsigned char key, int x, int y) { 
	controlManager.HandleKeyDown(key, x, y); 
}

GLvoid KeyUp(unsigned char key, int x, int y) { 
	controlManager.HandleKeyUp(key, x, y); 
}

GLvoid MouseMotion(int x, int y) { 
	controlManager.HandleMouseMotion(x, y); 
}

GLvoid MouseClick(int button, int state, int x, int y) {
	controlManager.HandleMouseClick(button, state, x, y); 
}

void UpdateHaptics(void) {
	cVector3d updatedPosition;
	hapticDevice->getPosition(updatedPosition);

	controlManager.GetHapticInput().SetPosition(updatedPosition);
}