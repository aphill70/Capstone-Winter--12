#include <GL\glut.h>

#include "PiecewiseLinearFunction.h"


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
