#pragma once


/**
 * Represents a point in 3D Euclidean space
 */
struct Point3D {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The x-coordinate of the point
	 */
	double x;

	/**
	 * The y-coordinate of the point
	 */
	double y;

	/**
	 * The z-coordinate of the point
	 */
	double z;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	Point3D();


	/** 
	 * Full constructor
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 */
	Point3D( double x, double y, double z );


	/**
	 * Copy constructor
	 * @param other The point to copy
	 */
	Point3D( const Point3D & other );
};
