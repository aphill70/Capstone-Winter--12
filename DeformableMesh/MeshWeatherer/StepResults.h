#pragma once

#include <iostream>

using namespace std;


/**
 * Stores statistical results of a simulation time step
 */
struct stepResults {
	
	//------------
	// MEMBER DATA
	//------------
	
	/**
	 * The number of vertices currently in the mesh
	 */
	int numVertices;
	
	/**
	 * The number of tetrahedrons currently in the mesh
	 */
	int numTetrahedrons;
	
	/**
	 * The approximate mean location of filled curcumcenters
	 */
	double midPoint[ 3 ];
	
	/**
	 * The total seconds spent in computation
	 */
	double secondsTotal;
	
	/**
	 * The seconds spent refining the mesh and moving the vertices (includes "softness" and "live" calls)
	 */
	double secondsMotion;
	
	/**
	 * The seconds CGAL spent building the tet mesh for the new points
	 */
	double secondsCGAL;
	
	/**
	 * The seconds spent identifying "inside" and "outside" tets (relabeling)
	 */
	double secondsLabeling;
	
	/**
	 * The seconds spent on normal and curvature computation and tagging redundant vertices
	 */
	double secondsAnalysis;
	
	
	//----------
	// OPERATORS
	//----------
	
	/**
	 * Insertion stream operator
	 * @param out The output stream to write to
	 * @param sr The step results to print
	 * @return A reference to the output stream for cascading
	 */
	friend ostream & operator << ( ostream & out, const stepResults & sr );
};
