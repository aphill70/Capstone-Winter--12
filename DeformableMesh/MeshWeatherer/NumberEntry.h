#pragma once

#include <cstring>
#include <cstdlib>
#include <cstdio>

/**
 * A numerical entry in the settings box
 */
struct numberEntry {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The text value for the number
	 */
	mutable char text[ 256 ];

	/**
	 * The title to display next to the number (the label)
	 */
	const char * heading;

	/**
	 * The horizontal length of the number (the length of "text")
	 */
	const int horizontalLength;

	/**
	 * The index of the last digit in the number in the text array
	 */
	int index;

	/**
	 * The position of the cursor in the text array
	 */
	int cursor;

	/**
	 * The x-coordinate of the text in screen space
	 */
	int x;

	/**
	 * The y-coordinate of the text in screen space
	 */
	int y;

	/**
	 * The width of the text in pixels
	 */
	int w;

	/**
	 * The height of the text in pixels
	 */
	int h;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Full constructor
	 * @param temp The label for the number
	 */
	inline numberEntry( const char * temp = 0 ) : heading( temp ), horizontalLength( strlen( temp ) ), index( 0 ), cursor( 0 ) {

		return;
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Sets the location of the cursor in the text array
	 * @param position The new location of the cursor in the text array
	 */
	inline void setCursor( int position ) {

		position -= horizontalLength;

		if ( position < 0 ) {

			position = 0;
		}

		if ( position > index ) {

			position = index;
		}
	}


	/**
	 * Handles keyboard input
	 * @param c The character input
	 * @return TRUE if the input was handled, FALSE otherwise
	 */
	bool acceptKey( char c );


	/**
	 * Handles special keyboard input
	 * @param c The character input
	 * @return TRUE if the key input was handled, FALSE otherwise
	 */
	bool acceptOtherKey( int c );


	/**
	 * Gets the value of the textual number as a double
	 * @return The numerical value of the textual number
	 */
	inline double getValue() const {

		text[ index ] = '\0';
		
		return atof( text );
	}


	/**
	 * Sets the value of the number
	 * @param x The new value of the number
	 */
	inline void setValue( double x ) {

		sprintf_s( text, 256, "%g", x );
		index = strlen( text );
		cursor = index;
	}


	/**
	 * Gets the length of the number (the number of digits)
	 * @return The number of characters in the number
	 */
	inline int getLength() const {

		return horizontalLength + index;
	}


	/**
	 * Gets the character at the specified index
	 * @param i The index of the character to get
	 * @return The character at the specified index
	 */
	inline char getCharacter( int i ) const {

		if ( i < horizontalLength ) {

			return heading[ i ];
		}
		else {

			return text[ i - horizontalLength ];
		}
	}
};


/**
 * The number of result labels
 */
static const int numberEntryLength = 6;


/**
 * The list of result labels
 */
static numberEntry ne[ numberEntryLength ] = {
	"Min Edge Length: ",
	"Max Edge Length: ",
	"Overdrive:       ",
	"Noise Power:     ",
	"Air/Water Ratio: ",
	"Fluid Scale:     "
};


/**
 * Gets the minimum edge length specified by the user
 * @return The minimum edge length specified by the user
 */
double getMinEdgeLength();


/**
 * Gets the maximum edge length specified by the user
 * @return The maximum edge length specified by the user
 */
double getMaxEdgeLength();


/**
 * Gets the overdrive value specified by the user
 * @return The overdrive value specified by the user
 */
double getOverdrive();


/**
 * Gets the noise power specified by the user
 * @return The noise power specified by the user
 */
double getNoisePower();


/**
 * Gets the air-to-water ratio specified by the user
 * @return The air-to-water ratio specified by the user
 */
double getAirWaterRatio();


/**
 * Gets the fluid scale specified by the user
 * @return The fluid scale specified by the user
 */
double getFluidScale();


/**
 * Sets the minimum edge length
 * @param x The new minimum edge length
 */
void setMinEdgeLength( double x );


/**
 * Sets the maximum edge length
 * @param x The new maximum edge length
 */
void setMaxEdgeLength( double x );


/**
 * Sets the overdrive value
 * @param x The new overdrive value
 */
void setOverdrive( double x );


/**
 * Sets the noise power
 * @param x The new noise power
 */
void setNoisePower( double x );


/**
 * Sets the air-to-water ratio
 * @param x The new air-to-water ratio
 */
void setAirWaterRatio( double x );


/**
 * Sets the fluid scale
  8@param x The new fluid scale
 */
void setFluidScale( double x );
