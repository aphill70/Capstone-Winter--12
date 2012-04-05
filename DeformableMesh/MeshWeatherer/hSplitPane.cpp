#include <GL\glut.h>

#include "hSplitPane.h"


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void hSplitPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	gutter = gutter >> 1;
	int mid = ( int ) ( w * split );
	left->reshape( x, y, mid - gutter, h );
	right->reshape( x + mid + gutter, y, ( w - mid ) - gutter, h );
	gutter = gutter << 1;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void hSplitPane::draw( const stepResults & results ) const {

	int mid = ( int ) ( w * split );
	glViewport( x + mid - ( gutter >> 1 ), y, gutter, h );
	glScissor( x + mid - ( gutter >> 1 ), y, gutter, h );
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	left->draw( results );
	right->draw( results );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * hSplitPane::acceptMouse( int button, int x, int y ) {

	if ( x < this->x + ( int ) ( w * split ) - ( gutter >> 1 ) ) {

		return left->acceptMouse( button, x, y );
	}

	if ( x > this->x + ( int ) ( w * split ) + ( gutter >> 1 ) ) {

		return right->acceptMouse( button, x, y );
	}

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void hSplitPane::dragMouse( int x, int y ) {

	split = ( double ) ( x - this->x ) / w;
	reshape( this->x, this->y, w, h );
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool hSplitPane::readjust() {

	return left->readjust() | right->readjust();
}
