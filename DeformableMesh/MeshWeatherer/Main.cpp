#include "CGAL_typedefs.h"
#include "Utils.h"
#include "EulerFluid.h"
#include "ScreenRegion.h"
#include "StoneWeatherer.h"
#include "SurfaceMesh.h"

#include <CGAL/Timer.h>
#include <GL/glut.h>
#include <iostream>
#include <omp.h>

using namespace std;

static bool simulationIsRunning = false;
static bool simulationShouldQuit = false;

//double relabelTime = 0.0;


/**
 * Determines if the simulation is running
 * @return TRUE if the simulation is running, FALSE if it is stopped
 */
bool & isRunning() {

	return simulationIsRunning;
}


/**
 * Determines if the program should quit
 * @return TRUE if the program should quit, FALSE otherwise
 */
bool & shouldQuit() {

	return simulationShouldQuit;
}


/**
 * The fluid grid for erosion
 */
FluidGrid3D * fluid = 0;
typedef FluidGrid3D::iterator fg_it;

/**
 * The number of cells in the grid
 */
static const int numCells = 10000;

/**
 * The size of a cell ???
 */
static double h = 0.1;

/**
 * The timestep for the simulation
 */
static double dt = 0.1;

/**
 * The amount of time that has passed so far in the simulation
 */
static double timeSoFar = 0.0;

/**
 * The results of the simulation step
 */
stepResults results;
stepResults cumulativeResults;

/**
 * The interface between the program and the material to weather
 */
StoneWeatherer sw;

/**
 * Create a new mesh of type ROCK
 */
SurfaceMesh rock( ROCK );
SurfaceMesh moreRock( MORE_ROCK );


/**
 * Draws the current mesh
 */
void drawMesh() {

	rock.draw();
	moreRock.draw();

	// If there is no water, return
	if ( fluid == 0 || getAirWaterRatio() >= 1.0 ) {

		return;
	}
	
	glDisable( GL_LIGHTING );
	glColor4d( 0.0, 0.0, 0.0, 1.0 );
	glBegin( GL_LINE_LOOP );
		glVertex3d( fluid->minPoint[ 0 ], fluid->minPoint[ 1 ], fluid->minPoint[ 2 ] );
		glVertex3d( fluid->minPoint[ 0 ] + fluid->dimensions[ 0 ] * fluid->scale, fluid->minPoint[ 1 ], fluid->minPoint[ 2 ] );
		glVertex3d( fluid->minPoint[ 0 ] + fluid->dimensions[ 0 ] * fluid->scale, fluid->minPoint[ 1 ], fluid->minPoint[ 2 ] + fluid->dimensions[ 2 ] * fluid->scale );
		glVertex3d( fluid->minPoint[ 0 ], fluid->minPoint[ 1 ], fluid->minPoint[ 2 ] + fluid->dimensions[ 2 ] * fluid->scale );
	glEnd();
	glBegin( GL_LINE_LOOP );
		glVertex3d( fluid->minPoint[ 0 ], fluid->minPoint[ 1 ] + fluid->dimensions[ 1 ] * fluid->scale, fluid->minPoint[ 2 ] );
		glVertex3d( fluid->minPoint[ 0 ] + fluid->dimensions[ 0 ] * fluid->scale, fluid->minPoint[ 1 ] + fluid->dimensions[ 1 ] * fluid->scale, fluid->minPoint[ 2 ] );
		glVertex3d( fluid->minPoint[ 0 ] + fluid->dimensions[ 0 ] * fluid->scale, fluid->minPoint[ 1 ] + fluid->dimensions[ 1 ] * fluid->scale, fluid->minPoint[ 2 ] + fluid->dimensions[ 2 ] * fluid->scale );
		glVertex3d( fluid->minPoint[ 0 ], fluid->minPoint[ 1 ] + fluid->dimensions[ 1 ] * fluid->scale, fluid->minPoint[ 2 ] + fluid->dimensions[ 2 ] * fluid->scale );
	glEnd();
	glColor4d( 0.0, 0.5, 1.0, 1.0 );

	int x;
	int y;
	int z;

	for ( fg_it it = fluid->begin(); it != fluid->end(); ++it ) {

		if ( it->rho <= 0.0 ) {

			continue;
		}

		glPointSize( sqrt( it->rho ) * 5.0 );
		glBegin( GL_POINTS );
			x = 0;
			y = 0;
			z = 0;
			fluid->unIndex( it.getIndex(), x, y, z );
			glVertex3d( h * ( x + 0.5 ) + fluid->minPoint[ 0 ], h * ( y + 0.5 ) + fluid->minPoint[ 1 ], h * ( z + 0.5 ) + fluid->minPoint[ 2 ] );
		glEnd();
	}

	glEnable( GL_LIGHTING );
}


