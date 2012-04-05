#pragma once

#include <cmath>

#include "IntTypes.h"


/**
 * A variation of Robert Jenkins' 96-bit mix function (effectively generates a random unsigned integer between 0x00000000 and 0xffffffff)
 * @param a First set of initialized random bits
 * @param b Second set of initialized random bits
 * @param c The input key
 * @return The hash result
 */
uint32_t mix( uint32_t a, uint32_t b, uint32_t c );


/**
 * Generates a random number in the range [0,1) using Robert Jenkins' 96-bit mix function
 * @param a First set of initialized random bits
 * @param b Second set of initialized random bits
 * @param c The input key
 * @return A random number in the range [0,1)
 */
double random( int a, int b, int c );


/**
 * Generates one-dimensional perlin noise about the given value
 * @param x The value to perturb
 * @param octaves The number of successive noise functions to add (default 0)
 * @param seed The seed for the random number generator (default 0)
 * @return The amount of noise to add to x
 */
double perlin1D( double x, int octaves = 0, int seed = 0 );


/**
 * Generates two-dimensional perlin noise about the given point
 * @param x The x-coordiante of the point to perturb
 * @param y The y-coordinate of the point to perturb
 * @param octaves The number of successive noise functions to add (default 0)
 * @param seed The seed for the random number generator (default 0)
 * @return The amount of noise to add to (x,y)
 */
double perlin2D( double x, double y, int octaves = 0, int seed = 0 );


/**
 * Generates three-dimensional perlin noise about the given point
 * @param x The x-coordiante of the point to perturb
 * @param y The y-coordinate of the point to perturb
 * @param z The z-coordinate of the point to perturb
 * @param octaves The number of successive noise functions to add (default 0)
 * @param seed The seed for the random number generator (default 0)
 * @return The amount of noise to add to (x,y,z)
 */
double perlin3D( double x, double y, double z, int octaves = 0, int seed = 0 );
