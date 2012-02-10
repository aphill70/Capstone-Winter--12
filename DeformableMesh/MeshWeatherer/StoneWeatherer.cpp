// #include <CGAL/number_utils.h>
#include <CGAL/algorithm.h>
#include <CGAL/Random.h>
#include <CGAL/Timer.h>
#include <fstream>
#include <iostream>

#include "MultiOBJReader.h"
#include "StoneWeatherer.h"
#include "UniformGrid.h"

using namespace std;


// Initialize static data
//double StoneWeatherer::secondsTotal = 0.0;
//double StoneWeatherer::secondsCGAL = 0.0;
//double StoneWeatherer::secondsLabeling = 0.0;

// 2 because we only have ROCK and DIRT right now
typedef MultiOBJReader<StoneWeatherer::NUMBER_OF_SOLID_MATERIAL_TYPES> OBJReader;

/**
 * Random number generator
 */
CGAL::Random random;


/**
 * Default constructor
 */
StoneWeatherer::StoneWeatherer() : newDT( dt + 0 ), oldDT( dt + 1 ), live( trueXYZ ), startsFilled( trueXYZ ), initialPoints() {

	return;
}


/**
 * Destructor
 */
StoneWeatherer::~StoneWeatherer() {

	newDT->clear();
	oldDT->clear();
}


/**
 * Swaps the pointers to the old mesh and the new one
 */
void StoneWeatherer::swapDT() {

	Delaunay * temp = newDT;
	newDT = oldDT;
	oldDT = temp;
}


/**
 * Sets the curvature response function
 *	hyperbolic interpolation alpha + (c + sqrt(c * c + beta))
 *		alpha is rate in pit regions
 *		beta is range of regions treated as flat
 *	if beta < 0 and alpha == 0, operator()(curvature) returns 1
 *	if beta < 0 and alpha != 0, operator()(curvature) returns alpha * curvature
 * @param alpha The new alpha value
 * @param beta The new beta value
 */
void StoneWeatherer::setCurveFunction( double alpha, double beta ) {

	ci.set( alpha, beta );
}


/**
 * Computes the circumcenters of the given cell
 * @param cell The cell to compute the circumcenters for
 * @param result Stores the circumcenter
 * @return TRUE always
 */
bool computeCircumcenter( const Cell_handle & cell, Point & result ) {

	// The four vertices of the cell (tetrahedron)
	const Point & p = cell->vertex( 0 )->point();
	const Point & q = cell->vertex( 1 )->point();
	const Point & r = cell->vertex( 2 )->point();
	const Point & s = cell->vertex( 3 )->point();

	// Translate p to the origin to simplify the expression
	double qpx = q.x() - p.x();
	double qpy = q.y() - p.y();
	double qpz = q.z() - p.z();
	double qp2 = square( qpx ) + square( qpy ) + square( qpz );
	double rpx = r.x() - p.x();
	double rpy = r.y() - p.y();
	double rpz = r.z() - p.z();
	double rp2 = square( rpx ) + square( rpy ) + square( rpz );
	double spx = s.x() - p.x();
	double spy = s.y() - p.y();
	double spz = s.z() - p.z();
	double sp2 = square( spx ) + square( spy ) + square( spz );

	double determinant = CGAL::determinant( qpx, qpy, qpz, rpx, rpy, rpz, spx, spy, spz );

	if ( determinant == 0 ) {

		result = CGAL::ORIGIN + ( ( p - CGAL::ORIGIN ) * 0.25 + ( q - CGAL::ORIGIN ) * 0.25 + ( r - CGAL::ORIGIN ) * 0.25 + ( s - CGAL::ORIGIN ) * 0.25 );

		return true;
	}

	double numX = CGAL::determinant( qpy, qpz, qp2, rpy, rpz, rp2, spy, spz, sp2 );
	double numY = CGAL::determinant( qpx, qpz, qp2, rpx, rpz, rp2, spx, spz, sp2 );
	double numZ = CGAL::determinant( qpx, qpy, qp2, rpx, rpy, rp2, spx, spy, sp2 );
	double inverse = 1.0 / ( 2.0 * determinant );

	result = Point( p.x() + numX * inverse, p.y() - numY * inverse, p.z() + numZ * inverse );

	return true;
}


