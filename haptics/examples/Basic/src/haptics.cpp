#include "haptics.h"
#include <windows.h>
#include <math.h>

#define WATER_DENSITY 00

// Continuous servo callback function
HDLServoOpExitCode ContactCB(void* pUserData)
{
    // Get pointer to haptics object
    HapticsClass* haptics = static_cast< HapticsClass* >( pUserData );

    // Get current state of haptic device
    hdlToolPosition(haptics->m_positionServo);
    hdlToolButton(&(haptics->m_buttonServo));

    // Call the function that does the heavy duty calculations.
    haptics->cubeContact();

    // Send forces to device
    hdlSetToolForce(haptics->m_forceServo);

    // Make sure to continue processing
    return HDL_SERVOOP_CONTINUE;
}

// On-demand synchronization callback function
HDLServoOpExitCode GetStateCB(void* pUserData)
{
    // Get pointer to haptics object
    HapticsClass* haptics = static_cast< HapticsClass* >( pUserData );

    // Call the function that copies data between servo side 
    // and client side
    haptics->synch();

    // Only do this once.  The application will decide when it
    // wants to do it again, and call CreateServoOp with
    // bBlocking = true
    return HDL_SERVOOP_EXIT;
}

// Constructor--just make sure needed variables are initialized.
HapticsClass::HapticsClass()
    : m_lastFace(FACE_NONE),
      m_deviceHandle(HDL_INVALID_HANDLE),
      m_servoOp(HDL_INVALID_HANDLE),
      m_cubeEdgeLength(1),
      m_cubeStiffness(1),
      m_inited(false)
{
    for (int i = 0; i < 3; i++)
        m_positionServo[i] = 0;
}

// Destructor--make sure devices are uninited.
HapticsClass::~HapticsClass()
{
    uninit();
}



void HapticsClass::init(double a_cubeSize, double a_stiffness)
{
    m_cubeEdgeLength = a_cubeSize;
    m_cubeStiffness = a_stiffness;


    HDLError err = HDL_NO_ERROR;

    // Passing "DEFAULT" or 0 initializes the default device based on the
    // [DEFAULT] section of HDAL.INI.   The names of other sections of HDAL.INI
    // could be passed instead, allowing run-time control of different devices
    // or the same device with different parameters.  See HDAL.INI for details.
    m_deviceHandle = hdlInitNamedDevice("DEFAULT");
    testHDLError("hdlInitDevice");

    if (m_deviceHandle == HDL_INVALID_HANDLE)
    {
        MessageBox(NULL, "Could not open device", "Device Failure", MB_OK);
        exit(0);
    }

    // Now that the device is fully initialized, start the servo thread.
    // Failing to do this will result in a non-funtional haptics application.
    hdlStart();
    testHDLError("hdlStart");

    // Set up callback function
    m_servoOp = hdlCreateServoOp(ContactCB, this, bNonBlocking);
    if (m_servoOp == HDL_INVALID_HANDLE)
    {
        MessageBox(NULL, "Invalid servo op handle", "Device Failure", MB_OK);
    }
    testHDLError("hdlCreateServoOp");

    // Make the device current.  All subsequent calls will
    // be directed towards the current device.
    hdlMakeCurrent(m_deviceHandle);
    testHDLError("hdlMakeCurrent");

    m_inited = true;
}

// uninit() undoes the setup in reverse order.  Note the setting of
// handles.  This prevents a problem if uninit() is called
// more than once.
void HapticsClass::uninit()
{
    if (m_servoOp != HDL_INVALID_HANDLE)
    {
        hdlDestroyServoOp(m_servoOp);
        m_servoOp = HDL_INVALID_HANDLE;
    }
    hdlStop();
    if (m_deviceHandle != HDL_INVALID_HANDLE)
    {
        hdlUninitDevice(m_deviceHandle);
        m_deviceHandle = HDL_INVALID_HANDLE;
    }
    m_inited = false;
}

// This is a simple function for testing error returns.  A production
// application would need to be more sophisticated than this.
void HapticsClass::testHDLError(const char* str)
{
    HDLError err = hdlGetError();
    if (err != HDL_NO_ERROR)
    {
        MessageBox(NULL, str, "HDAL ERROR", MB_OK);
        abort();
    }
}

// This is the entry point used by the application to synchronize
// data access to the device.  Using this function eliminates the 
// need for the application to worry about threads.
void HapticsClass::synchFromServo()
{
    hdlCreateServoOp(GetStateCB, this, bBlocking);
}

// GetStateCB calls this function to do the actual data movement.
void HapticsClass::synch()
{
    // m_positionApp is set in cubeContact().
    m_buttonApp = m_buttonServo;
}


