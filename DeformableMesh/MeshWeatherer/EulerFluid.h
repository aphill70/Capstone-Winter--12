#pragma once

#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <utility>

using namespace std;

typedef unsigned int uint;


/**
 * Grid cell double properties
 */
enum DoubleIndex { 
	// Values stored on faces (must be in triplicate, must come first)
	FLOW0 = 0, FLOW1, FLOW2, 
	TEMP0, TEMP1, TEMP2,
	// Values stored in cells
	RHO, DIRT_PERCENT,
	// Total number of items for array declaration
	NUMBER_OF_DOUBLES
};

#define FLOW( i ) ( FLOW0 + i )
#define TEMP( i ) ( TEMP0 + i )


/**
 * Grid cell int properties
 */
enum IntIndex {
	// Values stored on faces (must be in triplicate, must come first)
	FACE_TYPE0, FACE_TYPE1, FACE_TYPE2,
	// Values stored in cells
	USER_INT,
	// Total number of items for array declaration
	NUMBER_OF_INTS
};

#define FACE_TYPE( i ) ( FACE_TYPE0 + i )


/**
 * Possible face types for grid cell
 */
enum FaceTypes {
	OPEN_FACE,
	CLOSED_FACE,
	AIR_FACE
};


/**
 * Stores data for a fluid grid cell
 */
struct CellData {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * Grid cell double properties
	 */
	union {

		// Bits can be interpreted as a struct with members
		// or as an array indexed by the DoubleIndex enum values

		struct {

			double flow[ 3 ];
			mutable double temp[ 3 ];
			double rho;
			double dirt;
		};

		double doubleArray[ NUMBER_OF_DOUBLES ];
	};

	/**
	 * Grid cell int properties
	 */
	union {

		// Bits can be interpreted as a struct with members
		// or as an array indeed by the IntIndex enum values

		struct {

			int faceType[ 3 ];
			mutable int userInt;
		};

		int intArray[ NUMBER_OF_INTS ];
	};
};


/**
 * The 3D grid of fluid cells
 */
struct FluidGrid3D {

	//------------
	// MEMBER DATA
	//------------

	/**
	 * Error code indicating a bad cell index
	 */
	static const uint BAD_INDEX;

	/**
	 * The dimensions of the grid (width, height, depth)
	 */
	uint dimensions[ 3 ];

	/**
	 * The length of the data array
	 */
	uint length;

	/**
	 * The actual array of cells in the grid
	 */
	CellData * data;

	/**
	 * The coordinates for the minimum extent of the grid
	 */
	double minPoint[ 3 ];

	/**
	 * The size of a cell in any given dimension (since the cells are cubes)
	 */
	double scale;


	//-------------
	// CONSTRUCTORS
	//-------------

	/**
	 * Dimension constructor
	 * @param w The width of the grid
	 * @param h The height of the grid
	 * @param d The depth of the grid
	 */
	FluidGrid3D( uint w, uint h, uint d );


	/**
	 * Destructor
	 */
	~FluidGrid3D();


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Computes the index of the cell at the given point in the grid (with error checking)
	 * @param i The column
	 * @param j The row
	 * @param k The layer
	 * @return The index into the data array of the specified cell
	 */
	inline uint index( int i, int j, int k ) const {

		if ( i < 0 || j < 0 || k < 0 ) {

			return BAD_INDEX;
		}

		if ( ( uint ) i >= dimensions[ 0 ] || ( uint ) j >= dimensions[ 1 ] || ( uint ) k >= dimensions[ 2 ] ) {

			return BAD_INDEX;
		}

		return dumbIndex( i, j, k );
	}


	/**
	 * Computes the index of the cell at the given point in the grid (without error checking)
	 * @param i The column
	 * @param j The row
	 * @param k The layer
	 * @return The index into the data array of the specified cell
	 */
	inline uint dumbIndex( int i, int j, int k ) const {

		return ( k + dimensions[ 2 ] * ( j + dimensions[ 1 ] * i ) );
	}