/**
 * Sets the function to call for determining liveness. If not supplied, assume "live(x,y,z) = true"
 * @param nlive The function to call to determine if a point is live
 */
void StoneWeatherer::setLiveRegion( bool ( *nlive )( double x, double y, double z ) ) {

	if ( live ) {

		live = nlive;
	}
	else {

		live = trueXYZ;
	}
}


/**
 * Gets the material for the given point
 * @param p The point to get the material for
 * @return The material at the given point
 */
Contents StoneWeatherer::getContents( const Point & p ) const {

	// Get a handle to the cell that contains the given point
	Cell_handle ch = newDT->locate( p );

	if ( newDT->is_infinite( ch ) ) {

		return AIR;
	}
	else {

		return ch->info();
	}
}


/**
 * Sets all the contet flags (AIR, DIRT, ROCK) for the vertices
 * @param midPoint Stores the approximate center of mass of the object
 * @param pointMap Maps circumcenters to old locations
 */
void StoneWeatherer::setContentFlags( double midPoint[ 3 ], map<Point,Point> & pointMap ) {

	//static CGAL::Timer timer;
	//timer.start();
	//timer.reset();
	
	midPoint[ 0 ] = 0.0;
	midPoint[ 1 ] = 0.0;
	midPoint[ 2 ] = 0.0;
	double volume = 0;
	Point circumCenter;

	// Convert CGAL hierarchy of cells to an array

	// Get the size
	unsigned int size = newDT->number_of_cells();

	// Create the array to iterate over in parallel
	Cell_handle * cells = new Cell_handle[ size ];
	int i = 0;

	// Copy cells from hierarchy to array
	for ( All_Cell_iterator it = newDT->all_cells_begin(); it != newDT->all_cells_end(); ++it ) {

		cells[ i++ ] = it;
	}

	UniformGrid grid;

	for ( Cell_iterator it = oldDT->finite_cells_begin(); it != oldDT->finite_cells_end(); ++it ) {

		if ( it->info() > AIR && !oldDT->is_infinite( it ) ) {

			grid.add( it );
		}
	}

	grid.partition();
	
	#pragma omp parallel for private( circumCenter ) schedule( dynamic )
	for ( i = 0; i < size; ++i ) {

		if ( newDT->is_infinite( cells[ i ] ) ) {

			cells[ i ]->info() = AIR;
			continue;
		}

		if ( !computeCircumcenter( cells[ i ], circumCenter ) ) {

			cells[ i ]->info() = AIR;
			continue;
		}

		map<Point,Point>::iterator mit;
		Point p( cells[ i ]->vertex( 0 )->point().x(), cells[ i ]->vertex( 0 )->point().y(), cells[ i ]->vertex( 0 )->point().z() );
		Point q( cells[ i ]->vertex( 1 )->point().x(), cells[ i ]->vertex( 1 )->point().y(), cells[ i ]->vertex( 1 )->point().z() );
		Point r( cells[ i ]->vertex( 2 )->point().x(), cells[ i ]->vertex( 2 )->point().y(), cells[ i ]->vertex( 2 )->point().z() );
		Point s( cells[ i ]->vertex( 3 )->point().x(), cells[ i ]->vertex( 3 )->point().y(), cells[ i ]->vertex( 3 )->point().z() );
		Point p_old = ( mit = pointMap.find( p ) ) != pointMap.end() ? mit->second : p;
		Point q_old = ( mit = pointMap.find( q ) ) != pointMap.end() ? mit->second : q;
		Point r_old = ( mit = pointMap.find( r ) ) != pointMap.end() ? mit->second : r;
		Point s_old = ( mit = pointMap.find( s ) ) != pointMap.end() ? mit->second : s;

		Point avg(
			( ( p.x() - p_old.x() ) + ( q.x() - q_old.x() ) + ( r.x() - r_old.x() ) + ( s.x() - s_old.x() ) ) * 0.25,
			( ( p.y() - p_old.y() ) + ( q.y() - q_old.y() ) + ( r.y() - r_old.y() ) + ( s.y() - s_old.y() ) ) * 0.25,
			( ( p.z() - p_old.z() ) + ( q.z() - q_old.z() ) + ( r.z() - r_old.z() ) + ( s.z() - s_old.z() ) ) * 0.25
		);

		circumCenter = Point( circumCenter.x() - avg.x(), circumCenter.y() - avg.y(), circumCenter.z() - avg.z() );

		if ( grid.contains( Point3D( circumCenter.x(), circumCenter.y(), circumCenter.z() ) ) ) {

			grid.locateAndLabel( circumCenter, cells[ i ], midPoint, volume );
		}
		else {

			cells[ i ]->info() = AIR;
		}
	}
	
	delete[] cells;

	if ( volume > 0.0 ) {

		midPoint[ 0 ] /= volume;
		midPoint[ 1 ] /= volume;
		midPoint[ 2 ] /= volume;
	}

	//relabelTime += timer.time();
	//timer.stop();
}


