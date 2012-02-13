#include "OctTreeNode.h"

#include <iostream>
#include <limits>
#include <omp.h>

using namespace std;



//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
OctTreeNode::OctTreeNode() : circumcenters(), tetrahedrons(), box( numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), -numeric_limits<double>::infinity(), -numeric_limits<double>::infinity(), -numeric_limits<double>::infinity() ) {

	for ( int i = 0; i < 8; ++i ) {

		subspaces[ i ] = NULL;
	}
}


/**
 * Size constructor
 * @param xmin The x-coordinate of the minimum extent of the box
 * @param ymin The y-coordinate of the minimum extent of the box
 * @param zmin The z-coordinate of the minimum extent of the box
 * @param xmax the x-coordinate of the maximum extent of the box
 * @param ymax The y-coordinate of the maximum extent of the box
 * @param zmax The z-coordinate of the maximum extent of the box
 */
OctTreeNode::OctTreeNode( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax ) : circumcenters(), tetrahedrons(), box( xmin, ymin, zmin, xmax, ymax, zmax ) {

	for ( int i = 0; i < 8; ++i ) {

		subspaces[ i ] = NULL;
	}
}


/**
 * Size constructor
 * @param min The minimum extent of the box
 * @param max The maximum extent of the box
 */
OctTreeNode::OctTreeNode( const Point3D & min, const Point3D & max ) : circumcenters(), tetrahedrons(), box( min.x, min.y, min.z, max.x, max.y, max.z ) {

	for ( int i = 0; i < 8; ++i ) {

		subspaces[ i ] = NULL;
	}
}


/**
 * CGAL constructor
 * @param bbox The CGAL bounding box to copy
 */
OctTreeNode::OctTreeNode( const Bbox & bbox ) : circumcenters(), tetrahedrons(), box( bbox.xmin, bbox.ymin, bbox.zmin, bbox.xmax, bbox.ymax, bbox.zmax ) {

	for ( int i = 0; i < 8; ++i ) {

		subspaces[ i ] = NULL;
	}
}


/**
 * Destructor
 */
OctTreeNode::~OctTreeNode() {

	circumcenters.clear();
	tetrahedrons.clear();

	for ( int i = 0; i < 8; ++i ) {

		if ( subspaces[ i ] ) {
		
			delete subspaces[ i ];
			subspaces[ i ] = NULL;
		}
	}
}


//----------
// FUNCTIONS
//----------

/**
 * Adds a circumcenter to this node
 * @param circumcenter The circumcenter to add
 * @param expand Whether to expand the node to include the point (default is false)
 */
void OctTreeNode::add( Circumcenter * circumcenter, bool expand ) {

	circumcenters.push_back( circumcenter );

	if ( expand ) {

		this->expand( circumcenter->point );
	}
}


/**
 * Adds a tetrahedron to this node
 * @param tetrahedron The tetrahedron to add
 * @param expand Whether to expand the ndoe to include the tetrahedron (default is false)
 */
void OctTreeNode::add( Tetrahedron * tetrahedron, bool expand ) {

	tetrahedrons.push_back( tetrahedron );

	if ( expand ) {

		this->expand( tetrahedron->v0 );
		this->expand( tetrahedron->v1 );
		this->expand( tetrahedron->v2 );
		this->expand( tetrahedron->v3 );
	}
}


#include <CGAL\Timer.h>

/**
 * Recursively splits the node into half-spaces
 * @param depth The current depth in the hierarchy
 * @param midPoint Stores the midpoint of the non-empty tetrahedrons
 * @param volume Stores the volume of the non-empty tetrahedrons
 */
