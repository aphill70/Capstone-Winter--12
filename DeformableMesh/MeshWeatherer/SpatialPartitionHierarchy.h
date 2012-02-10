#pragma once

#include "OctTreeNode.h"
#include "Pair.h"
#include "StoneWeatherer.h"
#include "Circumcenter.h"
#include "Tetrahedron.h"

#include <vector>
#include <map>

using namespace std;


/**
 * A tree of spatial partition nodes
 */
class SpatialPartitionHierarchy {

private:

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The type of points
	 */
	typedef StoneWeatherer::Point Point;

	/**
	 * The type of cell handle
	 */
	typedef StoneWeatherer::Cell_handle Cell_handle;

	/**
	 * The list of tetrahedrons in the scene
	 */
	vector<Tetrahedron*> tetrahedrons;

	/**
	 * The root node of the tree
	 */
	OctTreeNode * root;


public:

	/**
	 * The list of circumcenters in the scene
	 */
	vector<Circumcenter*> circumcenters;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	SpatialPartitionHierarchy();


	/**
	 * Destructor
	 */
	~SpatialPartitionHierarchy();


	//----------
	// FUNCTIONS
	//----------
	

	/**
	 * Adds a new circumcenter to the hierarchy
	 * @param point The location of the circumcenter
	 * @param cell The cell associated with the circumcenter
	 */
	void add( const Point & point, Cell_handle & cell );


	/**
	 * Adds a new tetrahedron to the hierarchy
	 * @param cell The cell associated with the tetrahedron
	 */
	void add( Cell_handle cell );


	/**
	 * Checks to see if the given point is in the hierarchy's space
	 * @param point The point to check
	 * @return TRUE if the point is in the root node, FALSE otherwise
	 */
	bool contains( const Point3D & point ) const;


	/**
	 * Computes the list of pairs to check by partitioning the space
	 * @param midPoint Stores the midpoint of the non-empty tetrahedrons
	 * @param volume Stores the volume of the non-empty tetrahedrons
	 */
	void partition( double * midPoint, double & volume );
};
