// Copyright 2007 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
// main.cpp
//
// Basic application showing use of HDAL.
// Haptic interaction with a ball.
// Graphic display of cursor.
// Selection among multiple devices

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <glut.h>
#include <vector>

#include <hdl/hdl.h>
#include <hdlu/hdlu.h>

// A structure to maintain data for each device and associated cursor
struct HapticDevice
{
    HDLDeviceHandle handle;
    double cursorPos[3];
    double force[3];
    bool   button;
    GLfloat* color;
};

// Cursor colors
GLfloat CURSOR_BLUE[3]   = {0.0, 0.0, 1.0};
GLfloat CURSOR_RED[3]    = {1.0, 0.0, 0.0};
GLfloat CURSOR_GREEN[3]  = {0.0, 1.0, 0.0};
GLfloat CURSOR_YELLOW[3] = {1.0, 1.0, 0.0};
GLfloat CURSOR_ORANGE[3] = {1.0, 0.5, 0.0};
GLfloat* gColorChoice[4] = {CURSOR_BLUE, CURSOR_RED, CURSOR_GREEN, CURSOR_YELLOW};
char*   gColorName[4] = {"Blue", "Red", "Green", "Yellow"};

bool    gSelectionDone = true;

typedef std::vector<HapticDevice> HDContainer;
HDContainer g_device;

HDLServoOpExitCode g_touchOp = HDL_INVALID_HANDLE;
HDLDeviceHandle g_uninitHandle;


double gLastTime = 0;
double gCursorScale;
GLuint gCursorDisplayList = 0;

// These two constants control the relative sizes of ball and cursor.
const double gCursorRadius = 0.005;
const double gBallRadius = 0.025;

int    gNumCursors = 0;
int    gNumDevices = 0;
double gBallPosition[3] = {0, 0, 0};
double gBallVelocity[3] = {0, 0, 0};

// These two constants tune the ball dynamics.
const double gBallMass = 2500;
const double gBallViscosity = 80;

// This controls the cursor/ball force interaction
const double gBallStiffness = 50000.0;

void glutDisplay(void);
void glutReshape(int width, int height);
void glutIdle(void);    
void glutKeyboard(unsigned char key, int x, int y);

void exitHandler(void);

void initGL();
void initHDL();
void initScene();

void drawGraphics();
void drawCursor(HapticDevice& device);

void testHDLError(const char* str);


namespace 
{
double getSystemTime()
{
    static double s_wavelength = 0.0;
    static double s_wavelength_x_high = 0.0;
    static bool s_isFirstTime = true;

    if (s_isFirstTime)
    {
        LARGE_INTEGER l_frequency = { 0 };
        ::QueryPerformanceFrequency(&l_frequency);
        s_wavelength = 1.0 / double(l_frequency.LowPart);
        s_wavelength_x_high = s_wavelength * double(MAXDWORD);
        s_isFirstTime = false;
    }

    LARGE_INTEGER l_ticks = { 0 };
    ::QueryPerformanceCounter(&l_ticks);

    return s_wavelength_x_high * double(l_ticks.HighPart) +
           s_wavelength * double(l_ticks.LowPart);
}
};

int main(int argc, char *argv[])
{
    // Check command line.
    // This is not intended as a model for how to get command line options.

    if (argc > 1)
    {
        gNumCursors = atoi(argv[1]);
    }

    // Check to see if we have enough devices connected to support the
    // user's request.

    gNumDevices = hdlCountDevices();
    if (gNumDevices < gNumCursors)
    {
        printf("You requested %d devices, but only %d are connected.\n",
                gNumCursors, gNumDevices);
        printf("Continuing with %d devices/cursors.\n", gNumDevices);
        gNumCursors = gNumDevices;
    }

    // If we are in two-device mode and don't have two devices, just quit.
    if (gNumCursors == 0 && gNumDevices < 2) 
    {
        printf("You do not have enough Falcons connected.  I quit.\n");
        exit(1);
    }

    // General OpenGL and GLUT initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("HDAL Multi Demo");

    // Move graphic window away from console so user can see output
    glutPositionWindow(100, 500);

    glutDisplayFunc(glutDisplay);
    glutReshapeFunc(glutReshape);
    glutIdleFunc(glutIdle);
    glutKeyboardFunc(glutKeyboard);
   
    // Simple way to handle all exits
    atexit(exitHandler);

    // Specific graphic and haptic setup
    initScene();

    // Now do it.  We never return from here.
    glutMainLoop();

    // This keeps the compiler from whining about no return statement.
    return 0;
}

