#include <GL\glut.h>

#include "vSplitPane.h"


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void vSplitPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	gutter = gutter >> 1;
	int mid = ( int ) ( h * split );
	bottom->reshape( x, y, w, mid - gutter );
	top->reshape( x, y + mid + gutter, w, ( h - mid ) - gutter );
	gutter = gutter << 1;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void vSplitPane::draw( const stepResults & results ) const {

	int mid = ( int ) ( h * split );
	glViewport( x, y + mid - ( gutter >> 1 ), w, gutter );
	glScissor( x, y + mid - ( gutter >> 1 ), w, gutter );
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	bottom->draw( results );
	top->draw( results );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * vSplitPane::acceptMouse( int button, int x, int y ) {

	// The math here is odd because OpenGL and GLUT have opposite Y orientations

	if ( y > this->y + ( int ) ( h * split ) + ( gutter >> 1 ) ) {

		return top->acceptMouse( button, x, y );
	}

	if ( y < this->y + ( int ) ( h * split ) - ( gutter >> 1 ) ) {

		return bottom->acceptMouse( button, x, y );
	}

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void vSplitPane::dragMouse( int x, int y ) {

	split = ( double ) ( y - this->y ) / h;
	reshape( this->x, this->y, w, h );
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool vSplitPane::readjust() {

	return top->readjust() | bottom->readjust();
}
