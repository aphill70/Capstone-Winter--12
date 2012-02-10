#pragma once

#include <vector>
#include <map>

#include "Include\IntTypes.h"

using namespace std;


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
