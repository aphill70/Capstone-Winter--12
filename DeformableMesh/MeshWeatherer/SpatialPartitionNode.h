#pragma once

#define MIN_CIRCUMCENTERS 10
#define MIN_TETRAHEDRONS MIN_CIRCUMCENTERS
#define MAX_DEPTH 5

#include "Circumcenter.h"
#include "Tetrahedron.h"
#include "Pair.h"
#include "StoneWeatherer.h"
#include "Bbox.h"

#include <map>
#include <vector>

using namespace std;


/**
 * A single node in the spatial partitioning hierarchy
 */
class SpatialPartitionNode {

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
	 * A pointer to the lower half-space
	 */
	SpatialPartitionNode * left;

	/**
	 * The middle third-space
	 */
	SpatialPartitionNode * middle;

	/**
	 * A pointer to the upper half-space
	 */
	SpatialPartitionNode * right;

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
	SpatialPartitionNode();


	/**
	 * Size constructor
	 * @param xmin The x-coordinate of the minimum extent of the box
	 * @param ymin The y-coordinate of the minimum extent of the box
	 * @param zmin The z-coordinate of the minimum extent of the box
	 * @param xmax the x-coordinate of the maximum extent of the box
	 * @param ymax The y-coordinate of the maximum extent of the box
	 * @param zmax The z-coordinate of the maximum extent of the box
	 */
	SpatialPartitionNode( double xmin, double ymin, double zmin, double xmax, double ymax, double zmax );


	/**
	 * Size constructor
	 * @param min The minimum extent of the box
	 * @param max The maximum extent of the box
	 */
	SpatialPartitionNode( const Point3D & min, const Point3D & max );


	/**
	 * CGAL constructor
	 * @param bbox The CGAL bounding box to copy
	 */
	SpatialPartitionNode( const Bbox & bbox );


	/**
	 * Destructor
	 */
	~SpatialPartitionNode();


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
	void expand( const Point3D & point );


	/**
	 * Determines whether or not the node contains the given point
	 * @param point The point to check
	 * @return TRUE if the box contains the point, FALSE otherwise
	 */
	bool contains( const Point3D & point ) const;


	/**
	 * Determines whether or not the node intersects the given tetrahedron
	 * @param tetrahedron The tetrahedron to check
	 * @return TRUE if the tetrahedron intersects the box, FALSE otherwise
	 */
	bool intersects( const Tetrahedron & tetrahedron ) const;


	/**
	 * Recursively splits the node into half-spaces
	 * @param depth The current depth in the hierarchy
	 * @param nodesToProcess The list of leaf nodes that need to be processed after splitting
	 */
	void split( int depth, vector<SpatialPartitionNode*> & nodesToProcess );


	/**
	 * Recursively splits the node into half-spaces
	 * @param depth The current depth in the hierarchy
	 * @param pairs Stores the list of circumcenter/tetrahedron pairs
	 * @param circumcenters The circumcenters in the scene, in the order they are added to the pairs list
	 * @param resultsSize Stores the size of the results list (number of circumcenters total)
	 */
	void split( int depth, vector<Pair> & pairs, vector<Circumcenter*> & circumcenters, int & resultsSize );
};
