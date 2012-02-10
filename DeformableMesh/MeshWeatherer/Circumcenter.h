#pragma once

#include "CGAL_typedefs.h"
#include "Point3D.h"
#include "StoneWeatherer.h"


/**
 * A wrapper around a CGAL point and Cell_handle
 */
struct Circumcenter {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The location of the circumcenter
	 */
	Point3D point;

	/**
	 * The cell the circumcenter belongs to
	 */
	Cell_handle cell;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	Circumcenter();


	/**
	 * Full constructor
	 * @param point The location of the circumcenter
	 * @param cell The cell the circumcenter belongs to
	 */
	Circumcenter( const Point3D & point, Cell_handle & cell );


	/**
	 * Component constructor
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param cell The cell the circumcenter belongs to
	 */
	Circumcenter( double x, double y, double z, Cell_handle & cell );


	/**
	 * Copy constructor
	 * @param other The circumcenter to copy
	 */
	Circumcenter( const Circumcenter & other );
};
