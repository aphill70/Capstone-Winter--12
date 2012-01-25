///////////////////////////////////////////////////////////////////////
// Acknex engine constants and structs
// (c) Conitec / JCL 2004
///////////////////////////////////////////////////////////////////////
#ifndef atypes_h
#define atypes_h

///////////////////////////////////////////////////////////////////////
// Acknex flag/mask definitions
// note: all other flag bits are used internally and must not be modified!!
///////////////////////////////////////////////////////////////////////
#define ON	1
#define OFF	0

// entity flags
#define FLAG1			(1<<0)	// flag1..flag8 for C-Script use
#define FLAG2			(1<<1)
#define FLAG3			(1<<2)
#define FLAG4			(1<<3)
#define FLAG5			(1<<4)
#define FLAG6			(1<<5)
#define FLAG7			(1<<6)
#define FLAG8			(1<<7)

#define INVISIBLE		(1<<8)
#define PASSABLE		(1<<9)
#define TRANSLUCENT		(1<<10)	// transparent
#define ORIENTED		(1<<12)	// for sprites only
#define OVERLAY			(1<<12)	// for models only
#define FACING			(1<<13)	// for sprites only
#define UNLIT			(1<<17)	// no light from environment
#define SHADOW			(1<<18)	// cast dynamic shadows
#define LIGHT			(1<<19)	// tinted by own light color
#define NOFOG			(1<<20)	// ignores fog
#define BRIGHT			(1<<21)	// additive blending
#define METAL			(1<<22)	// specular material
#define DECAL			(1<<22)	// sprite without backside
#define CAST			(1<<23)	// don't receive shadows

//sky flags
#define SKY				(1<<17)	// sky entity
#define DOME			(1<<18)	// half dome
#define SCENE			(1<<19)	// don't use the z buffer
#define CUBE			(1<<20)	// six sided cube
#define CYLINDER		(1<<21)	// 360 degrees cylinder

//event definitions
#define EVENT_SURFACE	1	// collision with level surface
#define EVENT_ENTITY	2	// collisions with entity
#define EVENT_STUCK		3	// stuck in a corner
#define EVENT_PUSH		4	
#define EVENT_IMPACT	5	// hit by another entity
#define EVENT_FRICTION	6
#define EVENT_DETECT	7
#define EVENT_SCAN		8
#define EVENT_SHOOT		9
#define EVENT_TRIGGER	10
#define EVENT_SONAR		11
#define EVENT_FRAME		12

#define EVENT_RECEIVE	13	// Entity received skills
#define EVENT_DISCONNECT 20	// Entity client has disconnected

#define EVENT_BUTTONUP	15	// mouse button released over entity
#define EVENT_TOUCH		16	// mouse pointer moves over entity
#define EVENT_RELEASE	17	// mouse pointer leaves entity
#define EVENT_CLICK		18	// left click over entity
#define EVENT_RIGHTCLICK 19	// right click over entity

// Entity event flags
#define DYNAMIC			(1<<0)
#define ENABLE_SURFACE	(1<<EVENT_SURFACE)
#define ENABLE_ENTITY	(1<<EVENT_ENTITY)
#define ENABLE_STUCK	(1<<EVENT_STUCK)
#define ENABLE_PUSH		(1<<EVENT_PUSH)
#define ENABLE_IMPACT	(1<<EVENT_IMPACT)
#define ENABLE_FRICTION (1<<EVENT_FRICTION)
#define ENABLE_DETECT	(1<<EVENT_DETECT)
#define ENABLE_SCAN		(1<<EVENT_SCAN)
#define ENABLE_SHOOT	(1<<EVENT_SHOOT)
#define ENABLE_TRIGGER	(1<<EVENT_TRIGGER)
#define ENABLE_SONAR	(1<<EVENT_SONAR)

#define ENABLE_FRAME	(1<<EVENT_FRAME)
#define ENABLE_RECEIVE	(1<<EVENT_RECEIVE)
#define ENABLE_DISCONNECT (1<<EVENT_DISCONNECT)

#define ENABLE_BUTTONUP	(1<<EVENT_BUTTONUP)
#define ENABLE_TOUCH	(1<<EVENT_TOUCH)
#define ENABLE_RELEASE	(1<<EVENT_RELEASE)
#define ENABLE_CLICK	(1<<EVENT_CLICK)
#define ENABLE_RIGHTCLICK (1<<EVENT_RIGHTCLICK)

