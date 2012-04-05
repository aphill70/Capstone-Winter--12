#include "lib/kdtree.h"
#include "IFluidParticle.h"
#include <map>
#include <set>
#include <vector>
#include "FrameData.h"
#include "GEOParticle.h"

class TreeLoader {
public:
	/* creates list of kd trees from frame data */
	static void GetKdTrees(struct kdtree ** dest, FrameData ** data, int len);
}