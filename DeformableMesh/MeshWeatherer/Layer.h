#pragma once

#include "PiecewiseLinearFunction.h"


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
