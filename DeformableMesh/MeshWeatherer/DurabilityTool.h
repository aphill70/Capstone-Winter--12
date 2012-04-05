#pragma once

#include <vector>
#include "Layer.h"
#include "Include\IntTypes.h"

using namespace std;


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
