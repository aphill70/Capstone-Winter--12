#include "EulerFluid.h"
#include "Utils.h"

#include <cstdio>
#include <iostream>
#include <limits>
#include <CGAL/OpenNL/linear_solver.h>

using namespace std;


//---------
// TYPEDEFS
//---------

typedef OpenNL::SparseMatrix<double> ONLM;
typedef OpenNL::FullVector<double> ONLV;
typedef OpenNL::Jacobi_Preconditioner<double> ONLJ;
typedef OpenNL::Solver_preconditioned_CG<ONLM, ONLJ, ONLV> ONLP;
typedef OpenNL::Solver_CG<ONLM, ONLV> ONLS;

const uint FluidGrid3D::BAD_INDEX = 0xffffffff;


/**
 * Solves a system of equations represented by a matrix
 * @param A The matrix to solve
 * @param p ???
 * @param b ???
 * @param epsilon Used to determine tolerance range for solution (default 1.0e-6)
 */
void solveSystem( const ONLM & A, ONLV & p, const ONLV & b, double epsilon = 1.0e-6 ) {

	static ONLP psolve;
	static ONLS solve;
	psolve.set_epsilon( epsilon );
	solve.set_epsilon( epsilon );
	ONLJ C( A, 1.5 ); // 1.5 is a magic number...
	p = b;

	if ( psolve.solve( A, C, b, p ) ) {

		return;
	}

	p = b;
	solve.solve( A, b, p );
}


/**
 * Checks a condition and displays an error message if appropriate
 * @param condition The condition to check
 * @param errorCount Stores the number of errors found
 * @param str The format string to display if the condition is false
 */
void checkOrMessage( bool condition, int & errorCount, const char * str, ... ) {

	// Process the format string
	va_list formattedString;
	va_start( formattedString, str );

	// If false, display the error message
	if ( !condition ) {

		++errorCount;
		vprintf( str, formattedString );
	}

	// Release the formatted string
	va_end( formattedString );
}


/**
 * Determines the indices and weights of the cells adjacent to the cell containing the given point (uses clamping as needed)
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param indices Stores the indices of the cells adjacent to the cell containing the point
 * @param weights Stores the weights of the cells adjacent to the cell containing the point
 */
void FluidGrid3D::cellsAndWeights( double x, double y, double z, uint indices[ 8 ], double weights[ 8 ] ) const {

	int ix = ( int ) floor( x );
	x -= ix;

	int iy = ( int ) floor( y );
	y -= iy;

	int iz = ( int ) floor( z );
	z -= iz;

	indices[ 0 ] = clampIndex( ix, iy, iz );
	weights[ 0 ] = ( 1.0 - x ) * ( 1.0 - y ) * ( 1.0 - z );
	
	indices[ 1 ] = clampIndex( ix + 1, iy, iz );
	weights[ 1 ] = ( x ) * ( 1.0 - y ) * ( 1.0 - z );
	
	indices[ 2 ] = clampIndex( ix, iy + 1, iz );
	weights[ 2 ] = ( 1.0 - x ) * ( y ) * ( 1.0 - z );
	
	indices[ 3 ] = clampIndex( ix + 1, iy + 1, iz );
	weights[ 3 ] = ( x ) * ( y ) * ( 1.0 - z );
	
	indices[ 4 ] = clampIndex( ix, iy, iz + 1 );
	weights[ 4 ] = ( 1.0 - x ) * ( 1.0 - y ) * ( z );
	
	indices[ 5 ] = clampIndex( ix + 1, iy, iz + 1 );
	weights[ 5 ] = ( x ) * ( 1.0 - y ) * ( z );
	
	indices[ 6 ] = clampIndex( ix, iy + 1, iz + 1 );
	weights[ 6 ] = ( 1.0 - x ) * ( y ) * ( z );
	
	indices[ 7 ] = clampIndex( ix + 1, iy + 1, iz + 1 );
	weights[ 7 ] = ( x ) * ( y ) * ( z );
}