// Here is where the heavy calculations are done.  This function is
// called from ContactCB to calculate the forces based on current
// cursor position and cube dimensions.  A simple spring model is
// used.
void HapticsClass::cubeContact()
{
    m_positionApp[0] = m_positionServo[0];
    m_positionApp[1] = m_positionServo[1];
    m_positionApp[2] = m_positionServo[2];

    m_forceServo[0] = WATER_DENSITY; 
    m_forceServo[1] = WATER_DENSITY; 
    m_forceServo[2] = WATER_DENSITY;

    // Skip the whole thing if not initialized
    if (!m_inited) return;

    const double halfCube = m_cubeEdgeLength/2.0;

    double radiusWC = 0.0;
    double pointLC[3];

    // Get the cursor position in Local Coordinates.
    pointLC[0] = m_positionApp[0];
    pointLC[1] = m_positionApp[1];
    pointLC[2] = m_positionApp[2];

    // expand the cube faces by the radius of the tool
    // gives sphere-cube force model
    double l_size = halfCube + radiusWC;

    // compute signed distance to each face
    // distance > 0 implies tool position penetrates cube
    // according the specific face
    const int faces = 6;
    double l_distance[faces];
    l_distance[FACE_NEAR]   = l_size - pointLC[2];
    l_distance[FACE_RIGHT]  = l_size - pointLC[0];
    l_distance[FACE_FAR]    = pointLC[2] + l_size;
    l_distance[FACE_LEFT]   = pointLC[0] + l_size;
    l_distance[FACE_TOP]    = l_size - pointLC[1];
    l_distance[FACE_BOTTOM] = pointLC[1] + l_size;

    // Keep track of which cube face the cursor is nearest to.
    int l_nearestFace = FACE_NONE;
    if (l_nearestFace == FACE_NONE)
    {
        l_nearestFace = FACE_NEAR;
        double l_minDistance = fabs(l_distance[FACE_NEAR]);
        for (int index = FACE_RIGHT; index <= FACE_BOTTOM; ++index)
        {
            if (fabs(l_distance[index]) < l_minDistance)
            {
                l_nearestFace = index;
                l_minDistance = fabs(l_distance[index]);
            }
        }
    }

    // have the nearest face
    // tool must be interior to the face for a collision
    if (l_distance[l_nearestFace] < 0)
    {
        m_lastFace = FACE_NONE;
		m_forceServo[0] = WATER_DENSITY; 
        m_forceServo[1] = WATER_DENSITY; 
        m_forceServo[2] = WATER_DENSITY;
        return;
    }

    // We know what we need to know.  Handle one of six possibilities:
    switch (l_nearestFace)
    {
        case FACE_NEAR:
        if (fabs(pointLC[0]) < l_size && fabs(pointLC[1]) < l_size)
        {
            m_forceServo[2] = l_size - pointLC[2];
            m_lastFace = l_nearestFace;
        }
        break;

        case FACE_FAR:
        if (fabs(pointLC[0]) < l_size && fabs(pointLC[1]) < l_size)
        {
            m_forceServo[2] = -l_size - pointLC[2];
            m_lastFace = l_nearestFace;
        }
        break;
        
        case FACE_RIGHT:
        if (fabs(pointLC[1]) < l_size && fabs(pointLC[2]) < l_size)
        {
            m_forceServo[0] = l_size - pointLC[0];
            m_lastFace = l_nearestFace;
            break;
        }
        break;
        
        case FACE_LEFT:
        if (fabs(pointLC[1]) < l_size && fabs(pointLC[2]) < l_size)
        {
            m_forceServo[0] = -l_size - pointLC[0];
            m_lastFace = l_nearestFace;
        }
        break;
        
        case FACE_TOP:
        if (fabs(pointLC[0]) < l_size && fabs(pointLC[2]) < l_size)
        {
            m_forceServo[1] = l_size - pointLC[1];
            m_lastFace = l_nearestFace;
        }
        break;
        
        case FACE_BOTTOM:
        if (fabs(pointLC[0]) < l_size && fabs(pointLC[2]) < l_size)
        {
            m_forceServo[1] = -l_size - pointLC[1];
            m_lastFace = l_nearestFace;
        }
    }

    // add spring stiffness to force effect
    m_forceServo[0] *= m_cubeStiffness;
    m_forceServo[1] *= m_cubeStiffness;
    m_forceServo[2] *= m_cubeStiffness;
}

// Interface function to get current position
void HapticsClass::getPosition(double pos[3])
{
    pos[0] = m_positionApp[0];
    pos[1] = m_positionApp[1];
    pos[2] = m_positionApp[2];

}

// Interface function to get button state.  Only one button is used
// in this application.
bool HapticsClass::isButtonDown()
{
    return m_buttonApp;
}

// For this application, the only device status of interest is the
// calibration status.  A different application may want to test for
// HDAL_UNINITIALIZED and/or HDAL_SERVO_NOT_STARTED
bool HapticsClass::isDeviceCalibrated()
{
    unsigned int state = hdlGetState();

    return ((state & HDAL_NOT_CALIBRATED) == 0);
}
