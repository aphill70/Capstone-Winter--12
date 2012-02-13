#include <cmath>
#include <limits>

#include "UniformGrid.h"

using namespace std;


//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
UniformGrid::UniformGrid() : tetrahedrons(), grid( NULL ), L( NULL ), xmin( numeric_limits<double>::infinity() ), ymin( numeric_limits<double>::infinity() ), zmin( numeric_limits<double>::infinity() ), xmax( -numeric_limits<double>::infinity() ), ymax( -numeric_limits<double>::infinity() ), zmax( -numeric_limits<double>::infinity() ), Mx( 0 ), My( 0 ), Mz( 0 ), inv_Xsize( 0.0 ), inv_Ysize( 0.0 ), inv_Zsize( 0.0 ) {

	return;
}


/**
 * Destructor
 */
UniformGrid::~UniformGrid() {

	tetrahedrons.clear();

	if ( grid ) {

		delete[] grid;
	}

	if ( L ) {

		delete[] L;
	}
}


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

//----------
// FUNCTIONS
//----------

/**
 * Adds a new tetrahedron to the hierarchy
 * @param cell The cell associated with the tetrahedron
 */
void UniformGrid::add( Cell_handle cell ) {

	Point3D v0( cell->vertex( 0 )->point().x(), cell->vertex( 0 )->point().y(), cell->vertex( 0 )->point().z() );
	Point3D v1( cell->vertex( 1 )->point().x(), cell->vertex( 1 )->point().y(), cell->vertex( 1 )->point().z() );
	Point3D v2( cell->vertex( 2 )->point().x(), cell->vertex( 2 )->point().y(), cell->vertex( 2 )->point().z() );
	Point3D v3( cell->vertex( 3 )->point().x(), cell->vertex( 3 )->point().y(), cell->vertex( 3 )->point().z() );

	tetrahedrons.push_back( new Tetrahedron( v0, v1, v2, v3, cell ) );

	xmin = MIN( xmin, MIN( MIN( v0.x, v1.x ), MIN( v2.x, v3.x ) ) );
	ymin = MIN( ymin, MIN( MIN( v0.y, v1.y ), MIN( v2.y, v3.y ) ) );
	zmin = MIN( zmin, MIN( MIN( v0.z, v1.z ), MIN( v2.z, v3.z ) ) );

	xmax = MAX( xmax, MAX( MAX( v0.x, v1.x ), MAX( v2.x, v3.x ) ) );
	ymax = MAX( ymax, MAX( MAX( v0.y, v1.y ), MAX( v2.y, v3.y ) ) );
	zmax = MAX( zmax, MAX( MAX( v0.z, v1.z ), MAX( v2.z, v3.z ) ) );
}


/**
 * Tells the grid that all the tetrahedrons are added, and it's OK to go ahead and divide up the space
 */
void UniformGrid::partition() {

	// Expand the bounds slightly so we don't hit exactly on the edge of the bounding volume
	xmin *= 1.01;
	ymin *= 1.01;
	zmin *= 1.01;
	xmax *= 1.01;
	ymax *= 1.01;
	zmax *= 1.01;

	if ( grid ) {

		delete[] grid;
		grid = 0;
	}

	if ( L ) {

		delete[] L;
		L = 0;
	}

	// Scale = root_3( (rho*N) / volume)
	double sx = ( xmax - xmin );
	double sy = ( ymax - ymin );
	double sz = ( zmax - zmin );
	int objects = tetrahedrons.size();
	double scale = pow( ( RHO * objects ) / ( ( sx * sy * sz ) ), 1.0 / 3.0 );
	
	Mx = ( unsigned int ) ( sx * scale );
	My = ( unsigned int ) ( sy * scale );
	Mz = ( unsigned int ) ( sz * scale );

	inv_Xsize = Mx / sx;
	inv_Ysize = My / sy;
	inv_Zsize = Mz / sz;

	int gridSize = Mx * My * Mz + 1;

	grid = new unsigned int[ gridSize ];
	memset( grid, 0, gridSize * sizeof( unsigned int ) );

	int i;
	unsigned int minXgrid;
	unsigned int minYgrid;
	unsigned int minZgrid;
	unsigned int maxXgrid;
	unsigned int maxYgrid;
	unsigned int maxZgrid;
	unsigned int z;
	unsigned int y;
	unsigned int x;

	for ( i = 0; i < objects; ++i ) {

		minXgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.xmin - xmin ) * inv_Xsize );
		minYgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.ymin - ymin ) * inv_Ysize );
		minZgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.zmin - zmin ) * inv_Zsize );

		maxXgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.xmax - xmin ) * inv_Xsize );
		maxYgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.ymax - ymin ) * inv_Ysize );
		maxZgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.zmax - zmin ) * inv_Zsize );

		for ( z = minZgrid; z <= maxZgrid; ++z ) {

			for ( y = minYgrid; y <= maxYgrid; ++y ) {

				for ( x = minXgrid; x <= maxXgrid; ++x ) {

					++grid[ ( ( ( My * z ) + y ) * Mx ) + x ];
				}
			}
		}
	}

	for ( i = 1; i < gridSize; ++i ) {

		grid[ i ] += grid[ i - 1 ];
	}

	L = new unsigned int[ grid[ gridSize - 1 ] ];

	for ( i = objects - 1; i >= 0; --i ) {

		minXgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.xmin - xmin ) * inv_Xsize );
		minYgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.ymin - ymin ) * inv_Ysize );
		minZgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.zmin - zmin ) * inv_Zsize );

		maxXgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.xmax - xmin ) * inv_Xsize );
		maxYgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.ymax - ymin ) * inv_Ysize );
		maxZgrid = ( unsigned int ) ( ( tetrahedrons[ i ]->box.zmax - zmin ) * inv_Zsize );

		for ( z = minZgrid; z <= maxZgrid; ++z ) {

			for ( y = minYgrid; y <= maxYgrid; ++y ) {

				for ( x = minXgrid; x <= maxXgrid; ++x ) {

					L[ --grid[ ( ( ( My * z ) + y ) * Mx ) + x ] ] = i;
				}
			}
		}
	}
}