/**
 * Sets the new vertex info for the vertices in the new mesh
 */
void StoneWeatherer::setVertexInfo() {

	// Clear everything
	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		it->info().clear();
	}

	int i;

	// Label the vertices to keep (those defining boundaries between materials, where "infinite" means "air")
	for ( All_Cell_iterator it = newDT->all_cells_begin(); it != newDT->all_cells_end(); ++it ) {

		if ( newDT->is_infinite( it ) ) {

			for ( i = 0; i < 4; ++i ) {

				it->vertex( i )->info().flag |= AIR;
			}
		}
		else {

			for ( i = 0; i < 4; ++i ) {

				it->vertex( i )->info().flag |= it->info();
			}
		}
	}

	// Simplify the labels for mesh simplification later
	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		switch ( it->info().flag ) {

			case AIR | DIRT | ROCK | MORE_ROCK:

				// border
			case AIR | DIRT | ROCK:

				// border

			case AIR | DIRT | MORE_ROCK:

				// border

			case AIR | ROCK | MORE_ROCK:

				// border

			case DIRT | ROCK | MORE_ROCK:

				// border

			case AIR | ROCK:

				// border

			case AIR | DIRT:

				// border

			case AIR | MORE_ROCK:

				// border

			case ROCK | DIRT:

				// border

			case DIRT | MORE_ROCK:

				// border

			case ROCK | MORE_ROCK:

				// border

				it->info().kill = BORDER;
				break;

			default:

				it->info().kill = FLOATER;
		}
	}

	int n;

	// Accumulate face normals for border faces
	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		if ( it->info() != AIR ) {

			for ( n = 0; n < 4; ++n ) {

				if ( it->neighbor( n )->info() < it->info() || newDT->is_infinite( it->neighbor( n ) ) ) {

					Point t0 = it->vertex( n ^ 1 )->point();
					Vector s1 = it->vertex( n ^ 2 )->point() - t0;
					Vector s2 = it->vertex( n ^ 3 )->point() - t0;
					Vector norm = CGAL::cross_product( s1, s2 );

					for ( i = 1; i < 4; ++i ) {

						it->vertex( n ^ i )->info().normal = it->vertex( n ^ i )->info().normal + norm;
					}
				}
			}
		}
	}

	// Normalize the normals
	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		if ( it->info().kill != FLOATER ) {

			it->info().normal = it->info().normal / sqrt( it->info().normal.squared_length() );
		}
	}

	// Accumulate the edge curvatures and find range of edge lengths for future stepsize and vertex importance
	minEdgeLength = -1.0e300;
	int j;
	Vector e;
	double elen2;
	double curveMult;
	double importanceMult;
	double n_dot_e;

	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		if ( it->info() != AIR ) {

			for ( n = 0; n < 4; ++n ) {

				if ( it->neighbor( n )->info() < it->info() || newDT->is_infinite( it->neighbor( n ) ) ) {

					for ( i = 1; i < 4; ++i ) {

						for ( j = i + 1; j < 4; ++j ) {

							e = it->vertex( n ^ i )->point() - it->vertex( n ^ j )->point();
							elen2 = e.squared_length();
							SETMAX( minEdgeLength, -elen2 );
							curveMult = 1.0 / elen2;
							importanceMult = sqrt( curveMult ); // Geometric importance is edge-length independent
							n_dot_e = dot( it->vertex( n ^ i )->info().normal, e );
							it->vertex( n ^ i )->info().numEdges += 1;
							it->vertex( n ^ i )->info().curvature += 2.0 * n_dot_e * curveMult;
							SETMAX( it->vertex( n ^ i )->info().importance, fabs( n_dot_e * importanceMult ) );
						}
					}
				}
			}
		}
	}

	minEdgeLength = sqrt( -minEdgeLength );
	minCurvature = -1.0e300;
	maxCurvature = -1.0e300;

	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		if ( it->info().numEdges > 0 ) {

			it->info().curvature /= it->info().numEdges;
			SETMAX( minCurvature, -it->info().curvature );
			SETMAX( maxCurvature, +it->info().curvature );
		}
	}

	// Colorize the vertices
	//for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

	//	it->info().rgb[ 0 ] = 0.0;
	//	it->info().rgb[ 1 ] = 0.0;
	//	it->info().rgb[ 2 ] = 0.0;

	//	if ( it->info().flag & ROCK ) {

	//		it->info().rgb[ 2 ] = 1.0;
	//	}

	//	if ( it->info().flag & MORE_ROCK ) {

	//		it->info().rgb[ 0 ] = 1.0;
	//	}
	//}

	minCurvature *= -1.0;
}