// Handle keyboard.
void glutKeyboard(unsigned char key, int x, int y)
{
    static bool inited = true;

    // Enable the Escape key to exit the app
    if (key == 27) // esc key
    {
        // This looks nasty, but remember we set up the exit handler
        // during initialization.
        exit(0);
    }

    // Enable the 'r' key to reset the ball.  This should really be
    // smarter, since this simple-minded method will likely bump a
    // cursor.
    if (key == 'r')
    {
        for (int i = 0; i < 3; i++)
        {
            gBallPosition[i] = 0;
            gBallVelocity[i] = 0;
        }
    }


}

// draw function
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

void glutIdle()
{
    glutPostRedisplay();
}

void initScene()
{
    initGL();
    initHDL();
}

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

// This is the workhorse function to calculate the force exerted
// on the cursor due to contact with the ball.
void ballToolContact(double pos[3], double force[3])
{
    const double stiffness = 200.0;

    force[0] = 0; force[1] = 0; force[2] = 0;

    // Get cursor position relative to ball
    double rpos[3];
    rpos[0] = pos[0] - gBallPosition[0];
    rpos[1] = pos[1] - gBallPosition[1];
    rpos[2] = pos[2] - gBallPosition[2];

    // Force is proportional to distance, if touching ball, else zero
    double toolDist = rpos[0]*rpos[0] + rpos[1]*rpos[1] + rpos[2]*rpos[2];
    toolDist = sqrt(toolDist);

    if (toolDist < gBallRadius)
    {
        double penetrationDist = gBallRadius - toolDist;
        double forceScale = penetrationDist * gBallStiffness;

        force[0] = rpos[0] * forceScale;
        force[1] = rpos[1] * forceScale;
        force[2] = rpos[2] * forceScale;
    }
}

// This is the callback function that is exercised at servo rate (1KHz)
HDLServoOpExitCode touchScene(void* pUserData)
{
    double posDC[3];
    double posWC[3];
    double force[3];

    // Initialize time
    if (gLastTime == 0) gLastTime = getSystemTime();

    if (gSelectionDone)
    {
        for (HDContainer::iterator it = g_device.begin(); it != g_device.end(); ++it)
        {
            // Calculate and set forces for each device that has not
            // been uninited.
            if ((*it).handle != HDL_INVALID_HANDLE)
            {
                hdlMakeCurrent((*it).handle);

                hdlToolPosition(posDC);
                testHDLError("hdlToolPosition");
                posWC[0] = posDC[0];
                posWC[1] = posDC[1];
                posWC[2] = posDC[2];

                ballToolContact(posWC, force);
                hdlSetToolForce(force);
                for (int i = 0; i < 3; i++)
                    (*it).force[i] = force[i];
                testHDLError("hdlSetToolForce");
            }

        }

        // We have the forces, now compute a new ball position
        // Position algorithm:
        //    a = (F - zeta*v)/m    (F = m*a + zeta*v)
        //    v = a*dt + v0
        //    p = v*dt + p0

        double forceSum[3] = {0, 0, 0};
        double acc[3] = {0, 0, 0};
        int i;

        // Start by computing reaction force F from sum of all cursor forces
        for (HDContainer::iterator iter = g_device.begin(); iter != g_device.end(); ++iter)
            for (i = 0; i < 3; i++)
                forceSum[i] -= (*iter).force[i];

        // Add in viscosity force to get (F - zeta*v)
        for (i = 0; i < 3; i++)
            forceSum[i] -= gBallViscosity * gBallVelocity[i];

        // Divide by mass for a = (F - zeta*v)/m
        for (i = 0; i < 3; i++)
            acc[i] = forceSum[i] / gBallMass;

        // Now do Euler integration
        double thisTime = getSystemTime();
        double dt = thisTime - gLastTime;

        for (i = 0; i < 3; i++)
            gBallVelocity[i] += acc[i] * dt;
        for (i = 0; i < 3; i++)
            gBallPosition[i] += gBallVelocity[i] * dt;

        gLastTime = thisTime;

        // Uncomment the following to play around with ball dynamics
        // and see the results
        /*
        printf("%8.4f %8.4f %8.4f %8.4f %8.4f %8.4f\n",
            gBallVelocity[0], gBallVelocity[1], gBallVelocity[2],
            gBallPosition[0], gBallPosition[1], gBallPosition[2]);
        */
    }
    return HDL_SERVOOP_CONTINUE;
}

