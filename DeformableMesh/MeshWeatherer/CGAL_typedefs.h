#pragma once

#include <CGAL/basic.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_hierarchy_3.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Triangulation_hierarchy_vertex_base_3.h>

#include <CGAL/Segment_tree_k.h>
#include <CGAL/Range_segment_tree_traits.h>

using namespace CGAL;


//---------
// TYPEDEFS
//---------

typedef Filtered_kernel< Simple_cartesian<double> > RepClass;
typedef Vector_3<RepClass> Vector;

/**
 * Bitmask for types of material (bigger number = heavier material)
 */
enum Contents { AIR = 1, DIRT = 2, ROCK = 4, MORE_ROCK = 8 };

/**
 * Defines types of vertices for deciding whether to remove one
 */
enum VertexKiller { BORDER = 0, FLOATER = 1, TOO_CLOSE = 2 };


/**
 * Stores data about a vertex
 */
struct VertexData {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * Used to determine whether or not to remove the vertex
	 */
	VertexKiller kill;

	/**
	 * The surface normal at the vertex
	 */
	Vector normal;

	/**
	 * The surface curvature at the vertex (rough estimate of the gaussian curvature at a point
	 */
	double curvature;

	/**
	 * The absolute value of the sine of the maximum angular difference between the normal and the surrounding edges
	 */
	double importance;

	/**
	 * Stores the material type (e.g., AIR, STONE, etc.)
	 */
	int flag;

	/**
	 * The number of edges incident on this vertex
	 */
	int numEdges;

	/**
	 * Which color to use to color the object
	 */
	double rgb[ 3 ];

	/**
	 * The unique ID for the vertex
	 */
	mutable int userData;


public:

	//----------
	// FUNCTIONS
	//----------

	/**
	 * Clears all vertex data
	 */
	void clear() {

		kill = FLOATER;
		normal = Vector( 0.0, 0.0, 0.0 );
		curvature = 0.0;
		importance = 0.0;
		flag = 0;
		numEdges = 0;
		rgb[ 0 ] = 0.0;
		rgb[ 1 ] = 0.0;
		rgb[ 2 ] = 0.0;
	}


	/**
	 * Computes the vertex data for a midpoint between two vertices along the surface
	 * @param a The first vertex
	 * @param b The second vertex
	 * @return The data for the midpoint between the two vertices
	 */
	static inline VertexData midPoint( const VertexData & a, const VertexData & b ) {

		VertexData result;
		result.kill = a.kill < b.kill ? a.kill : b.kill;
		result.normal = ( a.normal + b.normal ) * 0.5;
		result.curvature = ( a.curvature + b.curvature ) * 0.5;
		result.importance = ( a.importance + b.importance ) * 0.5;
		result.flag = ( a.flag & b.flag );
		result.numEdges = 0;
		//result.rgb[ 0 ] = ( a.rgb[ 0 ] + b.rgb[ 0 ] ) * 0.5;
		//result.rgb[ 1 ] = ( a.rgb[ 1 ] + b.rgb[ 1 ] ) * 0.5;
		//result.rgb[ 2 ] = ( a.rgb[ 2 ] + b.rgb[ 2 ] ) * 0.5;

		return result;
	}
};


//---------
// TYPEDEFS
//---------
//(for StoneWeatherer)

typedef Triangulation_vertex_base_with_info_3<VertexData,RepClass> VB_;
typedef Triangulation_hierarchy_vertex_base_3<VB_> VB;
typedef Triangulation_cell_base_with_info_3<Contents,RepClass> CB;
typedef Triangulation_data_structure_3<VB,CB> TDS;
typedef Delaunay_triangulation_3<RepClass,TDS> Delaunay_;
typedef Triangulation_hierarchy_3<Delaunay_> Delaunay;
typedef Delaunay::Finite_cells_iterator Cell_iterator;
typedef Delaunay::All_cells_iterator All_Cell_iterator;
typedef Delaunay::Finite_vertices_iterator Vertex_iterator;
typedef Delaunay::Cell_handle Cell_handle;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef Delaunay::Vertex Vertex;
typedef Delaunay::Point Point;


//---------
// TYPEDEFS
//---------
//(for OBJReader)

typedef CGAL::Segment_tree_map_traits_2<RepClass, int> STTraits;
typedef CGAL::Segment_tree_2<STTraits> Segtree;
typedef STTraits::Interval Interval;
typedef STTraits::Pure_interval PureInterval;
typedef STTraits::Key Key;
