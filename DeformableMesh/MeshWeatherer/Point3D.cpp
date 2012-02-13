#include "Point3D.h"


//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
Point3D::Point3D() : x( 0.0 ), y( 0.0 ), z( 0.0 ) {

	return;
}


/** 
 * Full constructor
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 */
Point3D::Point3D( double x, double y, double z ) : x( x ), y( y ), z( z ) {

	return;
}


/**
 * Copy constructor
 * @param other The point to copy
 */
Point3D::Point3D( const Point3D & other ) : x( other.x ), y( other.y ), z( other.z ) {

	return;
}