	/**
	 * Computes the index of the cell at the given point in the grid (wrapping within each dimension as needed)
	 * @param i The column
	 * @param j The row
	 * @param k The layer
	 * @return The index into the data array of the specified cell
	 */
	inline uint wrapIndex( int i, int j, int k ) const {

		i = ( i + dimensions[ 0 ] ) % dimensions[ 0 ];
		j = ( j + dimensions[ 1 ] ) % dimensions[ 1 ];
		k = ( k + dimensions[ 2 ] ) % dimensions[ 2 ];

		return dumbIndex( i, j, k );
	}


	/**
	 * Computes the index of the cell at the given point in the grid (clampping grid coordinates instead of wrapping)
	 * @param i The column
	 * @param j The row
	 * @param k The layer
	 * @return The index into the data array of the specified cell
	 */
	inline uint clampIndex( int i, int j, int k ) const {

		i = ( i < 0 ) ? 0 : ( ( ( uint ) i >= dimensions[ 0 ] ) ? ( ( int ) dimensions[ 0 ] - 1 ) : i );
		j = ( j < 0 ) ? 0 : ( ( ( uint ) j >= dimensions[ 1 ] ) ? ( ( int ) dimensions[ 1 ] - 1 ) : j );
		k = ( k < 0 ) ? 0 : ( ( ( uint ) k >= dimensions[ 2 ] ) ? ( ( int ) dimensions[ 2 ] - 1 ) : k );

		return dumbIndex( i, j, k );
	}


	/**
	 * Computes the grid coordinates of the given cell
	 * @param index The index into the data array of the cell
	 * @param i Column gets added to existing value
	 * @param j Row gets added to existing value
	 * @param k Layer gets added to existing value
	 */
	inline void unIndex( uint index, int & i, int & j, int & k ) const {

		k += index % dimensions[ 2 ];
		index /= dimensions[ 2 ];
		j += index % dimensions[ 1 ];
		index /= dimensions[ 1 ];
		i += index;
	}


	/**
	 * Computes the index into the data array of a neighboring cell (wrapping within each dimension as needed)
	 * @param index The index of the cell
	 * @param i The neighbor's column
	 * @param j The neighbor's row
	 * @param k The neighbor's layer
	 * @return The index of the neighbor
	 */
	inline uint wrapNeighbor( uint index, int i, int j, int k ) const {

		unIndex( index, i, j, k );

		return wrapIndex( i, j, k );
	}


	/**
	 * Computes the index of the cell that is "forward" of the given cell in the given direction
	 * @param index The index of the cell to check
	 * @param direction The direction to check in
	 * @return The index of the cell that is "forward" of the given cell in the given direction
	 */
	inline uint forward( uint index, int direction ) const {

		switch ( direction ) {

			case 0: {

				uint i = ( index / ( dimensions[ 2 ] * dimensions[ 1 ] ) );

				return index + ( ( ( i + 1 ) % dimensions[ 0 ] ) - i ) * dimensions[ 2 ] * dimensions[ 1 ];
			}

			case 1: {

				uint j = ( ( index / dimensions[ 2 ] ) & dimensions[ 1 ] );

				return index + ( ( ( j + 1 ) % dimensions[ 1 ] ) - j ) * dimensions[ 2 ];
			}

			case 2: {

				uint k = ( index % dimensions[ 2 ] );

				return index + ( ( ( k + 1 ) % dimensions[ 2 ] ) - k );
			}
		}

		return index;
	}


	/**
	 * Computes the index of the cell that is "backward" of the given cell in the given direction
	 * @param index The index of the cell to check
	 * @param direction The direction to check in
	 * @return The index of the cell that is "backward" of the given cell in the given direction
	 */
	inline uint backward( uint index, int direction ) const {

		switch ( direction ) {

			case 0: {

				uint i = ( index / ( dimensions[ 2 ] * dimensions[ 1 ] ) );

				return index + ( ( ( i + dimensions[ 0 ] - 1 ) % dimensions[ 0 ] ) - i ) * dimensions[ 2 ] * dimensions[ 1 ];
			}

			case 1: {

				uint j = ( ( index / dimensions[ 2 ] ) % dimensions[ 1 ] );

				return index + ( ( ( j + dimensions[ 1 ] - 1 ) % dimensions[ 1 ] ) - j ) * dimensions[ 2 ];
			}

			case 2: {

				uint k = ( index % dimensions[ 2 ] );

				return index + ( ( ( k + dimensions[ 2 ] - 1 ) % dimensions[ 2 ] ) - k );
			}
		}

		return index;
	}


