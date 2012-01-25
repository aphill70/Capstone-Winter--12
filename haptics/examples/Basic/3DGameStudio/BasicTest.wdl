//var video_mode = 8; //1024x768 resolution 
//var video_screen = 1; // start settings for Fullscreen
var gHapticsReady = false;
var gIsActive = on;

font gbd1_arial_font = "Arial",1,20; // truetype font

// OnScreen Display message buffer
string gMessageStr = "#100";

string g_wmb_string = <BasicTest.wmb>; 

// Entities
var g_cube;
var g_cursor;

// Bind statements needed for publishing


// DLL entry points
dllfunction bt_startHaptics(cubeSize, stiffness); // Initialize with cube parameters
dllfunction bt_stopHaptics();                 // Let haptics clean up on exit
dllfunction bt_syncHaptics();                 // Update necessary data, call once per frame
dllfunction bt_getXPos();                     // X Position
dllfunction bt_getYPos();                     // Y Position
dllfunction bt_getZPos();                     // Z Position
dllfunction bt_isHapticButtonDepressed();     // Button down?
dllfunction bt_isDeviceCalibrated();          // Device calibrated?

// There are many ways to notify the user; this is a simple one.
text BoxDebugDisplay	
{
	layer = 3;
	pos_x = 50;
	pos_y = 50;
	size_y = 80;
	offset_y = 0;
	strings = 3;
	font = gbd1_arial_font;
	string = gMessageStr;
	alpha	= 100;
	flags = narrow, transparent;
	
	red = 250;
	green = 100;
	blue = 100;
	
//	flags = visible;
  
}

var cube_scale = 3.0;
action cube
{
	g_cube = my;
	my.invisible = off;
	my.x = 0;
	my.y = 0;
	my.z = 0;
	my.scale_x = cube_scale;
	my.scale_y = cube_scale;
	my.scale_z = cube_scale;
}

action cursor
{
	g_cursor = my;
	my.invisible = off;
	my.light = on;
	my.lightrange = 0;
	my.scale_x = 0.1;
	my.scale_y = 0.1;	
	my.scale_z = 0.1;
	
	while(gIsActive)
	{

		if (bt_isDeviceCalibrated())
		{
			str_cpy(gMessageStr, "");
			g_cursor = my;
			bt_syncHaptics();
			
			if (bt_isHapticButtonDepressed())
			{
				my.red = 255;
				my.blue = 0;
				my.green = 0;
			}
			else
			{
				my.red = 0;
				my.blue = 255;
				my.green = 0;
			}
			
			// Note transformation from HDAL coordinates to Game coordinates
			var distance_scale = 0.05;
			my.x = -bt_getZPos() * distance_scale;
			my.y = -bt_getXPos() * distance_scale;
			my.z = bt_getYPos() * distance_scale;
		}
		else
		{
			str_cpy(gMessageStr, "Please home the device by extending then pushing the arms all the way in.");
		}
		wait(1);
	}
	
}

function cameraSetup()
{
	// Game coordinates:
	// X increases away from viewer
	// Y increases to viewer's left
	// Z increases up
	camera.tilt = 0;
	camera.x = -100;
	camera.y = 0;
	camera.z = 0;
	camera.pan = 0;
	camera.tilt = 0;
	camera.roll = 0;
	camera.arc = 90;
	camera.ambient = 0;	
	camera.clip_near = 10;
	camera.clip_far = 5000;				

}

// Handle cleanup chores
function quit()
{
	gIsActive = off;
	bt_stopHaptics();
	sys_exit("");
}

function main
{
	BoxDebugDisplay.visible = on;
	
	// Set up exit handlers
	on_esc = quit;       // Escape key
	on_close = quit;     // Window close
	
	bt_startHaptics(1.0,100.0);
	
	// Load the level
	level_load (g_wmb_string);
	
	// Wait for level to load 
	wait (3);	
		
	cameraSetup();
}
