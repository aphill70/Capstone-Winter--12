#include <cmath>

#include "CurveToMotion.h"
#include "Utils.h"


/**
 * Sets the alpha and beta values
 * @param alpha The new alpha value
 * @param beta The new beta value
 */
void curveToMotion::set( double alpha, double beta ) {

	a = alpha;
	b = beta;
}


/**
 * Function operator
 * @param curvature The curvature of the mesh
 * @return The motion associated with the given curvature:
 *	if beta < 0 and alpha == 0, return 1.0
 *	if beta < 0 and alpha != 0, return alpha * curvature
 */
double curveToMotion::operator () ( double curvature ) const {

	if ( b < 0.0 ) {

		if ( a == 0.0 ) {

			return 1.0;
		}
		else {

			return a * curvature;
		}
	}
	else {

		return ( a + ( curvature + sqrt( square( curvature ) + b ) ) ) / 2.0;
	}
}