/**
 * Computes the minimum square distance between two vertices using the given function to compute average edge length
 * @param averageEdgeLength The function to call to compute average edge length
 * @param v0 The first vertex
 * @param v1 The second vertex
 * @return The minimum square distance between the two vertices
 */
double minSquareDistance( double ( *averageEdgeLength )( const Vertex_handle & v ), const Vertex_handle & v0, const Vertex_handle & v1 ) {

	double d0 = square( averageEdgeLength( v0 ) * 0.5 );
	double d1 = square( averageEdgeLength( v1 ) * 0.5 );

	return ( d0 > d1 ) ? ( d1 ) : ( d0 );
}


/**
 * Computes the maximum square distance between two vertices using the given function to compute average edge length
 * @param averageEdgeLength The function to call to compute average edge length
 * @param v0 The first vertex
 * @param v1 The second vertex
 * @return The maximum square distance between the two vertices
 */
double maxSquareDistance( double ( *averageEdgeLength )( const Vertex_handle & v ), const Vertex_handle & v0, const Vertex_handle & v1 ) {

	double d0 = square( averageEdgeLength( v0 ) * 1.5 );
	double d1 = square( averageEdgeLength( v1 ) * 1.5 );

	return ( d0 > d1 ) ? ( d0 ) : ( d1 );
}


// ***** MAIN SIMULATION ***** //

/**
 * Splits a single vertex data with both ROCK and MORE_ROCK into two disjoint vertex data (one with ROCK, one with MORE_ROCK)
 * @param d The data to split
 * @param d1 Stores the ROCK data
 * @param d2 Stores the MORE_ROCK data
 */
void splitVertexData( const VertexData & d, VertexData & d1, VertexData & d2 ) {

	d1.curvature = d.curvature;
	d1.flag = d.flag & ~MORE_ROCK;
	d1.importance = d.importance;
	d1.kill = d.kill;
	d1.normal = d.normal;
	d1.numEdges = d.numEdges;
	d1.userData = d.userData;
	
	d2.curvature = d.curvature;
	d2.flag = d.flag & ~ROCK;
	d2.importance = d.importance;
	d2.kill = d.kill;
	d2.normal = d.normal;
	d2.numEdges = d.numEdges;
	d2.userData = d.userData;
}


