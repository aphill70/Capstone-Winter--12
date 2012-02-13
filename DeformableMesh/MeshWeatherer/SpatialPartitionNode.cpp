#include "SpatialPartitionNode.h"

#include <iostream>
#include <limits>

using namespace std;



//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
SpatialPartitionNode::SpatialPartitionNode() : circumcenters(), tetrahedrons(), left( NULL ), right( NULL ), box( numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), -numeric_limits<double>::infinity(), -numeric_limits<double>::infinity(), -numeric_limits<double>::infinity() ) {

	return;
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
SpatialPartitionNode::SpatialPartitionNode( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax ) : circumcenters(), tetrahedrons(), left( NULL ), middle( NULL ), right( NULL ), box( xmin, ymin, zmin, xmax, ymax, zmax ) {

	return;
}


/**
 * Size constructor
 * @param min The minimum extent of the box
 * @param max The maximum extent of the box
 */
SpatialPartitionNode::SpatialPartitionNode( const Point3D & min, const Point3D & max ) : circumcenters(), tetrahedrons(), left( NULL ), middle( NULL ), right( NULL ), box( min.x, min.y, min.z, max.x, max.y, max.z ) {

	return;
}


/**
 * CGAL constructor
 * @param bbox The CGAL bounding box to copy
 */
SpatialPartitionNode::SpatialPartitionNode( const Bbox & bbox ) : circumcenters(), tetrahedrons(), left( NULL ), middle( NULL ), right( NULL ), box( bbox.xmin(), bbox.ymin(), bbox.zmin(), bbox.xmax(), bbox.ymax(), bbox.zmax() ) {

	return;
}


/**
 * Destructor
 */
SpatialPartitionNode::~SpatialPartitionNode() {

	circumcenters.clear();
	tetrahedrons.clear();

	if ( left ) {

		delete left;
	}

	if ( middle ) {

		delete middle;
	}

	if ( right ) {

		delete right;
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
void SpatialPartitionNode::add( Circumcenter * circumcenter, bool expand ) {

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
void SpatialPartitionNode::add( Tetrahedron * tetrahedron, bool expand ) {

	tetrahedrons.push_back( tetrahedron );

	if ( expand ) {

		this->expand( tetrahedron->v0 );
		this->expand( tetrahedron->v1 );
		this->expand( tetrahedron->v2 );
		this->expand( tetrahedron->v3 );
	}
}


/**
 * Determines the minimum of the two given values
 * @param a The first value
 * @param b The second value
 * @return The minimum of the two values
 */
inline double min( double a, double b ) {

	return ( a < b ? a : b );
}


/**
 * Determines the maximum of the two given values
 * @param a The first value
 * @param b The second value
 * @return The maximum of the two values
 */
inline double max( double a, double b ) {

	return ( a > b ? a : b );
}


/**
 * Expands the node to include the given point
 * @param point The point to add
 */
void SpatialPartitionNode::expand( const Point3D & point ) {

	box = Bbox( min( point.x, box.xmin() ), min( point.y, box.ymin() ), min( point.z, box.zmin() ), max( point.x, box.xmax() ), max( point.y, box.ymax() ), max( point.z, box.zmax() ) );
}


/**
 * Determines whether or not the node contains the given point
 * @param point The point to check
 * @return TRUE if the box contains the point, FALSE otherwise
 */
bool SpatialPartitionNode::contains( const Point3D & point ) const {

	return ( point.x >= box.xmin() ) && ( point.x <= box.xmax() ) && ( point.y >= box.ymin() ) && ( point.y <= box.ymax() ) && ( point.z >= box.zmin() ) && ( point.z <= box.zmax() );
}


/**
 * Determines if two bounding boxes overlap
 * @param b0 The first box to check
 * @param b1 The second box to check
 * @return TRUE if the boxes overlap, FALSE otherwise
 */
bool boxesOverlap( const Bbox & b0, const Bbox & b1 ) {

	return !( ( b0.xmin() > b1.xmax() ) || ( b0.xmax() < b1.xmin() ) || ( b0.ymin() > b1.ymax() ) || ( b0.ymax() < b1.ymin() ) || ( b0.zmin() > b1.zmax() ) || ( b0.zmax() < b1.zmin() ) );
}


/**
 * Determines whether or not the node intersects the given tetrahedron
 * @param tetrahedron The tetrahedron to check
 * @return TRUE if the tetrahedron intersects the box, FALSE otherwise
 */
bool SpatialPartitionNode::intersects( const Tetrahedron & tetrahedron ) const {

	return boxesOverlap( box, tetrahedron.bbox() );
}


/**
 * Recursively splits the node into half-spaces
 * @param depth The current depth in the hierarchy
 * @param nodesToProcess The list of leaf nodes that need to be processed after splitting
 */
void SpatialPartitionNode::split( int depth, vector<SpatialPartitionNode*> & nodesToProcess ) {

	// No circumcenters to worry about
	if ( circumcenters.size() == 0 ) {

		return;
	}

	// No tetrahedrons (all circumcenters are air)
	if ( tetrahedrons.size() == 0 ) {

		int size = circumcenters.size();

		for ( int i = 0; i < size; ++i ) {

			( ( TDS::Cell * ) ( circumcenters[ i ]->cell ) )->info() = StoneWeatherer::AIR;
		}

		return;
	}

	// No more splitting, so make list of pairs
	if ( tetrahedrons.size() < MIN_TETRAHEDRONS || circumcenters.size() < MIN_CIRCUMCENTERS ) {

		nodesToProcess.push_back( this );

		return;
	}

	// Split the node
	double xLength = box.xmax() - box.xmin();
	double yLength = box.ymax() - box.ymin();
	double zLength = box.zmax() - box.zmin();
	double mid;

	if ( xLength >= yLength && xLength >= zLength ) {

		mid = box.xmin() + ( xLength / 2.0 );
		left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), mid, box.ymax(), box.zmax() );
		right = new SpatialPartitionNode( mid, box.ymin(), box.zmin(), box.xmax(), box.ymax(), box.zmax() );
	}
	else if ( yLength >= xLength && yLength >= zLength ) {

		mid = box.ymin() + ( yLength / 2.0 );
		left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), box.xmax(), mid, box.zmax() );
		right = new SpatialPartitionNode( box.xmin(), mid, box.zmin(), box.xmax(), box.ymax(), box.zmax() );
	}
	else {

		mid = box.zmin() + ( zLength / 2.0 );
		left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), box.xmax(), box.ymax(), mid );
		right = new SpatialPartitionNode( box.xmin(), box.ymin(), mid, box.xmax(), box.ymax(), box.zmax() );
	}

	int size = circumcenters.size();
	int i;

	for ( i = 0; i < size; ++i ) {

		if ( left->contains( circumcenters[ i ]->point ) ) {

			left->add( circumcenters[ i ], false );
		}

		if ( right->contains( circumcenters[ i ]->point ) ) {

			right->add( circumcenters[ i ], false );
		}
	}

	size = tetrahedrons.size();

	for ( i = 0; i < size; ++i ) {

		if ( left->intersects( *( tetrahedrons[ i ] ) ) ) {

			left->add( tetrahedrons[ i ], false );
		}

		if ( right->intersects( *( tetrahedrons[ i ] ) ) ) {

			right->add( tetrahedrons[ i ], false );
		}
	}

	left->split( depth + 1, nodesToProcess );
	right->split( depth + 1, nodesToProcess );
}