/**
 * Determines the indices and weights of the cells adjacent to the cell containing the given point (bounds check failure results in BAD_INDEX value)
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param indices Stores the indices of the cells adjacent to the cell containing the point
 * @param weights Stores the weights of the cells adjacent to the cell containing the point
 */
void FluidGrid3D::cellsAndWeightsNullable( double x, double y, double z, uint indices[ 8 ], double weights[ 8 ] ) const {

	int ix = ( int ) floor( x );
	x -= ix;
	
	int iy = ( int ) floor( y );
	y -= iy;
	
	int iz = ( int ) floor( z );
	z -= iz;
	
	indices[ 0 ] = index( ix, iy, iz );
	weights[ 0 ] = ( 1.0 - x ) * ( 1.0 - y ) * ( 1.0 - z );
	
	indices[ 1 ] = index( ix + 1, iy, iz );
	weights[ 1 ] = ( x ) * ( 1.0 - y ) * ( 1.0 - z );
	
	indices[ 2 ] = index( ix, iy + 1, iz );
	weights[ 2 ] = ( 1.0 - x ) * ( y ) * ( 1.0 - z );
	
	indices[ 3 ] = index( ix + 1, iy + 1, iz );
	weights[ 3 ] = ( x ) * ( y ) * ( 1.0 - z );
	
	indices[ 4 ] = index( ix, iy, iz + 1 );
	weights[ 4 ] = ( 1.0 - x ) * ( 1.0 - y ) * ( z );
	
	indices[ 5 ] = index( ix + 1, iy, iz + 1 );
	weights[ 5 ] = ( x ) * ( 1.0 - y ) * ( z );
	
	indices[ 6 ] = index( ix, iy + 1, iz + 1 );
	weights[ 6 ] = ( 1.0 - x ) * ( y ) * ( z );
	
	indices[ 7 ] = index( ix + 1, iy + 1, iz + 1 );
	weights[ 7 ] = ( x ) * ( y ) * ( z );
}


/**
 * Determines the indices and weights of the cells adjacent to the cell containing the given point (uses wraping as needed)
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param indices Stores the indices of the cells adjacent to the cell containing the point
 * @param weights Stores the weights of the cells adjacent to the cell containing the point
 */
void FluidGrid3D::cellsAndWeightsWrap( double x, double y, double z, uint indices[ 8 ], double weights[ 8 ] ) const {

	int ix = ( int ) floor( x );
	x -= ix;

	int iy = ( int ) floor( y );
	y -= iy;

	int iz = ( int ) floor( z );
	z -= iz;

	indices[ 0 ] = wrapIndex( ix, iy, iz );
	weights[ 0 ] = ( 1.0 - x ) * ( 1.0 - y ) * ( 1.0 - z );
	
	indices[ 1 ] = wrapIndex( ix + 1, iy, iz );
	weights[ 1 ] = ( x ) * ( 1.0 - y ) * ( 1.0 - z );
	
	indices[ 2 ] = wrapIndex( ix, iy + 1, iz );
	weights[ 2 ] = ( 1.0 - x ) * ( y ) * ( 1.0 - z );
	
	indices[ 3 ] = wrapIndex( ix + 1, iy + 1, iz );
	weights[ 3 ] = ( x ) * ( y ) * ( 1.0 - z );
	
	indices[ 4 ] = wrapIndex( ix, iy, iz + 1 );
	weights[ 4 ] = ( 1.0 - x ) * ( 1.0 - y ) * ( z );
	
	indices[ 5 ] = wrapIndex( ix + 1, iy, iz + 1 );
	weights[ 5 ] = ( x ) * ( 1.0 - y ) * ( z );
	
	indices[ 6 ] = wrapIndex( ix, iy + 1, iz + 1 );
	weights[ 6 ] = ( 1.0 - x ) * ( y ) * ( z );
	
	indices[ 7 ] = wrapIndex( ix + 1, iy + 1, iz + 1 );
	weights[ 7 ] = ( x ) * ( y ) * ( z );
}


/**
 * Dimension constructor
 * @param w The width of the grid
 * @param h The height of the grid
 * @param d The depth of the grid
 */
