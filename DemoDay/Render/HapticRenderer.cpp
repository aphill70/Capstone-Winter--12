#include "HapticRenderer.h"

HapticRenderer::HapticRenderer()
{

}

void HapticRenderer::InitCursor(cWorld* w)
{
	world = w;
	cursor = new cShapeSphere(.1);
	world->addChild(cursor);
	cursor->setShowEnabled(false);
}

cGenericObject* HapticRenderer::GetCursor()
{
	return cursor;
}