void OctTreeNode::split( int depth, double * midPoint, double & volume ) {

	// No circumcenters to worry about
	if ( circumcenters.size() == 0 ) {

		return;
	}

	// No tetrahedrons (all circumcenters are air)
	if ( tetrahedrons.size() == 0 ) {

		return;
	}

	// No more splitting, so make list of pairs
	if ( tetrahedrons.size() < MIN_TETRAHEDRONS || circumcenters.size() < MIN_CIRCUMCENTERS ) {

		//CGAL::Timer timer;
		//timer.start();
		//timer.reset();
		int csize = circumcenters.size();
		int tsize = tetrahedrons.size();
		int c;
		int t;

		double tempMidpoint[ 3 ] = { 0.0, 0.0, 0.0 };
		double tempVolume = 0.0;

		for ( c = 0; c < csize; ++c ) {

			for ( t = 0; t < tsize; ++t ) {

				if ( tetrahedrons[ t ]->contains( circumcenters[ c ]->point ) ) {

					circumcenters[ c ]->cell->info() = tetrahedrons[ t ]->cell->info();

					if ( circumcenters[ c ]->cell->info() != StoneWeatherer::AIR ) {

						double tetrahedronVolume = Tetrahedron::volume( circumcenters[ c ]->cell );
						tempVolume += tetrahedronVolume;
						tempMidpoint[ 0 ] += circumcenters[ c ]->point.x * tetrahedronVolume;
						tempMidpoint[ 1 ] += circumcenters[ c ]->point.y * tetrahedronVolume;
						tempMidpoint[ 2 ] += circumcenters[ c ]->point.z * tetrahedronVolume;
					}

					continue;
				}
			}
		}

		//times[ id ] += timer.time();
		//timer.stop();

		#pragma omp critical(updateVolume)
		{
			midPoint[ 0 ] += tempMidpoint[ 0 ];
			midPoint[ 1 ] += tempMidpoint[ 1 ];
			midPoint[ 2 ] += tempMidpoint[ 2 ];
			volume += tempVolume;
		}

		return;
	}

	// Split the node
	double xOffset = ( box.xmax - box.xmin ) / 2.0;
	double yOffset = ( box.ymax - box.ymin ) / 2.0;
	double zOffset = ( box.zmax - box.zmin ) / 2.0;
	
	subspaces[ 0 ] = new OctTreeNode( box.xmin, box.ymin, box.zmin, box.xmin + xOffset, box.ymin + yOffset, box.zmin + zOffset );
	subspaces[ 1 ] = new OctTreeNode( box.xmin, box.ymin, box.zmin + zOffset, box.xmin + xOffset, box.ymin + yOffset, box.zmax );
	subspaces[ 2 ] = new OctTreeNode( box.xmin + xOffset, box.ymin, box.zmin + zOffset, box.xmax, box.ymin + yOffset, box.zmax );
	subspaces[ 3 ] = new OctTreeNode( box.xmin + xOffset, box.ymin, box.zmin, box.xmax, box.ymin + yOffset, box.zmin + zOffset );
	subspaces[ 4 ] = new OctTreeNode( box.xmin + xOffset, box.ymin + yOffset, box.zmin, box.xmax, box.ymax, box.zmin + zOffset );
	subspaces[ 5 ] = new OctTreeNode( box.xmin, box.ymin + yOffset, box.zmin, box.xmin + xOffset, box.ymax, box.zmin + zOffset );
	subspaces[ 6 ] = new OctTreeNode( box.xmin, box.ymin + yOffset, box.zmin + zOffset, box.xmin + xOffset, box.ymax, box.zmax );
	subspaces[ 7 ] = new OctTreeNode( box.xmin + xOffset, box.ymin + yOffset, box.zmin + zOffset, box.xmax, box.ymax, box.zmax );

	int size = circumcenters.size();
	int i;
	int j;
	double midX = box.xmin + xOffset;
	double midY = box.ymin + yOffset;
	double midZ = box.zmin + zOffset;

	for ( i = 0; i < size; ++i ) {

		if ( circumcenters[ i ]->point.x <= midX ) {

			if ( circumcenters[ i ]->point.y <= midY ) {

				if ( circumcenters[ i ]->point.z <= midZ ) {

					subspaces[ 0 ]->add( circumcenters[ i ], false );
				}
				else {

					subspaces[ 1 ]->add( circumcenters[ i ], false );
				}
			}
			else {

				if ( circumcenters[ i ]->point.z <= midZ ) {

					subspaces[ 5 ]->add( circumcenters[ i ], false );
				}
				else {

					subspaces[ 6 ]->add( circumcenters[ i ], false );
				}

			}
		}
		else {

			if ( circumcenters[ i ]->point.y <= midY ) {

				if ( circumcenters[ i ]->point.z <= midZ ) {

					subspaces[ 3 ]->add( circumcenters[ i ], false );
				}
				else {

					subspaces[ 2 ]->add( circumcenters[ i ], false );
				}
			}
			else {

				if ( circumcenters[ i ]->point.z <= midZ ) {

					subspaces[ 4 ]->add( circumcenters[ i ], false );
				}
				else {

					subspaces[ 7 ]->add( circumcenters[ i ], false );
				}

			}

		}
	}

	size = tetrahedrons.size();

	for ( i = 0; i < size; ++i ) {

		for ( j = 0; j < 8; ++j ) {

			if ( subspaces[ j ]->intersects( *( tetrahedrons[ i ] ) ) ) {

				subspaces[ j ]->add( tetrahedrons[ i ], false );
			}
		}
	}

	int numProcs = omp_get_num_procs();

	#pragma omp parallel for num_threads(numProcs) if(depth == 0)
	for ( i = 0; i < 8; ++i ) {
		
		subspaces[ i ]->split( depth + 1, midPoint, volume );
	}
}
