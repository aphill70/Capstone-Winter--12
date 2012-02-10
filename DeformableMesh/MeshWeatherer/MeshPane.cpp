#include <GL\glut.h>

#include "MeshPane.h"


/**
 * The current camera zoom
 */
extern double zoom = 2.0;


/**
 * Default constructor
 */
meshPane::meshPane() : theta( 0.0 ), phi( 0.0 ), scale( 1.0 / zoom ) {

	return;
}


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void meshPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void meshPane::draw( const stepResults & results ) const {

	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glScalef( scale * h / w, scale, -0.25 * scale ); // In projection, not modelview, so normals don't change
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		gluLookAt(
			results.midPoint[ 0 ], results.midPoint[ 1 ], results.midPoint[ 2 ],
			results.midPoint[ 0 ] + sin( theta ) * cos( phi ), results.midPoint[ 1 ] + sin( phi ), results.midPoint[ 2 ] - cos( theta ) * cos( phi ),
			0, 1, 0 );
	glEnable( GL_LIGHTING );
	drawMesh();
	glDisable( GL_LIGHTING );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * meshPane::acceptMouse( int button, int x, int y ) {

	currentMx = x;
	currentMy = y;
	currentButton = button;

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void meshPane::dragMouse( int x, int y ) {

	int dx = x - currentMx;
	int dy = y - currentMy;

	if ( !dx && !dy ) {

		return;
	}

	currentMx = x;
	currentMy = y;

	switch ( currentButton ) {

		case GLUT_LEFT_BUTTON:

			theta += dx * 0.01;
			phi += dy * 0.01;
			break;

		default:

			double offset = dy;

			if ( h ) {

				offset /= h;
			}

			offset += 1.0;

			if ( offset > 0.0 ) {

				scale *= offset;
			}

			zoom = 1.0 / scale;
			break;
	}
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool meshPane::readjust() {

	return false;
}