/**
 * Recolorizes the vertices of the object by their softness
 */
void recolorize() {

	rock.recolorize( liveSoftness );
	moreRock.recolorize( liveSoftness );
}


/**
 * Tells the mesh to rebuild its list of vertices and faces
 */
void rebuildArrays() {

	rock.rebuildArrays( sw );
	moreRock.rebuildArrays( sw );
}


/**
 * The root panel (viewport)
 */
screenRegion * rootPanel = 0;

/**
 * The currently active panel
 */
screenRegion * activePanel = 0;

/**
 * The height of the screen
 */
static int totalScreenHeight = 0;


/**
 * Mouse click callback
 * @param button The mouse button that was clicked
 * @param state The state of the button (up or down)
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coordinate of the mouse in screen coordinates
 */
void mouseButton( int button, int state, int x, int y ) {

	if ( state == GLUT_DOWN && rootPanel ) {

		activePanel = rootPanel->acceptMouse( button, x, totalScreenHeight - y );
		glutPostRedisplay();
	}
	else if ( rootPanel ) {

		if ( rootPanel->readjust() ) {

			glutPostRedisplay();
		}
	}
}


/**
 * Mouse drag callback (called when the mouse moves while a button is down)
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coordinate of the mouse in screen coordinates
 */
void mouseDrag( int x, int y ) {

	if ( activePanel ) {

		activePanel->dragMouse( x, totalScreenHeight - y );
	}

	glutPostRedisplay();
}


/**
 * Gets the edge length for the given vertex
 * @param v The vertex to get the edge length for
 * @return The edge length of the given vertex
 */
double edgeLength( const Vertex_handle & v ) {

	double flatness = 1.0 - v->info().importance;

	return ( flatness ) * ( getMaxEdgeLength() - getMinEdgeLength() ) + getMinEdgeLength();
}


/**
 * The minimum extent of the Euler fluid
 */
double minPoint[ 3 ];

/**
 * The maximum extent of the Euler fluid
 */
double maxPoint[ 3 ];


/**
 * Moves points in a torus based on which torus it is
 * @param p The point to move
 * @param d The vertex data for the point
 * @param caller The stone weathere simulating the erosion
 * @return The new point location after moving
 */
Point movePoint( const Point & p, const VertexData & d, const StoneWeatherer * caller ) {

	double stepSize = getOverdrive() * dt * caller->minEdgeLength * 10;

	//if ( ( d.flag & ROCK ) && !( d.flag & MORE_ROCK ) ) {
	if ( d.flag & ROCK ) {

		return p + Vector( 1.0, 0.0, 0.0 ) * stepSize;
	}
	//else if ( ( d.flag & MORE_ROCK ) && !( d.flag & ROCK ) ) {
	else if ( d.flag & MORE_ROCK ) {

		return p + Vector( -1.0, 0.0, 0.0 ) * stepSize;
	}
	else {

		return p;
	}
}


/**
 * Hydraulic and atmospheric erosion callback
 * @param p The point to erode
 * @param d The data for the vertex to erode
 * @param caller The stone weatherer simulating the erosion
 * @return The new point location after erosion
 */
Point erodePoint( const Point & p, const VertexData & d, const StoneWeatherer * caller ) {

	// Fix overdrive problem now that this is spheroidal as well as hydraulic
	double stepSize = getOverdrive() * dt * softness( p.x(), p.y(), p.z() ) * caller->minEdgeLength;

	if ( getAirWaterRatio() >= 1.0 ) {

		return p - d.normal * ( stepSize * caller->ci( d.curvature ) );
	}
	else if ( getAirWaterRatio() <= 0.0 ) {

		double speedScale = getFluidScale() * h;
		double drop = fluid->depositionRate( p, speedScale ) * stepSize;

		if ( drop != 0 ) {

			fluid->injectAlluvium( p, -drop * getMaxEdgeLength() * getMaxEdgeLength() / h );

			return p + d.normal * drop / h;
		}
		else {

			return p;
		}
	}
	else {

		double speedScale = getFluidScale() * h;
		double drop = fluid->depositionRate( p, speedScale ) * stepSize * ( 1.0 - getAirWaterRatio() );

		if ( drop != 0.0 ) {

			fluid->injectAlluvium( p, -drop * getMaxEdgeLength() * getMaxEdgeLength() / h );
		}

		return p + d.normal * ( drop / h - stepSize * getAirWaterRatio() * caller->ci( d.curvature ) );
	}
}


