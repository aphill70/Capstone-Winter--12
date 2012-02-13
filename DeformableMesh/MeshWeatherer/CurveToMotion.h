#pragma once


/**
 * Hyperbolic interpolation alpha + (c + sqrt(c*c + beta))
 *	alpha is rate in pit regions
 *	beta is range of regions treated as flat
 * if beta < 0 and alpha == 0, operator()(curvature) returns 1.0
 * if beta < 0 and alpha != 0, operator()(curvature) returns alpha * curvature
 */
struct curveToMotion {

protected:

	//------------
	// MEMBER DATA
	//------------

	/**
	 * Alpha
	 */
	double a;

	/**
	 * Beta
	 */
	double b;
	
	
public:

	//----------
	// FUNCTIONS
	//----------
	
	/**
	 * Sets the alpha and beta values
	 * @param alpha The new alpha value
	 * @param beta The new beta value
	 */
	void set( double alpha, double beta );
	
	
	//----------
	// OPERATORS
	//----------
	
	/**
	 * Function operator
	 * @param curvature The curvature of the mesh
	 * @return The motion associated with the given curvature:
	 *	if beta < 0 and alpha == 0, return 1.0
	 *	if beta < 0 and alpha != 0, return alpha * curvature
	 */
	double operator () ( double curvature ) const;
};