	/**
	 * Determines the indices and weights of the cells adjacent to the cell containing the given point (uses clamping as needed)
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param indices Stores the indices of the cells adjacent to the cell containing the point
	 * @param weights Stores the weights of the cells adjacent to the cell containing the point
	 */
	void cellsAndWeights( double x, double y, double z, uint indices[ 8 ], double weights[ 8 ] ) const;


	/**
	 * Determines the indices and weights of the cells adjacent to the cell containing the given point (bounds check failure results in BAD_INDEX value)
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param indices Stores the indices of the cells adjacent to the cell containing the point
	 * @param weights Stores the weights of the cells adjacent to the cell containing the point
	 */
	void cellsAndWeightsNullable( double x, double y, double z, uint indices[ 8 ], double weights[ 8 ] ) const;


	/**
	 * Determines the indices and weights of the cells adjacent to the cell containing the given point (uses wrapping as needed)
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param indices Stores the indices of the cells adjacent to the cell containing the point
	 * @param weights Stores the weights of the cells adjacent to the cell containing the point
	 */
	void cellsAndWeightsWrap( double x, double y, double z, uint indices[ 8 ], double weights[ 8 ] ) const;


	//-----------------
	// INTERNAL CLASSES
	//-----------------

	/**
	 * Forward declaration of const version of the iterator
	 */
	class const_iterator;


	/**
	 * A grid iterator
	 */
	class iterator : std::iterator<bidirectional_iterator_tag, CellData> {

		friend class const_iterator;

	private:

		//------------
		// MEMBER DATA
		//------------

		/**
		 * Pointer to the data to iterate over
		 */
		FluidGrid3D * data;

		/**
		 * The index of the current cell
		 */
		uint index;


	public:

		//-------------
		// CONSTRUCTORS
		//-------------

		/**
		 * Default constructor
		 */
		inline iterator() : data( 0 ), index( 0 ) {

			return;
		}


		/**
		 * Grid constructor
		 * @param data The grid to iterate over
		 */
		inline iterator( FluidGrid3D * data ) : data( data ), index( 0 ) {

			return;
		}


		/**
		 * Full constructor
		 * @param data The grid to iterate over
		 * @param index The index to start at
		 */
		inline iterator( FluidGrid3D * data, uint index ) : data( data ), index( index ) {

			return;
		}


		//----------
		// OPERATORS
		//----------

		/**
		 * Dereference operator
		 * @return A reference to the current cell
		 */
		inline CellData & operator * () {

			return data->data[ index ];
		}


		/**
		 * Indirection operator
		 * @return A pointer to the current cell
		 */
		inline CellData * operator -> () {

			return data->data + index;
		}


		/**
		 * Post increment operator
		 * @return A reference to the iterator after incrementing the index
		 */
		iterator & operator ++ ();


		/**
		 * Pre increment operator
		 * @return The iterator before incrementing the index
		 */
		inline iterator operator ++ ( int ) {

			iterator save = *this;
			++*this;

			return save;
		}


		/**
		 * Post decrement operator
		 * @return A reference to the iterator after decrementing the index
		 */
		iterator & operator -- ();


		/**
		 * Pre decrement operator
		 * @return The iterator before decrementing the index
		 */
		inline iterator operator -- ( int ) {

			iterator save = *this;
			--*this;

			return save;
		}


		/**
		 * Equality operator
		 * @param other The iterator to compare to
		 * @return TRUE if the iterators have the same index, FALSE otherwise
		 */
		inline bool operator == ( const iterator & other ) const {

			return index == other.index;
		}


