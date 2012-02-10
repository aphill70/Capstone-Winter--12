#pragma once

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "Include\IntTypes.h"
#include "StoneWeatherer.h"
#include "StepResults.h"

using namespace std;


/**
 * Draws the current mesh
 */
void drawMesh();


/**
 * Recolorizes the vertices of the object by their softness
 */
void recolorize();


/**
 * An abstract class for an OpenGL multi-pane view
 */
struct screenRegion {

	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Destructor
	 */
	virtual inline ~screenRegion() {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h ) = 0;


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const = 0;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y ) = 0;


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y ) = 0;


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust() = 0;


	/**
	 * Called when the user presses a key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool keyEvent( char c );


	/**
	 * Called when the user presses a special key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool specialKeyEvent( int c );
};


/**
 * A horizontal split pane (splits the region into left and right sub-panes)
 */
struct hSplitPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The left subregion
	 */
	screenRegion * left;

	/**
	 * The right subregion
	 */
	screenRegion * right;

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;

	/**
	 * The size of the divider between sub-regions
	 */
	int gutter;

	/**
	 * The relative location of the split (between 0.0 and 1.0)
	 */
	double split;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Full constructor
	 * @param l The left subregion
	 * @param r The right subregion
	 * @param s The relative location of the split (default is 0.5)
	 */
	inline hSplitPane( screenRegion * l, screenRegion * r, double s = 0.5 ) : left( l ), right( r ), gutter( 10 ), split( s ) {

		return;
	}


	/**
	 * Destructor
	 */
	virtual inline ~hSplitPane() {

		delete left;
		delete right;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();
};


/**
 * A vertical split pane (splits the region into top and bottom sub-panes)
 */
struct vSplitPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The bottom subregion
	 */
	screenRegion * bottom;

	/**
	 * The top subregion
	 */
	screenRegion * top;

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;

	/**
	 * The size of the divider between sub-regions
	 */
	int gutter;

	/**
	 * The relative location of the split (between 0.0 and 1.0)
	 */
	double split;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Full constructor
	 * @param b The left subregion
	 * @param t The right subregion
	 * @param s The relative location of the split (default is 0.5)
	 */
	inline vSplitPane( screenRegion * b, screenRegion * t, double s = 0.5 ) : bottom( b ), top( t ), gutter( 10 ), split( s ) {

		return;
	}


	/**
	 * Destructor
	 */
	virtual inline ~vSplitPane() {

		delete bottom;
		delete top;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();
};


/**
 * The pane that draws the mesh being deformed
 */
struct meshPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;

	/**
	 * The rotation of the mesh about the y-axis
	 */
	double theta;

	/**
	 * The rotation of the mesh about the z-axis
	 */
	double phi;

	/**
	 * The zoom level of the "camera"
	 */
	double scale;

	/**
	 * The mouse button that is currently depressed
	 */
	int currentButton;

	/**
	 * The current mouse x-coordinate
	 */
	int currentMx;

	/**
	 * The current mouse y-coordinate
	 */
	int currentMy;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	meshPane();


	/**
	 * Destructor
	 */
	virtual inline ~meshPane() {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();
};


/**
 * A piecewise function whose components are all line segments of the type "y = mx + b"
 */
struct piecewiseLinearFunction {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * A list of [t,f(t}] pairs
	 */
	vector< pair<double,double> > keys;


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Gets the key closest to the given t value
	 * @return The index of the closest key
	 */
	int nearestKeyTo( double t ) const;


	/**
	 * Gets the [t,f(t)] pair at the given index
	 * @param index The index of the pair to get
	 * @return The [t,f(t)] pair at the given index
	 */
	inline const pair<double,double> & valueOf( uint32_t index ) const {

		return keys[ index ];
	}


	/**
	 * Evaluates the function at the given value of t
	 * @return The value of the function at t
	 */
	double evaluate( double t ) const;


