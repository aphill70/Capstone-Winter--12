#include "Bbox.h"


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
Bbox::Bbox( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax ) : xmin( xmin ), ymin( ymin ), zmin( zmin ), xmax( xmax ), ymax( ymax ), zmax( zmax ) {

	return;
}


/**
 * Copy constructor
 * @param other The bounding box to copy
 */
Bbox::Bbox( const Bbox & other ) : xmin( other.xmin ), ymin( other.ymin ), zmin( other.zmin ), xmax( other.xmax ), ymax( other.ymax ), zmax( other.zmax ) {

	return;
}
