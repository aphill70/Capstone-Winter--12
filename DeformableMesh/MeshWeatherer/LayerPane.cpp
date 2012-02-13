#include <GL\glut.h>

#include "DurabilityTool.h"
#include "LayerPane.h"


/**
 * Stores the last observed y value
 */
static double lastObservedYValue;

/**
 * The current camera zoom
 */
static double zoom = 2.0;

extern durabilityTool durability;

/**
 * ???
 * @return ???
 */
double layerPane::sx() const {

	return ( currentMx - x ) * 1.2 / ( double ) w - 0.2;
}


/**
 * ???
 * @return ???
 */
double layerPane::sy() const {

	return ( currentMy - y ) * 2.0 * zoom / ( double ) h + lastObservedYValue - zoom;
}


/**
 * ???
 * @param sx ???
 * @return ???
 */
double layerPane::px( double sx ) const {

	return ( sx + 0.2 ) * w / 1.2 + x;
}


/**
 * ???
 * @param sy ???
 * @return ???
 */
double layerPane::py( double sy ) const {

	return ( sy - lastObservedYValue + zoom ) * h / ( 2.0 * zoom ) + y;
}


/**
 * Called when the screen region is resized or moved
 * @param x The x-coordinate of the screen region
 * @param y The y-coordinate of the screen region
 * @param w The width of the screen region
 * @param h The height of the screen region
 */
void layerPane::reshape( int x, int y, int w, int h ) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


/**
 * Draws the screen region
 * @param results The step results to draw
 */
void layerPane::draw( const stepResults & results ) const {

	lastObservedYValue = results.midPoint[ 1 ];
	glViewport( x, y, w, h );
	glScissor( x, y, w, h );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glOrtho( -0.2, 1.0, results.midPoint[ 1 ] - zoom, results.midPoint[ 1 ] + zoom, -1.0, 1.0 );
	durability.draw( results.midPoint[ 1 ] - zoom, results.midPoint[ 1 ] + zoom, h );
}


/**
 * Accepts moust input
 * @param button Which mouse button was triggered
 * @param x The x-coordinate of the mouse in screen coordinates
 * @param y The y-coofdinate of the mouse in screen coordinates
 * @return The screen region the mouse is in
 */
screenRegion * layerPane::acceptMouse( int button, int x, int y ) {

	currentMx = x;
	currentMy = y;

	// Hit layer split side
	if ( sx() <= 0.0 ) {

		int nearestSplit = durability.nearestSplitTo( sy() );

		if ( nearestSplit == -1 || abs( py( durability.splitValue( nearestSplit ) ) - currentMy ) > 5.0 ) {

			// Add a split or change y noise
			if ( button == GLUT_LEFT_BUTTON ) {

				thingID = durability.addSplit( sy() );
				mouseMode = MOVE_SPLIT;
			}
			else {

				liveLayer = durability.layerAt( sy() );
				mouseMode = CHANGE_LAYER_NOISE;
			}
		}

		// Select or delete a split
		else {

			if ( button == GLUT_LEFT_BUTTON ) {

				thingID = nearestSplit;
				mouseMode = MOVE_SPLIT;
			}
			else {

				durability.removeSplit( nearestSplit );
				mouseMode = IGNORE_MOUSE;
			}
		}
	}
	else {

		liveLayer = durability.layerAt( sy() );
		thingID = liveLayer->base.nearestKeyTo( sy() );
		pair<double,double> temp;

		if ( thingID != -1 ) {

			temp = liveLayer->base.valueOf( thingID );
		}

		// Change nodes
		if ( button == GLUT_LEFT_BUTTON ) {

			if ( thingID == -1 || abs( py( temp.first ) - y ) > 5.0 ) {

				thingID = liveLayer->base.addKeyAt( sy(), sx() );

				mouseMode = MOVE_POINT;
			}
			else {

				liveLayer->base.moveKeyTo( thingID, sy(), sx() );

				mouseMode = MOVE_POINT;
			}
		}

		// Delete node or change noise
		else {

			if ( thingID == -1 || abs( py( temp.first ) - y ) > 5.0 || abs( px( temp.second ) - x ) > 5.0 ) {

				mouseMode = CHANGE_GRAIN;
			}
			else {

				liveLayer->base.removeKey( thingID );

				mouseMode = IGNORE_MOUSE;
			}
		}
	}

	currentButton = button;

	return this;
}


/**
 * Called when the mouse is dragged across the screen region
 * @param x The current x-coordinate of the mouse in screen coordinates
 * @param y The current y-coordinate of the mouse in screen coordinates
 */
void layerPane::dragMouse( int x, int y ) {

	int dx = x - currentMx;
	int dy = y - currentMy;

	if ( !dx && !dy ) {

		return;
	}

	currentMx = x;
	currentMy = y;

	switch ( mouseMode ) {

		case IGNORE_MOUSE:

			break;

		case MOVE_SPLIT:

			durability.moveSplitTo( thingID, sy() );
			break;

		case MOVE_POINT:

			liveLayer->base.moveKeyTo( thingID, sy(), sx() );

			break;

		case CHANGE_GRAIN:

			if ( dy >= 100 ) {

				dy = 99;
			}

			liveLayer->grainAmplitude += dx * 1.2 / w;

			if ( liveLayer->grainAmplitude < 0.0 ) {

				liveLayer->grainAmplitude = 0.0;
			}

			if ( liveLayer->grainAmplitude > 1.0 ) {

				liveLayer->grainAmplitude = 1.0;
			}

			liveLayer->grainFrequency *= ( 1.0 - dy * 0.01 );

			break;

		case CHANGE_LAYER_NOISE:

			if ( dy >= 100 ) {

				dy = 99;
			}

			liveLayer->yNoiseAmplitude += dx * 1.2 / w;

			if ( liveLayer->yNoiseAmplitude < 0.0 ) {

				liveLayer->yNoiseAmplitude = 0.0;
			}

			if ( liveLayer->yNoiseAmplitude > 1.0 ) {

				liveLayer->yNoiseAmplitude = 1.0;
			}

			liveLayer->yNoiseFrequency *= ( 1.0 - dy * 0.01 );

			break;

		default:

			break;
	}
}


/**
 * Recursively re-adjusts the screen region and its sub-regions
 * @return TRUE if the adjustment occurred, FALSE otherwise
 */
bool layerPane::readjust() {

	if ( durability.normalize() ) {

		recolorize();

		return true;
	}
	else {

		return false;
	}
}
