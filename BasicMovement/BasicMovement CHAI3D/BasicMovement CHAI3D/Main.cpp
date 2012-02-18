#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chai3d.h"
#include "ControlManager.h"

const int WINDOW_SIZE_W         = 600;
const int WINDOW_SIZE_H         = 600;

//Vars
cWorld* world;
cCamera* camera;
cLight* light;
cShapeSphere* object0;
cShapeSphere* object1;
cShapeTorus* object2;
cShapeSphere* object3;
cShapeSphere* followme;

cShapeLine *XAxis,*YAxis,*ZAxis;

bool simulationRunning = false;

ControlManager controlManager;

int displayW  = 0;
int displayH  = 0;

void resizeWindow(int w, int h);
void updateGraphics(void);
GLvoid KeyDown(unsigned char key, int x, int y);
GLvoid KeyUp(unsigned char key, int x, int y);
GLvoid MouseMotion(int x, int y);
GLvoid MouseClick(int button, int state, int x, int y);

void BuildAxis() 
{

	XAxis = new cShapeLine(cVector3d(-5.0,0.0,0.0),cVector3d(5.0,0.0,0.0));
	world->addChild(XAxis);
	XAxis->m_ColorPointA = cColorf(0.0,0.0,0.3);
	XAxis->m_ColorPointB = cColorf(0.7,0.7,1.0);

	YAxis = new cShapeLine(cVector3d(0.0,-5.0,0.0),cVector3d(0.0,5.0,0.0));
	world->addChild(YAxis);
	YAxis->m_ColorPointA = cColorf(0.0,0.3,0.0);
	YAxis->m_ColorPointB = cColorf(0.7,1.0,0.7);

	ZAxis = new cShapeLine(cVector3d(0.0,0.0,-5.0),cVector3d(0.0,0.0,5.0));
	world->addChild(ZAxis);
	ZAxis->m_ColorPointA = cColorf(0.3,0.0,0.0);
	ZAxis->m_ColorPointB = cColorf(1.0,0.7,0.7);
}

int main(int argc, char* argv[])
{
	cMaterial orange;
	orange.m_ambient.set(0.5, 0.2, 0.0);
    orange.m_diffuse.set(1.0, 0.5, 0.0);
    orange.m_specular.set(1.0, 1.0, 1.0);

	world = new cWorld();
	world->setBackgroundColor(0.0,0.0,0.0);
	
	camera = new cCamera(world);
	world->addChild(camera);

	camera->set(cVector3d(5.0,5.0,5.0),
				cVector3d(0.0,0.0,0.0),
				cVector3d(0.0,1.0,0.0));

	camera->enableMultipassTransparency(true);

	light = new cLight(world);
	camera->addChild(light);
	light->setEnabled(true);
	light->setPos(cVector3d(2.0, 0.5, 1.0));
	light->setPos(cVector3d(-2.0, 0.5, 1.0));

	BuildAxis();

	/*object0 = new cShapeSphere(0.3);
	world->addChild(object0);
	object0->setPos(0.0, 0.0 , 1.0);*/

	object1 = new cShapeSphere(.1);
	world->addChild(object1);
	object1->setPos(1.0 , 0.0, 0.0);

	object2 = new cShapeTorus(.3,.5);
	world->addChild(object2);
	object2->setPos(0.0, 0.0, -1.0);

	object3 = new cShapeSphere(.1);
	world->addChild(object3);
	object3->setPos(-1.0, 0.0, 0.0);

	followme = new cShapeSphere(0.1);
	followme->m_material = orange;
	world->addChild(followme);
	followme->setPos(0.0,0.0,1.0);
	
	glutInit(&argc, argv);

	int screenW = glutGet(GLUT_SCREEN_WIDTH);
	int screenH = glutGet(GLUT_SCREEN_HEIGHT);

	int windowPosX = (screenW - WINDOW_SIZE_W) / 2;
	int windowPosY = (screenH - WINDOW_SIZE_H) / 2;

    glutInitWindowPosition(windowPosX, windowPosY);
    glutInitWindowSize(WINDOW_SIZE_W, WINDOW_SIZE_H);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(updateGraphics);
	glutIdleFunc(updateGraphics);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);

    glutSetWindowTitle("CHAI 3D Movement Test");

	simulationRunning = true;

    // start the main graphics rendering loop
    glutMainLoop();
}

void resizeWindow(int w, int h)
{
    // update the size of the viewport
    displayW = w;
    displayH = h;
    glViewport(0, 0, displayW, displayH);
}

void updateGraphics(void)
{
	controlManager.UpdateCamera(camera);

	cVector3d pos = camera->getPos();
	cVector3d at = camera->getLookVector();
	cVector3d up = camera->getUpVector();
	//printf("(%.1f %.1f %.1f)(%.4f %.4f %.4f)(%.1f %.1f %.1f)\n",pos.x, pos.y, pos.z, at.x, at.y, at.x, up.x, up.y, up.z);
	
	// render world
    camera->renderView(displayW, displayH);

    // Swap buffers
    glutSwapBuffers();

    // check for any OpenGL errors
    GLenum err;
    err = glGetError();
    if (err != GL_NO_ERROR) printf("Error:  %s\n", gluErrorString(err));

    // inform the GLUT window to call updateGraphics again (next frame)
    if (simulationRunning)
    {
        glutPostRedisplay();
    }
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