// c_trace / c_move mode flags
#define IGNORE_ME 		(1<<0)
#define IGNORE_YOU		(1<<1)
#define IGNORE_PASSABLE (1<<2)
#define IGNORE_PASSENTS (1<<3)
#define IGNORE_MODELS	(1<<4)
#define IGNORE_SPRITES	(1<<5)
#define IGNORE_MAPS		(1<<7)
#define IGNORE_WORLD	(1<<8)
#define IGNORE_PUSH		(1<<9)
#define ACTIVATE_SHOOT	(1<<10)
#define ACTIVATE_SONAR	(1<<11)
#define ACTIVATE_TRIGGER (1<<12)
#define USE_BOX			(1<<13)
#define SCAN_TEXTURE	(1<<16)
#define GLIDE			(1<<18)

// save mode flags
#define SV_VARS			(1<<0)
#define SV_INFO			(1<<1)
#define SV_STRINGS		(1<<2)
#define SV_POINTERS		(1<<3)
#define SV_BMAPS		(1<<4)
#define SV_FUNCTIONS	(1<<5)
#define SV_KEYS			(1<<6)
#define SV_LEVEL		(1<<7)
#define SV_SKY			(1<<8)
#define SV_VIEWS		(1<<9) 
#define SV_PANELS		(1<<10)
#define SV_SYS			(1<<12)
#define SV_MATERIAL		(1<<13) 
#define SV_PATH			(1<<14) 
#define SV_STRUCT		(1<<15) 
#define SV_PHYSICS		(1<<17)
#define SV_ALL			0x000FFFFF

// particle effect flags when different from entity flags
#define STREAK			(1<<8)
#define BEAM			(1<<9)
#define MOVE			(1<<11)

// material flags
#define TANGENT			(1<<16)
#define ENABLE_RENDER	(1<<17)	// call event before object rendering
#define ENABLE_VIEW		(1<<18)	// call event before view rendering

// bitmap flags
#define BMPF_ARGB		(1<<8)	// bitmap contains alpha channel
#define BMPF_SAVE		(1<<9)	// bitmap was modified
#define BMPF_MIPMAPS	(1<<10) // Bmap has 3 mipmaps following the pixels

// text, view & panel flags
#define NARROW			(1<<13) // draw text condensed
#define VISIBLE			(1<<14) // visible
#define AUDIBLE			(1<<15) // view has sound focus
#define CENTER_X		(1<<18) // center text horizontally
#define CENTER_Y		(1<<19) // center text vertically

// connection
#define CONNECT_SERVER	1
#define CONNECT_CLIENT	2

// send
#define SEND_VEC		1
#define SEND_ALL		2
#define SEND_RATE		4
#define SEND_UNRELIABLE	8

// animate
#define ANM_CYCLE		1
#define ANM_ADD			2

// physics
#define PH_RIGID		1		// Constant for setting up physics body
#define PH_WAVE			2		// Constant for setting up water body
#define PH_BOX			0
#define PH_SPHERE		1
#define PH_CYLINDER		2
#define PH_POLY			3
#define PH_BALL			1
#define PH_HINGE		2
#define PH_SLIDER		3
#define PH_WHEEL		4

///////////////////////////////////////////////////////////////////////
// Acknex object structs
// Note: some struct members are not available in all versions
// (see comments). If you use them, your code should check the
// engine version though the VERSION engine variable.

// use for the C-Script compiler 
typedef struct C_LINK {
	long	index;		// index number of the object
	struct C_LINK *next; // pointer to next object
	char	*name;		// pointer to C-Script name of the object (if any)
} C_LINK;				// C-Script object header

typedef byte* EVENT;	// just a pointer to a machine code function

typedef struct STRING {
	C_LINK	link;
	char	*chars;		// pointer to null terminated string
	long	length;		// allocated length of string (NEVER exceed!)
	long	flags;		// 0 = don't save, 1 = save string at SAVE/LOAD
} STRING;

typedef struct SOUND {
	C_LINK	link;
	long 	length;
	byte	*buffer;
	long	type;
} SOUND;

typedef struct BMAP {
	C_LINK	link;
	long 	width,height; // size of the bitmap
	long	bytespp;	// original bytes per pixel (1..4)
	void	*ptr;		// internal use
	byte	*pixels;	// ptr to palettized, 565, 4444, 888 or 8888 coded original image
	long	flags;		// see BMPF_.. flags
} BMAP;

typedef struct MATERIAL {	
	C_LINK	link;
	var		ambient_blue,ambient_green,ambient_red;
	var		diffuse_blue,diffuse_green,diffuse_red;
	var		specular_blue,specular_green,specular_red;
	var		emissive_blue,emissive_green,emissive_red;
	var		map_blue,map_green,map_red;
	var		alpha;
	var		power;		// for specular reflection
	var		sun_albedo;
	var		scale1,scale2;
	var		cycle;
	var		skill[20];
	float	matrix[16];
	long	flags;
	char	*effect;
	long	eflags;
	BMAP	*skin[4];
	EVENT	event;		// event function
} MATERIAL;