		/**
		 * Inequality operator
		 * @param other The iterator to compare to
		 * @return TRUE if the iterators have different indices, FALSE otherwise
		 */
		inline bool operator != ( const iterator & other ) const {

			return index != other.index;
		}


		/**
		 * Gets the cell data for the cell that is forward of the current cell in the given direction
		 * @param direction The direction to check in
		 * @return The cell data for the cell that is forward of the current cell in the given direction
		 */
		inline CellData & forwardCell( int direction ) {

			return data->data[ data->wrapNeighbor( index, +( direction == 0 ), +( direction == 1 ), +( direction == 2 ) ) ];
		}


		/**
		 * Gets the cell data for the cell that is backward of the current cell in the given direction
		 * @param direction The direction to check in
		 * @return The cell data for the cell that is backward of the current cell in the given direction
		 */
		inline CellData & backwardCell( int direction ) {

			return data->data[ data->wrapNeighbor( index, -( direction == 0 ), -( direction == 1 ), -( direction == 2 ) ) ];
		}


		/**
		 * Gets the index of the current cell
		 * @return The index of the current cell
		 */
		inline const uint & getIndex() const {

			return index;
		}
	};


	/**
	 * A const iterator for the grid
	 */
	class const_iterator : std::iterator<bidirectional_iterator_tag, CellData> {

	private:

		//------------
		// MEMBER DATA
		//------------

		/**
		 * A const pointer to the grid to iterate over
		 */
		const FluidGrid3D * data;

		/**
		 * The index of the current cell
		 */
		uint index;


	public:

		//-------------
		// CONSTRUCTORS
		//-------------

		/**
		 * Default constructor
		 */
		inline const_iterator() : data( 0 ), index( 0 ) {

			return;
		}


		/**
		 * Non-const copy constructor
		 * @param other The non-const iterator to copy
		 */
		inline const_iterator( const FluidGrid3D::iterator & other ) : data( other.data ), index( other.index ) {

			return;
		}


		/**
		 * Grid constructor
		 * @param data The grid to iterate over
		 */
		inline const_iterator( const FluidGrid3D * data ) : data( data ), index( 0 ) {

			return;
		}


		/**
		 * Full constructor
		 * @param data The grid to iterate over
		 * @param index The index to start at
		 */
		inline const_iterator( const FluidGrid3D * data, uint index ) : data( data ), index( index ) {

			return;
		}


		//----------
		// OPERATORS
		//----------

		/**
		 * Dereference operator
		 * @return A reference to the cell data at the current index
		 */
		inline const CellData & operator * () const {

			return data->data[ index ];
		}


		/**
		 * Indirection operator
		 * @return A pointer to the cell data at the current index
		 */
		inline const CellData * operator -> () const {

			return data->data + index;
		}


		/**
		 * Post increment operator
		 * @return A reference to the iterator after incrementing the index
		 */
		const_iterator & operator ++ ();


		/**
		 * Pre increment operator
		 * @return A copy of the iterator before incrementing the index
		 */
		inline const_iterator operator ++ ( int ) {

			const_iterator save = *this;
			++*this;

			return save;
		}


		/**
		 * Post decrement operator
		 * @return A reference to the iterator after decrementing the index
		 */
		const_iterator & operator -- ();


		/**
		 * Pre decrement operator
		 * @return A copy of the iterator before decrementing the index
		 */
		inline const_iterator operator -- ( int ) {

			const_iterator save = *this;
			--*this;

			return save;
		}


		/**
		 * Equality operator
		 * @param other The iterator to compare to
		 * @return TRUE if the iterators have the same index, FALSE otherwise
		 */
		inline bool operator == ( const const_iterator & other ) const {

			return index == other.index;
		}


		/**
		 * Inequality operator
		 * @param other The iterator to compare to
		 * @return TRUE if the iterators have different indices, FALSE otherwise
		 */
		inline bool operator != ( const const_iterator & other ) const {

			return index != other.index;
		}