/**
 * Jitters the location of the given point by a tiny, random amount
 * @param point The point to jitter
 * @return A jittered copy of the point
 */
Point jitterPoint( const Point & point ) {

	srand( 0 );
	double range = 0.01;
	double x = point.x() + ( ( rand() / ( double ) RAND_MAX ) * ( point.x() * 0.01 ) - ( point.x() * 0.005 ) );
	double y = point.y() + ( ( rand() / ( double ) RAND_MAX ) * ( point.y() * 0.01 ) - ( point.y() * 0.005 ) );
	double z = point.z() + ( ( rand() / ( double ) RAND_MAX ) * ( point.z() * 0.01 ) - ( point.z() * 0.005 ) );

	return Point( x, y, z );
}


extern stepResults cumulativeResults;

/**
 * Executes one custom timestep
 * @param averageEdgeLength The function to compute the average edge length for a given vertex
 * @param getOffsetPoint Computes the new location of the given point
 * @return The results of running the timestep
 */
stepResults StoneWeatherer::doOneCustomStep( double ( *averageEdgeLength )( const Vertex_handle & v ), Point( *getOffsetPoint )( const Point & p, const VertexData & d, const StoneWeatherer * caller ) ) {

	CGAL::Timer timestamp;
	stepResults result;
	result.secondsTotal = 0.0;
	timestamp.start();
	timestamp.reset();
	
	/**
	 * Maps circumcenters to where they really came from
	 */
	 map<Point,Point> pointMap;

	// Generate the correct-resolution offset points
	vector<Point> newPoints;
	int n;
	int i;
	int j;
	Vertex_handle vhi;
	Vertex_handle vhj;
	double dist2;
	Point a;
	Point b;
	Point c;
	VertexData d;

	// Put in midPoints of edges as needed, and flag redundant vertices
	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		if ( it->info() != AIR ) {
			
			for ( n = 0; n < 4; ++n ) {

				if ( it->neighbor( n )->info() != it->info() || newDT->is_infinite( it->neighbor( n ) ) ) {

					for ( i = 1; i < 4; ++i ) {

						Vertex_handle vhi = it->vertex( n ^ i );

						for ( j = i + 1; j < 4; ++j ) {

							Vertex_handle vhj = it->vertex( n ^ j );

							// Only check each edge once...
							if ( vhi < vhj ) {

								dist2 = ( vhi->point() - vhj->point() ).squared_length();

								if ( dist2 > maxSquareDistance( averageEdgeLength, vhi, vhj ) ) {

									// Don't split non-live edges
									a = vhi->point();
									b = vhj->point();

									if ( !live( a.x(), a.y(), a.z() ) && !live( b.x(), b.y(), b.z() ) ) {

										continue;
									}

									// Split edge
									a = CGAL::ORIGIN + ( ( a - CGAL::ORIGIN ) + ( b - CGAL::ORIGIN ) ) * 0.5;
									d = VertexData::midPoint( vhi->info(), vhj->info() );
									
									//// If the vertex is shared by both objects, split it
									//if ( ( d.flag & ROCK ) && ( d.flag & MORE_ROCK ) ) {

									//	VertexData d1;
									//	VertexData d2;
									//	splitVertexData( d, d1, d2 );
									//	
									//	Point a1 = jitterPoint( a );
									//	Point a2 = jitterPoint( a );
									//	
									//	c = getOffsetPoint( a1, d1, this );
									//	newPoints.push_back( c );
									//	pointMap.insert( pair<Point,Point>( c, a1 ) );

									//	c = getOffsetPoint( a2, d2, this );
									//	newPoints.push_back( c );
									//	pointMap.insert( pair<Point,Point>( c, a2 ) );
									//}
									//else {

										c = getOffsetPoint( a, d, this );
										newPoints.push_back( c );
										pointMap.insert( pair<Point,Point>( c, a ) );
									//}
								}
								else if ( vhi->info().kill != TOO_CLOSE && dist2 < minSquareDistance( averageEdgeLength, vhi, vhj ) ) {

									// The higher-address endpoint
									vhj->info().kill = TOO_CLOSE;
								}
							}
						}
					}
				}
			}
		}
	}

	double bound[ 3 ][ 2 ] = {
		{ 1.0e30, -1.0e30 },
		{ 1.0e30, -1.0e30 },
		{ 1.0e30, -1.0e30 }
	};

	// Put in all the border vertices
	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		if ( it->point().x() < bound[ 0 ][ 0 ] ) {

			bound[ 0 ][ 0 ] = it->point().x();
		}
		else if ( it->point().x() > bound[ 0 ][ 1 ] ) {

			bound[ 0 ][ 1 ] = it->point().x();
		}

		if ( it->point().y() < bound[ 1 ][ 0 ] ) {

			bound[ 1 ][ 0 ] = it->point().y();
		}
		else if ( it->point().y() > bound[ 1 ][ 1 ] ) {

			bound[ 1 ][ 1 ] = it->point().y();
		}

		if ( it->point().z() < bound[ 2 ][ 0 ] ) {

			bound[ 2 ][ 0 ] = it->point().z();
		}
		else if ( it->point().z() > bound[ 2 ][ 1 ] ) {

			bound[ 2 ][ 1 ] = it->point().z();
		}

		if ( !live( it->point().x(), it->point().y(), it->point().z() ) ) {

			newPoints.push_back( it->point() );
			pointMap.insert( pair<Point,Point>( it->point(), it->point() ) );
		}
		else if ( it->info().kill == BORDER ) {

			VertexData d = it->info();
			Point a = it->point();
			Point c;

			//// If the vertex is shared by both objects, split it
			//if ( ( d.flag & ROCK ) && ( d.flag & MORE_ROCK ) ) {

			//	VertexData d1;
			//	VertexData d2;
			//	splitVertexData( d, d1, d2 );
			//	
			//	Point a1 = jitterPoint( a );
			//	Point a2 = jitterPoint( a );
			//	
			//	c = getOffsetPoint( a1, d1, this );
			//	newPoints.push_back( c );
			//	pointMap.insert( pair<Point,Point>( c, a1 ) );

			//	c = getOffsetPoint( a2, d2, this );
			//	newPoints.push_back( c );
			//	pointMap.insert( pair<Point,Point>( c, a2 ) );
			//}
			//else {

				c = getOffsetPoint( a, d, this );
				newPoints.push_back( c );
				pointMap.insert( pair<Point,Point>( c, a ) );
			//}
		}
	}

	result.secondsTotal += ( result.secondsMotion = timestamp.time() );
	timestamp.reset();

	// Create the new mesh
	swapDT();
	newDT->clear();
	newDT->insert( newPoints.begin(), newPoints.end() );

	result.secondsTotal += ( result.secondsCGAL = timestamp.time() );
	//secondsTotal += result.secondsCGAL;
	//secondsCGAL += result.secondsCGAL;
	timestamp.reset();

	// Update the inside-outside flags of new tetrahedrons
	setContentFlags( result.midPoint, pointMap );
	result.midPoint[ 0 ] = ( bound[ 0 ][ 0 ] + bound[ 0 ][ 1 ] ) * 0.5;
	result.midPoint[ 1 ] = ( bound[ 1 ][ 0 ] + bound[ 1 ][ 1 ] ) * 0.5;
	result.midPoint[ 2 ] = ( bound[ 2 ][ 0 ] + bound[ 2 ][ 1 ] ) * 0.5;

	result.secondsTotal += ( result.secondsLabeling = timestamp.time() );
	//secondsTotal += result.secondsLabeling;
	//secondsLabeling += result.secondsLabeling;
	timestamp.reset();

	// Update vertex information
	setVertexInfo();

	result.secondsTotal += ( result.secondsAnalysis = timestamp.time() );
	timestamp.reset();
	timestamp.stop();

	result.numVertices = newDT->number_of_vertices();
	result.numTetrahedrons = newDT->number_of_cells();

	cumulativeResults.secondsTotal += result.secondsTotal;
	cumulativeResults.secondsMotion += result.secondsMotion;
	cumulativeResults.secondsCGAL += result.secondsCGAL;
	cumulativeResults.secondsLabeling += result.secondsLabeling;
	cumulativeResults.secondsAnalysis += result.secondsAnalysis;

	return result;
}