/**
 * Deciding Euler grid cell contents
 * @param p The point to check
 * @reutrn TRUE if the given point is rock, FALSE otherwise
 */
bool isSolid( const Point & p ) {

	//return sw.getContents( p ) == ROCK;
	return sw.getContents( p ) != AIR;
}


/**
 * Deciding Euler grid cell contents
 * @param p The point to check
 * @return TRUE if the point is a source of water, FALSE otherwise
 */
bool isSource( const Point & p ) {

	return p.y() > maxPoint[ 1 ] &&
		p.x() > ( maxPoint[ 0 ] - minPoint[ 0 ] ) * 0.4 + minPoint[ 0 ] &&
		p.x() < ( maxPoint[ 0 ] - minPoint[ 0 ] ) * 0.6 + minPoint[ 0 ] &&
		p.z() > ( maxPoint[ 2 ] - minPoint[ 2 ] ) * 0.4 + minPoint[ 2 ] &&
		p.z() < ( maxPoint[ 2 ] - minPoint[ 2 ] ) * 0.6 + minPoint[ 2 ];
}


/**
 * Helper to extend bounds for Euler grid
 * @param v The vertex to extend to if any of its components isn't included yet
 */
void findBounds( const Vertex & v ) {

	for ( int i = 0; i < 3; ++i ) {

		if ( v.point()[ i ] < minPoint[ i ] ) {

			minPoint[ i ] = v.point()[ i ];
		}
		else if ( v.point()[ i ] > maxPoint[ i ] ) {

			maxPoint[ i ] = v.point()[ i ];
		}
	}
}


/**
 * Set to true if redrawing is needed
 */
bool needRedraw = false;

void asciiKeys( unsigned char, int, int );

/**
 * The function called by threads while they're idle
 */
void threadIdle( void ) {

	double speedScale = getFluidScale() * h;

	if ( fluid == 0 ) {

		// Initialize the fluid grid
		minPoint[ 0 ] = +1.0e300;
		minPoint[ 1 ] = +1.0e300;
		minPoint[ 2 ] = +1.0e300;
		maxPoint[ 0 ] = -1.0e300;
		maxPoint[ 1 ] = -1.0e300;
		maxPoint[ 2 ] = -1.0e300;
		sw.callOnVertices( findBounds, ( Contents )( ~0u ) );

		// Find the grid that just covers the data with no less than the target cell count
		double width = maxPoint[ 0 ] - minPoint[ 0 ];
		double height = maxPoint[ 1 ] - minPoint[ 1 ];
		double depth = maxPoint[ 2 ] - minPoint[ 2 ];
		double volume = width * height * depth;

		h = pow( volume / numCells, 1.0 / 3.0 );

		int xCells = ( int ) ceil( width / h );
		minPoint[ 0 ] -= ( xCells * h - width ) * 0.5;
		maxPoint[ 0 ] -= ( xCells * h - width ) * 0.5;

		int yCells = ( int ) ceil( height / h );
		minPoint[ 1 ] -= ( yCells * h - height ) * 0.5;
		maxPoint[ 1 ] -= ( yCells * h - height ) * 0.5;

		int zCells = ( int ) ceil( depth / h );
		minPoint[ 2 ] -= ( zCells * h - depth ) * 0.5;
		maxPoint[ 2 ] -= ( zCells * h - depth ) * 0.5;

		fluid = new FluidGrid3D( xCells + 4, yCells + 4, zCells + 4 );
		fluid->scale = h;
		fluid->minPoint[ 0 ] = minPoint[ 0 ] - 2 * h;
		fluid->minPoint[ 1 ] = minPoint[ 1 ] - 2 * h;
		fluid->minPoint[ 2 ] = minPoint[ 2 ] - 2 * h;
		fluid->setAllOpenEdges();
		cout << "Created fluid grid with " << fluid->length << " cells\n";
	}

	updateLockedDurability();

	fluid->preInjection();
	// this is where you switch between erosion and collision.
	// mdj 11/7/2011
//	results = sw.doOneCustomStep( edgeLength, erodePoint );
	results = sw.doOneCustomStep( edgeLength, movePoint );
	timeSoFar += results.secondsTotal;
	rebuildArrays();
	fluid->postInjection( speedScale, 0.2 );

	fluid->setSolidity( isSolid );
	fluid->injectFluid( isSource );

	static int frames = 0;
	static const int eulerOverdrive = 4;
	static double maxVelocity = 100.0;
	double g[ 3 ] = { 0.0, -9.8, 0.0 };
	double eulerDt = ( maxVelocity > 1.0 ) ? ( 1.0 / maxVelocity ) : ( 1.0 );
	//cout << "eulerDt = " << eulerDt << "\n";
	++frames;
	cout << "Frames: " << frames << "\n";

	if ( frames == 2573 ) {

		asciiKeys( ( unsigned char ) ' ', 0, 0 );
	}

	for ( int i = 0; i < 1 + eulerOverdrive; ++i ) {

		fluid->advectContents( eulerDt, 0, 0 );
	}

	fluid->advectVelocities( eulerDt * ( 1 + eulerOverdrive ), g, 0.05 );
	fluid->gatherStatistics( &maxVelocity, 0, 0 );

	needRedraw = true;
}


