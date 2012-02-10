#pragma once

#include <vector>

#include "CGAL_typedefs.h"
#include "Circumcenter.h"
#include "Point3D.h"
#include "StoneWeatherer.h"
#include "Tetrahedron.h"

using namespace std;

/** 
 * Determines the resolution of the grid
 */
#define RHO 1u


/**
 * Represents a uniform grid implemented with the Compact Grid Method
 * See Ares Lagae & Philip Dutré, "Compact, Fast and Robust Grids for Ray Tracing"
 */
class UniformGrid {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The list of tetrahedrons in the scene
	 */
	vector<Tetrahedron*> tetrahedrons;

	/**
	 * The array of grid cells (of size Mx * My * Mz)
	 * Basically, stores offsets into L, the concatenation of every cell's objects list
	 */
	unsigned int * grid;

	/**
	 * The concatenation of every cell's objects list
	 * Basically, stores indices into the tetrahedrons list
	 */
	unsigned int * L;

	/**
	 * The coordinates of the minimum extent of the grid
	 */
	double xmin;
	double ymin;
	double zmin;

	/**
	 * The coordinates of the maximum extent of the grid
	 */
	double xmax;
	double ymax;
	double zmax;

	/**
	 * The dimensions of the grid (number of cells in each dimension)
	 */
	unsigned int Mx;
	unsigned int My;
	unsigned int Mz;

	/**
	 * The inverse of the size in world coordinates of each grid dimension
	 * (I use the inverse because the computations later require a division by the size of each dimension, and computing the inverse once and multiplying later is faster than dividing every time).
	 */
	double inv_Xsize;
	double inv_Ysize;
	double inv_Zsize;


public:

	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	UniformGrid();


	/**
	 * Destructor
	 */
	~UniformGrid();


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Adds a new tetrahedron to the hierarchy
	 * @param cell The cell associated with the tetrahedron
	 */
	void add( Cell_handle cell );


	/**
	 * Tells the grid that all the tetrahedrons are added, and it's OK to go ahead and divide up the space
	 */
	void partition();


	/**
	 * Checks to see if the given point is in the hierarchy's space
	 * @param point The point to check
	 * @return TRUE if the point is in the root node, FALSE otherwise
	 */
	bool contains( const Point3D & point ) const;
	

	/**
	 * Finds a circumcenter in the grid and labels its associated cell
	 * @param point The location of the circumcenter
	 * @param cell The cell to label
	 * @param midpoint Stores the weighted midpoint of non-air cells
	 * @param volume Accumulates the volume of non-air cells
	 */
	void locateAndLabel( const Point & point, Cell_handle & cell, double * midpoint, double & volume ) const;


	/**
	 * Finds the label (material type) for the given point
	 * @param point The point to check
	 * @return The label (material type) for the given point
	 */
	Contents getLabel( const Point & point ) const;
};