	/**
	 * Adds a new key to the list
	 * @param t The t value
	 * @param f_t The f(t) value
	 * @return The indx of the newly added key
	 */
	int addKeyAt( double t, double f_t );


	/**
	 * Moves the specified key to the new position
	 * @param index The index of the key to move
	 * @param t The new t value
	 * @param f_t The new f(t) value
	 * @return TRUE if the key was moved successfully, FALSE otherwise
	 */
	bool moveKeyTo( uint32_t index, double t, double f_t );


	/**
	 * Removes the specified key from the list
	 * @param index The index of the key to remove
	 */
	void removeKey( uint32_t index );


	/**
	 * Finds the maximum f(t) value in the function
	 * @return The maximum f(t) value in the function
	 */
	double maxValue() const;


	/**
	 * Removes all the keys before the specified key
	 * @param t The first desired key in the function
	 */
	void clearAbove( double t );


	/**
	 * Removes all keys after the specified key
	 * @param t The last desired key in the function
	 */
	void clearBelow( double t );


	/**
	 * Draws the function
	 */
	void draw() const;


	//----------
	// OPERATORS
	//----------

	/**
	 * Multiplication assignment operator
	 * @param s The amount to scale the function
	 * @return A reference to the function after scaling
	 */
	piecewiseLinearFunction & operator *= ( double s );


	/**
	 * Division assignment oeprator
	 * @param s The amount to shrink the function
	 * @return A reference to the function after shrinking
	 */
	piecewiseLinearFunction & operator /= ( double s );
};


/**
 * A numerical entry in the settings box
 */
struct numberEntry {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The text value for the number
	 */
	mutable char text[ 256 ];

	/**
	 * The title to display next to the number (the label)
	 */
	const char * heading;

	/**
	 * The horizontal length of the number (the length of "text")
	 */
	const int horizontalLength;

	/**
	 * The index of the last digit in the number in the text array
	 */
	int index;

	/**
	 * The position of the cursor in the text array
	 */
	int cursor;

	/**
	 * The x-coordinate of the text in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the text in screen space
	 */
	int y;

	/**
	 * The width of the text in pixels
	 */
	int w;

	/**
	 * The height of the text in pixels
	 */
	int h;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Full constructor
	 * @param temp The label for the number
	 */
	inline numberEntry( const char * temp = 0 ) : heading( temp ), horizontalLength( strlen( temp ) ), index( 0 ), cursor( 0 ) {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Sets the location of the cursor in the text array
	 * @param position The new location of the cursor in the text array
	 */
	inline void setCursor( int position ) {

		position -= horizontalLength;

		if ( position < 0 ) {

			position = 0;
		}

		if ( position > index ) {

			position = index;
		}

		// ??? cursor = position;
	}


	/**
	 * Handles keyboard input
	 * @param c The character input
	 * @return TRUE if the input was handled, FALSE otherwise
	 */
	bool acceptKey( char c );


	/**
	 * Handles special keyboard input
	 * @param c The character input
	 * @return TRUE if the key input was handled, FALSE otherwise
	 */
	bool acceptOtherKey( int c );


	/**
	 * Gets the value of the textual number as a double
	 * @return The numerical value of the textual number
	 */
	inline double getValue() const {

		text[ index ] = '\0';
		
		return atof( text );
	}


	/**
	 * Sets the value of the number
	 * @param x The new value of the number
	 */
	inline void setValue( double x ) {

#ifdef __GNUC__
		sprintf( text, "%g", x );
#else
		sprintf_s( text, 256, "%g", x );
#endif
		index = strlen( text );
		cursor = index;
	}


	/**
	 * Gets the length of the number (the number of digits)
	 * @return The number of characters in the number
	 */
	inline int getLength() const {

		return horizontalLength + index;
	}


	/**
	 * Gets the character at the specified index
	 * @param i The index of the character to get
	 * @return The character at the specified index
	 */
	inline char getCharacter( int i ) const {

		if ( i < horizontalLength ) {

			return heading[ i ];
		}
		else {

			return text[ i - horizontalLength ];
		}
	}
};


/**
 * Gets the minimum edge length specified by the user
 * @return The minimum edge length specified by the user
 */
double getMinEdgeLength();


/**
 * Gets the maximum edge length specified by the user
 * @return The maximum edge length specified by the user
 */
double getMaxEdgeLength();


/**
 * Gets the overdrive value specified by the user
 * @return The overdrive value specified by the user
 */
double getOverdrive();


/**
 * Gets the noise power specified by the user
 * @return The noise power specified by the user
 */
double getNoisePower();


/**
 * Gets the air-to-water ratio specified by the user
 * @return The air-to-water ratio specified by the user
 */
double getAirWaterRatio();


/**
 * Gets the fluid scale specified by the user
 * @return The fluid scale specified by the user
 */
double getFluidScale();


/**
 * Sets the minimum edge length
 * @param x The new minimum edge length
 */
void setMinEdgeLength( double x );


/**
 * Sets the maximum edge length
 * @param x The new maximum edge length
 */
void setMaxEdgeLength( double x );


/**
 * Sets the overdrive value
 * @param x The new overdrive value
 */
void setOverdrive( double x );


/**
 * Sets the noise power
 * @param x The new noise power
 */
void setNoisePower( double x );


/**
 * Sets the air-to-water ratio
 * @param x The new air-to-water ratio
 */
void setAirWaterRatio( double x );


/**
 * Sets the fluid scale
  8@param x The new fluid scale
 */
void setFluidScale( double x );


/**
 * Represents a rock layer in the durability widget (associated with a single
 * piece of the piecewise function)
 */
struct layer {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The piecewise function that serves as the base durability measure
	 */
	piecewiseLinearFunction base;

