#include "OctTree.h"


//-------------
// CONSTRUCTORS
//-------------

/**
 * Default constructor
 */
OctTree::OctTree() : root( new OctTreeNode() ), circumcenters(), tetrahedrons() {

	return;
}


/**
 * Destructor
 */
OctTree::~OctTree() {

	if ( root ) {

		delete root;
	}

	circumcenters.clear();
	tetrahedrons.clear();
}


//----------
// FUNCTIONS
//----------

/**
 * Adds a new circumcenter to the hierarchy
 * @param point The location of the circumcenter
 * @param cell The cell associated with the circumcenter
 */
void OctTree::add( const Point & point, Cell_handle cell ) {

	circumcenters.push_back( new Circumcenter( point, cell ) );
	root->add( circumcenters.back(), false );
}


/**
 * Adds a new tetrahedron to the hierarchy
 * @param cell The cell associated with the tetrahedron
 */
void OctTree::add( Cell_handle cell ) {

	tetrahedrons.push_back( new Tetrahedron( cell ) );
	root->add( tetrahedrons.back(), true );
}


/**
 * Checks to see if the given point is in the hierarchy's space
 * @param point The point to check
 * @return TRUE if the point is in the root node, FALSE otherwise
 */
bool OctTree::contains( const Point & point ) const {

	return root->contains( point );
}


/**
 * Computes the list of pairs to check by partitioning the space
 * @param pairs The list of pairs to add to while partitioning
 */
void OctTree::partition( vector< pair< Circumcenter*, vector<Tetrahedron*> > > & pairs ) {

	vector<OctTreeNode*> nodesToProcess;
	root->split( 0, nodesToProcess );
	int size = nodesToProcess.size();
	int csize;
	int tsize;
	int c;
	int t;
	vector<Tetrahedron*> tets;

	#pragma omp parallel for private(csize,tsize,c,t,tets)
	for ( int i = 0; i < size; ++i ) {

		// TODO: Eventually this list will be a GPU-ready list of GPU-ready objects or object pairs (THRUST)
		
		csize = nodesToProcess[ i ]->circumcenters.size();
		tsize = nodesToProcess[ i ]->tetrahedrons.size();

		for ( c = 0; c < csize; ++c ) {

			tets.clear();

			for ( t = 0; t < tsize; ++t ) {

				tets.push_back( nodesToProcess[ i ]->tetrahedrons[ t ] );
			}

			#pragma omp critical(addToPairs)
			{
				pairs.push_back( pair< Circumcenter*, vector<Tetrahedron*> >( nodesToProcess[ i ]->circumcenters[ c ], tets ) );
			}
		}
	}
}
