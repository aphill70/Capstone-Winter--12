// Copyright 2007 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.
//
//
// Basic application showing use of HDAL.
// Haptic interaction with a ball.
// Graphic display of cursor.
//
//

#ifdef _DEBUG
#ifdef _VC6
#error ("DirectX9 does not support VC6 Debug mode.  Use Release mode.")
#endif
#endif

#include <windows.h>
#include "haptics.h"

// DX9 requirements
#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9tex.h>
#include <dinput.h>

LPDIRECT3D9           pD3D;         // D3D object
LPDIRECT3DDEVICE9     pd3dDevice;   // D3D device
LPDIRECTINPUT8        g_lpDI;	    // the direct input object
LPDIRECTINPUTDEVICE8  g_lpDIDevice; // the direct input device

#define KEYDOWN(name, key) (name[key] &0x80)
char    kbdBuffer[256];

static int windowWidth = 640;
static int windowHeight = 480;

HICON icon;

// definitions for graphic/haptic interaction.
// Compare to other basic_* examples.
const double gStiffness = 100.0;
const float gCubeEdgeLength = 0.7f;

static float gCursorRadius = 0.05f;

static double colorRed[]  = {1.0, 0.0, 0.0};
static double colorTeal[] = {0.0, 0.5, 0.5};
static double* gCurrentColor;

static D3DXMATRIX gMatView;
static D3DXVECTOR3 gCameraPosition;
static D3DXVECTOR3 gCameraDirection;

static LPD3DXMESH gCube;
static D3DXMATERIAL gCubeMaterial;
static LPD3DXMESH gCursor;
static D3DXMATERIAL gCursorMaterial;

HapticsClass gHaptics;

void createCamera(float nearDist, float farDist)
{
    D3DXMATRIX matProj;

    D3DXMatrixPerspectiveFovLH(
        &matProj, D3DX_PI/4, 
        (float)windowWidth/(float)windowHeight,
        nearDist, 
        farDist);
    pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void pointCamera(D3DXVECTOR3 direction)
{
    gCameraDirection = direction;

    D3DXMatrixLookAtLH(
        &gMatView, &gCameraPosition, &gCameraDirection,
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    pd3dDevice->SetTransform(D3DTS_VIEW, &gMatView);
}


void moveCamera(D3DXVECTOR3 position)
{
    gCameraPosition = position;
    pointCamera(gCameraDirection);
}

void initHaptics()
{
    // Start Haptics
    gHaptics.init(gCubeEdgeLength, gStiffness);

    // Some time is required between gHaptics.init() and checking status,
    // for the device to initialize and stabilize.  In a complex
    // application, this time can be consumed in the scene setup
    // function.  Here, it is simulated with Sleep().
    Sleep(100);

    if (!gHaptics.isDeviceCalibrated())
        MessageBox(NULL, 
                   // The next two lines are one long string
                   "Please home the device by extending\n"
                   "then pushing the arms all the way in.",
                   "Not Homed",
                   MB_OK);
}

bool initScene()
{
    const double kFovY = 40;


    // Lighting

    pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    // Ambient
    pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(77, 77, 77));
    
    // Directional
    D3DLIGHT9 light;
    light.Type = D3DLIGHT_DIRECTIONAL;
    D3DXVECTOR3 dir(0.0f, -0.4f, 1.0f);
    D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir);
    light.Diffuse.r = 0.9f;
    light.Diffuse.g = 0.9f;
    light.Diffuse.b = 0.9f;
    light.Ambient.r = 0.5f;
    light.Ambient.g = 0.5f;
    light.Ambient.b = 0.5f;
    light.Range = sqrtf(FLT_MAX);
    pd3dDevice->SetLight(0, &light);
    pd3dDevice->LightEnable(0, TRUE);

    // Create the cube
    HRESULT hr = D3DXCreateBox(
        pd3dDevice, 
        gCubeEdgeLength,
        gCubeEdgeLength,
        gCubeEdgeLength,
        &gCube,
        NULL);
    gCubeMaterial.MatD3D.Diffuse.r = 0.5f;
    gCubeMaterial.MatD3D.Diffuse.g = 0.5f;
    gCubeMaterial.MatD3D.Diffuse.b = 0.5f;
    gCubeMaterial.MatD3D.Ambient.r = 0.5f;
    gCubeMaterial.MatD3D.Ambient.g = 0.5f;
    gCubeMaterial.MatD3D.Ambient.b = 0.5f;

    // Create the cursor
    hr = D3DXCreateSphere(
        pd3dDevice, 
        gCursorRadius,
        15,
        15,
        &gCursor,
        NULL);
    gCursorMaterial.MatD3D.Diffuse.r = 0.0f;
    gCursorMaterial.MatD3D.Diffuse.g = 0.5f;
    gCursorMaterial.MatD3D.Diffuse.b = 0.5f;
    gCursorMaterial.MatD3D.Ambient.r = 0.0f;
    gCursorMaterial.MatD3D.Ambient.g = 0.5f;
    gCursorMaterial.MatD3D.Ambient.b = 0.5f;

    return true;
}

