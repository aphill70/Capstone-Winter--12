#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "CGAL_typedefs.h"

using namespace std;


/**
 * Reads an OBJ file containing up to n closed triangular meshes separated with "g name" commands
 */
template <int n> class MultiOBJReader {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The closed triangular meshes for the scene
	 */
	Segtree * tree[ n ];


public:

	//------------
	// MEMBER DATA
	//------------

	/**
	 * The vertices in the object(s)
	 */
	vector<Point> vertices;

	/**
	 * The triangles joining the vertices in the object(s)
	 */
	vector<int> triangles[ n ];

	/**
	 * The names of the objects
	 */
	string names[ n ];

	/**
	 * The number of object names specified
	 */
	int numNames;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Basic constructor
	 * @param in The input file stream to read to create the object
	 */
	MultiOBJReader( istream & in ) : numNames( 0 ) {

		static const int BUFFER_LENGTH = 4096;
		char line[ BUFFER_LENGTH ];
		char token[ BUFFER_LENGTH ];
		char indexBuffer[ 64 ];

		vector<Interval> allIntervals[ n ];
		int liveGroup = 0;
		int nextGroup = 1;

		double x;
		double y;
		double z;

		int v0;
		int vOld;
		int vNew;

		while ( in ) {

			in.getline( line, BUFFER_LENGTH );
			istringstream ss( line );
			ss >> token;

			if ( !token[ 0 ] ) {

				continue;
			}

			if ( strcmp( "v", token ) == 0 ) {

				// Read a vertex
				ss >> x >> y >> z;
				vertices.push_back( Point( x, y, z ) );
			}
			else if ( strcmp( "vn", token ) == 0 ) {

				// Read a vertex normal (nothing to do for now)
			}
			else if ( strcmp( "vt", token ) == 0 ) {

				// Read a texture coordinate (nothing to do for now)
			}
			else if ( strcmp( "f", token ) == 0 ) {

				// Read a face as a triangle fan
				ss >> indexBuffer;
				v0 = atoi( indexBuffer ) - 1;
				ss >> indexBuffer;
				vOld = atoi( indexBuffer ) - 1;

				// Deal with negative indices
				if ( v0 < -1 ) {

					v0 = vertices.size() + v0 + 1;
				}

				if ( vOld < -1 ) {

					vOld = vertices.size() + vOld + 1;
				}

				while ( ss ) {

					ss >> indexBuffer;
					vNew = atoi( indexBuffer ) - 1;

					// Deal with negative vertex indices
					if ( vNew < -1 ) {

						vNew = vertices.size() + vNew + 1;
					}

					if ( vNew > 0 ) {

						if ( v0 != vOld && v0 != vNew && vOld != vNew ) {

							triangles[ liveGroup ].push_back( v0 );
							triangles[ liveGroup ].push_back( vOld );
							triangles[ liveGroup ].push_back( vNew );
							Interval interval = yzBound( liveGroup, triangles[ liveGroup ].size() - 3 );

							if ( interval.first.first.x() < interval.first.second.x() && interval.first.first.y() < interval.first.second.y() ) {

								allIntervals[ liveGroup ].push_back( interval );
							}
						}

						vOld = vNew;
					}

					// Deal with negative vertex indices
					else {

						break;
					}
				}
			}
			else if ( strcmp( "g", token ) == 0 ) {

				// Change to a new vertex group
				if ( allIntervals[ liveGroup ].empty() ) {

					ss >> names[ liveGroup ];

					if ( numNames <= liveGroup ) {

						numNames = liveGroup + 1;
					}
				}
				else {

					string name;
					ss >> name;
					liveGroup = find( names, names + nextGroup, name ) - names;

					if ( liveGroup >= n ) {

						cerr << "The input file has at least " << liveGroup + 1 << " groups, but we can only manage " << n << endl;
						liveGroup = n - 1;
					}

					names[ liveGroup ] = name;

					if ( liveGroup == nextGroup ) {

						nextGroup += 1;
					}

					numNames = nextGroup;
				}
			}
			else {

				// Ignore all other tokens for now (mtllib, usemtl, s, o, spline-based commands)
			}
		}

		for ( int i = 0; i < n; ++i ) {

			tree[ i ] = new Segtree( allIntervals[ i ].begin(), allIntervals[ i ].end() );
			allIntervals[ i ].clear();
		}
	}


	/**
	 * Destructor
	 */
	~MultiOBJReader() {

		for ( int i = 0; i < n; ++i ) {

			if ( tree[ i ] ) {

				delete tree[ i ];
			}

			tree[ i ] = 0;
		}
	}


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Sets the minimum or maximum to value if value is less than the min or greater than the max
	 * @param currentMin The current min
	 * @param value The value to check
	 * @param currentMax The current max
	 */
	inline void setMinMax( double & currentMin, double value, double & currentMax ) {

		if ( value < currentMin ) {

			currentMin = value;
		}

		if ( value > currentMax ) {

			currentMax = value;
		}
	}


	/**
	 * Creates a query window that contains the supplied face
	 * @param material The material type for the face
	 * @param face The index of the face
	 * @return The query window (interval) containing the face
	 */
	Interval yzBound( int material, int face ) {

		double minY;
		double maxY;
		double minZ;
		double maxZ;
		
		maxY = vertices[ triangles[ material ][ face + 0 ] ].y();
		minY = maxY;

		maxZ = vertices[ triangles[ material ][ face + 0 ] ].z();
		minZ = maxZ;

		setMinMax( minY, vertices[ triangles[ material ][ face + 1 ] ].y(), maxY );
		setMinMax( minZ, vertices[ triangles[ material ][ face + 1 ] ].z(), maxZ );
		setMinMax( minY, vertices[ triangles[ material ][ face + 2 ] ].y(), maxY );
		setMinMax( minZ, vertices[ triangles[ material ][ face + 2 ] ].z(), maxZ );
		Key yKey( minY, minZ );
		Key zKey( maxY, maxZ );

		return Interval( PureInterval( yKey, zKey ), face );
	}


	/**
	 * Returns TRUE if p is to the right (+x) of the given triangle
	 * @param p The point to check
	 * @param t0 The first vertex of the triangle
	 * @param t1 The second vertex of the triangle
	 * @param t2 The third vertex of the triangle
	 * @param x ???
	 * @return TRUE if p is to the right (+x) side of the given triangle, FALSE otherwise
	 */
	bool countYZTriangle( const Point & p, const Point & t0, const Point & t1, const Point & t2, double * x = 0 ) {

		double temp;

		if ( !x ) {

			x = &temp;
		}

		double wind0 = ( t1.z() - p.z() ) * ( t2.y() - p.y() ) - ( t2.z() - p.z() ) * ( t1.y() - p.y() );
		double wind1 = ( t2.z() - p.z() ) * ( t0.y() - p.y() ) - ( t0.z() - p.z() ) * ( t2.y() - p.y() );
		double wind2 = ( t0.z() - p.z() ) * ( t1.y() - p.y() ) - ( t1.z() - p.z() ) * ( t0.y() - p.y() );

		// All same sign means INSIDE
		if ( ( wind0 + wind1 + wind2 ) != 0.0 && wind0 * wind1 >= 0.0 && wind0 * wind2 >= 0.0 ) {

			*x = ( t0.x() * wind0 + t1.x() * wind1 + t2.x() * wind2 ) / ( wind0 + wind1 + wind2 );

			return *x < p.x();
		}

		return false;
	}

	
	/**
	 * (result & (2^k)) != 0 if and only if p is inside the kth group in the OBJ file
	 * @param p The point to check
	 * @return The object ID the point is in
	 */
	int isInside( const Point & p ) {

		int result = 0;
		double x;
		uint32_t i;
		int face;
		double lastX;
		int numCrossings;

		for ( int k = 0; k < n; ++k ) {

			vector<Interval> yzFaces;
			tree[ k ]->window_query( Interval( PureInterval( Key( p[ 1 ], p[ 2 ] ), Key( p[ 1 ] + 1.0e-7, p[ 2 ] + 1.0e-7 ) ), -1 ), back_inserter( yzFaces ) );

			if ( yzFaces.size() <= 1 ) {

				continue;
			}

			vector<double> xs;
			
			for ( i = 0; i < yzFaces.size(); ++i ) {

				face = yzFaces[ i ].second;

				if ( countYZTriangle( p, vertices[ triangles[ k ][ face + 0 ] ], vertices[ triangles[ k ][ face + 1 ] ], vertices[ triangles[ k ][ face + 2 ] ], &x ) ) {

					xs.push_back( x );
				}
			}

			if ( xs.size() == 0 ) {

				continue;
			}

			if ( xs.size() == 1 ) {

				result |= ( 1 << k );
				continue;
			}

			sort( xs.begin(), xs.end() );
			lastX = p[ 0 ];
			numCrossings = 0;

			// We might get near-duplicates at edges
			for ( i = 0; i < xs.size(); ++i ) {

				if ( fabs( ( xs[ i ] - lastX ) / ( xs[ i ] + lastX ) ) > 1.0e-8 ) {

					numCrossings += 1;
				}

				lastX = xs[ i ];
			}

			if ( ( numCrossings & 1 ) == 1 ) {

				result |= ( 1 << k );
			}
		}

		return result;
	}
};