FluidGrid3D::FluidGrid3D( uint w, uint h, uint d ) {

	dimensions[ 0 ] = w;
	dimensions[ 1 ] = h;
	dimensions[ 2 ] = d;
	length = w * h * d;
	data = ( CellData * ) malloc( sizeof( CellData ) * length );

	uint i;
	uint direction;

	for ( i = 0; i < length; ++i ) {

		for ( direction = 0; direction < 3; ++direction ) {

			data[ i ].flow[ direction ] = 0.0;
			data[ i ].faceType[ direction ] = OPEN_FACE;
			data[ i ].temp[ direction ] = 0.0;
		}

		data[ i ].rho = 0.0;
		data[ i ].dirt = 0.0;
		data[ i ].userInt = 0;
	}
}


/**
 * Destructor
 */
FluidGrid3D::~FluidGrid3D() {

	free( data );
	data = 0;
	length = 0;
	dimensions[ 0 ] = 0;
	dimensions[ 1 ] = 0;
	dimensions[ 2 ] = 0;
}


/**
 * Post increment operator
 * @return A reference to the iterator after incrementing the index
 */
FluidGrid3D::iterator & FluidGrid3D::iterator::operator ++ () {

	if ( index < data->length ) {

		do {

			++index;
		}
		while ( index < data->length && data->data[ index ].rho <= 0.0 && data->data[ index ].flow[ 0 ] != 0.0 && data->data[ index ].flow[ 1 ] != 0.0 && data->data[ index ].flow[ 2 ] != 0.0 );
	}

	return *this;
}


/**
 * Post decrement operator
 * @return A reference to the iterator after decrementing the index
 */
FluidGrid3D::iterator & FluidGrid3D::iterator::operator -- () {

	if ( index > 0 ) {

		do {

			--index;
		}
		while ( index > 0 && data->data[ index ].rho <= 0.0 && data->data[ index ].flow[ 0 ] != 0.0 && data->data[ index ].flow[ 1 ] != 0.0 && data->data[ index ].flow[ 2 ] != 0.0 );
	}

	return *this;
}


/**
 * Post increment operator
 * @return A reference to the iterator after incrementing the index
 */
FluidGrid3D::const_iterator & FluidGrid3D::const_iterator::operator ++ () {

	if ( index < data->length ) {

		do {

			++index;
		}
		while ( index < data->length && data->data[ index ].rho <= 0.0 && data->data[ index ].flow[ 0 ] != 0.0 && data->data[ index ].flow[ 1 ] != 0.0 && data->data[ index ].flow[ 2 ] != 0.0 );
	}

	return *this;
}


/**
 * Post decrement operator
 * @return A reference to the iterator after decrementing the index
 */
FluidGrid3D::const_iterator & FluidGrid3D::const_iterator::operator -- () {

	if ( index > 0 ) {

		do {

			--index;
		}
		while ( index > 0 && data->data[ index ].rho <= 0.0 && data->data[ index ].flow[ 0 ] != 0.0 && data->data[ index ].flow[ 1 ] != 0.0 && data->data[ index ].flow[ 2 ] != 0.0 );
	}

	return *this;
}


/**
 * Interpolate the value of the specified double attribute for the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordiante of the point
 * @param value The attribute to get
 * @return The interpolated value of the specified attribute at the given point
 */
double FluidGrid3D::cellValue( double x, double y, double z, int value ) const {

	x = ( x - minPoint[ 0 ] ) / scale;
	y = ( y - minPoint[ 1 ] ) / scale;
	z = ( z - minPoint[ 2 ] ) / scale;
	uint cells[ 8 ];
	double weights[ 9 ];
	cellsAndWeights( x, y, z, cells, weights );
	double result = 0.0;

	for ( int i = 0; i < 8; ++i ) {

		result += data[ cells[ i ] ].doubleArray[ value ] * weights[ i ];
	}

	return result;
}


/**
 * Interpolate a rho-dependent value of the specified double attribute for the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param value The attribute to get
 * @return The value of the specified attribute at the given point
 */