	/**
	 * The frequency of the noise in the y-direction
	 */
	double yNoiseFrequency;

	/**
	 * The amplitude of the noise in the y-direction
	 */
	double yNoiseAmplitude;

	/**
	 * The frequency of the grain
	 */
	double grainFrequency;

	/**
	 * The amplitude of the grain
	 */
	double grainAmplitude;

	
	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	inline layer() : base(), yNoiseFrequency( 1.0 ), yNoiseAmplitude( 0.0 ), grainFrequency( 4.0 ), grainAmplitude( 0.99 ) {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Evaluates the layer at the given point
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param seed The seed value for random number generator (default 0)
	 * @param userGrain Whether or not to use the grain (default TRUE)
	 * @return The durability of the layer at the given point
	 */
	double evaluate( double x, double y, double z, uint32_t seed = 0, bool useGrain = true ) const;


	//----------
	// OPERATORS
	//----------

	/**
	 * Multiplication assignment operator
	 * @param s The amount to scale the layer
	 * @return A reference to the layer after scaling
	 */
	inline layer & operator *= ( double s ) {

		base *= s;
		
		return *this;
	}


	/**
	 * Division assignment operator
	 * @param s The amount to shrink the layer
	 * @return A reference to the layer after shrinking
	 */
	inline layer & operator /= ( double s ) {

		base /= s;

		return *this;
	}
};


/**
 * The widget that defines the durability of the object
 */
struct durabilityTool {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The locations of the divisions between layers
	 */
	vector<double> dividers;

