// Wrapper class for connecting 3D Game Studio app to haptics class

#include "StdAfx.h"

// engine specific header file
#define DLL_USE	// always define before including adll.h
#include "adll.h"	
#include "haptics.h"

const double SCALE_FACTOR = 1000; // Scale to accommodate 3DGS resolution

HapticsClass gHaptics;

// Standard boilerplate for DLLs
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        gHaptics.uninit();
    }

	// make sure the library is linked
	engine_bind();
    return TRUE;
}

// bt_ prefix used for Basic Test
// GameStudio looks through all DLLs for entry point names.
// Since acknex_plugins directory may have many DLLs, it is
// good practice to keep their names unique to the application.

DLLFUNC void bt_startHaptics(var cubeSize, var cubeStiffness)
{
    // See 3D Game Studio for documentation on _FLOAT macro
    double size = _FLOAT(cubeSize);
    double stiffness = _FLOAT(cubeStiffness);
    gHaptics.init(size, stiffness);
}

DLLFUNC void bt_stopHaptics()
{
    gHaptics.uninit();
}

// Called each graphics cycle to trigger async between threads
DLLFUNC void bt_syncHaptics()
{
    gHaptics.synchFromServo();
}


// Get Position
DLLFUNC var bt_getXPos()
{
    double pos[3];

    gHaptics.getPosition(pos);

    double xf = SCALE_FACTOR * pos[0];

    return (_VAR((float)xf));
}


DLLFUNC var bt_getYPos()
{
    double pos[3];

    gHaptics.getPosition(pos);

    double yf = SCALE_FACTOR * pos[1];

    return (_VAR((float)yf));
}

DLLFUNC var bt_getZPos()
{
    double pos[3];

    gHaptics.getPosition(pos);

    double zf = SCALE_FACTOR * pos[2];

    return (_VAR((float)zf));
}



// Haptic button state
DLLFUNC var bt_isHapticButtonDepressed()
{
    bool l_state = gHaptics.isButtonDown();

    return (_VAR(l_state));
}

DLLFUNC var bt_isDeviceCalibrated()
{
    return (_VAR(gHaptics.isDeviceCalibrated()));
}