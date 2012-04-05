#pragma once

#include <vector>
#include <map>
#include "CGAL_typedefs.h"
#include "CurveToMotion.h"
#include "Include/StdInt.h"
#include "stepResults.h"

using namespace std;


//---------------------
// FORWARD DECLARATIONS
//---------------------

void saveRocks (int saved); 
struct Tetrahedron;

//extern double relabelTime;


/**
 * Used for weathering/eroding stone
 */
class StoneWeatherer {

public:

	//-------------------
	// STATIC MEMBER DATA
	//-------------------

	/**
	 * Total seconds spent in computation
	 */
	//static double secondsTotal;

	/**
	 * Seconds CGAL spent building the tetrahedron mesh for new points
	 */
	//static double secondsCGAL;

	/**
	 * Seconds spent identifying "inside" and "outside" tetrahedrons (relabeling)
	 */
	//static double secondsLabeling;

	/**
	 * Hard-code the number of solid material types as 2
	 */
	//enum { NUMBER_OF_SOLID_MATERIAL_TYPES = 2 };
	enum { NUMBER_OF_SOLID_MATERIAL_TYPES = 3 };


	//------------
	// MEMBER DATA
	//------------

	/**
	 * The minimum edge length for the mesh
	 */
	double minEdgeLength;

	/**
	 * The minimum curvature for the mesh
	 */
	double minCurvature;

	/**
	 * The maximum curvature for the mesh
	 */
	double maxCurvature;

	/**
	 * Used for computing motion of points based on local curvature
	 */
	curveToMotion ci;

	/**
	 * The list of the two Delaunay meshes (old and new)
	 */
	Delaunay dt[ 2 ];

	/**
	 * The newly computed mesh
	 */
	Delaunay * newDT;

	/**
	 * The old mesh from the previous step
	 */
	Delaunay * oldDT;


	/**
	 * The function to call to determine if a point is live
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @return TRUE if the point is live, FALSE otherwise
	 */
	bool ( *live )( double x, double y, double z );


	/**
	 * Determines if the given point starts out filled
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @return TRUE if the point starts filled, FALSE otherwise
	 */
	bool ( *startsFilled )( double x, double y, double z );


	/**
	 * The initial locations of the vertices in the mesh
	 */
	vector<Point> initialPoints;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Default constructor
	 */
	StoneWeatherer();


	/**
	 * Destructor
	 */
	~StoneWeatherer();


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Sets the curvature response function
	 *	hyperbolic interpolation alpha + (c + sqrt(c * c + beta))
	 *		alpha is rate in pit regions
	 *		beta is range of regions treated as flat
	 *	if beta < 0 and alpha == 0, operator()(curvature) returns 1
	 *	if beta < 0 and alpha != 0, operator()(curvature) returns alpha * curvature
	 * @param alpha The new alpha value
	 * @param beta The new beta value
	 */
	void setCurveFunction( double alpha, double beta );


	/**
	 * Sets the function to call for determining liveness. If not supplied, assume "live(x,y,z) = true"
	 * @param nlive The function to call to determine if a point is live
	 */
	void setLiveRegion( bool ( *nlive )( double x, double y, double z ) );


	/**
	 * Sets the initial inside/outside test and initial points. Uses odd/even crossing rule for insideness.
	 * @param objFileName The file containing the initial mesh
	 * @return TRUE if the file loaded successfully, FALSE otherwise
	 */
	bool setInitialMesh( const char * objFileName );


	/**
	 * Gets the list of points from the mesh
	 * @param points Stores the list of points
	 */
	void getPoints( vector<Point> & points ) const;


	/**
	 * Gets all the tetrahedrons in the mesh
	 * @param tetrahedrons Stores the tetrahedrons
	 */
	void getTetrahedrons( vector<Tetrahedron> & tetrahedrons ) const;


	/**
	 * Gets the type of material at the given point
	 * @param point The point to check
	 * @return The material type at the given point
	 */
	Contents getMaterialType( const Point & point ) const;


	//----------------
	// MAIN SIMULATION
	//----------------

	/**
	 * Executes one timestep
	 * @param averageEdgeLength The function to compute the average edge length for a given vertex
	 * @param softness The function to compute the softness of a given point
	 * @param overdrive The overdrive value (default 1)
	 * @return The results of running the timestep
	 */
	stepResults doOneStep( double ( *averageEdgeLength )( const Vertex_handle & v ), double ( *softness )( double x, double y, double z ), double overdrive = 1 );


	/**
	 * Executes one custom timestep
	 * @param averageEdgeLength The function to compute the average edge length for a given vertex
	 * @param getOffsetPoint Computes the new location of the given point
	 * @return The results of running the timestep
	 */
	stepResults doOneCustomStep( double ( *averageEdgeLength )( const Vertex_handle & v ), Point( *getOffsetPoint )( const Point & p, const VertexData & d, const StoneWeatherer * caller ) );


	/**
	 * Calls the given function on all vertices that are on a boundary and have a normal and curvature
	 * @param callback The function to call
	 * @param material The material type (default ROCK)
	 */
	void callOnVertices( void ( *callback )( const Vertex & v ), Contents material = ROCK ) const;


	/**
	 * Calls the given function on all triangles in the mesh (will always provide vertices in CCW order as viewed from the outside)
	 * @param callback The function to call
	 * @param material The material type (default ROCK)
	 */
	void callOnFaces( void ( *callback )( const Vertex & v0, const Vertex & v1, const Vertex & v2 ), Contents material = ROCK ) const;


	/**
	 * Calls the given function on all cells in the mesh
	 * @param callback The function to call
	 */
	void callOnCells( void ( *callback )( const Cell_handle & c, const Delaunay & dt ) ) const;


	/**
	 * Gets the material for the given point
	 * @param p The point to get the material for
	 * @return The material at the given point
	 */
	Contents getContents( const Point & p ) const;


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Swaps the pointers to the old mesh and the new one
	 */
	void swapDT();


	/**
	 * Sets all the contet flags (AIR, DIRT, ROCK) for the vertices
	 * @param midPoint Stores the approximate center of mass of the object
	 * @param pointMap Maps circumcenters to old locations
	 */
	void setContentFlags( double midPoint[ 3 ], map<Point,Point> & pointMap );


	/**
	 * Sets the new vertex info for the vertices in the new mesh
	 */
	void setVertexInfo();
};


/**
 * Computes the circumcenters of the given cell
 * @param cell The cell to compute the circumcenters for
 * @param result Stores the circumcenter
 * @return TRUE always
 */
 bool computeCircumcenter( const Cell_handle & cell, Point & result );