double FluidGrid3D::cellValueIfRho( double x, double y, double z, int value ) const {

	x = ( x - minPoint[ 0 ] ) / scale;
	y = ( y - minPoint[ 1 ] ) / scale;
	z = ( z - minPoint[ 2 ] ) / scale;
	uint cells[ 8 ];
	double weights[ 9 ];
	cellsAndWeightsNullable( x, y, z, cells, weights );
	double result = 0.0;
	double denominator = 0.0;

	for ( int i = 0; i < 8; ++i ) {

		if ( cells[ i ] != BAD_INDEX && data[ cells[ i ] ].doubleArray[ RHO ] > 0.0 ) {

			result += data[ cells[ i ] ].doubleArray[ value ] * weights[ i ];
			denominator += weights[ i ];
		}
	}

	if ( denominator ) {

		return result / denominator;
	}
	else {

		return result;
	}
}


/**
 * Interpolates the value of the specified face attribute at the given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordiante of the point
 * @param value The attribute to get
 * @return The value of the specified face attribute at the given point
 */
double FluidGrid3D::faceValue( double x, double y, double z, int value ) const {

	x = ( x - minPoint[ 0 ] ) / scale;
	y = ( y - minPoint[ 1 ] ) / scale;
	z = ( z - minPoint[ 2 ] ) / scale;

	return faceValueNoScale( x, y, z, value );
}


/**
 * Interpolates the value of the specified face attribute at the given point without scaling by the size of a cell
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordiante of the point
 * @param value The attribute to get
 * @return The value of the specified face attribute at the given point
 */
double FluidGrid3D::faceValueNoScale( double x, double y, double z, int value ) const {

	int coordinate = value % 3;
	x -= 0.5 * ( coordinate != 0 );
	y -= 0.5 * ( coordinate != 1 );
	z -= 0.5 * ( coordinate != 2 );
	uint cells[ 8 ];
	double weights[ 8 ];
	cellsAndWeightsWrap( x, y, z, cells, weights );
	double result = 0.0;

	for ( int i = 0; i < 8; ++i ) {

		result += data[ cells[ i ] ].doubleArray[ value ] * weights[ i ];
	}

	return result;
}


/**
 * Gets the velocity vector in the special case of a face-centered point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @param direction The face to compute the velocity for
 * @param velocity Stores the velocity vector
 */
void FluidGrid3D::velocityOnFace( int x, int y, int z, int direction, double velocity[ 3 ] ) {

	velocity[ 0 ] = ( direction == 0 ) ? ( doubleValueFast( x, y, z, FLOW0 ) ) : ( doubleValueFast( x, y, z, FLOW0 ) + doubleValueWrap( x + 1, y, z, FLOW0 ) ) * 0.5;
	velocity[ 1 ] = ( direction == 1 ) ? ( doubleValueFast( x, y, z, FLOW1 ) ) : ( doubleValueFast( x, y, z, FLOW1 ) + doubleValueWrap( x, y + 1, z, FLOW1 ) ) * 0.5;
	velocity[ 2 ] = ( direction == 2 ) ? ( doubleValueFast( x, y, z, FLOW2 ) ) : ( doubleValueFast( x, y, z, FLOW2 ) + doubleValueWrap( x, y, z + 1, FLOW2 ) ) * 0.5;
}


/**
 * Gathers fluid statistics
 * @param maxVelocityComponent Stores the maximum velocity component
 * @param kineticEnergy Stores the fluids kinetic energy
 * @param totalRho Stores the fluid's total Rho
 */