void drawMesh(LPD3DXMESH mesh, D3DXMATERIAL material)
{
	// draw the first subset of the mesh
    pd3dDevice->SetMaterial(&material.MatD3D);
	mesh->DrawSubset(0);
}

void drawCube()
{	
	D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);
	D3DXMatrixMultiply(&matWorld, &matWorld, &gMatView);	
    pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    drawMesh(gCube, gCubeMaterial);
}

void drawCursor()
{
    double pos[3];
    gHaptics.synchFromServo();
    gHaptics.getPosition(pos);
    pos[2] *= -1;   // Reverse Z for DirectX

    // Check button state
    if (gHaptics.isButtonDown())
    {
        // Red when button is down
        gCursorMaterial.MatD3D.Diffuse.r = 1.0f;
        gCursorMaterial.MatD3D.Diffuse.g = 0.0f;
        gCursorMaterial.MatD3D.Diffuse.b = 0.0f;
        gCursorMaterial.MatD3D.Ambient.r = 1.0f;
        gCursorMaterial.MatD3D.Ambient.g = 0.0f;
        gCursorMaterial.MatD3D.Ambient.b = 0.0f;
    }
    else
    {
        // Teal when button is up
        gCursorMaterial.MatD3D.Diffuse.r = 0.0f;
        gCursorMaterial.MatD3D.Diffuse.g = 0.5f;
        gCursorMaterial.MatD3D.Diffuse.b = 0.5f;
        gCursorMaterial.MatD3D.Ambient.r = 0.0f;
        gCursorMaterial.MatD3D.Ambient.g = 0.5f;
        gCursorMaterial.MatD3D.Ambient.b = 0.5f;
    }

    //Now position cursor
	D3DXMATRIX matXlt;
	D3DXMatrixTranslation(&matXlt, (float)pos[0], (float)pos[1], (float)pos[2]);
	
	D3DXMATRIX matWorld;
	D3DXMatrixMultiply(&matWorld, &matXlt, &gMatView);	
    pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    drawMesh(gCursor, gCursorMaterial);
}


void render()
{
    if (NULL != pd3dDevice)
    {
        pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                          D3DCOLOR_XRGB(0, 0, 0),
                          1.0f, 0);
		pd3dDevice->BeginScene();

		// create the camera
		createCamera(0.1f, 10.0f);		// near clip plane, far clip plane

        // Position camera so cube is same projected size as in OpenGL version
		moveCamera(D3DXVECTOR3(0.0f, 0.0f, -1.6f));
		pointCamera(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// draw the objects
		drawCube();
        drawCursor();
		
		pd3dDevice->EndScene();
        pd3dDevice->Present(NULL, NULL, NULL, NULL);

    }
}

