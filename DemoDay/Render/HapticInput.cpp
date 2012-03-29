#include "HapticInput.h"

HapticInput::HapticInput()
{

}

void HapticInput::SetCursor(cGenericObject* c)
{
		cursor = c;
}

void HapticInput::UpdateCursor(dmat4x4 CamToWorld)
{
	if(modeFlag->GetMode() != EXPLORE)
		return;

	cVector3d HapticPos;
	//hapticDevice->GetCursorPosition(HapticPos);

	cout << HapticPos.x << " " << HapticPos.y << " " << HapticPos.z << endl;
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