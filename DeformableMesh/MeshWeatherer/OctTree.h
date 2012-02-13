#pragma once

#include "OctTreeNode.h"
#include "Circumcenter.h"
#include "Tetrahedron.h"

#include <vector>
#include <map>

using namespace std;


/**
 * A tree of spatial partition nodes
 */
class OctTree {

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
	 * The list of circumcenters in the scene
	 */
	vector<Circumcenter*> circumcenters;

	/**
	 * The list of tetrahedrons in the scene
	 */
	vector<Tetrahedron*> tetrahedrons;

	/**
	 * The root node of the tree
	 */
	OctTreeNode * root;


public:

	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	OctTree();


	/**
	 * Destructor
	 */
	~OctTree();


	//----------
	// FUNCTIONS
	//----------
	

	/**
	 * Adds a new circumcenter to the hierarchy
	 * @param point The location of the circumcenter
	 * @param cell The cell associated with the circumcenter
	 */
	void add( const Point & point, Cell_handle cell );


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
	bool contains( const Point & point ) const;


	/**
	 * Computes the list of pairs to check by partitioning the space
	 * @param pairs The list of pairs to add to while partitioning
	 */
	void partition( vector< pair< Circumcenter*, vector<Tetrahedron*> > > & pairs );
};
