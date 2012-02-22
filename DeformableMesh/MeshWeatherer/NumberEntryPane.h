#pragma once

#include "NumberEntry.h"
#include "ScreenRegion.h"


/**
 * A screen pane that displays the number entries
 */
struct numberEntryPane : public screenRegion {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The number entry data to display
	 */
	numberEntry ne;

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


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Label constructor
	 * @param temp The label for the number entry data
	 */
	inline numberEntryPane( const char * temp = 0 ) : ne( temp ) {

		return;
	}


	/**
	 * Destructor
	 */
	virtual inline ~numberEntryPane() {

		return;
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


	/**
	 * Called when the user presses a key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool keyEvent( char c );


	/**
	 * Called when the user presses a special key
	 * @param c The key pressed
	 * @return TRUE if the event was handled, FALSE otherwise
	 */
	virtual bool specialKeyEvent( int c );


	/**
	 * Gets the value of the number entry data
	 * @return The value of the number entry data
	 */
	inline double getValue() const {

		return ne.getValue();
	}
};