////////////////////////////////////////////////////
///// Generic Windows/DX9 application code  ////////
//    Adapted from: Beginning DirectX9, Wendy Jones, Premier Press, 2004

HINSTANCE hInst;   // App instance handle
HWND hWin;         // Window handle

// Window callback function
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Window Initialization
bool initWindow(HINSTANCE hInstance)
{

	// Get Handle for icon
    // Works only when running from within Visual Studio.
	icon = ExtractIcon( GetModuleHandle(NULL),"..\\..\\src\\novint.ico", 0);
	if(icon == NULL)
	{
	   MessageBox(NULL, "ExtractIcon() Failed!", "Error:", MB_OK);
	}

    WNDCLASSEX wcex;
    
    wcex.cbSize            = sizeof(WNDCLASSEX);
    wcex.style             = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc       = (WNDPROC)WndProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance         = hInstance;
    wcex.hIcon             = icon;
    wcex.hCursor           = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground     = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName      = NULL;   // No menu used in this version
    wcex.lpszClassName     = "BasicDirectX";
    wcex.hIconSm           = icon;
    
    RegisterClassEx(&wcex);

    // Now create the window
    hWin = CreateWindow(
               "BasicDirectX",    // OS name
               "Basic Direct X",  // Window title caption
               WS_OVERLAPPEDWINDOW,
               CW_USEDEFAULT,
               CW_USEDEFAULT,
               windowWidth,
               windowHeight,
               NULL,
               NULL,
               hInstance,
               NULL);

    bool result = false;
    if (hWin)
    {
        ShowWindow(hWin, SW_SHOW);
        UpdateWindow(hWin);
        result = true;
    }
    
    return result;
}

bool initDirect3D(HINSTANCE hInstance)
{
    pD3D = NULL;
    pd3dDevice = NULL;

    bool result = false;

    if (NULL != (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        d3dpp.BackBufferCount = 1;
        d3dpp.BackBufferHeight = windowHeight;
        d3dpp.BackBufferWidth = windowWidth;
        d3dpp.hDeviceWindow = hWin;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        result = ! FAILED(pD3D->CreateDevice(
                                    D3DADAPTER_DEFAULT,
                                    D3DDEVTYPE_HAL,
                                    hWin,
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                    &d3dpp,
                                    &pd3dDevice));

        pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

        // Create the DirectInput object. 
        HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
                                IID_IDirectInput8, (void**)&g_lpDI, NULL); 

	    if FAILED(hr) return FALSE; 

        // Retrieve a pointer to an IDirectInputDevice8 interface 
        hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDevice, NULL); 

	    hr = g_lpDIDevice->SetDataFormat(&c_dfDIKeyboard); 

	    if FAILED(hr) return FALSE; 
	    

	    // Set the cooperative level 
        hr = g_lpDIDevice->SetCooperativeLevel(hWin, 
                                 DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
        if FAILED(hr) return FALSE; 
   

        // Get access to the input device. 
        hr = g_lpDIDevice->Acquire(); 
        if FAILED(hr) 
            return FALSE; 
        }

    return result;
}

void cleanup()
{

	DestroyIcon(icon);

    if (NULL != pd3dDevice)
        pd3dDevice->Release();

    if (NULL != pD3D)
        pD3D->Release();
}

// Application entry point and message handling loop
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    int result = 0;
    initHaptics();

    if (initWindow(hInstance) && initDirect3D(hInstance) && initScene())
    {
        // Main Loop
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while (msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                // this is called when no messages are pending
                HRESULT hr = g_lpDIDevice->GetDeviceState(sizeof(kbdBuffer),(LPVOID)&kbdBuffer);
                if FAILED(hr) break;
                if (KEYDOWN(kbdBuffer, DIK_ESCAPE))
                    break;
                render();
            }
        }
        result = (int)msg.wParam;
    }
    
    cleanup();
    return result;
}

void exitHandler()
{
    gHaptics.uninit();
}

