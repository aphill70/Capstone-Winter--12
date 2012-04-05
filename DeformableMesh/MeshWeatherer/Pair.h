#pragma once

#include "Circumcenter.h"
#include "Tetrahedron.h"

#ifndef __host__
#include <cuda.h>
#include <host_defines.h>
#endif

struct Pair {
	Circumcenter circumcenter;
	Tetrahedron tetrahedron;
	__host__ __device__ Pair() : circumcenter(), tetrahedron() {}
	__host__ __device__ Pair( const Circumcenter & circumcenter, const Tetrahedron & tetrahedron ) : circumcenter( circumcenter ), tetrahedron( tetrahedron ) {}
	__host__ __device__ Pair( const Pair & other ) : circumcenter( other.circumcenter ), tetrahedron( other.tetrahedron ) {}
};