		/**
		 * Gets the cell data of the cell that is forward of the current cell in the given direction
		 * @param direction The direction to check in
		 * @return The cell data of the cell that is forward of the current cell in the given direction
		 */
		inline const CellData & forwardCell( int direction ) const {

			return data->data[ data->wrapNeighbor( index, +( direction == 0 ), +( direction == 1 ), +( direction == 2 ) ) ];
		}


		/**
		 * Gets the cell data of the cell that is backward of the current cell in the given direction
		 * @param direction The direction to check in
		 * @return The cell data of the cell that is backward of the current cell in the given direction
		 */
		inline const CellData & backwardCell( int direction ) const {

			return data->data[ data->wrapNeighbor( index, -( direction == 0 ), -( direction == 1 ), -( direction == 2 ) ) ];
		}


		/**
		 * Gets the index of the current cell
		 * @return The index of the current cell
		 */
		inline const uint & getIndex() const {

			return index;
		}
	};


	//----------
	// FUNCTIONS
	//----------

	/**
	 * Gets the value of the specified double attribute in the specified cell (uses clamping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified double attribute
	 */
	inline const double & doubleValue( int i, int j, int k, int value ) const {

		return data[ clampIndex( i, j, k ) ].doubleArray[ value ];
	}


	/**
	 * Gets the value of the specified integer attribute in the specified cell (uses clamping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified integer attribute
	 */
	inline const int & intValue( int i, int j, int k, int value ) const {

		return data[ clampIndex( i, j, k ) ].intArray[ value ];
	}


	/**
	 * Gets the value of the specified double attribute in the specified cell (uses clamping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified double attribute
	 */
	inline double & doubleValue( int i, int j, int k, int value ) {

		return data[ clampIndex( i, j, k ) ].doubleArray[ value ];
	}


	/**
	 * Gets the value of the specified integer attribute in the specified cell (uses clamping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified integer attribute
	 */
	inline int & intValue( int i, int j, int k, int value ) {

		return data[ clampIndex( i, j, k ) ].intArray[ value ];
	}


	/**
	 * Gets the value of the specified double attribute in the specified cell (uses wrapping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified double attribute
	 */
	inline const double & doubleValueWrap( int i, int j, int k, int value ) const {

		return data[ wrapIndex( i, j, k ) ].doubleArray[ value ];
	}


	/**
	 * Gets the value of the specified integer attribute in the specified cell (uses wrapping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified integer attribute
	 */
	inline const int & intValueWrap( int i, int j, int k, int value ) const {

		return data[ wrapIndex( i, j, k ) ].intArray[ value ];
	}


	/**
	 * Gets the value of the specified double attribute in the specified cell (uses wrapping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified double attribute
	 */
	inline double & doubleValueWrap( int i, int j, int k, int value ) {

		return data[ wrapIndex( i, j, k ) ].doubleArray[ value ];
	}


	/**
	 * Gets the value of the specified integer attribute in the specified cell (uses wrapping)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified integer attribute
	 */
	inline int & intValueWrap( int i, int j, int k, int value ) {

		return data[ wrapIndex( i, j, k ) ].intArray[ value ];
	}


	/**
	 * Gets the value of the specified double attribute in the specified cell (no error checking)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified double attribute
	 */
	inline const double & doubleValueFast( int i, int j, int k, int value ) const {

		return data[ dumbIndex( i, j, k ) ].doubleArray[ value ];
	}


	/**
	 * Gets the value of the specified integer attribute in the specified cell (no error checking)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified integer attribute
	 */
	inline const int & intValueFast( int i, int j, int k, int value ) const {

		return data[ dumbIndex( i, j, k ) ].intArray[ value ];
	}


	/**
	 * Gets the value of the specified double attribute in the specified cell (no error checking)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified double attribute
	 */
	inline double & doubleValueFast( int i, int j, int k, int value ) {

		return data[ dumbIndex( i, j, k ) ].doubleArray[ value ];
	}


	/**
	 * Gets the value of the specified integer attribute in the specified cell (no error checking)
	 * @param i The column
	 * @param j The row
	 * @param k The column
	 * @param value The attribute to get
	 * @return The value of the specified integer attribute
	 */
	inline int & intValueFast( int i, int j, int k, int value ) {

		return data[ dumbIndex( i, j, k ) ].intArray[ value ];
	}


