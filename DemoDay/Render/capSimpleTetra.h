#pragma once

#include "chai3d.h"
#include <math.h>

class capSimpleTetra : public cMesh
{
private:
	void BuildTetrahedron(double edge)
	{
		double faceHeight = (sqrt((float)3) / 2 ) * edge;
		
		//Add vertices
		cMesh::newVertex(0,0,0);
		cMesh::newVertex(edge / 2.0, 0, 0 - faceHeight);
		cMesh::newVertex(edge / -2.0, 0, 0 - faceHeight);
		cMesh::newVertex(0, sqrt((float)6) / 3.0 * edge, faceHeight * -2 / 3);

		//Create Triangles
		cMesh::newTriangle(2, 1, 0);
		cMesh::newTriangle(0, 1, 3);
		cMesh::newTriangle(3, 2, 0);
		cMesh::newTriangle(1, 2, 3);
	}

public:
	capSimpleTetra(cWorld* world, double edgeLength) : cMesh(world) 
	{
		BuildTetrahedron(edgeLength);
	}

};