// Check HDAL error and report with user-defined string
void testHDLError(const char* str)
{
    HDLError err = HDL_NO_ERROR;
    err = hdlGetError();
    if (err != HDL_NO_ERROR)
    {
        char msg[200];
        sprintf(msg, "HDAL ERROR %d", err);
        MessageBox(NULL, str, msg, MB_OK);
        exit(1);
    }
}

// Initialize device by hdal.ini section name
void initDevice(const char* deviceName, GLfloat* color = CURSOR_ORANGE)
{
    HDLError err = HDL_NO_ERROR;

    HapticDevice hd;
    hd.handle = hdlInitNamedDevice(deviceName);
    hd.color = color;
    for (int i = 0; i < 3; i++)
    {
        hd.force[i] = 0;
    }
    char msg[200];
    sprintf(msg, "Cannot init %s", deviceName);
    testHDLError(msg);
    if (hd.handle < 0)
    {
        MessageBox(NULL, msg, "Named Initialization error", MB_OK);
        abort();
    }

    g_device.push_back(hd);
}

// Initialize device by index into (hidden) device list.
// Note that the application does not have access to list of device
// serial numbers.  It only knows how many devices are in the list.
void initIndexedDevice(const int ndx,  GLfloat* color = CURSOR_ORANGE)
{
    HDLError err = HDL_NO_ERROR;

    HapticDevice hd;
    hd.handle = hdlInitIndexedDevice(ndx);
    hd.color = color;
    for (int i = 0; i < 3; i++)
    {
        hd.force[i] = 0;
    }
    char msg[200];
    sprintf(msg, "Cannot init %d", ndx);
    testHDLError(msg);
    if (hd.handle < 0)
    {
        MessageBox(NULL, msg, "Indexed Initialization error", MB_OK);
        abort();
    }

    g_device.push_back(hd);
}

// Get all the devices' state information
HDLServoOpExitCode GetStateCB(void* pUserData)
{
    for (HDContainer::iterator it = g_device.begin(); it != g_device.end(); ++it)
    {
        if ((*it).handle != HDL_INVALID_HANDLE)
        {
            HapticDevice& hd = *it;

            hdlMakeCurrent(hd.handle);
            hdlToolPosition(&(hd.cursorPos[0]));
            hdlToolButton(&(hd.button));
            testHDLError("hdlToolPosition");
        }
    }

    return HDL_SERVOOP_EXIT;
}

// This is the entry point used by the application to synchronize
// data access to the device.  Using this function eliminates the 
// need for the application to worry about threads.
void synchFromServo()
{
    hdlCreateServoOp(GetStateCB, 0, true);
}

// Wait for user to press, then release, a button on a device.
// Return index into application's list of handles.  Skip devices
// that have already been selected.  Use orange color to indicate
// devices that have not yet been selected.
int waitForButton()
{
    int id = 0;

    // Rotate through all attached devices looking for a button press
    while (true)
    {
        if (g_device[id].handle != HDL_INVALID_HANDLE && 
            g_device[id].color == CURSOR_ORANGE)
        {
            synchFromServo();
            if (g_device[id].button) break;
        }

        if (++id == gNumDevices) id = 0;

    }

    // Got a button, now wait until it's released
    while (g_device[id].button)
    {
        // Cheap debouncing
        synchFromServo();
        Sleep(100);
    }

    return id;
}

