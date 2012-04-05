#include "Utils.h"
#include "SurfaceMesh.h"
#define ROCK_COLOR 0.11
#define OTHER_ROCK_COLOR 0.9

/**
 * Basic constructor (creates an empty mesh)
 * @param m The material label for the mesh
 */
SurfaceMesh::SurfaceMesh( Contents m ) : material( m ), numVertices( 0 ), capVertices( 0 ), c4fn3fv3f( 0 ), curvatures( 0 ), softnesses( 0 ), numFaces( 0 ), capFaces( 0 ), triangles( 0 ), meshList( 0 ), needUpdate( false ) {

	return;
}


/**
 * Destructor
 */
SurfaceMesh::~SurfaceMesh() {

	if ( c4fn3fv3f ) {

		free( c4fn3fv3f );
		c4fn3fv3f = 0;
	}

	if ( curvatures ) {

		free( curvatures );
		curvatures = 0;
	}

	if ( softnesses ) {

		free( softnesses );
		softnesses = 0;
	}

	if ( triangles ) {

		free( triangles );
		triangles = 0;
	}

	numVertices = 0;
	capVertices = 0;
	numFaces = 0;
	capFaces = 0;
}


/**
 * Adds a new vertex to the mesh (intended to be called only by meshVertexCallback (see below)).
 * @param v The vertex to add
 */
void SurfaceMesh::addVertex( const Vertex & v ) {

	v.info().userData = numVertices;

	if ( numVertices >= capVertices ) {

		upsizeVertices();
	}

	double x = v.point().x();
	double y = v.point().y();
	double z = v.point().z();
	double s = softness( x, y, z );
	double c = v.info().curvature;
	double nx = v.info().normal.x();
	double ny = v.info().normal.y();
	double nz = v.info().normal.z();

	if ( material == DIRT ) {

		c4fn3fv3f[ numVertices * 10 + 0 ] = 0.75;
		c4fn3fv3f[ numVertices * 10 + 1 ] = 0.75;
		c4fn3fv3f[ numVertices * 10 + 2 ] = 0.75;
	}
	else {
		if (material == ROCK) 
		{ 
			hueToRGB<GLfloat>( c4fn3fv3f + numVertices * 10, ROCK_COLOR * ( 1.0 - s ) );
		}
		if (material == MORE_ROCK) 
		{
			hueToRGB<GLfloat>( c4fn3fv3f + numVertices * 10, OTHER_ROCK_COLOR * ( 1.0 - s ) );
		}
	}

	//c4fn3fv3f[ numVertices * 10 + 0 ] = v.info().rgb[ 0 ];
	//c4fn3fv3f[ numVertices * 10 + 1 ] = v.info().rgb[ 1 ];
	//c4fn3fv3f[ numVertices * 10 + 2 ] = v.info().rgb[ 2 ];

	c4fn3fv3f[ numVertices * 10 + 3 ] = 1.0; // opaque
	c4fn3fv3f[ numVertices * 10 + 4 ] = nx;
	c4fn3fv3f[ numVertices * 10 + 5 ] = ny;
	c4fn3fv3f[ numVertices * 10 + 6 ] = nz;
	c4fn3fv3f[ numVertices * 10 + 7 ] = x;
	c4fn3fv3f[ numVertices * 10 + 8 ] = y;
	c4fn3fv3f[ numVertices * 10 + 9 ] = z;
	curvatures[ numVertices ] = c;
	curvatures[ numVertices ] = s;
	++numVertices;
}


/**
 * Connects three vertices into a face (no error checking is performed) (intended to be called only by meshFaceCallback (see below)).
 * @param a The index of the first vertex
 * @param b The index of the second vertex
 * @param c The index of the third vertex
 */
void SurfaceMesh::addFace( int a, int b, int c ) {

	if ( numFaces >= capFaces ) {

		upsizeFaces();
	}

	triangles[ numFaces * 3 + 0 ] = a;
	triangles[ numFaces * 3 + 1 ] = b;
	triangles[ numFaces * 3 + 2 ] = c;
	++numFaces;
}


/**
 * Recolorizes the mesh (to be called by the GUI when softness changes; enforces serial entry)
 * @param soft The function to call to determine the softness of a given point
 */
void SurfaceMesh::recolorize( double ( *soft )( double, double, double ) ) {

	if ( material == DIRT ) {

		return;
	}

	
	for ( int i = 0; i < numVertices; ++i ) {

		softnesses[ i ] = soft( c4fn3fv3f[ i * 10 + 7 ], c4fn3fv3f[ i * 10 + 8 ], c4fn3fv3f[ i * 10 + 9 ] );

		if (material == ROCK) 
		{ 
			hueToRGB<GLfloat>( c4fn3fv3f + i * 10, ROCK_COLOR * ( 1.0 - softnesses[ i ] ));
		} 
		if (material == MORE_ROCK) 
		{
				hueToRGB<GLfloat>( c4fn3fv3f + i * 10, OTHER_ROCK_COLOR * ( 1.0 - softnesses[ i ] ));
		}

//		hueToRGB<GLfloat>( c4fn3fv3f + i * 10, 0.6666666666666667 * ( 1.0 - softnesses[ i ] ) );
	}

	needUpdate = true;
}