/**
 * Recursively splits the node into half-spaces
 * @param depth The current depth in the hierarchy
 * @param pairs Stores the list of circumcenter/tetrahedron pairs
 * @param out_circumcenters The circumcenters in the scene, in the order they are added to the pairs list
 * @param resultsSize Stores the size of the results list (number of circumcenters total)
 */
void SpatialPartitionNode::split( int depth, vector<Pair> & pairs, vector<Circumcenter*> & out_circumcenters, int & resultsSize ) {

	// No circumcenters to worry about
	if ( circumcenters.size() == 0 ) {

		return;
	}

	// No tetrahedrons (all circumcenters are air)
	if ( tetrahedrons.size() == 0 ) {

		int size = circumcenters.size();

		for ( int i = 0; i < size; ++i ) {

			( ( TDS::Cell * ) ( circumcenters[ i ]->cell ) )->info() = StoneWeatherer::AIR;
		}

		return;
	}

	// No more splitting, so make list of pairs
	if ( tetrahedrons.size() < MIN_TETRAHEDRONS || circumcenters.size() < MIN_CIRCUMCENTERS ) {

		int csize = circumcenters.size();
		int tsize = tetrahedrons.size();
		int c;
		int t;

		for ( c = 0; c < csize; ++c ) {

			circumcenters[ c ]->index = resultsSize++;
			out_circumcenters.push_back( circumcenters[ c ] );

			for ( t = 0; t < tsize; ++t ) {

				pairs.push_back( Pair( *( circumcenters[ c ] ), *( tetrahedrons[ t ] ) ) );
			}
		}

		return;
	}

	// Split the node
	double xLength = box.xmax() - box.xmin();
	double yLength = box.ymax() - box.ymin();
	double zLength = box.zmax() - box.zmin();
	double mid;

	if ( xLength >= yLength && xLength >= zLength ) {

		//mid = box.xmin() + ( xLength / 2.0 );
		//left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), mid, box.ymax(), box.zmax() );
		//right = new SpatialPartitionNode( mid, box.ymin(), box.zmin(), box.xmax(), box.ymax(), box.zmax() );
		mid = box.xmin() + ( xLength / 3.0 );
		left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), mid, box.ymax(), box.zmax() );
		middle = new SpatialPartitionNode( mid, box.ymin(), box.zmin(), mid + ( xLength / 3.0 ), box.ymax(), box.zmax() );
		right = new SpatialPartitionNode( mid + ( xLength / 3.0 ), box.ymin(), box.zmin(), box.xmax(), box.ymax(), box.zmax() );
	}
	else if ( yLength >= xLength && yLength >= zLength ) {

		//mid = box.ymin() + ( yLength / 2.0 );
		//left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), box.xmax(), mid, box.zmax() );
		//right = new SpatialPartitionNode( box.xmin(), mid, box.zmin(), box.xmax(), box.ymax(), box.zmax() );
		mid = box.ymin() + ( yLength / 3.0 );
		left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), box.xmax(), mid, box.zmax() );
		middle = new SpatialPartitionNode( box.xmin(), mid, box.zmin(), box.xmax(), mid + ( yLength / 3.0 ), box.zmax() );
		right = new SpatialPartitionNode( box.xmin(), mid + ( yLength / 3.0 ), box.zmin(), box.xmax(), box.ymax(), box.zmax() );
	}
	else {

		//mid = box.zmin() + ( zLength / 2.0 );
		//left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), box.xmax(), box.ymax(), mid );
		//right = new SpatialPartitionNode( box.xmin(), box.ymin(), mid, box.xmax(), box.ymax(), box.zmax() );
		mid = box.zmin() + ( zLength / 3.0 );
		left = new SpatialPartitionNode( box.xmin(), box.ymin(), box.zmin(), box.xmax(), box.ymax(), mid );
		middle = new SpatialPartitionNode( box.xmin(), box.ymin(), mid, box.xmax(), box.ymax(), mid + ( zLength / 3.0 ) );
		right = new SpatialPartitionNode( box.xmin(), box.ymin(), mid + ( zLength / 3.0 ), box.xmax(), box.ymax(), box.zmax() );
	}

	int size = circumcenters.size();
	int i;

	for ( i = 0; i < size; ++i ) {

		if ( left->contains( circumcenters[ i ]->point ) ) {

			left->add( circumcenters[ i ], false );
		}
		else if ( middle->contains( circumcenters[ i ]->point ) ) {

			middle->add( circumcenters[ i ], false );
		}
		else {

			right->add( circumcenters[ i ], false );
		}
	}

	size = tetrahedrons.size();

	for ( i = 0; i < size; ++i ) {

		if ( left->intersects( *( tetrahedrons[ i ] ) ) ) {

			left->add( tetrahedrons[ i ], false );
		}

		if ( middle->intersects( *( tetrahedrons[ i ] ) ) ) {

			middle->add( tetrahedrons[ i ], false );
		}

		if ( right->intersects( *( tetrahedrons[ i ] ) ) ) {

			right->add( tetrahedrons[ i ], false );
		}
	}

	left->split( depth + 1, pairs, out_circumcenters, resultsSize );
	middle->split( depth + 1, pairs, out_circumcenters, resultsSize );
	right->split( depth + 1, pairs, out_circumcenters, resultsSize );
}