	/**
	 * The layers in the object
	 */
	vector<layer> layers;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	inline durabilityTool() {

		layers.push_back( layer() );
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Finds the nearest division to the given y value
	 * @param y The y-coordinate of the mouse
	 * @return The index of the nearest split to the given y value
	 */
	int nearestSplitTo( double y ) const;


	/**
	 * Gets the value of the split at the given index
	 * @param index The index of the split to get
	 * @return The value of the specified split
	 */
	inline double splitValue( uint32_t index ) const {

		return dividers[ index ];
	}


	/**
	 * Adds a new split
	 * @param y The new split value
	 * @return The index of the new split
	 */
	int addSplit( double y );


	/**
	 * Removes the specified split
	 * @param index the index of the split to remove
	 */
	void removeSplit( uint32_t index );


	/**
	 * Moves the specified split to the new location (does not allow reordering of y values)
	 * @param index The index of the split to move
	 * @param y The new location of the split
	 * @return TRUE if the move succeeded, FALSE otherwise
	 */
	bool moveSplitTo( uint32_t index, double y );


	/**
	 * Gets the layer at the specified y value
	 * @param y The y value to get the layer for
	 * @return The layer at the given y value
	 */
	layer * layerAt( double y );


	/**
	 * Gets the layer at the specified y value
	 * @param y The y value to get the layer for
	 * @param index Stores the index of the layer
	 * @return The layer at the given y value
	 */
	const layer * layerAt( double y, int * index = 0 ) const;


	/**
	 * Evaluates the durability of the object at the given point
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param useGrain Whether or not to use the grain (default TRUE)
	 * @return The durability of the object at the given point
	 */
	inline double evaluate( double x, double y, double z, bool useGrain = true ) const {

		int temp;
		const layer * l = layerAt( y, &temp );

		return l->evaluate( x, y, z, temp, useGrain );
	}


	/**
	 * Draws the durability widget
	 * @param minY The minimum y-coordinate
	 * @param maxY The maximum y-coordinate
	 * @param steps The number of steps between minY and maxY
	 */
	void draw( double minY, double maxY, int steps ) const;


	/**
	 * ???
	 * @return ???
	 */
	bool normalize();
};


/**
 * Updates the locked version of the durability widget
 */
void updateLockedDurability();


/**
 * Computes the softness at the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the object at the given point
 */
double softness( double x, double y, double z );


/**
 * Computes the softness at the given point if it is live
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the given point if it is live
 */
double liveSoftness( double x, double y, double z );


/**
 * Pane to hold the durability widget
 */
struct layerPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;

	/**
	 * The mouse button that is currently depressed
	 */
	int currentButton;

	/**
	 * The current mouse x-coordinate
	 */
	int currentMx;

	/**
	 * The current mouse y-coordinate
	 */
	int currentMy;

	/**
	 * The various actions to take based on the mouse mode
	 */
	enum {
		IGNORE_MOUSE,
		MOVE_SPLIT,
		MOVE_POINT,
		CHANGE_GRAIN,
		CHANGE_LAYER_NOISE
	} mouseMode;

	/**
	 * ???
	 */
	int thingID;

	/**
	 * A pointer to the currently active layer
	 */
	layer * liveLayer;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	inline layerPane() {

		return;
	}


	/**
	 * Destructor
	 */
	virtual inline ~layerPane() {

		return;
	}


	//----------
	// FUNCTIONS
	//----------


	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * ???
	 * @return ???
	 */
	double sx() const;


	/**
	 * ???
	 * @return ???
	 */
	double sy() const;


	/**
	 * ???
	 * @param sx ???
	 * @return ???
	 */
	double px( double sx ) const;


	/**
	 * ???
	 * @param sy ???
	 * @return ???
	 */
	double py( double sy ) const;


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();
};


/**
 * The pane that displays results data
 */
struct dataPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The row of data
	 */
	int row;

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	dataPane();


	/**
	 * Destructor
	 */
	virtual inline ~dataPane() {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();


	/**
	 * Called when the user presses a key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool keyEvent( char c );


	/**
	 * Called when the user presses a special key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool specialKeyEvent( int c );


	/**
	 * Gets teh value of the specified data
	 * @param index The index of the data to get
	 * @return The value of the specified data
	 */
	double getValue( uint32_t index ) const;
};


/**
 * A window pane that displays the current status
 */
struct statusPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	inline statusPane() {

		return;
	}


	/**
	 * Destructor
	 */
	virtual inline ~statusPane() {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w Teh width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();


	/**
	 * Called when the user presses a key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool keyEvent( char c );


	/**
	 * Called when the user presses a special key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool specialKeyEvent( int c );
};