/**
 * Checks to see if the given point is in the hierarchy's space
 * @param point The point to check
 * @return TRUE if the point is in the root node, FALSE otherwise
 */
bool UniformGrid::contains( const Point3D & point ) const {

	return ( point.x >= xmin && point.x <= xmax ) && ( point.y >= ymin && point.y <= ymax ) && ( point.z >= zmin && point.z <= zmax );
}
	

/**
 * Finds a circumcenter in the grid and labels its associated cell
 * @param point The location of the circumcenter
 * @param cell The cell to label
 * @param midpoint Stores the weighted midpoint of non-air cells
 * @param volume Accumulates the volume of non-air cells
 */
void UniformGrid::locateAndLabel( const Point & point, Cell_handle & cell, double * midpoint, double & volume ) const {

	Point3D p( point.x(), point.y(), point.z() );
	unsigned int x = ( unsigned int ) ( ( p.x - xmin ) * inv_Xsize );
	unsigned int y = ( unsigned int ) ( ( p.y - ymin ) * inv_Ysize );
	unsigned int z = ( unsigned int ) ( ( p.z - zmin ) * inv_Zsize );

	unsigned int i = ( ( ( My * z ) + y ) * Mx ) + x;
	unsigned int start = grid[ i ];
	unsigned int end = grid[ i + 1 ];
	double tetrahedronVolume = 0.0;

	for ( i = start; i < end; ++i ) {

		if ( tetrahedrons[ L[ i ] ]->contains( p ) ) {

			cell->info() = tetrahedrons[ L[ i ] ]->cell->info();

			if ( cell->info() > AIR ) {

				tetrahedronVolume = Tetrahedron::volume( cell );
				
				#pragma omp critical(updateVolume)
				{
					volume += tetrahedronVolume;
					midpoint[ 0 ] += p.x * tetrahedronVolume;
					midpoint[ 1 ] += p.y * tetrahedronVolume;
					midpoint[ 2 ] += p.z * tetrahedronVolume;
				}
			}

			return;
		}
	}

	cell->info() = AIR;
}


/**
 * Finds the label (material type) for the given point
 * @param point The point to check
 * @return The label (material type) for the given point
 */
Contents UniformGrid::getLabel( const Point & point ) const {

	Point3D p( point.x(), point.y(), point.z() );
	unsigned int x = ( unsigned int ) ( ( p.x - xmin ) * inv_Xsize );
	unsigned int y = ( unsigned int ) ( ( p.y - ymin ) * inv_Ysize );
	unsigned int z = ( unsigned int ) ( ( p.z - zmin ) * inv_Zsize );

	unsigned int i = ( ( ( My * z ) + y ) * Mx ) + x;
	unsigned int start = grid[ i ];
	unsigned int end = grid[ i + 1 ];

	for ( i = start; i < end; ++i ) {

		if ( tetrahedrons[ L[ i ] ]->contains( p ) ) {

			return tetrahedrons[ L[ i ] ]->cell->info();;
		}
	}

	return AIR;
}
