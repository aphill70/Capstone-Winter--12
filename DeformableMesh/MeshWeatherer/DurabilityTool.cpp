#include <GL\glut.h>

#include "DurabilityTool.h"
#include "Layer.h"


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


/**
 * Create the durability widget
 */
durabilityTool durability;

/**
 * Create a locked durability widget (locked while simulating)
 */
durabilityTool lockedDurability;


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

	normalize();

	return result;
}


/**
 * Removes the specified split
 * @param index the index of the split to remove
 */
void durabilityTool::removeSplit( uint32_t index ) {

	bool shouldNormalize = false;

	if ( index >= 0 && index < dividers.size() ) {

		shouldNormalize = true;

		for ( ; index + 1 < dividers.size(); ++index ) {

			dividers[ index ] = dividers[ index + 1 ];
			layers[ index + 1 ] = layers[ index + 2 ];
		}

		dividers.pop_back();
		layers.pop_back();
	}
	
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

	if ( index > 0 && y <= dividers[ index - 1 ] ) {

		result = false;
	}
	else if ( index < dividers.size() - 1 && y >= dividers[ index + 1 ] ) {

		result = false;
	}
	else {

		dividers[ index ] = y;
	}

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

	return result;
}


/**
 * Updates the locked version of the durability widget
 */
void updateLockedDurability() {

	lockedDurability = durability;

	lockedDurability.normalize();
}


/**
 * Computes the softness at the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the object at the given point
 */
double softness( double x, double y, double z ) {

	return lockedDurability.evaluate( x, y, z );
}


/**
 * Computes the softness at the given point if it is live
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the given point if it is live
 */
double liveSoftness( double x, double y, double z ) {

	return durability.evaluate( x, y, z );
}
