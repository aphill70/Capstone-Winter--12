#include "HapticRenderer.h"

HapticRenderer::HapticRenderer()
{

}

void HapticRenderer::InitCursor(cWorld* w)
{
	world = w;
	cursor = new cShapeSphere(0.1);
	
	cMaterial hapticCursorMaterial;
	hapticCursorMaterial.m_ambient.set(0.5, 0.2, 0.0);
    hapticCursorMaterial.m_diffuse.set(1.0, 0.5, 0.0);
    hapticCursorMaterial.m_specular.set(1.0, 1.0, 1.0);
	cursor->m_material = hapticCursorMaterial;

	world->addChild(cursor);
	cursor->setShowEnabled(false);
}

cGenericObject* HapticRenderer::GetCursor()
{
	return cursor;
}