/**
 * The function to call to compute softness of a given point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param z The z-coordinate of the point
 * @return The softness of the specified point
 */
double ( *n_softness )( double x, double y, double z );


/**
 * The duration of a timestep
 */
double n_dt;


/**
 * Computes the new position of the given point
 * @param p The point to move
 * @param d The vertex data for the point
 * param caller The stone weatherer weathering the material
 * @return The new position of the point
 */
Point offsetFunction( const Point & p, const VertexData & d, const StoneWeatherer * caller ) {

	return p;
}


/**
 * Executes one timestep
 * @param averageEdgeLength The function to compute the average edge length for a given vertex
 * @param softness The function to compute the softness of a given point
 * @param overdrive The overdrive value (default 1)
 * @return The results of running the timestep
 */
stepResults StoneWeatherer::doOneStep( double ( *averageEdgeLength )( const Vertex_handle & v ), double ( *softness )( double x, double y, double z ), double overdrive ) {

	n_dt = 0.5 * overdrive * minEdgeLength / ( ( ci( minCurvature ) > ci( maxCurvature ) ) ? ci( minCurvature ) : ci( maxCurvature ) );
	n_softness = softness;

	return doOneCustomStep( averageEdgeLength, offsetFunction );
}


/**
 * Calls the given function on all vertices that are on a boundary and have a normal and curvature
 * @param callback The function to call
 * @param material The material type (default ROCK)
 */
