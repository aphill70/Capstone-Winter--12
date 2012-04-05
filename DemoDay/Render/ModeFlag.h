#pragma once
#include <vector>
#include "IInput.h"

enum Mode
{
	FREE, EXPLORE
};

using namespace std;

class ModeFlag
{
private:
	vector<IInput*> listeners;
	Mode CurrentMode;

	void NotifyListeners()
	{
		for(unsigned int i = 0; i < listeners.size(); i++)
		{
			listeners[i]->ModeChange();
		}
	}

public:
	ModeFlag()
	{
		CurrentMode = FREE;
	}
	
	void AddListener(IInput* listener)
	{
		listeners.push_back(listener);
	}

	void SetMode(Mode m)
	{
		CurrentMode = m;
		NotifyListeners();
	}

	Mode GetMode()
	{
		return CurrentMode;
	}
};