typedef struct ENTITY {
	C_LINK	link;
	char	*filename;	// internal use only
	var		x,y,z;		// position of the entity
	var		pan,tilt,roll;	// euler angles
	var		scale_x,scale_y,scale_z;	// scale factors, 0..255
	long	flags;		// entity property flags, see ENF...
	var		frame;		// frame number for sprites & models
	var		nextframe;	// interpolation target frame for models
	var		skin;		// skin texture number for models
	var		ambient;	// -100..+100
	var		albedo;		// 0..100, light reflectivity
	var		alpha;		// 0..100, transparency, default 50
	var		lightrange;	// dynamic light range in quants
	var		blue,green,red;	// dynamic light color, 0..255
	long	emask;		// event enable flags, EVF...
	long	eflags;		// internal status flags
	var		min[3],max[3];	// bounding box for entity->entity collisions
	var		trigger_range;
	var		push;
	var		floor_dist; // distance to the floor - A6.23 or above 
	long	smask;		// nosend_... flags - A6.20 or above
	long 	client_id;	// client # that has created this entity
	var		skill[100];	// entity skills (skills > 40 only available for A6.2 or above)
	var		pose;		// current bones pose number
	MATERIAL* material;	// entity material
	long	unused[5];	// internal use
	EVENT	event;		// event function
	EVENT	local;		// client side function
} ENTITY;

typedef struct PARTICLE {
	C_LINK	link;
	var		lifespan;	// time until removal in ticks; set to 0 for removing
	var		x,y,z;		// position
	var		vel_x,vel_y,vel_z;	// speed or beam vector
	var		size;		// in quants
	BMAP	*bmap;		// bitmap, or NULL
	EVENT	event;		// particle function, executed every frame
	long	flags;		
	var		skill_x,skill_y,skill_z;
	var		gravity;
	var		unused;
	var		alpha;		// opacity 0..100
	ENTITY	*creator;	// Entity that created the particle effect, or NULL
	var		blue,green,red;
	var		skill[4];	// more general purpose skills
} PARTICLE;

typedef struct FONT {
	C_LINK	link;
	BMAP	bmap;		// bitmap that holds the characters
	int		dx,dy;		// width/height of single character
	int		num;		// number of characters - 11, 128, or 256
	int		cpl;		// characters per bitmap line, or -1 for a TTF font
} FONT;

typedef struct TEXT {
	C_LINK	link;
	long	type;		// internal use only
	var		layer;		// layer number (read only)
	var		pos_x,pos_y;	// screen position in pixels
	long	flags;		// see TPVF...
	var		alpha;		// opacity
	var		offset;		// vertical offset
	var		lines;		// number of lines to display
	FONT	*font;		// font used for the text
	long	length;		// internal use only
	STRING **pstrings;  // pointer to array of string pointers
	var		scale_x,scale_y;
	var		blue,green,red;	// color for Truetype fonts
	var		strings;	// number of strings (read only)
} TEXT;

typedef struct PANEL {
	C_LINK	link;
	long	type;		// internal use only
	var		layer;		// layer number (read only)
	var		pos_x,pos_y;	// screen position in pixels
	long	flags;		// see TPVF...
	var		alpha;		// transparency factor
	BMAP	*bmap;		// background bitmap
	var		scale_x,scale_y;
	var		blue,green,red;	// color for Truetype fonts
} PANEL;

typedef struct VIEW {
	C_LINK	link;
	long	type;		// internal use only
	var		layer;		// layer number (read only)
	var		pos_x,pos_y;	// screen position in pixels
	long	flags;		// see TPVF...
	var		size_x,size_y;	// screen size in pixels
	var		x,y,z;		// position of the camera
	var		pan,tilt,roll;	// camera angles
	var		offset_x,offset_y;	// eye offsets
	var		arc;		// camera FOV, used for zooming
	var		aspect;		// width to height ratio
	var		ambient;	// brightness
	var		fog;		// fog strength (only used when fog_end == 0)
	var		alpha;		// transparency
	var		portal_x,portal_y,portal_z;	// portal plane vertex
	var		pnormal_x,pnormal_y,pnormal_z;// portal plane normal
	var		fog_start,fog_end;	// fog start/end range in D3D mode
	var		unused;
	ENTITY *genius;	// calculates valid BSP tree leaf for the view
	struct VIEW	*portal;	// portal view
	BMAP	*bmap;		// render target
	var		clip_near,clip_far;
} VIEW;

// vector types ////////////////////////////////////////////
typedef struct {
	var	x,y,z;
} VECTOR;

typedef struct {
	var	blue,green,red;
} COLOR;

typedef struct {
	var blue,green,red,alpha;
} COLOR4;

typedef struct {
	var pan,tilt,roll;
} ANGLE;

////////////////////////////////////////////////////////////
#endif //atypes.h
