#pragma once

#define MIN_CIRCUMCENTERS 20
#define MIN_TETRAHEDRONS MIN_CIRCUMCENTERS

#include "Circumcenter.h"
#include "Tetrahedron.h"
#include "Pair.h"
#include "StoneWeatherer.h"
#include "Bbox.h"

#include <map>
#include <vector>

using namespace std;


/**
 * Determines if two bounding boxes overlap
 * @param b0 The first box to check
 * @param b1 The second box to check
 * @return TRUE if the boxes overlap, FALSE otherwise
 */
inline bool boxesOverlapOct( const Bbox & b0, const Bbox & b1 ) {

	return !( ( b0.xmin > b1.xmax ) || ( b0.xmax < b1.xmin ) || ( b0.ymin > b1.ymax ) || ( b0.ymax < b1.ymin ) || ( b0.zmin > b1.zmax ) || ( b0.zmax < b1.zmin ) );
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
 * A single node in the spatial partitioning hierarchy
 */
class OctTreeNode {

	friend class SpatialPartitionHierarchy;

public:

	/**
	 * The triangulation data structure
	 */
	typedef StoneWeatherer::TDS TDS;


private:

	//------------
	// MEMBER DATA
	//------------
	
	/**
	 * A list of pointers to the circumcenters from the hierarchy that are contained in this sub-space
	 */
	vector<Circumcenter*> circumcenters;

	/**
	 * A list of pointers to the tetrahedrons from the hierarchy that intersect this sub-space
	 */
	vector<Tetrahedron*> tetrahedrons;

	/**
	 * Pointers to the subspaces
	 */
	OctTreeNode * subspaces[ 8 ];

	/**
	 * The bounding box for the space (stores the space's dimensions)
	 */
	Bbox box;


public:

	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	OctTreeNode();


	/**
	 * Size constructor
	 * @param xmin The x-coordinate of the minimum extent of the box
	 * @param ymin The y-coordinate of the minimum extent of the box
	 * @param zmin The z-coordinate of the minimum extent of the box
	 * @param xmax the x-coordinate of the maximum extent of the box
	 * @param ymax The y-coordinate of the maximum extent of the box
	 * @param zmax The z-coordinate of the maximum extent of the box
	 */
	OctTreeNode( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax );


	/**
	 * Size constructor
	 * @param min The minimum extent of the box
	 * @param max The maximum extent of the box
	 */
	OctTreeNode( const Point3D & min, const Point3D & max );


	/**
	 * CGAL constructor
	 * @param bbox The CGAL bounding box to copy
	 */
	OctTreeNode( const Bbox & bbox );


	/**
	 * Destructor
	 */
	~OctTreeNode();


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Adds a circumcenter to this node
	 * @param circumcenter The circumcenter to add
	 * @param expand Whether to expand the node to include the point (default is false)
	 */
	void add( Circumcenter * circumcenter, bool expand = false );


	/**
	 * Adds a tetrahedron to this node
	 * @param tetrahedron The tetrahedron to add
	 * @param expand Whether to expand the ndoe to include the tetrahedron (default is false)
	 */
	void add( Tetrahedron * tetrahedron, bool expand = false );


	/**
	 * Expands the node to include the given point
	 * @param point The point to add
	 */
	inline void expand( const Point3D & point ) {

		box.xmin = min( point.x, box.xmin );
		box.ymin = min( point.y, box.ymin );
		box.zmin = min( point.z, box.zmin );
		box.xmax = max( point.x, box.xmax );
		box.ymax = max( point.y, box.ymax );
		box.zmax = max( point.z, box.zmax );
	}


	/**
	 * Determines whether or not the node contains the given point
	 * @param point The point to check
	 * @return TRUE if the box contains the point, FALSE otherwise
	 */
	inline bool contains( const Point3D & point ) const {

		return ( point.x >= box.xmin ) && ( point.x <= box.xmax ) && ( point.y >= box.ymin ) && ( point.y <= box.ymax ) && ( point.z >= box.zmin ) && ( point.z <= box.zmax );
	}


	/**
	 * Determines whether or not the node intersects the given tetrahedron
	 * @param tetrahedron The tetrahedron to check
	 * @return TRUE if the tetrahedron intersects the box, FALSE otherwise
	 */
	inline bool intersects( const Tetrahedron & tetrahedron ) const {

		return boxesOverlapOct( box, tetrahedron.bbox() );
	}


	/**
	 * Recursively splits the node into half-spaces
	 * @param depth The current depth in the hierarchy
	 * @param midPoint Stores the midpoint of the non-empty tetrahedrons
	 * @param volume Stores the volume of the non-empty tetrahedrons
	 */
	void split( int depth, double * midPoint, double & volume );
};