void FluidGrid3D::gatherStatistics( double * maxVelocityComponent, double * kineticEnergy, double * totalRho ) const {

	if ( maxVelocityComponent ) {

		*maxVelocityComponent = 0.0;
	}

	if ( kineticEnergy ) {

		*kineticEnergy = 0.0;
	}

	if ( totalRho ) {

		*totalRho = 0.0;
	}

	int i;

	for ( const_iterator it = begin(); it != end(); ++it ) {

		if ( totalRho ) {

			*totalRho += it->rho;
		}

		if ( maxVelocityComponent || kineticEnergy ) {

			for ( i = 0; i < 3; ++i ) {

				if ( maxVelocityComponent && *maxVelocityComponent < fabs( it->rho * it->flow[ i ] ) ) {

					*maxVelocityComponent = fabs( it->rho * it->flow[ i ] );
				}

				if ( kineticEnergy ) {

					*kineticEnergy += square( it->flow[ i ] ) * 0.25 * ( it->rho + it.backwardCell( i ).rho );
				}
			}
		}
	}
}


/**
 * Advects fluid grid velocities (wraps unless faces are closed or air)
 * @param dt The timestep
 * @param g The gravity (???)
 * @param waterEpsilon Regulates how much dirt the water can carry (default 0.05)
 */
