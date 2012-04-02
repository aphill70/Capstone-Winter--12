// ------------------------------
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// ------------------------------

#include "GEOFileFluid.h"

int main(void) {
	GEOFileFluid fluid("../Fluids/fluidBake/brad_test_geometry", 1, 1);

	//-----------
	_CrtDumpMemoryLeaks();
	//-----------
}