#pragma once

#include "ScreenRegion.h"


/**
 * A vertical split pane (splits the region into top and bottom sub-panes)
 */
struct vSplitPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The bottom subregion
	 */
	screenRegion * bottom;

	/**
	 * The top subregion
	 */
	screenRegion * top;

	/**
	 * The x-coordinate of the region in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the region in screen space
	 */
	int y;

	/**
	 * The width of the region in pixels
	 */
	int w;

	/**
	 * The height of the region in pixels
	 */
	int h;

	/**
	 * The size of the divider between sub-regions
	 */
	int gutter;

	/**
	 * The relative location of the split (between 0.0 and 1.0)
	 */
	double split;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Full constructor
	 * @param b The left subregion
	 * @param t The right subregion
	 * @param s The relative location of the split (default is 0.5)
	 */
	inline vSplitPane( screenRegion * b, screenRegion * t, double s = 0.5 ) : bottom( b ), top( t ), gutter( 10 ), split( s ) {

		return;
	}


	/**
	 * Destructor
	 */
	virtual inline ~vSplitPane() {

		delete bottom;
		delete top;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Called when the screen region is resized or moved
	 * @param x The x-coordinate of the screen region
	 * @param y The y-coordinate of the screen region
	 * @param w The width of the screen region
	 * @param h The height of the screen region
	 */
	virtual void reshape( int x, int y, int w, int h );


	/**
	 * Draws the screen region
	 * @param results The step results to draw
	 */
	virtual void draw( const stepResults & results ) const;


	/**
	 * Accepts moust input
	 * @param button Which mouse button was triggered
	 * @param x The x-coordinate of the mouse in screen coordinates
	 * @param y The y-coofdinate of the mouse in screen coordinates
	 * @return The screen region the mouse is in
	 */
	virtual screenRegion * acceptMouse( int button, int x, int y );


	/**
	 * Called when the mouse is dragged across the screen region
	 * @param x The current x-coordinate of the mouse in screen coordinates
	 * @param y The current y-coordinate of the mouse in screen coordinates
	 */
	virtual void dragMouse( int x, int y );


	/**
	 * Recursively re-adjusts the screen region and its sub-regions
	 * @return TRUE if the adjustment occurred, FALSE otherwise
	 */
	virtual bool readjust();
};