	/**
	 * Interpolate the value of the specified double attribute for the given point
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordiante of the point
	 * @param value The attribute to get
	 * @return The interpolated value of the specified attribute at the given point
	 */
	double cellValue( double x, double y, double z, int value ) const;


	/**
	 * Interpolate a rho-dependent value of the specified double attribute for the given point
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param value The attribute to get
	 * @return The value of the specified attribute at the given point
	 */
	double cellValueIfRho( double x, double y, double z, int value ) const;


	/**
	 * Interpolates the value of the specified face attribute at the given point
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordiante of the point
	 * @param value The attribute to get
	 * @return The value of the specified face attribute at the given point
	 */
	double faceValue( double x, double y, double z, int value ) const;


	/**
	 * Interpolates the value of the specified face attribute at the given point without scaling by the size of a cell
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordiante of the point
	 * @param value The attribute to get
	 * @return The value of the specified face attribute at the given point
	 */
	double faceValueNoScale( double x, double y, double z, int value ) const;


	/**
	 * Gets the velocity vector in the special case of a face-centered point
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param direction The face to compute the velocity for
	 * @param velocity Stores the velocity vector
	 */
	void velocityOnFace( int x, int y, int z, int direction, double velocity[ 3 ] );


	/**
	 * Gets the velocity vector in the special case of a face-centered point without scaling
	 * @param x The x-coordinate of the point
	 * @param y The y-coordinate of the point
	 * @param z The z-coordinate of the point
	 * @param velocity Stores the velocity vector
	 */
	inline void velocityNoScale( double x, double y, double z, double velocity[ 3 ] ) {

		velocity[ 0 ] = faceValueNoScale( x, y, z, FLOW0 );
		velocity[ 1 ] = faceValueNoScale( x, y, z, FLOW1 );
		velocity[ 2 ] = faceValueNoScale( x, y, z, FLOW2 );
	}


	/**
	 * Gathers fluid statistics
	 * @param maxVelocityComponent Stores the maximum velocity component
	 * @param kineticEnergy Stores the fluids kinetic energy
	 * @param totalRho Stores the fluid's total Rho
	 */
	void gatherStatistics( double * maxVelocityComponent = 0, double * kineticEnergy = 0, double * totalRho = 0 ) const;


	/**
	 * Advects fluid grid velocities (wraps unless faces are closed or air)
	 * @param dt The timestep
	 * @param g The gravity (???)
	 * @param waterEpsilon Regulates how much dirt the water can carry (default 0.05)
	 */
	void advectVelocities( double dt, const double g[ 3 ], double waterEpsilon = 0.05 );


	/**
	 * Advects the contents of the grid cells (including dirt)
	 * @param dt The timestep
	 * @param dRho ???
	 * @param dDirt An array storing the amount of dirt in each cell of the grid
	 */
	void advectContents( double dt, double * dRho, double * dirt );


	/**
	 * Extrapolates the flow of the fluid in the cell based on the cell's neighbors
	 * @param cell The index of the cell
	 * @param direction The direction to extrapolate in
	 */
	void extrapolateFlow( uint cell, int direction );


	/**
	 * Gets an iterator for the beginning of the grid
	 * @return An iterator starting at the beginning of the grid
	 */
	inline iterator begin() {

		return iterator( this );
	}


	/**
	 * Gets an iterator at the back of the grid
	 * @return An iterator starting at the end of the grid
	 */
	inline iterator end() {

		return iterator( this, length );
	}


	/**
	 * Gets a const iterator for the beginning of the grid
	 * @return A const iterator starting at the beginning of the grid
	 */
	inline const_iterator begin() const {

		return const_iterator( this );
	}


	/**
	 * Gets a const iterator at the back of the grid
	 * @return A const iterator starting at the end of the grid
	 */
	inline const_iterator end() const {

		return const_iterator( this, length );
	}