void FluidGrid3D::advectVelocities( double dt, const double g[ 3 ], double waterEpsilon ) {

	double velocities[ 3 ];

	for ( iterator it = begin(); it != end(); ++it ) {

		if ( it->rho <= 0 ) {

			continue;
		}

		if ( it->faceType[ 0 ] != CLOSED_FACE ) {

			continue;
		}

		if ( it->faceType[ 1 ] != CLOSED_FACE ) {

			continue;
		}

		if ( it->faceType[ 2 ] != CLOSED_FACE ) {

			continue;
		}

		if ( it.forwardCell( 0 ).faceType[ 0 ] != CLOSED_FACE ) {

			continue;
		}

		if ( it.forwardCell( 1 ).faceType[ 1 ] != CLOSED_FACE ) {

			continue;
		}

		if ( it.forwardCell( 2 ).faceType[ 1 ] != CLOSED_FACE ) {

			continue;
		}

		int x = 0;
		int y = 0;
		int z = 0;
		unIndex( it.getIndex(), x, y, z );

		//cerr << "Zeroing cell " << x << ", " << y << ", " << z << " of " << dimensions[ 0 ] << ", " << dimensions[ 1 ] << ", " << dimensions[ 2 ] << "\n";
		it->rho = 0.0;
	}

	int direction;
	int i;
	int j;
	int k;

	for ( iterator it = begin(); it != end(); ++it ) {

		for ( direction = 0; direction < 3; ++direction ) {

			if ( it->faceType[ direction ] == CLOSED_FACE ) {

				continue;
			}

			if ( it->rho <= 0.0 && it.backwardCell( direction ).rho <= 0.0 ) {

				// Use "temp" as a temporary flow holder
				it->temp[ direction ] = 0.0;
				continue;
			}

			i = 0;
			j = 0;
			k = 0;
			unIndex( it.getIndex(), i, j, k );
			velocityOnFace( i, j, k, direction, velocities );
			velocityNoScale( i - velocities[ 0 ] * dt * 0.5, j - velocities[ 1 ] * dt * 0.5, k - velocities[ 2 ] * dt * 0.5, velocities );
			it->temp[ direction ] = faceValueNoScale( i - velocities[ 0 ] * dt, j - velocities[ 1 ] * dt, k - velocities[ 2 ] * dt, FLOW( direction ) );
			it->temp[ direction ] += g[ direction ] * dt;
		}
	}

	// Note that a proper AIR_FACE implementation would have two independent
	// flows. Instead, we assume that one cell near each AIR_FACE has no fluid
	// and treat it, for divergence purposes, exactly like an OPEN_FACE cell.
	int numWet = 0;

	for ( iterator it = begin(); it != end(); ++it ) {

		if ( it->rho > 1.0 - waterEpsilon ) {

			it->userInt = numWet++;
		}
		else {

			it->userInt = 0;
		}
	}

	if ( numWet == 0 ) {

		return;
	}

	ONLM A( numWet );
	ONLV p( numWet );
	ONLV b( numWet );

	int open;
	double airPressure;
	double divergence;
	
	for ( const_iterator it = begin(); it != end(); ++it ) {

		if ( it->rho <= 1.0 - waterEpsilon ) {

			continue;
		}

		open = 0;
		airPressure = 0.0;
		divergence = 0.0;

		for ( direction = 0; direction < 3; ++direction ) {

			const CellData * previous = &it.backwardCell( direction );
			const CellData * next = &it.forwardCell( direction );

			// Ignore "closed," put "open" in matrix, accumulate air pressure

			// Open or air face
			if ( it->faceType[ direction ] != CLOSED_FACE ) {

				++open;
				divergence -= it->temp[ direction ];

				if ( previous->rho <= 1.0 - waterEpsilon ) {

					airPressure += 0.5 + previous->rho;
					previous->userInt = 1;
				}
				else {

					A.add_coef( it->userInt, previous->userInt, 1.0 );
				}
			}

			// Open or air face
			if ( next->faceType[ direction ] != CLOSED_FACE ) {

				++open;
				divergence += next->temp[ direction ];

				if ( next->rho <= 1.0 - waterEpsilon ) {

					airPressure += 0.5 + next->rho;
					next->userInt = 1;
				}
				else {

					A.add_coef( it->userInt, next->userInt, 1.0 );
				}
			}
		}

		if ( it->rho > 1.0 ) {

			divergence -= ( it->rho - 1.0 );
		}

		A.add_coef( it->userInt, it->userInt, -open );
		b[ it->userInt ] = divergence - airPressure;
	}

	// High tolerance works fine for this problem
	solveSystem( A, p, b, 1.0e-3 );

	// Because the borders can change between frames, we might end up with an
	// isolated cell, making a singular matrix. Since what happens to those
	// cells doesn't matter, we simply detect and ignore them.
	for ( i = 0; i < numWet; ++i ) {

		if ( p[ i ] > 1.0e10 || p[ i ] < -1.0e10 ) {

			p[ i ] = 0.0;
		}
	}

	for ( iterator it = begin(); it != end(); ++it ) {

		bool itWet = ( it->rho > 1.0 - waterEpsilon );
		double iPressure = itWet ? ( p[ it->userInt ] ) : ( 0.5 + it->rho );

		for ( direction = 0; direction < 3; ++direction ) {

			if ( it->faceType[ direction ] == CLOSED_FACE ) {

				continue;
			}

			const CellData * ot = &it.backwardCell( direction );
			bool otWet = ( ot->rho > 1.0 - waterEpsilon );

			if ( !itWet && !otWet ) {

				it->flow[ direction ] = it->temp[ direction ];
				continue;
			}

			double oPressure = otWet ? ( p[ ot->userInt ] ) : ( 0.5 + ot->rho );
			it->flow[ direction ] = it->temp[ direction ] + ( oPressure - iPressure );
		}
	}

	for ( uint ui = 0; ui < length; ++ui ) {

		if ( data[ ui ].userInt && data[ ui ].rho <= 1.0 - waterEpsilon ) {

			for ( direction = 0; direction < 3; ++direction ) {

				uint f = forward( ui, direction );
				uint b = backward( ui, direction );

				if ( data[ b ].rho <= 1.0 - waterEpsilon && data[ ui ].faceType[ direction ] != CLOSED_FACE ) {

					extrapolateFlow( ui, direction );
				}

				if ( data[ f ].rho <= 1.0 - waterEpsilon && data[ f ].faceType[ direction ] != CLOSED_FACE ) {

					extrapolateFlow( f, direction );
				}
			}
		}
	}
}


/**
 * Advects the contents of the grid cells (including dirt)
 * @param dt The timestep
 * @param dRho ???
 * @param dDirt An array storing the amount of dirt in each cell of the grid
 */