/**
 * Sends data to OpenGL (will crash if called by a non-OpenGL thread) (enforces serial entry if and only if the display list is out of date)
 */
void SurfaceMesh::draw() const {

	if ( needUpdate ) {

		if ( !glIsList( meshList ) ) {

			meshList = glGenLists( 1 );
		}
		
		needUpdate = false;
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glNewList( meshList, GL_COMPILE_AND_EXECUTE );
			glInterleavedArrays( GL_C4F_N3F_V3F, 0, c4fn3fv3f );
			glDrawElements( GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, triangles );
		glEndList();
	}
	else if ( glIsList( meshList ) ) {

		glCallList( meshList );
	}
	else {

		// Nothing to draw
	}
}


/**
 * Sends data as a WaveFront OBJ to the given output stream (enforces serial entry)
 * @param out The output stream to write to
 * @param vertexOffset The offset into the vertex list to start with (default 0)
 */
void SurfaceMesh::writeOBJFile( ostream & out, int vertexOffset ) const {

	vertexOffset += 1;

	out << "# A single mesh with " << numVertices << " vertices and " << numFaces << " faces.\n"
		<< "# Texture coordinates are (softness, curvature) as measured during simulation.\n"
		<< "g " << ( material == DIRT ? "dirt" : "rock" ) << "\n"
		<< "usemtl " << ( material == DIRT ? "dirt" : "rock" ) << "\n";

	int i;

	for ( i = 0; i < numVertices; ++i ) {

		out << "v " << c4fn3fv3f[ 10 * i + 7 ] << " " << c4fn3fv3f[ 10 * i + 8 ] << " " << c4fn3fv3f[ 10 * i + 9 ] << "\n";
	}

	for ( i = 0; i < numVertices; ++i ) {

		out << "vn " << c4fn3fv3f[ 10 * i + 4 ] << " " << c4fn3fv3f[ 10 * i + 5 ] << " " << c4fn3fv3f[ 10 * i + 6 ] << "\n";
	}

	for ( i = 0; i < numFaces; ++i ) {

		out << "f " << triangles[ 3 * i + 0 ] + vertexOffset << "/" << triangles[ 3 * i + 0 ] + vertexOffset << "/" << triangles[ 3 * i + 0 ] + vertexOffset
			<< " " << triangles[ 3 * i + 1 ] + vertexOffset << "/" << triangles[ 3 * i + 1 ] + vertexOffset << "/" << triangles[ 3 * i + 1 ] + vertexOffset
			<< " " << triangles[ 3 * i + 2 ] + vertexOffset << "/" << triangles[ 3 * i + 2 ] + vertexOffset << "/" << triangles[ 3 * i + 2 ] + vertexOffset;
	}

	out << "# End of mesh.\n";
}


/**
 * Helper to expand internal arrays (for vertices)
 */
void SurfaceMesh::upsizeVertices() {

	if ( numVertices <= 0 ) {

		capVertices = 256;
	}
	else {

		capVertices *= 2;
	}

	c4fn3fv3f = ( GLfloat * ) realloc( c4fn3fv3f, capVertices * 10 * sizeof( GLfloat ) );
	curvatures = ( GLfloat * ) realloc( curvatures, capVertices * 1 * sizeof( GLfloat ) );
	softnesses = ( GLfloat * ) realloc( softnesses, capVertices * 1 * sizeof( GLfloat ) );
}


/**
 * Helper to expand internal arrays (for triangles)
 */
void SurfaceMesh::upsizeFaces() {

	if ( numFaces <= 0 ) {

		capFaces = 512;
	}
	else {

		capFaces *= 2;
	}

	triangles = ( GLuint * ) realloc( triangles, capFaces * 3 * sizeof( GLuint ) );
}


/**
 * Pointer to the surface mesh, used by callback functions
 */
SurfaceMesh * surfaceMeshPointer = 0;


/**
 * The function called to add a face to the mesh
 * @param v0 The first vertex of a triangle
 * @param v1 The second vertex of a triangle
 * @param v2 The third vertex of a triangle
 */
void meshFaceCallback( const Vertex & v0, const Vertex & v1, const Vertex & v2 ) {

	assert( v0.info().kill <= 0 );
	assert( v1.info().kill <= 0 );
	assert( v2.info().kill <= 0 );
	surfaceMeshPointer->addFace( v0.info().userData, v1.info().userData, v2.info().userData );
}


/**
 * The function called to add a vertex to the mesh
  *@param v0 The vertex to add to the mesh
 */
void meshVertexCallback( const Vertex & v0 ) {

	surfaceMeshPointer->addVertex( v0 );
}


/**
 * Rebuilds the lists of vertices and triangles (to be called by the simulation loop after each update; enforces serial entry)
 * @param sw The weatherer whose arrays to rebuild
 */
void SurfaceMesh::rebuildArrays( const StoneWeatherer & sw ) {

	numVertices = 0;
	numFaces = 0;

	surfaceMeshPointer = this;
	sw.callOnVertices( meshVertexCallback, material );
	sw.callOnFaces( meshFaceCallback, material );
	surfaceMeshPointer = 0;

	needUpdate = true;
}
