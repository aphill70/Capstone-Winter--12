#include "stepResults.h"


/**
 * Insertion stream operator
 * @param out The output stream to write to
 * @param sr The step results to print
 * @return A reference to the output stream for cascading
 */
ostream & operator << ( ostream & out, const stepResults & sr ) {

	out << "With " << sr.numVertices
		<< " vertices and " << sr.numTetrahedrons
		<< " tetrahedrons took " << sr.secondsTotal
		<< " seconds (motion " << sr.secondsMotion
		<< ", CGAL " << sr.secondsCGAL
		<< ", labeling " << sr.secondsLabeling
		<< ", analysis " << sr.secondsAnalysis
		<< ")";

	return out;
}
