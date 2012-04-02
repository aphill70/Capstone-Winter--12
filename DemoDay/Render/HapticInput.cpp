#include "HapticInput.h"

HapticInput::HapticInput()
{

}

void HapticInput::SetCursor(cGenericObject* c)
{
		cursor = c;
}

void HapticInput::GetCursorPosition(cVector3d& pos)
{
	pos = cursor->getPos();
}

void HapticInput::UpdateCursor(dmat4x4 CamToWorld)
{
	if(modeFlag->GetMode() != EXPLORE)
		return;

	cVector3d HapticPos;
	hapticDevice->GetCursorPosition(HapticPos);

	//just adjusting stuff for now
	HapticPos.x = HapticPos.x / hapticDevice->GetHapticRadius() * BOX_RADIUS;
	HapticPos.y = HapticPos.y / hapticDevice->GetHapticRadius() * BOX_RADIUS;
	HapticPos.z = ( -1 * HapticPos.z / hapticDevice->GetHapticRadius() + 1) * BOX_RADIUS + MIN_DIST;

	dvec4 point = CamToWorld * dvec4(HapticPos.x, HapticPos.y, HapticPos.z, 1);

	cout << HapticPos.x << " " << HapticPos.y << " " << HapticPos.z << endl;
	//cursor->setPos(HapticPos);
	cursor->setPos(cVector3d(point.x, point.y, point.z));
}

void HapticInput::SetFlag(ModeFlag* flag)
{
	modeFlag = flag;
	modeFlag->AddListener(this);
}

void HapticInput::ModeChange()
{
	Mode m = modeFlag->GetMode();
	if(m == FREE)
	{
		cursor->setShowEnabled(false);
	}
	else if(m == EXPLORE)
	{
		cursor->setShowEnabled(true);
	}
}