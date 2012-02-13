#include "Circumcenter.h"


//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
Circumcenter::Circumcenter() : point(), cell() {

	return;
}


/**
 * Full constructor
 * @param point The location of the circumcenter
 * @param cell The cell the circumcenter belongs to
 */
Circumcenter::Circumcenter( const Point3D & point, Cell_handle & cell ) : point( point ), cell( cell ) {
	
	return;
}


/**
 * Component constructor
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param cell The cell the circumcenter belongs to
 */
Circumcenter::Circumcenter( double x, double y, double z, Cell_handle & cell ) : point( x, y, z ), cell( cell ) {
	
	return;
}


/**
 * Copy constructor
 * @param other The circumcenter to copy
 */
Circumcenter::Circumcenter( const Circumcenter & other ) : point( other.point ), cell( other.cell ) {

	return;
}
