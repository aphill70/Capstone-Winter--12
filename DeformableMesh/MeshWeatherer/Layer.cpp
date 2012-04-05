#include "NumberEntry.h"
#include "Layer.h"
#include "PerlinNoise.h"


/**
 * Adjusts the noise by clamping it to the range [0,1] and then raising it to the user-specified power
 * @param noise The noise value to adjust
 * @return The adjusted noise value
 */
double fixNoise( double noise ) {

	if ( noise < 0.0 ) {

		noise = 0.0;
	}

	if ( noise > 1.0 ) {

		noise = 1.0;
	}

	return pow( noise, getNoisePower() );
}


/**
 * Evaluates the layer at the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param seed The seed value for random number generator (default 0)
 * @param userGrain Whether or not to use the grain (default TRUE)
 * @return The durability of the layer at the given point
 */
double layer::evaluate( double x, double y, double z, uint32_t seed, bool useGrain ) const {

	if ( useGrain ) {

		return base.evaluate( y ) * ( 1.0 - fixNoise( perlin1D( y * yNoiseFrequency, 4, seed ) ) * yNoiseAmplitude ) * ( 1.0 - fixNoise( perlin3D( x * grainFrequency, y * grainFrequency, z * grainFrequency, 4, seed + 0x005a5a5a ) ) * grainAmplitude );
	}
	else {

		return base.evaluate( y ) * ( 1.0 - fixNoise( perlin1D( y * yNoiseFrequency, 4, seed ) ) * yNoiseAmplitude );
	}
}
