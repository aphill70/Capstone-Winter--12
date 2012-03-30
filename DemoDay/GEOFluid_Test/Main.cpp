#include <direct.h>

#include "GEOFileFluid.h"

int main(void) {
	char path[1024];
	_getcwd(path, 1024);
	GEOFileFluid fluid("../Fluids/fluidBake/brad_test_geometry", 1, 1);
}