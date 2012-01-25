// Copyright 2007 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// Basic application showing use of HDAL.
// Haptic interaction with a ball.
// Graphic display of cursor.
//
//
// Comment the following line to get a console window on startup
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

// windows.h must precede glut.h to eliminate the "exit" compiler error in VS2003, VS2005
#include <windows.h>
#include "glut.h"
#include <math.h>
#include "haptics.h"

// Cube parameters
const double gStiffness = 5000.0;
const double gCubeEdgeLength = 0.05;

// Some OpenGL values
static GLuint gCursorDisplayList = 0;
static double gCursorRadius = 0.01;
static GLfloat colorRed[]  = {1.0, 0.0, 0.0};
static GLfloat colorTeal[] = {0.0, 0.5, 0.5};
static GLfloat* gCurrentColor;

// The haptics object, with which we must interact
HapticsClass gHaptics;

// Forward declarations
void glutDisplay(void);
void glutReshape(int width, int height);
void glutIdle(void);
void glutKeyboard(unsigned char key, int x, int y);
void exitHandler(void);
void initGL();
void initScene();
void drawGraphics();
void drawCursor();

int main(int argc, char *argv[])
{

    // Normal OpenGL Setup
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Basic--OpenGL");
    glutDisplayFunc(glutDisplay);
    glutReshapeFunc(glutReshape);
    glutIdleFunc(glutIdle);
    glutKeyboardFunc(glutKeyboard);
    
    // Set up handler to make sure teardown is done.
    atexit(exitHandler);

    // Set up the scene with graphics and haptics
    initScene();

    // Now start the simulation
    glutMainLoop();

    return 0;
}

// drawing callback function
void glutDisplay()
{   
    drawGraphics();
    glutSwapBuffers();
}

// reshape function (handle window resize)
void glutReshape(int width, int height)
{
    static const double kPI = 3.1415926535897932384626433832795;
    static const double kFovY = 40;

    double nearDist, farDist, aspect;

    glViewport(0, 0, width, height);

    nearDist = 0.075 / tan((kFovY / 2.0) * kPI / 180.0);
    farDist = nearDist + 0.05;
    aspect = (double) width / height;
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(kFovY, aspect, nearDist, farDist);

    /* Place the camera down the Z axis looking at the origin */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    gluLookAt(0, 0, nearDist + 0.06,
              0, 0, 0,
              0, 1, 0);
}

// What to do when doing nothing else
void glutIdle()
{
    glutPostRedisplay();
}


// Handle keyboard.  In this case, just the Escape key
void glutKeyboard(unsigned char key, int x, int y)
{
    static bool inited = true;

    if (key == 27) // esc key
    {
        exit(0);
    }
}

// Scene setup
void initScene()
{

    // Call the haptics initialization function
    gHaptics.init(gCubeEdgeLength, gStiffness);

    // Set up the OpenGL graphics
    initGL();

    // Some time is required between init() and checking status,
    // for the device to initialize and stabilize.  In a complex
    // application, this time can be consumed in the initGL()
    // function.  Here, it is simulated with Sleep().
    Sleep(100);

    // Tell the user what to do if the device is not calibrated
    if (!gHaptics.isDeviceCalibrated())
        MessageBox(NULL, 
                   // The next two lines are one long string
                   "Please home the device by extending\n"
                   "then pushing the arms all the way in.",
                   "Not Homed",
                   MB_OK);


}

// Set up OpenGL.  Details are left to the reader
void initGL()
{
    static const GLfloat light_model_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    static const GLfloat light0_diffuse[] = {0.9f, 0.9f, 0.9f, 0.9f};   
    static const GLfloat light0_direction[] = {0.0f, -0.4f, 1.0f, 0.0f};    
    
    /* Enable depth buffering for hidden surface removal. */
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    /* Cull back faces. */
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    /* Set lighting parameters */
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glEnable(GL_LIGHT0);   
}

// Make sure we exit cleanly
void exitHandler()
{
    gHaptics.uninit();
}

// Draw the cursor and the cube.  In a real application,
// this function would be much more complex.
void drawGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           

    drawCursor();
    glutSolidCube(gCubeEdgeLength);
}

// Draw the cursor
void drawCursor()
{
    static const int kCursorTess = 15;

    // Haptic cursor position in "world coordinates"
    double cursorPosWC[3];

    // Must synch before data is valid
    gHaptics.synchFromServo();
    gHaptics.getPosition(cursorPosWC);

    // The color will depend on the button state.
    gCurrentColor = gHaptics.isButtonDown() ? colorRed : colorTeal;

    GLUquadricObj *qobj = 0;

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushMatrix();

    // Page 505-506 of The Red Book recommends using a display list
    // for static quadrics such as spheres
    if (!gCursorDisplayList)
    {
        gCursorDisplayList = glGenLists(1);
        glNewList(gCursorDisplayList, GL_COMPILE);
        qobj = gluNewQuadric();

        gluSphere(qobj, gCursorRadius, kCursorTess, kCursorTess);
        
        gluDeleteQuadric(qobj);
        glEndList();
    }

    glTranslatef(cursorPosWC[0], cursorPosWC[1], cursorPosWC[2]);

    glEnable(GL_COLOR_MATERIAL);
    glColor3fv(gCurrentColor);

    glCallList(gCursorDisplayList);

    glPopMatrix(); 
    glPopAttrib();
}
