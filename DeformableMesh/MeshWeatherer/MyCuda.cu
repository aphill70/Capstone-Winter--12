#include "MyCuda.h"
#include "Pair.h"

#include <cstdio>


static void HandleError( cudaError_t err, const char * file, int line ) {

	if ( err != cudaSuccess ) {

		printf( "%s in %s at line %d\n", cudaGetErrorString( err ), file, line );
		exit( EXIT_FAILURE );
	}
}

#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))


__global__ void relabelKernel( const Pair * pairs, int * results, int pairsSize, int resultsSize ) {
	
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	int stride = blockDim.x * gridDim.x;

	while ( tid < pairsSize ) {

		if ( pairs[ tid ].tetrahedron.contains( pairs[ tid ].circumcenter.point ) ) {

			results[ pairs[ tid ].circumcenter.index ] = pairs[ tid ].tetrahedron.type;
		}

		tid += stride;
	}
}


void relabel( const Pair * pairs, int * results, int pairsSize, int resultsSize ) {

	int * dev_results;
	Pair * dev_pairs;
	
	HANDLE_ERROR( cudaMalloc( ( int ** ) &dev_results, resultsSize * sizeof( int ) ) );
	HANDLE_ERROR( cudaMemcpy( dev_results, results, resultsSize * sizeof( int ), cudaMemcpyHostToDevice ) );

	HANDLE_ERROR( cudaMalloc( ( Pair ** ) &dev_pairs, pairsSize * sizeof( Pair ) ) );
	HANDLE_ERROR( cudaMemcpy( dev_pairs, pairs, pairsSize * sizeof( Pair ), cudaMemcpyHostToDevice ) );

	cudaEvent_t start;
	cudaEvent_t stop;
	cudaEventCreate( &start );
	cudaEventCreate( &stop );
	cudaEventRecord( start, 0 );
	relabelKernel<<<1024,512>>>( dev_pairs, dev_results, pairsSize, resultsSize );
	cudaEventRecord( stop, 0 );
	cudaEventSynchronize( stop );
	float elapsedTime;
	HANDLE_ERROR( cudaEventElapsedTime( &elapsedTime, start, stop ) );
	printf( "Time to run kernel: %.4f sec\n", elapsedTime / 1000.0f );

	HANDLE_ERROR( cudaMemcpy( results, dev_results, resultsSize * sizeof( int ), cudaMemcpyDeviceToHost ) );

	HANDLE_ERROR( cudaFree( dev_results ) );
	HANDLE_ERROR( cudaFree( dev_pairs ) );
	HANDLE_ERROR( cudaEventDestroy( start ) );
	HANDLE_ERROR( cudaEventDestroy( stop ) );
}
