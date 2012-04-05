// ------------------------------
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// ------------------------------

#include "FluidRenderer.h"
#include "GEOFileFluid.h"
#include "IFluid.h"
#include "IHapticDevice.h"
#include "ITerrain.h"

IFluid* fluid = 0;
ITerrain* terrain = 0;
IHapticDevice* hapticDevice = 0;

int main(void) {
	GEOFileFluid geoFluid("../Fluids/fluidBake/brad_test_geometry", 50, 100);
	fluid = &geoFluid;
	
	cWorld world;
	FluidRenderer renderer;
	renderer.InitFluids(&world, fluid);
	
	while(true) {
		geoFluid.AdvanceFrame();
		renderer.UpdateFluid(&world);
	}
}