	/**
	 * Sets the face type of all edges that are open to the air to "AIR_FACE"
	 */
	void setAllOpenEdges();


	/**
	 * Updates the grid's faces with new types
	 * @param isSolid The function to call to determine solidity
	 */
	template <typename point> void setSolidity( bool ( *isSolid )( const point & p ) );


	/**
	 * Updates the grid's with new sinks
	 * @param isSink The function to call to determine if a point is a sink
	 */
	template <typename point> void setSinks( bool ( *isSink )( const point & p ) );


	/**
	 * Updates the grid with new fluid amounts
	 * @param injectHere The function to call to inject fluid
	 */
	template <typename point> void injectFluid( bool ( *injectHere )( const point & p ) );


	/**
	 * Computes the amount of dirt the water drops at a given point (or takes, if the return value is negative) in one second
	 * @param p The point to check
	 * @param speedScale The amount to scale the time
	 * @return The amount of dirt the water drops/takes at the given point
	 */
	template <typename point> double depositionRate( const point & p, double speedScale );


	/**
	 * Must be called before injecting dirt
	 */
	void preInjection();


	/**
	 * Adds (or removes, for negative values) dirt at the given point
	 * @param p The point to add dirt to (take dirt from)
	 * @param alluvium The amount of dirt to add/take
	 */
	template <typename point> void injectAlluvium( const point & p, double alluvium );


	/**
	 * Must be called after injecting dirt
	 * @param speedScale The amount to scale the time
	 * @param dropSpeed The amount to drop the speed (of what???)
	 */
	void postInjection( double speedScale, double dropSpeed );
};


/**
 * Updates the grid's faces with new types
 * @param isSolid The function to call to determine solidity
 */
template <typename point> void FluidGrid3D::setSolidity( bool ( *isSolid )( const point & p ) ) {

	double dx;
	double dy;
	double dz;
	uint index = 0;
	dx = minPoint[ 0 ];

	uint x;
	uint y;
	uint z;

	for ( x = 0; x < dimensions[ 0 ]; ++x, dx += scale ) {

		dy = minPoint[ 1 ];

		for ( y = 0; y < dimensions[ 1 ]; ++y, dy += scale ) {

			dz = minPoint[ 2 ];

			for ( z = 0; z < dimensions[ 2 ]; ++z, dz += scale ) {

				// Do x-direction first
				{
					point p( dx, dy + 0.5 * scale, dz + 0.5 * scale );

					if ( isSolid( p ) ) {

						data[ index ].faceType[ 0 ] = CLOSED_FACE;
						data[ index ].flow[ 0 ] = 0.0;
					}
					else if ( data[ index ].faceType[ 0 ] == CLOSED_FACE ) {

						data[ index ].faceType[ 0 ] = OPEN_FACE;
					}
				}

				// Do y-direction next
				{
					point p( dx + 0.5 * scale, dy, dz + 0.5 * scale );

					if ( isSolid( p ) ) {

						data[ index ].faceType[ 1 ] = CLOSED_FACE;
						data[ index ].flow[ 1 ] = 0.0;
					}
					else if ( data[ index ].faceType[ 1 ] == CLOSED_FACE ) {

						data[ index ].faceType[ 1 ] = OPEN_FACE;
					}
				}

				// Finally, do z-direction
				{
					point p( dx + 0.5 * scale, dy + 0.5 * scale, dz );

					if ( isSolid( p ) ) {

						data[ index ].faceType[ 2 ] = CLOSED_FACE;
						data[ index ].flow[ 2 ] = 0.0;
					}
					else if ( data[ index ].faceType[ 2 ] == CLOSED_FACE ) {

						data[ index ].faceType[ 2 ] = OPEN_FACE;
					}
				}

				++index;
			}
		}
	}
}


/**
 * Updates the grid's with new sinks
 * @param isSink The function to call to determine if a point is a sink
 */
