#include "Tetrahedron.h"


//------------
// CONSTRUCTOR
//------------

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/**
 * Default constructor
 */
Tetrahedron::Tetrahedron() : v0(), v1(), v2(), v3(), cell(), box( 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 ), _volume( 0.0 ) {

	return;
}


/**
 * Full constructor
 * @param v0 The first vertex of the tetrahedron
 * @param v1 The first vertex of the tetrahedron
 * @param v2 The first vertex of the tetrahedron
 * @param v2 The first vertex of the tetrahedron
 * @param type The material type
 */
Tetrahedron::Tetrahedron( const Point3D & v0, const Point3D & v1, const Point3D & v2, const Point3D & v3, Cell_handle & cell ) : v0( v0 ), v1( v1 ), v2( v2 ), v3( v3 ), cell( cell ), box( MIN( MIN( v0.x, v1.x ), MIN( v2.x, v3.x ) ),  MIN( MIN( v0.y, v1.y ), MIN( v2.y, v3.y ) ),  MIN( MIN( v0.z, v1.z ), MIN( v2.z, v3.z ) ),  MAX( MAX( v0.x, v1.x ), MAX( v2.x, v3.x ) ), MAX( MAX( v0.y, v1.y ), MAX( v2.y, v3.y ) ), MAX( MAX( v0.z, v1.z ), MAX( v2.z, v3.z ) ) ) {

	_volume = MACRO_ABS( determinant(
		v0.x, v0.y, v0.z, 1.0,
		v1.x, v1.y, v1.z, 1.0,
		v2.x, v2.y, v2.z, 1.0,
		v3.x, v3.y, v3.z, 1.0
	) / 6.0 );
}


/**
 * Copy constructor
 * @param other The tetrahedron to copy
 */
Tetrahedron::Tetrahedron( const Tetrahedron & other ) : v0( other.v0 ), v1( other.v1 ), v2( other.v2 ), v3( other.v3 ), cell( cell ), box( other.box ) {

	_volume = other._volume;
}
