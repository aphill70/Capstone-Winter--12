#include <GL\glut.h>
#include <cstdarg>
#include <CGAL/Memory_sizer.h>
#include "StatusPane.h"


/**
 * Draws the given string using OpenGL
 * @param row The raster 2D row to draw to
 * @param s The format string to draw
 * @param ... The parameters for s
 */
void myDrawString( int row, const char * s, ... ) {

	char temp[ 256 ];
	va_list list;

	va_start( list, s );
	vsnprintf_s( temp, 256, 256, s, list );
	va_end( list );

	glRasterPos2f( 4.0f, 4.0f + row * 15.0f );

	for ( int i = 0; temp[ i ]; ++i ) {

		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, temp[ i ] );
	}
}


// ***** STATUS PANE ***** //

/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void statusPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void statusPane::draw( const stepResults & results ) const {

	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 0.8, 0.8, 0.8, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	glOrtho( 0, w, 0, h, -1, 1 );

	glColor4f( 0.2f, 0.2f, 0.2f, 1.0f );

	// WORKAROUND: glColor, on some platforms, does not change the color until glVertex is called...
	glBegin( GL_LINES );
		glVertex2f( -1.0f, -1.0f );
		glVertex2f( -2.0f, -2.0f );
	glEnd();

	int i = 0;

	myDrawString( i++, "Last frame took %.2g seconds", results.secondsTotal );
#ifdef __GNUC__
	myDrawString( i++, "Memory: %f MB", ( memorySizer.virtual_size() / 10486 ) * 0.01 );
#endif
	myDrawString( i++, "Tetrahedra: %d", results.numTetrahedrons );
	myDrawString( i++, "Triangles:  %d", results.numVertices * 2 - 4 );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * statusPane::acceptMouse( int button, int x, int y ) {

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void statusPane::dragMouse( int x, int y ) {

	return;
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool statusPane::readjust() {

	return false;
}


/**
 * Called when the user presses a key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool statusPane::keyEvent( char c ) {

	return false;
}


/**
 * Called when the user presses a special key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool statusPane::specialKeyEvent( int c ) {

	return false;
}
