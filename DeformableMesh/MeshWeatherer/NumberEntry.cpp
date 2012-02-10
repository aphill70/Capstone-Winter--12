#include <GL\glut.h>

#include "NumberEntry.h"


/**
 * Handles keyboard input
 * @param c The character input
 * @return TRUE if the input was handled, FALSE otherwise
 */
bool numberEntry::acceptKey( char c ) {

	// Make sure the character is a valid input
	if ( ( ( c == '-' || c == '+' ) && ( cursor == 0 || text[ cursor - 1 ] == 'e' || text[ cursor - 1 ] == 'E' ) )
		|| ( c >= '0' && c <= '9' )
		|| ( c == '\b' || c == 127 ) // "delete" key...
		|| ( c == 'e' || c == 'E' || c == '.' ) )
	{
		int i;

		// Make sure it's OK to put an e in (for scientific notation)
		if ( c == 'e' || c == 'E' ) {

			// Invalid position for an "e" (scientific notation)
			if ( cursor == 0 || ( cursor == 1 && text[ 0 ] == '.' ) || ( cursor == 2 && text[ 1 ] == '.' && ( text[ 0 ] == '-' || text[ 0 ] == '+' ) ) ) {

				return false;
			}

			// Check for an existing "e" (already in scientific notation)
			for (i = 0; i < index; ++i ) {

				if ( text[ i ] == 'i' || text[ i ] == 'E' ) {

					return false;
				}
			}
		}

		// Check for an existing decimal point
		if ( c == '.' ) {

			for ( i = 0; i < index; ++i ) {

				if ( text[ i ] == '.' ) {

					return false;
				}
			}
		}

		if ( c == '\b' ) {

			if ( cursor == 0 ) {

				return false;
			}

			if ( text[ cursor - 1 ] == 'e' && cursor < index && text[ cursor ] < '0' ) {

				// If we have a -##e+## and the "e" is deleted, delete the "+", too
				for ( i = cursor + 1; i < index; ++i ) {

					text[ i - 2 ] = text[ i ];
				}

				index -= 2;
				--cursor;
			}
			else {

				for ( i = cursor; i < index; ++i ) {

					text[ i - 1 ] = text[ i ];
				}

				--index;
				--cursor;
			}

			return true;
		}

		// "delete" key
		if ( c == 127 ) {

			if ( cursor >= index ) {

				return false;
			}

			if ( text[ cursor ] == 'e' && cursor < index - 1 && text[ cursor + 1 ] < '0' ) {

				// If we have a -##e+## and the "e" is deleted, delete the "+", too
				for ( i = cursor + 1; i < index - 1; ++i ) {

					text[ i - 1 ] = text[ i + 1 ];
				}

				index -= 2;
			}
			else {

				for ( i = cursor; i < index - 1; ++i ) {

					text[ i ] = text[ i + 1 ];
				}

				--index;
			}

			return true;
		}

		// Can't have more than 256 characters (declared buffer limit)
		if ( index >= 256 ) {

			return false;
		}

		// Move other characters out of the way
		for ( i = index; i > cursor; --i ) {

			text[ i ] = text[ i - 1 ];
		}

		// Insert the character
		text[ cursor ] = c;
		++index;
		++cursor;

		return true;
	}

	return false;
}


/**
 * Handles special keyboard input
 * @param c The character input
 * @return TRUE if the key input was handled, FALSE otherwise
 */
bool numberEntry::acceptOtherKey( int c ) {

	// Only deal with valid key inputs
	if ( c == GLUT_KEY_LEFT || c == GLUT_KEY_RIGHT || c == GLUT_KEY_HOME || c == GLUT_KEY_END ) {

		if ( c == GLUT_KEY_LEFT && cursor > 0 ) {

			--cursor;
		}

		if ( c == GLUT_KEY_RIGHT && cursor < index ) {

			++cursor;
		}

		if ( c == GLUT_KEY_HOME ) {

			cursor = 0;
		}

		if ( c == GLUT_KEY_END ) {

			cursor = index;
		}

		return true;
	}

	return false;
}


/**
 * Gets the minimum edge length specified by the user
 * @return The minimum edge length specified by the user
 */
double getMinEdgeLength() {

	return ne[ 0 ].getValue();
}


/**
 * Gets the maximum edge length specified by the user
 * @return The maximum edge length specified by the user
 */
double getMaxEdgeLength() {

	return ne[ 1 ].getValue();
}


/**
 * Gets the overdrive value specified by the user
 * @return The overdrive value specified by the user
 */
double getOverdrive() {

	return ne[ 2 ].getValue();
}


/**
 * Gets the noise power specified by the user
 * @return The noise power specified by the user
 */
double getNoisePower() {

	return ne[ 3 ].getValue();
}


/**
 * Gets the air-to-water ratio specified by the user
 * @return The air-to-water ratio specified by the user
 */
double getAirWaterRatio() {

	return ne[ 4 ].getValue();
}


/**
 * Gets the fluid scale specified by the user
 * @return The fluid scale specified by the user
 */
double getFluidScale() {

	return ne[ 5 ].getValue();
}


/**
 * Sets the minimum edge length
 * @param x The new minimum edge length
 */
void setMinEdgeLength( double x ) {

	ne[ 0 ].setValue( x );
}


/**
 * Sets the maximum edge length
 * @param x The new maximum edge length
 */
void setMaxEdgeLength( double x ) {

	ne[ 1 ].setValue( x );
}


/**
 * Sets the overdrive value
 * @param x The new overdrive value
 */
void setOverdrive( double x ) {

	ne[ 2 ].setValue( x );
}


/**
 * Sets the noise power
 * @param x The new noise power
 */
void setNoisePower( double x ) {

	ne[ 3 ].setValue( x );
}


/**
 * Sets the air-to-water ratio
 * @param x The new air-to-water ratio
 */
void setAirWaterRatio( double x ) {

	ne[ 4 ].setValue( x );
}


/**
 * Sets the fluid scale
  8@param x The new fluid scale
 */
void setFluidScale( double x ) {

	ne[ 5 ].setValue( x );
}
