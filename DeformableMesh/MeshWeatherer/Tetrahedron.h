#pragma once

#include "Bbox.h"
#include "CGAL_typedefs.h"
#include "Point3D.h"
#include "StoneWeatherer.h"
#include "Utils.h"


/**
 * A wrapper around a tetrahedron and the cell it belongs to
 */
struct Tetrahedron {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The first vertex of the tetrahedron
	 */
	Point3D v0;

	/**
	 * The second vertex of the tetrahedron
	 */
	Point3D v1;

	/**
	 * The third vertex of the tetrahedron
	 */
	Point3D v2;

	/**
	 * The fourth vertex of the tetrahedron
	 */
	Point3D v3;

	/**
	 * The cell the tetrahedron was made from
	 */
	Cell_handle cell;

	/**
	 * The tetrahedron's bounding box
	 */
	Bbox box;

	/**
	 * The volume of the tetrahedron
	 */
	double _volume;


	//------------
	// CONSTRUCTOR
	//------------

	/**
	 * Default constructor
	 */
	Tetrahedron();


	/**
	 * Full constructor
	 * @param v0 The first vertex of the tetrahedron
	 * @param v1 The first vertex of the tetrahedron
	 * @param v2 The first vertex of the tetrahedron
	 * @param v2 The first vertex of the tetrahedron
	 * @param type The material type
	 */
	Tetrahedron( const Point3D & v0, const Point3D & v1, const Point3D & v2, const Point3D & v3, Cell_handle & cell );


	/**
	 * Copy constructor
	 * @param other The tetrahedron to copy
	 */
	Tetrahedron( const Tetrahedron & other );


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Determines if the given point is inside the tetrahedron
	 * @param x The x-coordinate of the point to check
	 * @param y The y-coordinate of the point to check
	 * @param z The z-coordinate of the point to check
	 * @return TRUE if the point is inside the tetrahedron, FALSE otherwise
	 */
	inline bool contains( const Point3D & p ) const {

		double d0 = determinant( v0.x, v0.y, v0.z, 1.0, v1.x, v1.y, v1.z, 1.0, v2.x, v2.y, v2.z, 1.0, v3.x, v3.y, v3.z, 1.0 );
		
		// Degenerate tetrahedron (just say no)
		if ( d0 == 0.0 ) {

			return false;
		}

		long long mask = 0x8000000000000000;
		long long sign_d0 = *( ( long long * ) ( &d0 ) ) & mask;

		double d1 = determinant( p.x, p.y, p.z, 1.0, v1.x, v1.y, v1.z, 1.0, v2.x, v2.y, v2.z, 1.0, v3.x, v3.y, v3.z, 1.0 );
		
		// Point on face 0
		if ( d1 == 0.0 ) {

			return true;
		}

		if ( sign_d0 ^ ( *( ( long long * ) ( &d1 ) ) & mask ) ) {

			return false;
		}

		double d2 = determinant( v0.x, v0.y, v0.z, 1.0, p.x, p.y, p.z, 1.0, v2.x, v2.y, v2.z, 1.0, v3.x, v3.y, v3.z, 1.0 );

		// Point on face 1
		if ( d2 == 0.0 ) {

			return true;
		}

		if ( sign_d0 ^ ( *( ( long long * ) ( &d2 ) ) & mask ) ) {

			return false;
		}

		double d3 = determinant( v0.x, v0.y, v0.z, 1.0, v1.x, v1.y, v1.z, 1.0, p.x, p.y, p.z, 1.0, v3.x, v3.y, v3.z, 1.0 );

		// Point on face 2
		if ( d3 == 0.0 ) {

			return true;
		}

		if ( sign_d0 ^ ( *( ( long long * ) ( &d3 ) ) & mask ) ) {

			return false;
		}

		double d4 = determinant( v0.x, v0.y, v0.z, 1.0, v1.x, v1.y, v1.z, 1.0, v2.x, v2.y, v2.z, 1.0, p.x, p.y, p.z, 1.0 );

		// Point on face 3
		if ( d4 == 0.0 ) {

			return true;
		}

		if ( sign_d0 ^ ( *( ( long long * ) ( &d4 ) ) & mask ) ) {

			return false;
		}

		return true;
	}


	/**
	 * Computes the volume of the tetrahedron
	 * @return The volume of the tetrahedron
	 */
	inline double volume() const {

		return _volume;
	}


	/**
	 * Computes the bounding box for the tetrahedron
	 * @return The bounding box of the tetrahedron
	 */
	inline const Bbox & bbox() const {

		return box;
	}


	/**
	 * Computes the volume of a tetrahedron based on a cell
	 * @param cell The cell to compute the volume for
	 * @return The volume of the cell
	 */
	static inline double volume( const Cell_handle & cell ) {

		return MACRO_ABS( determinant(
			cell->vertex( 0 )->point().x(),
			cell->vertex( 0 )->point().y(),
			cell->vertex( 0 )->point().z(),
			1.0,
			cell->vertex( 1 )->point().x(),
			cell->vertex( 1 )->point().y(),
			cell->vertex( 1 )->point().z(),
			1.0,
			cell->vertex( 2 )->point().x(),
			cell->vertex( 2 )->point().y(),
			cell->vertex( 2 )->point().z(),
			1.0,
			cell->vertex( 3 )->point().x(),
			cell->vertex( 3 )->point().y(),
			cell->vertex( 3 )->point().z(),
			1.0
		) / 6.0 );
	}
};