void FluidGrid3D::advectContents( double dt, double * dRho, double * dDirt ) {

	static int runCount = 1;
	++runCount;

	if ( !runCount ) {

		++runCount;
	}

	for ( iterator it = begin(); it != end(); ++it ) {

		it->temp[ 0 ] = it->rho;
		it->temp[ 1 ] = it->dirt;
		it->userInt = runCount;
	}

	int i;

	for ( iterator it = begin(); it != end(); ++it ) {

		for ( i = 0; i < 3; ++i ) {

			if ( it->faceType[ i ] == CLOSED_FACE || it->flow[ i ] == 0 ) {

				continue;
			}

			CellData * ot = &it.backwardCell( i );

			if ( ot->userInt != runCount ) {

				ot->temp[ 0 ] = 0.0;
				ot->temp[ 1 ] = 0.0;
				ot->userInt = runCount;
			}

			// Into "it", out of "ot"
			if ( it->flow[ i ] > 0.0 ) {

				ot->temp[ 0 ] -= ot->rho * ( dt * 0.5 * it->flow[ i ] );
				ot->temp[ 1 ] -= ot->dirt * ( dt * 0.5 * it->flow[ i ] );

				if ( it->faceType[ i ] == AIR_FACE ) {

					if ( dRho ) {

						*dRho -= ot->rho * ( dt * 0.5 * it->flow[ i ] );
					}

					if ( dDirt ) {

						*dDirt -= ot->dirt * ( dt * 0.5 * it->flow[ i ] );
					}
				}
				else {

					it->temp[ 0 ] += ot->rho * ( dt * 0.5 * it->flow[ i ] );
					it->temp[ 1 ] += ot->dirt * ( dt * 0.5 * it->flow[ i ] );
				}
			}

			// Into "ot", out of "it"
			else {

				it->temp[ 0 ] += it->rho * ( dt * 0.5 * it->flow[ i ] );
				it->temp[ 1 ] += it->dirt * ( dt * 0.5 * it->flow[ i ] );

				if ( it->faceType[ i ] == AIR_FACE ) {

					if ( dRho ) {

						*dRho += it->rho * ( dt * 0.5 * it->flow[ i ] );
					}

					if ( dDirt ) {

						*dDirt += it->dirt * ( dt * 0.5 * it->flow[ i ] );
					}
				}
				else {

					ot->temp[ 0 ] -= it->rho * ( dt * 0.5 * it->flow[ i ] );
					ot->temp[ 1 ] -= it->dirt * ( dt * 0.5 * it->flow[ i ] );
				}
			}
		}
	}

	// The standard iterator doesn't work here...
	for ( uint ui = 0; ui < length; ++ui ) {

		if ( data[ ui ].userInt == runCount ) {

			data[ ui ].userInt = 0;
			data[ ui ].rho = data[ ui ].temp[ 0 ];
			data[ ui ].dirt = data[ ui ].temp[ 1 ];
		}
	}
}


/**
 * Extrapolates the flow of the fluid in the cell based on the cell's neighbors
 * @param cell The index of the cell
 * @param direction The direction to extrapolate in
 */