template <typename point> void FluidGrid3D::setSinks( bool ( *isSink )( const point & p ) ) {

	double dx;
	double dy;
	double dz;
	uint index = 0;
	dx = minPoint[ 0 ];

	uint x;
	uint y;
	uint z;

	for ( x = 0; x < dimensions[ 0 ]; ++x, dx += scale ) {

		dy = minPoint[ 1 ];

		for ( y = 0; y < dimensions[ 1 ]; ++y, dy += scale ) {

			dz = minPoint[ 2 ];

			for ( z = 0; z < dimensions[ 2 ]; ++z, dz += scale ) {

				// Start with x-direction
				{
					point p( dx, dy + 0.5 * scale, dz + 0.5 * scale );

					if ( isSink( p ) ) {

						data[ index ].faceType[ 0 ] = AIR_FACE;
					}
					else if ( data[ index ].faceType[ 0 ] == AIR_FACE ) {

						data[ index ].faceType[ 0 ] = OPEN_FACE;
					}
				}

				// Now do y-direction
				{
					point p( dx + 0.5 * scale, dy, dz + 0.5 * scale );

					if ( isSink( p ) ) {

						data[ index ].faceType[ 1 ] = AIR_FACE;
					}
					else if ( data[ index ].faceType[ 1 ] == AIR_FACE ) {

						data[ index ].faceType[ 1 ] = OPEN_FACE;
					}
				}

				// Finally do the z-direction
				{
					point p( dx + 0.5 * scale, dy + 0.5 * scale, dz );

					if ( isSink( p ) ) {

						data[ index ].faceType[ 2 ] = AIR_FACE;
					}
					else if ( data[ index ].faceType[ 2 ] == AIR_FACE ) {

						data[ index ].faceType[ 2 ] = OPEN_FACE;
					}
				}

				++index;
			}
		}
	}
}


/**
 * Updates the grid with new fluid amounts
 * @param injectHere The function to call to inject fluid
 */
template <typename point> void FluidGrid3D::injectFluid( bool ( *injectHere )( const point & p ) ) {

	double dx;
	double dy;
	double dz;
	uint index = 0;
	dx = minPoint[ 0 ] + scale / 2.0;

	uint x;
	uint y;
	uint z;

	for ( x = 0; x < dimensions[ 0 ]; ++x, dx += scale ) {

		dy = minPoint[ 1 ] + scale / 2.0;

		for ( y = 0; y < dimensions[ 1 ]; ++y, dy += scale ) {

			dz = minPoint[ 2 ] + scale / 2.0;

			for ( z = 0; z < dimensions[ 2 ]; ++z, dz += scale ) {

				point p( dx, dy, dz );

				if ( injectHere( p ) ) {

					data[ index ].rho = 1.0;
				}

				++index;
			}
		}
	}
}


/**
 * Computes the amount of dirt the water drops at a given point (or takes, if the return value is negative) in one second
 * @param p The point to check
 * @param speedScale The amount to scale the time
 * @return The amount of dirt the water drops/takes at the given point
 */
template <typename point> double FluidGrid3D::depositionRate( const point & p, double speedScale ) {

	double x = p[ 0 ];
	double y = p[ 1 ];
	double z = p[ 2 ];

	double rho = cellValue( x, y, z, RHO );
	double dirt = cellValue( x, y, z, DIRT_PERCENT );

	if ( rho <= 0.5 ) {

		return dirt;
	}

	double speed = sqrt( square( faceValue( x, y, z, FLOW0 ) ) + square( faceValue( x, y, z, FLOW1 ) ) + square( faceValue( x, y, z, FLOW2 ) ) );

	return dirt - speedScale * speed * rho;
}


/**
 * Adds (or removes, for negative values) dirt at the given point
 * @param p The point to add dirt to (take dirt from)
 * @param alluvium The amount of dirt to add/take
 */
template <typename point> void FluidGrid3D::injectAlluvium( const point & p, double alluvium ) {

	uint cells[ 8 ];
	double weights[ 8 ];
	cellsAndWeights( p[ 0 ], p[ 1 ], p[ 2 ], cells, weights );

	for ( int i = 0; i < 8; ++i ) {

		data[ cells[ i ] ].temp[ 1 ] += alluvium * weights[ i ];
	}
}
