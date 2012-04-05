#include "ScreenRegion.h"

#include <GL/glut.h>


//-----------------
// GLOBAL VARIABLES
//-----------------

/**
 * The current camera zoom
 */
static double zoom = 2.0;

/**
 * Create the durability widget
 */
durabilityTool dt;

/**
 * Create a locked durability widget (locked while simulating)
 */
durabilityTool lockedDt;

/**
 * The number of result labels
 */
static const int numberEntryLength = 6;

/**
 * The list of result labels
 */
static numberEntry ne[ numberEntryLength ] = {
	"Min Edge Length: ",
	"Max Edge Length: ",
	"Overdrive:       ",
	"Noise Power:     ",
	"Air/Water Ratio: ",
	"Fluid Scale:     "
};


#include <CGAL/Memory_sizer.h>

/**
 * Used for displaying memory usage
 */
CGAL::Memory_sizer memorySizer;

/**
 * Computes the absolute value of the data specified
 */
#define MACRO_ABS(a) ((a) < 0 ? -(a) : (a))


/**
 * Clamps the value given to the range [0,1]
 */
#define MACRO_CLAMP(a) ((a) < 0 ? 0 : ((a) > 1 ? 1 : (a)))


/**
 * For visualizing softness, importance, curvature, or other range variable
 * @param rgb Stores the RGB value
 * @param hue The hue to convert to RGB
 */
template <typename N> void hueToRGB( N * rgb, double hue ) {

	rgb[ 0 ] = MACRO_CLAMP( MACRO_ABS( hue * 6.0 - 3.0 ) - 1.0 );
	rgb[ 1 ] = MACRO_CLAMP( 2.0 - MACRO_ABS( hue * 6.0 - 2.0 ) );
	rgb[ 2 ] = MACRO_CLAMP( 2.0 - MACRO_ABS( hue * 6.0 - 4.0 ) );
}


// ***** SCREEN REGION ***** //

/**
 * Called when the user presses a key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool screenRegion::keyEvent( char c ) {

	return false;
}


/**
 * Called when the user presses a special key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool screenRegion::specialKeyEvent( int c ) {

	return false;
}


// ****** H-SPLIT PANE ***** //

/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w Teh width of the screen region
 * @param h The height of the screen region
 */
void hSplitPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	gutter = gutter >> 1;
	int mid = ( int ) ( w * split );
	left->reshape( x, y, mid - gutter, h );
	right->reshape( x + mid + gutter, y, ( w - mid ) - gutter, h );
	gutter = gutter << 1;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void hSplitPane::draw( const stepResults & results ) const {

	int mid = ( int ) ( w * split );
	glViewport( x + mid - ( gutter >> 1 ), y, gutter, h );
	glScissor( x + mid - ( gutter >> 1 ), y, gutter, h );
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	left->draw( results );
	right->draw( results );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * hSplitPane::acceptMouse( int button, int x, int y ) {

	if ( x < this->x + ( int ) ( w * split ) - ( gutter >> 1 ) ) {

		return left->acceptMouse( button, x, y );
	}

	if ( x > this->x + ( int ) ( w * split ) + ( gutter >> 1 ) ) {

		return right->acceptMouse( button, x, y );
	}

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void hSplitPane::dragMouse( int x, int y ) {

	split = ( double ) ( x - this->x ) / w;
	reshape( this->x, this->y, w, h );
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool hSplitPane::readjust() {

	return left->readjust() | right->readjust();
}


// ***** V-SPLIT PANE ***** //

/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w Teh width of the screen region
 * @param h The height of the screen region
 */
void vSplitPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	gutter = gutter >> 1;
	int mid = ( int ) ( h * split );
	bottom->reshape( x, y, w, mid - gutter );
	top->reshape( x, y + mid + gutter, w, ( h - mid ) - gutter );
	gutter = gutter << 1;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void vSplitPane::draw( const stepResults & results ) const {

	int mid = ( int ) ( h * split );
	glViewport( x, y + mid - ( gutter >> 1 ), w, gutter );
	glScissor( x, y + mid - ( gutter >> 1 ), w, gutter );
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	bottom->draw( results );
	top->draw( results );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * vSplitPane::acceptMouse( int button, int x, int y ) {

	// The math here is odd because OpenGL and GLUT have opposite Y orientations

	if ( y > this->y + ( int ) ( h * split ) + ( gutter >> 1 ) ) {

		return top->acceptMouse( button, x, y );
	}

	if ( y < this->y + ( int ) ( h * split ) - ( gutter >> 1 ) ) {

		return bottom->acceptMouse( button, x, y );
	}

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void vSplitPane::dragMouse( int x, int y ) {

	split = ( double ) ( y - this->y ) / h;
	reshape( this->x, this->y, w, h );
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool vSplitPane::readjust() {

	return top->readjust() | bottom->readjust();
}


// ***** MESH PANE ***** //

/**
 * Default constructor
 */
meshPane::meshPane() : theta( 0.0 ), phi( 0.0 ), scale( 1.0 / zoom ) {

	return;
}


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w Teh width of the screen region
 * @param h The height of the screen region
 */
void meshPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void meshPane::draw( const stepResults & results ) const {

	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glScalef( scale * h / w, scale, -0.25 * scale ); // In projection, not modelview, so normals don't change
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		gluLookAt(
			results.midPoint[ 0 ], results.midPoint[ 1 ], results.midPoint[ 2 ],
			results.midPoint[ 0 ] + sin( theta ) * cos( phi ), results.midPoint[ 1 ] + sin( phi ), results.midPoint[ 2 ] - cos( theta ) * cos( phi ),
			0, 1, 0 );
	glEnable( GL_LIGHTING );
	drawMesh();
	glDisable( GL_LIGHTING );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * meshPane::acceptMouse( int button, int x, int y ) {

	currentMx = x;
	currentMy = y;
	currentButton = button;

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void meshPane::dragMouse( int x, int y ) {

	int dx = x - currentMx;
	int dy = y - currentMy;

	if ( !dx && !dy ) {

		return;
	}

	currentMx = x;
	currentMy = y;

	switch ( currentButton ) {

		case GLUT_LEFT_BUTTON:

			theta += dx * 0.01;
			phi += dy * 0.01;
			break;

		default:

			double offset = dy;

			if ( h ) {

				offset /= h;
			}

			offset += 1.0;

			if ( offset > 0.0 ) {

				scale *= offset;
			}

			zoom = 1.0 / scale;
			break;
	}
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool meshPane::readjust() {

	return false;
}


// ***** PIECE-WISE LINEAR FUNCTION ***** //

/**
 * Gets the key closest to the given t value
 * @return The index of the closest key
 */
int piecewiseLinearFunction::nearestKeyTo( double t ) const {

	if ( keys.size() <= 0 ) {

		return -1;
	}

	if ( t <= keys[ 0 ].first ) {

		return 0;
	}

	if ( t >= keys.back().first ) {

		return keys.size() - 1;
	}

	int min = 0;
	int max = keys.size() - 1;
	int k;

	// Binary search
	while ( max > min + 1 ) {

		k = ( ( min + max ) >> 1 );

		if ( keys[ k ].first < t ) {

			min = k;
		}
		else {

			max = k;
		}
	}

	if ( min == max ) {

		return min;
	}

	if ( keys[ max ].first - t > t - keys[ min ].first ) {

		return min;
	}

	return max;
}


/**
 * Evaluates the function at the given value of t
 * @return The value of the function at t
 */
double piecewiseLinearFunction::evaluate( double t ) const {

	if ( keys.size() <= 0 ) {

		return 1.0;
	}

	if ( t <= keys.front().first ) {

		return keys.front().second;
	}

	if ( t >= keys.back().first ) {

		return keys.back().second;
	}

	int min = 0;
	int max = keys.size() - 1;
	int k;

	// Binary search
	while ( max > min + 1 ) {

		k = ( ( min + max ) >> 1 );

		if ( keys[ k ].first < t ) {

			min = k;
		}
		else {

			max = k;
		}
	}

	if ( keys[ min ].first == keys[ max ].first ) {

		return keys[ min ].second;
	}
	else {

		return ( keys[ min ].second * ( keys[ max ].first - t ) + keys[ max ].second * ( t - keys[ min ].first ) ) / ( keys[ max ].first - keys[ min ].first );
	}
}


/**
 * Adds a new key to the list
 * @param t The t value
 * @param f_t The f(t) value
 * @return The indx of the newly added key
 */
int piecewiseLinearFunction::addKeyAt( double t, double f_t ) {

	if ( f_t < 0.0 ) {

		f_t = 0.0;
	}

	int result = keys.size();
	keys.push_back( pair<double,double>( t, f_t ) );

	// Push the new key up in the list until it is in place (sorted)
	while ( result > 0 && keys[ result - 1 ].first > t ) {

		keys[ result ] = keys[ result - 1 ];
		keys[ result - 1 ].first = t;
		keys[ result - 1 ].second = f_t;
		--result;
	}

	return result;
}


/**
 * Moves the specified key to the new position
 * @param index The index of the key to move
 * @param t The new t value
 * @param f_t The new f(t) value
 * @return TRUE if the key was moved successfully, FALSE otherwise
 */
bool piecewiseLinearFunction::moveKeyTo( uint32_t index, double t, double f_t ) {

	if ( index > 0 && t <= keys[ index - 1 ].first ) {

		return false;
	}

	if ( index < keys.size() - 1 && t >= keys[ index + 1 ].first ) {

		return false;
	}

	if ( f_t < 0.0 ) {

		f_t = 0.0;
	}

	keys[ index ].first = t;
	keys[ index ].second = f_t;

	return true;
}


/**
 * Removes the specified key from the list
 * @param index The index of the key to remove
 */
void piecewiseLinearFunction::removeKey( uint32_t index ) {

	if ( index < 0 || index >= keys.size() ) {

		return;
	}

	// Move all keys after given key up one
	for ( ; index + 1 < keys.size(); ++index ) {

		keys[ index ] = keys[ index + 1 ];
	}

	keys.pop_back();
}


/**
 * Finds the maximum f(t) value in the function
 * @return The maximum f(t) value in the function
 */
double piecewiseLinearFunction::maxValue() const {

	double result = -1.0e300;

	for ( uint32_t i = 0; i < keys.size(); ++i ) {

		if ( keys[ i ].second > result ) {

			result = keys[ i ].second;
		}
	}

	return result;
}


/**
 * Removes all the keys before the specified key
 * @param t The first desired key in the function
 */
void piecewiseLinearFunction::clearAbove( double t ) {

	while ( !keys.empty() && keys.back().first > t ) {

		keys.pop_back();
	}
}


/**
 * Removes all keys after the specified key
 * @param t The last desired key in the function
 */
void piecewiseLinearFunction::clearBelow( double t ) {

	uint32_t i = 0;

	// Find the desired key
	while ( i < keys.size() && keys[ i ].first < t ) {

		++i;
	}

	// If the key was not the first key
	if ( i > 0 ) {

		uint32_t j;

		// Copy all keys from t to the end to the front of the list
		for ( j = 0; j + i < keys.size(); ++j ) {

			keys[ j ] = keys[ j + i ];
		}

		// Empty the back of the list
		while ( keys.size() > j ) {

			keys.pop_back();
		}
	}
}


/**
 * Draws the function
 */
void piecewiseLinearFunction::draw() const {

	if ( keys.empty() ) {

		return;
	}

	glColor3ub( 0, 0, 0 );
	glPointSize( 10 );
	uint32_t i;

	glBegin( GL_POINTS );
		for ( i = 0; i < keys.size(); ++i ) {
			
			glVertex2d( keys[ i ].second, keys[ i ].first );
		}
	glEnd();

	glPointSize( 1 );
	glLineWidth( 1 );

	glBegin( GL_LINE_STRIP );
		glVertex2d( keys.front().second, -100000.0 );

		for ( i = 0; i < keys.size(); ++i ) {
			
			glVertex2d( keys[ i ].second, keys[ i ].first );
		}

		glVertex2d( keys.back().second, 100000.0 );
	glEnd();
}


/**
 * Multiplication assignment operator
 * @param s The amount to scale the function
 * @return A reference to the function after scaling
 */
piecewiseLinearFunction & piecewiseLinearFunction::operator *= ( double s ) {

	for ( uint32_t i = 0; i < keys.size(); ++i ) {

		keys[ i ].second *= s;
	}

	return *this;
}


/**
 * Division assignment oeprator
 * @param s The amount to shrink the function
 * @return A reference to the function after shrinking
 */
piecewiseLinearFunction & piecewiseLinearFunction::operator /= ( double s ) {

	for ( uint32_t i = 0; i < keys.size(); ++i ) {

		keys[ i ].second /= s;
	}

	return *this;
}


// ***** NUMBER ENTRY ***** //

/**
 * Handles keyboard input
 * @param c The character input
 * @return TRUE if the input was handled, FALSE otherwise
 */
bool numberEntry::acceptKey( char c ) {

	// Make sure the character is a valid input
	if ( ( ( c == '-' || c == '+' ) && ( cursor == 0 || text[ cursor - 1 ] == 'e' || text[ cursor - 1 ] == 'E' ) )
		|| ( c >= '0' && c <= '9' )
		|| ( c == '\b' || c == 127 ) // "delete" key...
		|| ( c == 'e' || c == 'E' || c == '.' ) )
	{
		int i;

		// Make sure it's OK to put an e in (for scientific notation)
		if ( c == 'e' || c == 'E' ) {

			// Invalid position for an "e" (scientific notation)
			if ( cursor == 0 || ( cursor == 1 && text[ 0 ] == '.' ) || ( cursor == 2 && text[ 1 ] == '.' && ( text[ 0 ] == '-' || text[ 0 ] == '+' ) ) ) {

				return false;
			}

			// Check for an existing "e" (already in scientific notation)
			for (i = 0; i < index; ++i ) {

				if ( text[ i ] == 'i' || text[ i ] == 'E' ) {

					return false;
				}
			}
		}

		// Check for an existing decimal point
		if ( c == '.' ) {

			for ( i = 0; i < index; ++i ) {

				if ( text[ i ] == '.' ) {

					return false;
				}
			}
		}

		if ( c == '\b' ) {

			if ( cursor == 0 ) {

				return false;
			}

			if ( text[ cursor - 1 ] == 'e' && cursor < index && text[ cursor ] < '0' ) {

				// If we have a -##e+## and the "e" is deleted, delete the "+", too
				for ( i = cursor + 1; i < index; ++i ) {

					text[ i - 2 ] = text[ i ];
				}

				index -= 2;
				--cursor;
			}
			else {

				for ( i = cursor; i < index; ++i ) {

					text[ i - 1 ] = text[ i ];
				}

				--index;
				--cursor;
			}

			return true;
		}

		// "delete" key
		if ( c == 127 ) {

			if ( cursor >= index ) {

				return false;
			}

			if ( text[ cursor ] == 'e' && cursor < index - 1 && text[ cursor + 1 ] < '0' ) {

				// If we have a -##e+## and the "e" is deleted, delete the "+", too
				for ( i = cursor + 1; i < index - 1; ++i ) {

					text[ i - 1 ] = text[ i + 1 ];
				}

				index -= 2;
			}
			else {

				for ( i = cursor; i < index - 1; ++i ) {

					text[ i ] = text[ i + 1 ];
				}

				--index;
			}

			return true;
		}

		// Can't have more than 256 characters (declared buffer limit)
		if ( index >= 256 ) {

			return false;
		}

		// Move other characters out of the way
		for ( i = index; i > cursor; --i ) {

			text[ i ] = text[ i - 1 ];
		}

		// Insert the character
		text[ cursor ] = c;
		++index;
		++cursor;

		return true;
	}

	return false;
}


/**
 * Handles special keyboard input
 * @param c The character input
 * @return TRUE if the key input was handled, FALSE otherwise
 */
bool numberEntry::acceptOtherKey( int c ) {

	// Only deal with valid key inputs
	if ( c == GLUT_KEY_LEFT || c == GLUT_KEY_RIGHT || c == GLUT_KEY_HOME || c == GLUT_KEY_END ) {

		if ( c == GLUT_KEY_LEFT && cursor > 0 ) {

			--cursor;
		}

		if ( c == GLUT_KEY_RIGHT && cursor < index ) {

			++cursor;
		}

		if ( c == GLUT_KEY_HOME ) {

			cursor = 0;
		}

		if ( c == GLUT_KEY_END ) {

			cursor = index;
		}

		return true;
	}

	return false;
}


// ***** USER-ENTERED NUMBERS ***** //

/**
 * Gets the minimum edge length specified by the user
 * @return The minimum edge length specified by the user
 */
double getMinEdgeLength() {

	return ne[ 0 ].getValue();
}


/**
 * Gets the maximum edge length specified by the user
 * @return The maximum edge length specified by the user
 */
double getMaxEdgeLength() {

	return ne[ 1 ].getValue();
}


/**
 * Gets the overdrive value specified by the user
 * @return The overdrive value specified by the user
 */
double getOverdrive() {

	return ne[ 2 ].getValue();
}


/**
 * Gets the noise power specified by the user
 * @return The noise power specified by the user
 */
double getNoisePower() {

	return ne[ 3 ].getValue();
}


/**
 * Gets the air-to-water ratio specified by the user
 * @return The air-to-water ratio specified by the user
 */
double getAirWaterRatio() {

	return ne[ 4 ].getValue();
}


/**
 * Gets the fluid scale specified by the user
 * @return The fluid scale specified by the user
 */
double getFluidScale() {

	return ne[ 5 ].getValue();
}


/**
 * Sets the minimum edge length
 * @param x The new minimum edge length
 */
void setMinEdgeLength( double x ) {

	ne[ 0 ].setValue( x );
}


/**
 * Sets the maximum edge length
 * @param x The new maximum edge length
 */
void setMaxEdgeLength( double x ) {

	ne[ 1 ].setValue( x );
}


/**
 * Sets the overdrive value
 * @param x The new overdrive value
 */
void setOverdrive( double x ) {

	ne[ 2 ].setValue( x );
}


/**
 * Sets the noise power
 * @param x The new noise power
 */
void setNoisePower( double x ) {

	ne[ 3 ].setValue( x );
}


/**
 * Sets the air-to-water ratio
 * @param x The new air-to-water ratio
 */
void setAirWaterRatio( double x ) {

	ne[ 4 ].setValue( x );
}


/**
 * Sets the fluid scale
  8@param x The new fluid scale
 */
void setFluidScale( double x ) {

	ne[ 5 ].setValue( x );
}


// ***** LAYER ***** //

#include "PerlinNoise.h"

/**
 * Adjusts the noise by clamping it to the range [0,1] and then raising it to the user-specified power
 * @param noise The noise value to adjust
 * @return The adjusted noise value
 */
double fixNoise( double noise ) {

	if ( noise < 0.0 ) {

		noise = 0.0;
	}

	if ( noise > 1.0 ) {

		noise = 1.0;
	}

	return pow( noise, getNoisePower() );
}


/**
 * Evaluates the layer at the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param seed The seed value for random number generator (default 0)
 * @param userGrain Whether or not to use the grain (default TRUE)
 * @return The durability of the layer at the given point
 */
double layer::evaluate( double x, double y, double z, uint32_t seed, bool useGrain ) const {

	if ( useGrain ) {

		return base.evaluate( y ) * ( 1.0 - fixNoise( perlin1D( y * yNoiseFrequency, 4, seed ) ) * yNoiseAmplitude ) * ( 1.0 - fixNoise( perlin3D( x * grainFrequency, y * grainFrequency, z * grainFrequency, 4, seed + 0x005a5a5a ) ) * grainAmplitude );
	}
	else {

		return base.evaluate( y ) * ( 1.0 - fixNoise( perlin1D( y * yNoiseFrequency, 4, seed ) ) * yNoiseAmplitude );
	}
}


// ***** DURABILITY TOOL ***** //

/**
 * Finds the nearest division to the given y value
 * @param y The y-coordinate of the mouse
 * @return The index of the nearest split to the given y value
 */
int durabilityTool::nearestSplitTo( double y ) const {

	if ( 0 >= dividers.size() ) {

		return -1;
	}

	if ( y <= dividers.front() ) {

		return 0;
	}

	if ( y >= dividers.back() ) {

		return dividers.size() - 1;
	}

	int min = 0;
	int max = dividers.size() - 1;
	int k;

	// Binary searcy
	while ( max > min + 1 ) {

		k = ( ( min + max ) >> 1 );

		if ( dividers[ k ] < y ) {

			min = k;
		}
		else {

			max = k;
		}
	}

	if ( min == max ) {

		return min;
	}

	if ( dividers[ max ] - y > y - dividers[ min ] ) {

		return min;
	}

	return max;
}


/**
 * Adds a new split
 * @param y The new split value
 * @return The index of the new split
 */
int durabilityTool::addSplit( double y ) {

	int result;

	//#pragma omp critical( softnessCopier )
	//{
		result = dividers.size();
		layer clone = *layerAt( y );
		dividers.push_back( y );
		layers.push_back( clone );

		while ( result > 0 && dividers[ result ] < dividers[ result - 1 ] ) {

			dividers[ result ] = dividers[ result - 1 ];
			dividers[ result - 1 ] = y;
			layers[ result + 1 ] = layers[ result ];
			layers[ result ] = clone;
			--result;
		}
	//}

	normalize();

	return result;
}


/**
 * Removes the specified split
 * @param index the index of the split to remove
 */
void durabilityTool::removeSplit( uint32_t index ) {

	bool shouldNormalize = false;

	//#pragma omp critical( softnessCopier )
	//{
		if ( index >= 0 && index < dividers.size() ) {

			shouldNormalize = true;

			for ( ; index + 1 < dividers.size(); ++index ) {

				dividers[ index ] = dividers[ index + 1 ];
				layers[ index + 1 ] = layers[ index + 2 ];
			}

			dividers.pop_back();
			layers.pop_back();
		}
	//}
	
	if ( shouldNormalize ) {

		normalize();
	}
}


/**
 * Moves the specified split to the new location (does not allow reordering of y values)
 * @param index The index of the split to move
 * @param y The new location of the split
 * @return TRUE if the move succeeded, FALSE otherwise
 */
bool durabilityTool::moveSplitTo( uint32_t index, double y ) {

	bool result = true;

	//#pragma omp critical( softnessCopier )
	//{
		if ( index > 0 && y <= dividers[ index - 1 ] ) {

			result = false;
		}
		else if ( index < dividers.size() - 1 && y >= dividers[ index + 1 ] ) {

			result = false;
		}
		else {

			dividers[ index ] = y;
		}
	//}

	return result;
}


/**
 * Gets the layer at the specified y value
 * @param y The y value to get the layer for
 * @return The layer at the given y value
 */
layer * durabilityTool::layerAt( double y ) {

	if ( 0 >= dividers.size() || y <= dividers.front() ) {

		return &layers[ 0 ];
	}

	if ( y >= dividers.back() ) {

		return &layers[ dividers.size() ];
	}

	int min = 0;
	int max = dividers.size() - 1;
	int k;

	while ( max > min + 1 ) {

		k = ( ( min + max ) >> 1 );

		if ( dividers[ k ] < y ) {

			min = k;
		}
		else {

			max = k;
		}
	}

	return &layers[ min + 1 ];
}


/**
 * Gets the layer at the specified y value
 * @param y The y value to get the layer for
 * @param index Stores the index of the layer
 * @return The layer at the given y value
 */
const layer * durabilityTool::layerAt( double y, int * index ) const {

	if ( 0 >= dividers.size() || y <= dividers.front() ) {

		if ( index ) {

			*index = 0;
		}

		return &layers[ 0 ];
	}

	if ( y >= dividers.back() ) {

		if ( index ) {

			*index = dividers.size();
		}

		return &layers[ dividers.size() ];
	}

	int min = 0;
	int max = dividers.size() - 1;
	int k;

	while ( max > min + 1 ) {

		k = ( ( min + max ) >> 1 );

		if ( dividers[ k ] < y ) {

			min = k;
		}
		else {

			max = k;
		}
	}

	if ( index ) {

		*index = min + 1;
	}

	return &layers[ min + 1 ];
}


/**
 * Draws the durability widget
 * @param minY The minimum y-coordinate
 * @param maxY The maximum y-coordinate
 * @param steps The number of steps between minY and maxY
 */
void durabilityTool::draw( double minY, double maxY, int steps ) const {

	if ( maxY <= minY ) {

		return;
	}

	// First clip-and-draw the control polyline for each layer
	glEnable( GL_CLIP_PLANE0 );
	glEnable( GL_CLIP_PLANE1 );
	GLdouble planeEq0[ 4 ] = { 0.0, -1.0, 0.0, 1.0 };
	GLdouble planeEq1[ 4 ] = { 0.0, +1.0, 0.0, 1.0 };
	double bottom;
	double top;

	for ( uint32_t i = 0; i < layers.size(); ++i ) {

		if ( i != 0 ) {

			bottom = dividers[ i - 1 ];
		}
		else {

			bottom = -100000.0;
		}

		if ( i < dividers.size() ) {

			top = dividers[ i ];
		}
		else {

			top = +100000.0;
		}

		planeEq0[ 3 ] = +top;
		glClipPlane( GL_CLIP_PLANE0, planeEq0 );

		planeEq1[ 3 ] = -bottom;
		glClipPlane( GL_CLIP_PLANE1, planeEq1 );

		layers[ i ].base.draw();
	}

	glDisable( GL_CLIP_PLANE0 );
	glDisable( GL_CLIP_PLANE1 );

	// Now draw the rock density plot
	double y;
	double x;

	// Air first
	glColor3ub( 0, 0, 0 );
	glBegin( GL_LINE_STRIP );
		for ( y = minY; y <= maxY; y += ( maxY - minY ) / steps ) {
		
			x = evaluate( 0, y, 0, false );
			glVertex2d( x, y );
		}
	glEnd();

	double d;
	GLdouble rgb[ 3 ];

	// Now grain noise
	glPointSize( 5 );
	glBegin( GL_POINTS );
		for ( y = minY; y <= maxY; y += 3.0 * ( maxY - minY ) / steps ) {

			for ( x = 0.0; x <= 1.0; x += 0.02 ) {

				d = evaluate( x, y, 0.0 );
				hueToRGB( rgb, 0.6666666666666667 * ( 1.0 - d ) );
				glColor3dv( rgb );
				glVertex2d( x, y );
			}
		}
	glEnd();
	glPointSize( 1 );

	// Finally the layer splits
	glColor3f( 0.0f, 0.0f, 0.0f );
	glBegin( GL_LINES );
		for ( uint32_t i = 0; i < dividers.size(); ++i ) {
			
			glVertex2d( -0.2, dividers[ i ] );
			glVertex2d( 0.0, dividers[ i ] );
		}
	glEnd();
}


/**
 * ???
 * @return ???
 */
bool durabilityTool::normalize() {

	bool result = false;

	//#pragma omp critical( softnessCopier )
	//{
		uint32_t i;

		for ( i = 0; i < dividers.size(); ++i ) {

			layers[ i ].base.clearAbove( dividers[ i ] );
			layers[ i + 1 ].base.clearBelow( dividers[ i ] );
		}

		double ms = 0.0;
		double temp;

		for ( i = 0; i < layers.size(); ++i ) {

			temp = layers[ i ].base.maxValue();

			if ( temp > ms ) {

				ms = temp;
			}
		}

		if ( ms == 1.0 ) {

			result = true;
		}
		else {

			for ( i = 0; i < layers.size(); ++i ) {

				layers[ i ].base /= ms;
			}

			result = true;
		}
	//}

	return result;
}


// ***** SOFTNESS CALLBACK METHODS ***** //

/**
 * Updates the locked version of the durability widget
 */
void updateLockedDurability() {

	//#pragma omp critical( softnessCopier )
	//{
		lockedDt = dt;
	//}

	lockedDt.normalize();
}


/**
 * Computes the softness at the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the object at the given point
 */
double softness( double x, double y, double z ) {

	return lockedDt.evaluate( x, y, z );
}


/**
 * Computes the softness at the given point if it is live
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the given point if it is live
 */
double liveSoftness( double x, double y, double z ) {

	return dt.evaluate( x, y, z );
}


// ***** LAYER PANE ***** //

/**
 * Stores the last observed y value
 */
static double lastObservedYValue;


/**
 * ???
 * @return ???
 */
double layerPane::sx() const {

	return ( currentMx - x ) * 1.2 / ( double ) w - 0.2;
}


/**
 * ???
 * @return ???
 */
double layerPane::sy() const {

	return ( currentMy - y ) * 2.0 * zoom / ( double ) h + lastObservedYValue - zoom;
}


/**
 * ???
 * @param sx ???
 * @return ???
 */
double layerPane::px( double sx ) const {

	return ( sx + 0.2 ) * w / 1.2 + x;
}


/**
 * ???
 * @param sy ???
 * @return ???
 */
double layerPane::py( double sy ) const {

	return ( sy - lastObservedYValue + zoom ) * h / ( 2.0 * zoom ) + y;
}


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w Teh width of the screen region
 * @param h The height of the screen region
 */
void layerPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void layerPane::draw( const stepResults & results ) const {

	lastObservedYValue = results.midPoint[ 1 ];
	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glOrtho( -0.2, 1.0, results.midPoint[ 1 ] - zoom, results.midPoint[ 1 ] + zoom, -1.0, 1.0 );
	dt.draw( results.midPoint[ 1 ] - zoom, results.midPoint[ 1 ] + zoom, h );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * layerPane::acceptMouse( int button, int x, int y ) {

	currentMx = x;
	currentMy = y;

	// Hit layer split side
	if ( sx() <= 0.0 ) {

		int nearestSplit = dt.nearestSplitTo( sy() );

		if ( nearestSplit == -1 || abs( py( dt.splitValue( nearestSplit ) ) - currentMy ) > 5.0 ) {

			// Add a split or change y noise
			if ( button == GLUT_LEFT_BUTTON ) {

				thingID = dt.addSplit( sy() );
				mouseMode = MOVE_SPLIT;
			}
			else {

				liveLayer = dt.layerAt( sy() );
				mouseMode = CHANGE_LAYER_NOISE;
			}
		}

		// Select or delete a split
		else {

			if ( button == GLUT_LEFT_BUTTON ) {

				thingID = nearestSplit;
				mouseMode = MOVE_SPLIT;
			}
			else {

				dt.removeSplit( nearestSplit );
				mouseMode = IGNORE_MOUSE;
			}
		}
	}
	else {

		liveLayer = dt.layerAt( sy() );
		thingID = liveLayer->base.nearestKeyTo( sy() );
		pair<double,double> temp;

		if ( thingID != -1 ) {

			temp = liveLayer->base.valueOf( thingID );
		}

		// Change nodes
		if ( button == GLUT_LEFT_BUTTON ) {

			if ( thingID == -1 || abs( py( temp.first ) - y ) > 5.0 ) {

				//#pragma omp critical( softnessCopier )
				//{
					thingID = liveLayer->base.addKeyAt( sy(), sx() );
				//}

				mouseMode = MOVE_POINT;
			}
			else {

				//#pragma omp critical( softnessCopier )
				//{
					liveLayer->base.moveKeyTo( thingID, sy(), sx() );
				//}

				mouseMode = MOVE_POINT;
			}
		}

		// Delete node or change noise
		else {

			if ( thingID == -1 || abs( py( temp.first ) - y ) > 5.0 || abs( px( temp.second ) - x ) > 5.0 ) {

				mouseMode = CHANGE_GRAIN;
			}
			else {

				//#pragma omp critical( softnessCopier )
				//{
					liveLayer->base.removeKey( thingID );
				//}

				mouseMode = IGNORE_MOUSE;
			}
		}
	}

	currentButton = button;

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void layerPane::dragMouse( int x, int y ) {

	int dx = x - currentMx;
	int dy = y - currentMy;

	if ( !dx && !dy ) {

		return;
	}

	currentMx = x;
	currentMy = y;

	switch ( mouseMode ) {

		case IGNORE_MOUSE:

			break;

		case MOVE_SPLIT:

			dt.moveSplitTo( thingID, sy() );
			break;

		case MOVE_POINT:

			//#pragma omp critical( softnessCopier )
			//{
				liveLayer->base.moveKeyTo( thingID, sy(), sx() );
			//}

			break;

		case CHANGE_GRAIN:

			if ( dy >= 100 ) {

				dy = 99;
			}

			//#pragma omp critical( softnessCopier )
			//{
				liveLayer->grainAmplitude += dx * 1.2 / w;

				if ( liveLayer->grainAmplitude < 0.0 ) {

					liveLayer->grainAmplitude = 0.0;
				}

				if ( liveLayer->grainAmplitude > 1.0 ) {

					liveLayer->grainAmplitude = 1.0;
				}

				liveLayer->grainFrequency *= ( 1.0 - dy * 0.01 );
			//}

			break;

		case CHANGE_LAYER_NOISE:

			if ( dy >= 100 ) {

				dy = 99;
			}

			//#pragma omp critical( softnessCopier )
			//{
				liveLayer->yNoiseAmplitude += dx * 1.2 / w;

				if ( liveLayer->yNoiseAmplitude < 0.0 ) {

					liveLayer->yNoiseAmplitude = 0.0;
				}

				if ( liveLayer->yNoiseAmplitude > 1.0 ) {

					liveLayer->yNoiseAmplitude = 1.0;
				}

				liveLayer->yNoiseFrequency *= ( 1.0 - dy * 0.01 );
			//}

			break;

		default:

			break;
	}
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool layerPane::readjust() {

	if ( dt.normalize() ) {

		recolorize();

		return true;
	}
	else {

		return false;
	}
}


// ***** DATA PANE ***** //

/**
 * Default constructor
 */
dataPane::dataPane() : row( 0 ) {

	setMinEdgeLength( 0.02 );
	setMaxEdgeLength( 0.10 );
	setOverdrive( 1.0 );
	setNoisePower( 3.0 );
	setAirWaterRatio( 1.0 );
	setFluidScale( 0.0 );
}


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w Teh width of the screen region
 * @param h The height of the screen region
 */
void dataPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void dataPane::draw( const stepResults & results ) const {

	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 0.8, 0.8, 0.8, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glOrtho( 0, w, 0, h, -1, 1 );
	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
	glBegin( GL_LINES );
		glVertex2f( 4 + ( ne[ row ].cursor + ne[ row ].horizontalLength ) * 8, 3 + row * 15 );
		glVertex2f( 4 + ( ne[ row ].cursor + ne[ row ].horizontalLength ) * 8, 17 + row * 15 );
	glEnd();

	int i;
	int k;

	for ( i = numberEntryLength - 1; i >= 0; --i ) {

		glRasterPos2f( 4.0f, 4.0f + i * 15.0f );

		for ( k = 0; k < ne[ i ].getLength(); ++k ) {

			glutBitmapCharacter( GLUT_BITMAP_8_BY_13, ne[ i ].getCharacter( k ) );
		}
	}
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * dataPane::acceptMouse( int button, int x, int y ) {

	row = ( y - this->y - 4 ) / 15;

	if ( row < 0 ) {

		row = 0;
	}

	if ( row >= numberEntryLength ) {

		row = numberEntryLength - 1;
	}

	ne[ row ].setCursor( ( x - this->x + 4 ) / 8 );

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void dataPane::dragMouse( int x, int y ) {

	return;
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool dataPane::readjust() {

	return false;
}


/**
 * Called when the user presses a key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool dataPane::keyEvent( char c ) {

	bool result = ne[ row ].acceptKey( c );

	if ( result ) {

		if ( row != 0 && ne[ row ].getValue() < 0 ) {

			ne[ row ].setValue( 0.0 );
		}

		if ( row == 0 && ne[ 1 ].getValue() < ne[ 0 ].getValue() ) {

			ne[ 1 ].setValue( ne[ 0 ].getValue() );
		}

		if ( row == 1 && ne[ 0 ].getValue() > ne[ 1 ].getValue() ) {

			ne[ 0 ].setValue( ne[ 1 ].getValue() );
		}

		if ( getAirWaterRatio() > 1.0 ) {

			setAirWaterRatio( 1.0 );
		}

		if ( getAirWaterRatio() < 0.0 ) {

			setAirWaterRatio( 0.0 );
		}
	}

	return result;
}


/**
 * Called when the user presses a special key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool dataPane::specialKeyEvent( int c ) {

	return ne[ row ].acceptOtherKey( c );
}


/**
 * Gets teh value of the specified data
 * @param index The index of the data to get
 * @return The value of the specified data
 */
double dataPane::getValue( uint32_t index ) const {

	return ne[ index ].getValue();
}


#include <cstdarg>

/**
 * Draws the given string using OpenGL
 * @param row The raster 2D row to draw to
 * @param s The format string to draw
 * @param ... The parameters for s
 */
void myDrawString( int row, const char * s, ... ) {

	char temp[ 256 ];
	va_list list;

	va_start( list, s );
#ifdef __GNUC__
	vsnprintf( temp, 256, s, list );
#else
	vsnprintf_s( temp, 256, 256, s, list );
#endif
	va_end( list );

	glRasterPos2f( 4.0f, 4.0f + row * 15.0f );

	for ( int i = 0; temp[ i ]; ++i ) {

		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, temp[ i ] );
	}
}


// ***** STATUS PANE ***** //

/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w Teh width of the screen region
 * @param h The height of the screen region
 */
void statusPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void statusPane::draw( const stepResults & results ) const {

	static double lastSecondsTotal = 0.0;
	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 0.8, 0.8, 0.8, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	glOrtho( 0, w, 0, h, -1, 1 );

	glColor4f( 0.2f, 0.2f, 0.2f, 1.0f );

	// WORKAROUND: glColor, on some platforms, does not change the color until glVertex is called...
	glBegin( GL_LINES );
		glVertex2f( -1.0f, -1.0f );
		glVertex2f( -2.0f, -2.0f );
	glEnd();

	int i = 0;

	myDrawString( i++, "Last frame took %.2g seconds", results.secondsTotal );
	myDrawString( i++, "CGAL Time: %.4g seconds", results.secondsCGAL );
	myDrawString( i++, "Label Time: %.4g seconds", results.secondsLabeling );
	//printf( "Label Time: %.4g seconds\n", results.secondsLabeling );
	//printf( "CGAL Time: %.4g seconds\n", results.secondsCGAL );
	//printf( "Comp. Time: %.4g seconds\n", StoneWeatherer::secondsTotal - lastSecondsTotal );
	//printf( "Last frame took %.2g seconds\n\n", results.secondsTotal );
	//lastSecondsTotal = StoneWeatherer::secondsTotal;
#ifdef __GNUC__
	myDrawString( i++, "Memory: %f MB", ( memorySizer.virtual_size() / 10486 ) * 0.01 );
#endif
	myDrawString( i++, "Tetrahedra: %d", results.numTetrahedrons );
	myDrawString( i++, "Triangles:  %d", results.numVertices * 2 - 4 );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * statusPane::acceptMouse( int button, int x, int y ) {

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void statusPane::dragMouse( int x, int y ) {

	return;
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool statusPane::readjust() {

	return false;
}


/**
 * Called when the user presses a key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool statusPane::keyEvent( char c ) {

	return false;
}


/**
 * Called when the user presses a special key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool statusPane::specialKeyEvent( int c ) {

	return false;
}