void StoneWeatherer::callOnVertices( void ( *callback )( const Vertex & v ), Contents material ) const {

	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		if ( it->info().kill <= BORDER && ( it->info().flag & material ) ) {

			callback( *it );
		}
	}
}


/**
 * Calls the given function on all triangles in the mesh (will always provide vertices in CCW order as viewed from the outside)
 * @param callback The function to call
 * @param material The material type (default ROCK)
 */
void StoneWeatherer::callOnFaces( void ( *callback )( const Vertex & v0, const Vertex & v1, const Vertex & v2 ), Contents material ) const {

	int n;

	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		if ( it->info() == material ) {

			for ( n = 0; n < 4; ++n ) {

				if ( newDT->is_infinite( it->neighbor( n ) ) || it->neighbor( n )->info() != material ) {

					callback( *( it->vertex( n ^ 1 ) ), *( it->vertex( n ^ 2 ) ), *( it->vertex( n ^ 3 ) ) );
				}
			}
		}
	}
}


/**
 * Calls the given function on all cells in the mesh
 * @param callback The function to call
 */
void StoneWeatherer::callOnCells( void ( *callback )( const Cell_handle & c, const Delaunay & dt ) ) const {

	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		callback( Cell_handle( it ), *newDT );
	}
}


/**
 * Sets the initial inside/outside test and initial points. Uses odd/even crossing rule for insideness.
 * @param objFileName The file containing the initial mesh
 * @return TRUE if the file loaded successfully, FALSE otherwise
 */
