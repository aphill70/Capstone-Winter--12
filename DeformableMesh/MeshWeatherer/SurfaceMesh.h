#pragma once

#include "StoneWeatherer.h"
#include <GL/glut.h>


/**
 * This class serves as the main communication from the simulation thread to the GUI thread,
 * and hence has several methods that enforce serial entry.
 * The class reads the surface mesh out of a StoneWeatherer and sends it to OpenGL or an OBJ file.
 * It can also update the color of the mesh on the fly, without waiting for the weatherer.
 */
class SurfaceMesh {

protected:

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The type of material for the surface
	 */
	const Contents material;

	// Lists for each vertex

	/**
	 * The number of vertices in the mesh
	 */
	int numVertices;

	/**
	 * The maximum number of vertices
	 */
	int capVertices;

	/**
	 * The list of vertices
	 */
	GLfloat * c4fn3fv3f;

	/**
	 * The list of curvatures for the vertices
	 */
	GLfloat * curvatures;

	/**
	 * The list of softnesses for the vertices
	 */
	GLfloat * softnesses;

	// Lists for each face

	/**
	 * The number of faces
	 */
	int numFaces;

	/**
	 * The maximum number of faces
	 */
	int capFaces;

	/**
	 * The list of triangles (faces) (by index into the vertices array)
	 */
	GLuint * triangles;

	// A display list, and a flag to know when to update it

	/**
	 * Display list for mesh
	 */
	mutable GLuint meshList;

	/**
	 * Flag for when to update the display list
	 */
	mutable bool needUpdate;


public:

	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Basic constructor (creates an empty mesh)
	 * @param m The material label for the mesh
	 */
	SurfaceMesh( Contents m );


	/**
	 * Destructor
	 */
	~SurfaceMesh();


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Adds a new vertex to the mesh (intended to be called only by meshVertexCallback (see below)).
	 * @param v The vertex to add
	 */
	void addVertex( const Vertex & v );


	/**
	 * Connects three vertices into a face (no error checking is performed) (intended to be called only by meshFaceCallback (see below)).
	 * @param a The index of the first vertex
	 * @param b The index of the second vertex
	 * @param c The index of the third vertex
	 */
	void addFace( int a, int b, int c );


	/**
	 * Rebuilds the lists of vertices and triangles (to be called by the simulation loop after each update; enforces serial entry)
	 * @param sw The weatherer whose arrays to rebuild
	 */
	void rebuildArrays( const StoneWeatherer & sw );


	/**
	 * Recolorizes the mesh (to be called by the GUI when softness changes; enforces serial entry)
	 * @param soft The function to call to determine the softness of a given point
	 */
	void recolorize( double ( *soft )( double, double, double ) );


	/**
	 * Sends data to OpenGL (will crash if called by a non-OpenGL thread) (enforces serial entry if and only if the display list is out of date)
	 */
	void draw() const;


	/**
	 * Sends data as a WaveFront OBJ to the given output stream (enforces serial entry)
	 * @param out The output stream to write to
	 * @param vertexOffset The offset into the vertex list to start with (default 0)
	 */
	void writeOBJFile( ostream & out, int vertexOffset = 0 ) const;


	/**
	 * Gets the number of vertices in the mesh
	 * @return The number of vertices in the mesh
	 */
	inline int vertices() const {

		return numVertices;
	}


protected:

	/**
	 * Helper to expand internal arrays (for vertices)
	 */
	void upsizeVertices();


	/**
	 * Helper to expand internal arrays (for triangles)
	 */
	void upsizeFaces();
};