/* initHDL() sets up the HDAL connections.
   The main purpose is to associate specific devices with application functions.
   In this case, the functions are colored cursors.
   Option 1: No command line parameters
             Use named sections in hdal.ini.  Requires device SNs
   Option 2: Execution-time selection
             Number of cursors to use is passed on the command line and
             remembered in global variable gNumCursors.
             Prompt the user to press a button on the device to use for
             control of specified cursors.  Uninitialize left-over
             devices if there are any. */
void initHDL()
{

    
    if (gNumCursors == 0)
    {
        // Option 1: Use device names that match your config file
        initDevice("FALCON_1", CURSOR_RED);
        initDevice("FALCON_2", CURSOR_YELLOW);
        hdlStart();
    }
    else
    {
        // Option 2: Let user select from connected devices.
        // Start up required devices
        gSelectionDone = false;
        for (int i = 0; i < gNumDevices; i++)
        {
            initIndexedDevice(i);
        }
        hdlStart();
        Sleep(1000);  // Wait for data to stabilize

        // It is possible to have more devices connected than needed
        for (int ic = 0; ic < gNumCursors; ic++)
        {
            printf("Press a button on the Falcon you wish to use to control the %s cursor: ",
                gColorName[ic]);
            int id = waitForButton();
            printf("%d\n", id);
            hdlMakeCurrent(g_device[id].handle);
            g_device[id].color = gColorChoice[ic];
        }

        // Now we know which devices are used, let's delete the ones not used.
        // Note that one of the selected devices is current, and all selected devices
        //   have colors other than orange.
        for (HDContainer::iterator it = g_device.begin(); it != g_device.end(); it++)
        {
            if ((*it).color == CURSOR_ORANGE)
            {
                hdlUninitDevice((*it).handle);
                (*it).handle = HDL_INVALID_HANDLE;
            }
        }
        

        gSelectionDone = true;
    }

    const bool bBlocking = false;
    g_touchOp = hdlCreateServoOp(touchScene,
                                 (void*)0,
                                 bBlocking);
    testHDLError("hdlCreateServoOp");
}

void exitHandler()
{
    if (g_touchOp != HDL_INVALID_HANDLE)
    {
        hdlDestroyServoOp(g_touchOp);
        g_touchOp = HDL_INVALID_HANDLE;
    }
    hdlStop();

    for (HDContainer::iterator it = g_device.begin(); it != g_device.end(); ++it)
    {
        if ((*it).handle != HDL_INVALID_HANDLE)
            hdlUninitDevice((*it).handle);
    }
}

void drawGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           

    for (HDContainer::iterator it = g_device.begin(); it != g_device.end(); ++it)
    {
        if ((*it).color != CURSOR_ORANGE)
            drawCursor(*it);
    }

    glPushMatrix();
    glTranslatef(gBallPosition[0], gBallPosition[1], gBallPosition[2]);
    glutSolidSphere(gBallRadius, 32, 32);
    glPopMatrix();
}

void drawCursor(HapticDevice& device)
{
    static const int kCursorTess = 15;
    synchFromServo();
    testHDLError("hdlCreateServoOp");

    // transform cursor position from device coordinates
    // to world coordinates
    double cursorPosWC[3];
    cursorPosWC[0] = device.cursorPos[0];
    cursorPosWC[1] = device.cursorPos[1];
    cursorPosWC[2] = device.cursorPos[2];

    GLUquadricObj *qobj = 0;

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushMatrix();

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
    if (device.button)
    {
        glColor3fv(CURSOR_ORANGE);
    }
    else
    {
        glColor3fv(device.color);
    }

    glCallList(gCursorDisplayList);

    glPopMatrix(); 
    glPopAttrib();
}