/**
 * Updates the frames-per-second display
 * @param value ??? (not used)
 */
void fpsUpdate( int value ) {

	if ( needRedraw ) {

		glutPostRedisplay();
		needRedraw = false;
	}

	glutTimerFunc( 20, fpsUpdate, 20 );
}


/**
 * The callback for when no drawing needs to occur
 */
void idle() {

	if ( !isRunning() ) {

		return;
	}

	threadIdle();
	glutPostRedisplay();
}


#include <fstream>

void saveRocks (int saved) 
{
			printf( "saving...\n" );
			char temp[ 256 ];
			sprintf_s( temp, 256, "mesh_after_%.3f_seconds.obj", timeSoFar );
			++saved;
			ofstream obj( temp );
			rock.writeOBJFile( obj );
			moreRock.writeOBJFile( obj );
			obj.close();
			printf( "done\n" );
}

/**
 * Callback for key press event
 * @param key The key that was pressed
 * @param x The x-coordinate of the mouse at the time of the key press
 * @param y The y-coordinate of the mouse at the time of the key press
 */
void asciiKeys( unsigned char key, int x, int y ) {

	//static double lastSecondsTotal = 0.0;
	static int saved = 0;
	//static CGAL::Timer timer;
	//static double totalTime = 0.0;

	if ( activePanel && activePanel->keyEvent( key ) ) {

		glutPostRedisplay();

		return;
	}

	switch ( key ) {

		case ' ':

			printf( "toggle running from %u to %u\n", isRunning(), !isRunning() );

			if ( isRunning() ) {

				//printf( "Runtime Info\n   Last frame: %f\n   Total time: %f\n   CGAL time: %f\n   Labeling time: %f\n", StoneWeatherer::secondsTotal - lastSecondsTotal, StoneWeatherer::secondsTotal, StoneWeatherer::secondsCGAL, StoneWeatherer::secondsLabeling );
				//totalTime += timer.time();
				//timer.stop();
				//printf( "Wall time so far: %f\nRelabel time so far: %f\n", totalTime, relabelTime );
				printf( "\nResults time:\n   seconds motion   - %4.6f\n   seconds CGAL     - %4.6f\n   seconds labeling - %4.6f\n   seconds analysis - %4.6f\n   seconds total    - %4.6f\n", cumulativeResults.secondsMotion, cumulativeResults.secondsCGAL, cumulativeResults.secondsLabeling, cumulativeResults.secondsAnalysis, cumulativeResults.secondsTotal );
			}
			//else {
			//
			//	timer.start();
			//	timer.reset();
			//}

			isRunning() = !isRunning();
			break;

		case 's': {
			saveRocks(saved); 
		} break;

		case 'l':

			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			needRedraw = true;
			glutPostRedisplay();
			break;

		case 'f':

			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			needRedraw = true;
			glutPostRedisplay();
			break;

		case 'z':

			printf( "decreasing edge length (max from %f to %f) ...\n", getMaxEdgeLength(), getMaxEdgeLength() * 0.8 );
			setMaxEdgeLength( getMaxEdgeLength() * 0.8 );
			setMinEdgeLength( getMinEdgeLength() * 0.8 );

			if ( !isRunning() ) {

				sw.doOneStep( edgeLength, softness, 1.0 );
				rebuildArrays();
				needRedraw = true;
			}

			printf( "done\n" );
			glutPostRedisplay();
			break;


		case 'x':

			printf( "increasing edge length (max from %f to %f) ...\n", getMaxEdgeLength(), getMaxEdgeLength() * 2.0 );
			setMaxEdgeLength( getMaxEdgeLength() * 2.0 );
			setMinEdgeLength( getMinEdgeLength() * 2.0 );

			if ( !isRunning() ) {

				sw.doOneStep( edgeLength, softness, 1.0 );
				rebuildArrays();
				needRedraw = true;
			}

			printf( "done\n" );
			glutPostRedisplay();
			break;
	}

	//lastSecondsTotal = StoneWeatherer::secondsTotal;
}


