#include <GL\glut.h>

#include "DataPane.h"


/**
 * Default constructor
 */
dataPane::dataPane() : row( 0 ) {

	setMinEdgeLength( 0.01 );
	setMaxEdgeLength( 0.05 );
	setOverdrive( 1.0 );
	setNoisePower( 3.0 );
	setAirWaterRatio( 1.0 );
	setFluidScale( 0.0 );
}


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void dataPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void dataPane::draw( const stepResults & results ) const {

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
		glVertex2f( 4 + ( ne[ row ].cursor + ne[ row ].horizontalLength ) * 8, 3 + row * 15 );
		glVertex2f( 4 + ( ne[ row ].cursor + ne[ row ].horizontalLength ) * 8, 17 + row * 15 );
	glEnd();

	int i;
	int k;

	for ( i = numberEntryLength - 1; i >= 0; --i ) {

		glRasterPos2f( 4.0f, 4.0f + i * 15.0f );

		for ( k = 0; k < ne[ i ].getLength(); ++k ) {

			glutBitmapCharacter( GLUT_BITMAP_8_BY_13, ne[ i ].getCharacter( k ) );
		}
	}
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * dataPane::acceptMouse( int button, int x, int y ) {

	row = ( y - this->y - 4 ) / 15;

	if ( row < 0 ) {

		row = 0;
	}

	if ( row >= numberEntryLength ) {

		row = numberEntryLength - 1;
	}

	ne[ row ].setCursor( ( x - this->x + 4 ) / 8 );

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void dataPane::dragMouse( int x, int y ) {

	return;
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool dataPane::readjust() {

	return false;
}


/**
 * Called when the user presses a key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool dataPane::keyEvent( char c ) {

	bool result = ne[ row ].acceptKey( c );

	if ( result ) {

		if ( row != 0 && ne[ row ].getValue() < 0 ) {

			ne[ row ].setValue( 0.0 );
		}

		if ( row == 0 && ne[ 1 ].getValue() < ne[ 0 ].getValue() ) {

			ne[ 1 ].setValue( ne[ 0 ].getValue() );
		}

		if ( row == 1 && ne[ 0 ].getValue() > ne[ 1 ].getValue() ) {

			ne[ 0 ].setValue( ne[ 1 ].getValue() );
		}

		if ( getAirWaterRatio() > 1.0 ) {

			setAirWaterRatio( 1.0 );
		}

		if ( getAirWaterRatio() < 0.0 ) {

			setAirWaterRatio( 0.0 );
		}
	}

	return result;
}


/**
 * Called when the user presses a special key
 * @param c The key pressed
 * @return TRUE if the event was handled, FALSE otherwise
 */
bool dataPane::specialKeyEvent( int c ) {

	return ne[ row ].acceptOtherKey( c );
}


/**
 * Gets the value of the specified data
 * @param index The index of the data to get
 * @return The value of the specified data
 */
double dataPane::getValue( uint32_t index ) const {

	return ne[ index ].getValue();
}
