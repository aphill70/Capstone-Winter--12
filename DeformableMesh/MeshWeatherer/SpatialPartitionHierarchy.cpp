#include "SpatialPartitionHierarchy.h"


//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
SpatialPartitionHierarchy::SpatialPartitionHierarchy() : root( new OctTreeNode() ), circumcenters(), tetrahedrons() {

	return;
}


/**
 * Destructor
 */
SpatialPartitionHierarchy::~SpatialPartitionHierarchy() {

	if ( root ) {

		delete root;
	}

	circumcenters.clear();
	tetrahedrons.clear();
}


//----------
// FUNCTIONS
//----------

/**
 * Adds a new circumcenter to the hierarchy
 * @param point The location of the circumcenter
 * @param cell The cell associated with the circumcenter
 */
void SpatialPartitionHierarchy::add( const Point & point, Cell_handle & cell ) {

	circumcenters.push_back( new Circumcenter( point.x(), point.y(), point.z(), cell ) );
	circumcenters.back()->cell->info() = StoneWeatherer::AIR;
	root->add( circumcenters.back(), false );
}


/**
 * Adds a new tetrahedron to the hierarchy
 * @param cell The cell associated with the tetrahedron
 */
void SpatialPartitionHierarchy::add( Cell_handle cell ) {

	tetrahedrons.push_back(
		new Tetrahedron(
			Point3D(
				cell->vertex( 0 )->point().x(),
				cell->vertex( 0 )->point().y(),
				cell->vertex( 0 )->point().z()
			),
			Point3D(
				cell->vertex( 1 )->point().x(),
				cell->vertex( 1 )->point().y(),
				cell->vertex( 1 )->point().z()
			),
			Point3D(
				cell->vertex( 2 )->point().x(),
				cell->vertex( 2 )->point().y(),
				cell->vertex( 2 )->point().z()
			),
			Point3D(
				cell->vertex( 3 )->point().x(),
				cell->vertex( 3 )->point().y(),
				cell->vertex( 3 )->point().z()
			),
			cell
		)
	);

	root->add( tetrahedrons.back(), true );
}


/**
 * Checks to see if the given point is in the hierarchy's space
 * @param point The point to check
 * @return TRUE if the point is in the root node, FALSE otherwise
 */
bool SpatialPartitionHierarchy::contains( const Point3D & point ) const {

	return root->contains( point );
}


#include <CGAL\Timer.h>

#define MAX(a,b) ((a)>(b)?(a):(b))

/**
 * Computes the list of pairs to check by partitioning the space
 * @param midPoint Stores the midpoint of the non-empty tetrahedrons
 * @param volume Stores the volume of the non-empty tetrahedrons
 */
void SpatialPartitionHierarchy::partition( double * midPoint, double & volume ) {

	//CGAL::Timer timer;
	//timer.start();
	//timer.reset();
	//double times[ 8 ] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	//root->split( 0, midPoint, volume, times );
	root->split( 0, midPoint, volume );
	//double totalTime = timer.time();
	//timer.stop();
	//cout << "Relabel time: " << MAX( MAX( MAX( times[ 0 ], times[ 1 ] ), MAX( times[ 2 ], times[ 3 ] ) ), MAX( MAX( times[ 4 ], times[ 5 ] ), MAX( times[ 6 ], times[ 7 ] ) ) ) << endl;
	//cout << "Total time: " << totalTime << endl;
}
