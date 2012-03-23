#include "PerlinNoise.h"

/**
 * Used to scale random number from range [0x00000000,0xffffffff] to range [0.0,1.0)
 */
#define RAND_MAX_DOUBLE 4294967296.0


/**
 * Rotates the bits of x as part of Robert Jenkins' 96-bit mix function (compiles down to ror or rol instruction with reasonably optimizing compiler)
 * @param x The bits to rotate
 * @param k The amount to rotate the bits
 * @return A rotated copy of x
 */
inline uint32_t rotate( const uint32_t & x, unsigned char k ) {

	return ( x << k ) | ( x >> ( 32 - k ) );
}


/**
 * A variation of Robert Jenkins' 96-bit mix function (effectively generates a random unsigned integer between 0x00000000 and 0xffffffff)
 * @param a First set of initialized random bits
 * @param b Second set of initialized random bits
 * @param c The input key
 * @return The hash result
 */
uint32_t mix( uint32_t a, uint32_t b, uint32_t c ) {

	c ^= b;
	b = rotate( b, 14 );
	c -= b;

	a ^= c;
	c = rotate( c, 11 );
	a -= c;

	b ^= a;
	a = rotate( a, 25 );
	b -= a;

	c ^= b;
	b = rotate( b, 16 );
	c -= b;

	a ^= c;
	c = rotate( c, 4 );
	a -= c;

	b ^= a;
	a = rotate( a, 14 );
	b -= a;

	c ^= b;
	b = rotate( b, 24 );
	c -= b;

	return c;
}


/**
 * Generates a random number in the range [0,1) using Robert Jenkins' 96-bit mix function
 * @param a First set of initialized random bits
 * @param b Second set of initialized random bits
 * @param c The input key
 * @return A random number in the range [0,1)
 */
double random( int a, int b, int c ) {

	return mix( *( ( unsigned int * ) &a ), *( ( unsigned int * ) &b ), *( ( unsigned int * ) &c ) ) / RAND_MAX_DOUBLE;
}


/**
 * Computes the mathimatical floor of the given number as an int
 * @param x The number to compute the floor for
 * @return The floor of the given number
 */
inline int ifloor( double x ) {

	return ( int ) floor( x );
}


/**
 * The Perlin noise function in 1D
 * @param x The frequency
 * @param seed The seed for the random number generation (default is 0)
 * @return The noise to add
 */
inline double random1D( double x, int seed ) {

	int ix = ifloor( x );
	int iy = seed;
	int iz = 0;

	x -= ix;

	return random( ix, iy, iz ) * ( 1 - x ) + random( 1 + ix, iy, iz ) * x;
}


/**
 * The Perlin noise function in 2D
 * @param x The frequency in x
 * @param y The frequency in y
 * @param seed The seed for the random number generation (default is 0)
 * @return The noise to add
 */
inline double random2D( double x, double y, int seed ) {

	int ix = ifloor( x );
	int iy = ifloor( y );
	int iz = seed;

	x -= ix;
	y -= iy;

	return random( ix, iy, iz ) * ( 1 - x ) * ( 1 - y ) +
		random( ix, 1 + iy, iz ) * ( 1 - x ) * ( y ) +
		random( 1 + ix, iy, iz ) * ( x ) * ( 1 - y ) +
		random( 1 + ix, 1 + iy, iz ) * ( x ) * ( y );
}


/**
 * The Perlin noise function in 3D
 * @param x The frequency in x
 * @param y The frequency in y
 * @param z The frequency in z
 * @param seed The seed for the random number generation (default is 0)
 * @return The noise to add
 */
inline double random3D( double x, double y, double z, int seed ) {

	z += ( seed * 53813 );
	int ix = ifloor( x );
	int iy = ifloor( y );
	int iz = ifloor( z );

	x -= ix;
	y -= iy;
	z -= iz;

	return random( ix, iy, iz ) * ( 1 - x ) * ( 1 - y ) * ( 1 - z ) +
		random( ix, iy, 1 + iz ) * ( 1 - x ) * ( 1 - y ) * ( z ) +
		random( ix, 1 + iy, iz ) * ( 1 - x ) * ( y ) * ( 1 - z ) +
		random( ix, 1 + iy, 1 + iz ) * ( 1 - x ) * ( y ) * ( z ) +
		random( 1 + ix, iy, iz ) * ( x ) * ( 1 - y ) * ( 1 - z ) +
		random( 1 + ix, iy, 1 + iz ) * ( x ) * ( 1 - y ) * ( z ) +
		random( 1 + ix, 1 + iy, iz ) * ( x ) * ( y ) * ( 1 - z ) +
		random( 1 + ix, 1 + iy, 1 + iz ) * ( x ) * ( y ) * ( z );
}


/**
 * Generates one-dimensional perlin noise about the given value
 * @param x The value to perturb
 * @param octaves The number of successive noise functions to add (default 0)
 * @param seed The seed for the random number generator (default 0)
 * @return The amount of noise to add to x
 */
double perlin1D( double x, int octaves, int seed ) {

	double result = random1D( x, seed );
	double chop = 1.0;

	for ( int i = 0; i < octaves; ++i ) {

		chop /= 2.0;
		x *= 2.0;
		++seed;
		result += chop * random1D( x, seed );
	}

	return result;
}


/**
 * Generates two-dimensional perlin noise about the given point
 * @param x The x-coordiante of the point to perturb
 * @param y The y-coordinate of the point to perturb
 * @param octaves The number of successive noise functions to add (default 0)
 * @param seed The seed for the random number generator (default 0)
 * @return The amount of noise to add to (x,y)
 */
double perlin2D( double x, double y, int octaves, int seed ) {

	double result = random2D( x, y, seed );
	double chop = 1.0;

	for ( int i = 0; i < octaves; ++i ) {

		chop /= 2.0;
		x *= 2.0;
		y *= 2.0;
		++seed;
		result += chop * random2D( x, y, seed );
	}

	return result;
}


/**
 * Generates three-dimensional perlin noise about the given point
 * @param x The x-coordiante of the point to perturb
 * @param y The y-coordinate of the point to perturb
 * @param z The z-coordinate of the point to perturb
 * @param octaves The number of successive noise functions to add (default 0)
 * @param seed The seed for the random number generator (default 0)
 * @return The amount of noise to add to (x,y,z)
 */
double perlin3D( double x, double y, double z, int octaves, int seed ) {

	double result = random3D( x, y, z, seed );
	double chop = 1.0;

	for ( int i = 0; i < octaves; ++i ) {

		chop /= 2.0;
		x *= 2.0;
		y *= 2.0;
		z *= 2.0;
		++seed;
		result += chop * random3D( x, y, z, seed );
	}

	return result;
}
