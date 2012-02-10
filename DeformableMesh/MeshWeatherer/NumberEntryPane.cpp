#include <GL\glut.h>

#include "NumberEntryPane.h"


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void numberEntryPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void numberEntryPane::draw( const stepResults & results ) const {

	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 0.8, 0.8, 0.8, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glOrtho( 0, w, 0, h, -1, 1 );
	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
	glBegin( GL_LINES );
		glVertex2f( 4 + ( ne.cursor + ne.horizontalLength ) * 8, 3 );
		glVertex2f( 4 + ( ne.cursor + ne.horizontalLength ) * 8, 18 );
	glEnd();
	glRasterPos2f( 4.0f, 4.0f );

	for ( int i = 0; i < ne.getLength(); ++i ) {

		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, ne.getCharacter( i ) );
	}
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * numberEntryPane::acceptMouse( int button, int x, int y ) {

	ne.setCursor( ( x - this->x + 5 ) / 8 );

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void numberEntryPane::dragMouse( int x, int y ) {

	return;
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool numberEntryPane::readjust() {

	return false;
}


/**
 * Called when the user presses a key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool numberEntryPane::keyEvent( char c ) {

	return ne.acceptKey( c );
}


/**
 * Called when the user presses a special key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool numberEntryPane::specialKeyEvent( int c ) {

	return ne.acceptOtherKey( c );
}
