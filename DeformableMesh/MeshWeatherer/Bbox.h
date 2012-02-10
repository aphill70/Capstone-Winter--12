#pragma once

#include "Point3D.h"


/**
 * A bounding box, with minimum and maximum points
 */
struct Bbox {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The x-coordinate of the minimum point
	 */
	double xmin;

	/**
	 * The y-coordinate of the minimum point
	 */
	double ymin;

	/**
	 * The z-coordinate of the minimum point
	 */
	double zmin;

	/**
	 * The x-coordinate of the maximum point
	 */
	double xmax;

	/**
	 * The y-coordinate of the maximum point
	 */
	double ymax;

	/**
	 * The z-coordinate of the maximum point
	 */
	double zmax;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Full constructor
	 * @param xmin The x-coordinate of the minimum point
	 * @param ymin The y-coordinate of the minimum point
	 * @param zmin The z-coordinate of the minimum point
	 * @param xmax The x-coordinate of the maximum point
	 * @param ymax The y-coordinate of the maximum point
	 * @param zmax The z-coordinate of the maximum point
	 */
	Bbox( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax );


	/**
	 * Copy constructor
	 * @param other The bounding box to copy
	 */
	Bbox( const Bbox & other );
};