/**
 * Callback for special key press event
 * @param code The special key that was pressed
 * @param x The x-coordinate of the mouse at the time of the key press
 * @param y The y-coordinate of the mouse at the time of the key press
 */
void specialKeys( int code, int x, int y ) {

	if ( activePanel && activePanel->specialKeyEvent( code ) ) {

		glutPostRedisplay();

		return;
	}
}


/**
 * Callback for resized window
 * @param w The new widgh of the window
 * @param h The new height of the window
 */
void reshape( int w, int h ) {

	totalScreenHeight = h;

	if ( !rootPanel ) {

		rootPanel = new vSplitPane( new hSplitPane( new dataPane(), new statusPane() ), new hSplitPane( new layerPane(), new meshPane(), 0.3 ), 0.165 );
		rootPanel->readjust();
	}

	rootPanel->reshape( 0, 0, w, h );
	glutPostRedisplay();
}


/**
 * Callback to display the scene
 */
void display() {

	if ( !rootPanel ) {

		return;
	}

	rootPanel->draw( results );
	glutSwapBuffers();
}


/**
 * Reads an OBJ file for the mesh
 * @param filename The name of the OBJ file to read
 */
void readLumpy( const char * filename ) {

	sw.setInitialMesh( filename );
}


/**
 * Program execution starts here
 * @param argc The number of command-line arguments
 * @param argv The list of command-line arguments
 * @return An error code (0 for success)
 */
int main( int argc, char * argv[] ) {

	if ( argc <= 1 ) {

		printf( "Optional usage: %s [obj filename]\n", argv[ 0 ] );
		printf( "Optional usage: %s [obj filename] [mcf OR rock OR uniform]\n", argv[ 0 ] );
		printf( "Optional usage: %s [obj filename] [alpha] [beta]\n", argv[ 0 ] );
	}

	if ( argc > 1 ) {

		readLumpy( argv[ 1 ] );
	}
	else {
		// set the file name mdj 
		readLumpy( "ObjFiles/TwoTori.obj" );
		//readLumpy( "ObjFiles/TheY.obj" );
	}

	printf( "done reading\n" );

	if ( argc > 2 ) {

		if ( argc > 3 ) {

			sw.setCurveFunction( atof( argv[ 2 ] ), atof( argv[ 3 ] ) );
		}
		else {

			if ( argv[ 2 ][ 0 ] == 'm' || argv[ 2 ][ 0 ] == 'M' ) {

				sw.setCurveFunction( 1.0, -1.0 );
			}
			else if ( argv[ 2 ][ 0 ] == 'u' || argv[ 2 ][ 0 ] == 'U' ) {

				sw.setCurveFunction( 0.0, -1.0 );
			}
			else {

				sw.setCurveFunction( 0.125, 0.001 );
			}
		}
	}
	else {

		sw.setCurveFunction( 0.125, 0.001 );
	}

	rebuildArrays();

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Delaunay-based Mesh Weathering" );

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutIdleFunc( idle );
	glutMouseFunc( mouseButton );
	glutMotionFunc( mouseDrag );
	glutKeyboardFunc( asciiKeys );
	glutSpecialFunc( specialKeys );

	glEnable( GL_SCISSOR_TEST );
	glEnable( GL_DEPTH_TEST );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_CULL_FACE );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	{
		GLfloat p[ 4 ] = { -0.2f, 0.4f, 0.4f, 0.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, p );
	}

	{
		GLfloat c[ 4 ] = { 0.5f, 0.5f, 0.5f, 0.0f };
		glMaterialfv( GL_FRONT, GL_SPECULAR, c );
	}

	glMaterialf( GL_FRONT, GL_SHININESS, 32 );
	glEnable( GL_COLOR_MATERIAL );
	
	cumulativeResults.secondsMotion = 0.0;
	cumulativeResults.secondsCGAL = 0.0;
	cumulativeResults.secondsLabeling = 0.0;
	cumulativeResults.secondsAnalysis = 0.0;
	cumulativeResults.secondsTotal = 0.0;

	glutMainLoop();

	return 0;
}