void FluidGrid3D::extrapolateFlow( uint cell, int direction ) {

	double weightedSum = 0.0;
	double weights = 0.0;
	int i = 0;
	int j = 0;
	int k = 0;
	unIndex( cell, i, j, k );

	double flow[ 6 ] = {
		// px py pz nx ny nz
		data[ wrapIndex( i + 1, j, k ) ].flow[ direction ],
		data[ wrapIndex( i, j + 1, k ) ].flow[ direction ],
		data[ wrapIndex( i, j, k + 1 ) ].flow[ direction ],
		data[ wrapIndex( i - 1, j, k ) ].flow[ direction ],
		data[ wrapIndex( i, j - 1, k ) ].flow[ direction ],
		data[ wrapIndex( i, j, k - 1 ) ].flow[ direction ]
	};

	// 14 lookups performed, only 12 needed; 6 overlap with last call
	double rho[ 7 ] = {
		// px py pz nx ny nz this
		data[ wrapIndex( i + 1, j, k ) ].rho + data[ wrapIndex( i + 1 - ( direction == 0 ), j - ( direction == 1 ), k - ( direction == 2 ) ) ].rho,
		data[ wrapIndex( i, j + 1, k ) ].rho + data[ wrapIndex( i - ( direction == 0 ), j + 1 - ( direction == 1 ), k - ( direction == 2 ) ) ].rho,
		data[ wrapIndex( i, j, k + 1 ) ].rho + data[ wrapIndex( i - ( direction == 0 ), j - ( direction == 1 ), k + 1 - ( direction == 2 ) ) ].rho,
		data[ wrapIndex( i - 1, j, k ) ].rho + data[ wrapIndex( i - 1 - ( direction == 0 ), j - ( direction == 1 ), k - ( direction == 2 ) ) ].rho,
		data[ wrapIndex( i, j - 1, k ) ].rho + data[ wrapIndex( i - ( direction == 0 ), j - 1 - ( direction == 1 ), k - ( direction == 2 ) ) ].rho,
		data[ wrapIndex( i, j, k - 1 ) ].rho + data[ wrapIndex( i - ( direction == 0 ), j - ( direction == 1 ), k - 1 - ( direction == 2 ) ) ].rho,
		data[ wrapIndex( i, j, k ) ].rho + data[ wrapIndex( i - ( direction == 0 ), j - ( direction == 1 ), k - ( direction == 2 ) ) ].rho
	};

	for ( int i = 0; i < 6; ++i ) {

		if ( rho[ i ] > rho[ 6 ] ) {

			weightedSum += flow[ i ] * ( rho[ i ] - rho[ 6 ] );
			weights += ( rho[ i ] - rho[ 6 ] );
		}
	}

	if ( weights ) {

		data[ cell ].flow[ direction ] = weightedSum / weights;
	}
}


/**
 * Sets the face type of all edges that are open to the air to "AIR_FACE"
 */
void FluidGrid3D::setAllOpenEdges() {

	int i;
	int j;
	int k;

	for ( i = 0; i < ( int ) dimensions[ 0 ]; ++i ) {

		for ( j = 0; j < ( int ) dimensions[ 1 ]; ++j ) {

			data[ index( i, j, 0 ) ].faceType[ 2 ] = AIR_FACE;
		}
	}

	for ( i = 0; i < ( int ) dimensions[ 0 ]; ++i ) {

		for ( k = 0; k < ( int ) dimensions[ 2 ]; ++k ) {

			data[ index( i, 0, k ) ].faceType[ 1 ] = AIR_FACE;
		}
	}

	for ( j = 0; j < ( int ) dimensions[ 1 ]; ++j ) {

		for ( k = 0; k < ( int ) dimensions[ 2 ]; ++k ) {

			data[ index( 0, j, k ) ].faceType[ 0 ] = AIR_FACE;
		}
	}
}


/**
 * Must be called before injecting dirt
 */
void FluidGrid3D::preInjection() {

	for ( uint index = 0; index < length; ++index ) {

		data[ index ].temp[ 1 ] = data[ index ].dirt;
	}
}


/**
 * Must be called after injecting dirt
 * @param speedScale The amount to scale the time
 * @param dropSpeed The amount to drop the speed (of what???)
 */
void FluidGrid3D::postInjection( double speedScale, double dropSpeed ) {

	uint index;
	int direction;
	double speed;
	double equilibrium;
	double change;

	for ( index = 0; index < length; ++index ) {

		data[ index ].dirt = data[ index ].temp[ 1 ];
	}

	for ( index = 0; index < length; ++index ) {

		if ( data[ index ].faceType[ 1 ] == CLOSED_FACE ) {

			continue;
		}

		if ( data[ index ].temp[ 1 ] <= 0.0 ) {

			speed = 0.0;

			for ( direction = 0; direction < 3; ++direction ) {

				speed += square( data[ index ].flow[ direction ] + data[ forward( index, direction ) ].flow[ direction ] );
			}

			speed = sqrt( speed ) / 2.0;
			equilibrium = speedScale * speed * data[ index ].rho;

			if ( equilibrium < 0.0 ) {

				equilibrium = 0.0;
			}

			if ( data[ index ].temp[ 1 ] > equilibrium ) {

				change = ( data[ index ].dirt - equilibrium ) * dropSpeed;
				data[ index ].dirt -= change;
				data[ backward( index, 1 ) ].dirt += change;
			}
		}
	}
}
