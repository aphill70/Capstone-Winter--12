#include "TreeLoader.h"
static void GetKdTrees(struct kdtree ** dest, FrameData ** data, int len) {
	using namespace std;
	vector<GEOParticle*>::iterator itr;
	vector<GEOParticle*> *particles;
	dest = new struct kdtree*[len];
	
	for (int i = 0; i < len; i++) {
		particles = &data[i]->particleList;
		dest[i] = kd_create(3); 
		for (itr = particles->begin(); itr < particles->end(); itr++) {
			kd_insert3(dest[i], itr->GetPositionX(), itr->GetPositionY(), itr->GetPositionZ(), &(*itr));
		}
	}
}