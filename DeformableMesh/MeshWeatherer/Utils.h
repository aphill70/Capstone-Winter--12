#pragma once

#include <GL\glut.h>
#include "CGAL_typedefs.h"


/**
 * Computes the absolute value of the data specified
 */
#define MACRO_ABS(a) ((a) < 0 ? -(a) : (a))


/**
 * Clamps the value given to the range [0,1]
 */
#define MACRO_CLAMP(a) ((a) < 0 ? 0 : ((a) > 1 ? 1 : (a)))


/**
 * For visualizing softness, importance, curvature, or other range variable
 * @param rgb Stores the RGB value
 * @param hue The hue to convert to RGB
 */
template <typename N> void hueToRGB( N * rgb, double hue ) {

	rgb[ 0 ] = MACRO_CLAMP( MACRO_ABS( hue * 6.0 - 3.0 ) - 1.0 );
	rgb[ 1 ] = MACRO_CLAMP( 2.0 - MACRO_ABS( hue * 6.0 - 2.0 ) );
	rgb[ 2 ] = MACRO_CLAMP( 2.0 - MACRO_ABS( hue * 6.0 - 4.0 ) );
}



/**
 * A double-buffered, locked method that only updates between simulation passes (provided by the user)
 * @param x The x-coordinate of the point to measure
 * @param y The y-coordinate of the point to measure
 * @param z The z-coordinate of the point to measure
 * @return The softness at the given point
 */
double softness( double x, double y, double z );


/**
 * Computes the softness at the given point if it is live
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the given point if it is live
 */
double liveSoftness( double x, double y, double z );


/**
 * Computes the square of the given number
 * @param value The number to square
 * @return The square of the number
 */
template <typename T> inline T square( const T & value ) {

	return value * value;
}


/**
 * Computes the cube of the given number
 * @param value The number to cube
 * @return The cube of the given number
 */
template <typename T> inline T cube( const T & value ) {

	return value * value * value;
}


/**
 * The default function to call for checking "live" and "startsFilled"
 * @param x The x-coordinate of the point to check
 * @param y The y-coordinate of the point to check
 * @param z The z-coordinate of the point to check
 * @return TRUE always
 */
inline bool trueXYZ( double x, double y, double z ) {

	return true;
}


/**
 * Sets "io" to the max of "io" and "in"
 * @param io The first item to compare
 * @param in The second item to compare
 */
template <typename N> inline void SETMAX( N & io, const N & in ) {

	if ( io < in ) {

		io = in;
	}
}


/**
 * Computes the dot product of a and b
 * @param a The first vector
 * @param b The second vector
 * @return The dot product of the two vectors
 */
inline double dot( const Vector & a, const Vector & b ) {

	return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}


/**
 * Computes the square of the given number
 * @param x The number to square
 * @return The square of the given number
 */
inline double square( const double & x ) {

	return x * x;
}


/**
 * Computes the determinant of the given 3x3 matrix
 * @param m00 The element in the first row, first column
 * @param m01 The element in the first row, second column
 * @param m02 The element in the first row, third column
 * @param m10 The element in the second row, first column
 * @param m11 The element in the second row, second column
 * @param m12 The element in the second row, third column
 * @param m20 The element in the third row, first column
 * @param m21 The element in the third row, second column
 * @param m22 The element in the third row, third column
 */
inline double determinant( double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22 ) {

	return ( m00 * m11 * m22 ) + ( m01 * m12 * m20 ) + ( m02 * m10 * m21 ) - ( m02 * m11 * m20 ) - ( m01 * m10 * m22 ) - ( m00 * m12 * m21 );
}


/**
 * Computes the determinant of the given 4x4 matrix
 * @param m00 The element in the first row, first column
 * @param m01 The element in the first row, second column
 * @param m02 The element in the first row, third column
 * @param m03 The element in the first row, fourth column
 * @param m10 The element in the second row, first column
 * @param m11 The element in the second row, second column
 * @param m12 The element in the second row, third column
 * @param m13 The element in the second row, fourth column
 * @param m20 The element in the third row, first column
 * @param m21 The element in the third row, second column
 * @param m22 The element in the third row, third column
 * @param m23 The element in the third row, fourth column
 * @param m30 The element in the fourth row, first column
 * @param m31 The element in the fourth row, second column
 * @param m32 The element in the fourth row, third column
 * @param m33 The element in the fourth row, fourth column
 */
inline double determinant( double m00, double m01, double m02, double m03, double m10, double m11, double m12, double m13, double m20, double m21, double m22, double m23, double m30, double m31, double m32, double m33 ) {

	return m00 * determinant( m11, m12, m13, m21, m22, m23, m31, m32, m33 )
		- m01 * determinant( m10, m12, m13, m20, m22, m23, m30, m32, m33 )
		+ m02 * determinant( m10, m11, m13, m20, m21, m23, m30, m31, m33 )
		- m03 * determinant( m10, m11, m12, m20, m21, m22, m30, m31, m32 );
}