bool StoneWeatherer::setInitialMesh( const char * objFileName ) {

	ifstream infile( objFileName );

	if ( !infile ) {
	
		cerr << "Cannot open the file \"" << objFileName << "\"\n";

		return false;
	}

	OBJReader maker( infile );
	infile.close();

	// If there is only one material, make it rock.
	// If there are two, the alphabetically first name is dirt, and the other is rock.
	// Cells in both materials are treaded as dirt.
	Contents labelLookup[ 1 << NUMBER_OF_SOLID_MATERIAL_TYPES ];
	labelLookup[ 0 ] = AIR;
	labelLookup[ 1 ] = ROCK;//maker.numNames <= 1 || maker.names[ 0 ] > maker.names[ 1 ] ? ROCK : DIRT;//ROCK;
	labelLookup[ 2 ] = MORE_ROCK;//labelLookup[ 1 ] == ROCK ? DIRT : ROCK;//MORE_ROCK;
	labelLookup[ 3 ] = DIRT;

	if ( ( 1 << NUMBER_OF_SOLID_MATERIAL_TYPES ) != 4 ) {

		cerr << "WARNING: there are more materials than " << __FILE__ << " (" << __LINE__ << ") can read." << endl;
	}

	for ( uint32_t i = 0; i < maker.vertices.size(); ++i ) {

		initialPoints.push_back( maker.vertices[ i ] );
	}

	newDT->clear();
	newDT->insert( initialPoints.begin(), initialPoints.end() );
	initialPoints.clear();

	Point circumCenter;
	int numit = newDT->number_of_cells();
	cout << "Beginning to fill in " << numit << " cells.\n";

	int rock = 0;
	int moreRock = 0;
	int air = 0;
	int dirt = 0;
	int done = 0;

	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		++done;

		if ( ( 10 * done / numit ) != ( 10 * ( done + 1 ) / numit ) ) {

			cout << "  " << ( 100 * ( done + 1 ) / numit ) << "% done\n";
		}

		if ( !computeCircumcenter( it, circumCenter ) ) {

			it->info() = AIR;
		}
		else {

			it->info() = labelLookup[ maker.isInside( circumCenter ) ];
		}

		if ( it->info() == AIR ) {

			++air;
		}

		if ( it->info() == DIRT ) {

			++dirt;
		}

		if ( it->info() == ROCK ) {

			++rock;
		}

		if ( it->info() == MORE_ROCK ) {

			++moreRock;
		}
	}

	cout << "Filled " << rock << " with rock, " << moreRock << " with more rock, " << dirt << " with dirt, and " << air << " with air.\n";

	this->setVertexInfo();

	return true;
}


/**
 * Gets the list of points from the mesh
 * @param points Stores the list of points
 */
void StoneWeatherer::getPoints( vector<Point> & points ) const {

	for ( Vertex_iterator it = newDT->finite_vertices_begin(); it != newDT->finite_vertices_end(); ++it ) {

		points.push_back( it->point() );
	}
}


/**
 * Gets all the tetrahedrons in the mesh
 * @param tetrahedrons Stores the tetrahedrons
 */
void StoneWeatherer::getTetrahedrons( vector<Tetrahedron> & tetrahedrons ) const {

	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		tetrahedrons.push_back(
			Tetrahedron(
				Point3D( it->vertex( 0 )->point().x(), it->vertex( 0 )->point().y(), it->vertex( 0 )->point().z() ),
				Point3D( it->vertex( 1 )->point().x(), it->vertex( 1 )->point().y(), it->vertex( 1 )->point().z() ),
				Point3D( it->vertex( 2 )->point().x(), it->vertex( 2 )->point().y(), it->vertex( 2 )->point().z() ),
				Point3D( it->vertex( 3 )->point().x(), it->vertex( 3 )->point().y(), it->vertex( 3 )->point().z() ),
				( Cell_handle ) it
			)
		);
	}
}


/**
 * Gets the type of material at the given point
 * @param point The point to check
 * @return The material type at the given point
 */
Contents StoneWeatherer::getMaterialType( const Point & point ) const {

	UniformGrid grid;

	for ( Cell_iterator it = newDT->finite_cells_begin(); it != newDT->finite_cells_end(); ++it ) {

		if ( it->info() > AIR && !newDT->is_infinite( it ) ) {

			grid.add( it );
		}
	}

	if ( grid.contains( Point3D( point.x(), point.y(), point.z() ) ) ) {

		return grid.getLabel( point );
	}
	else {

		return AIR;
	}
}
