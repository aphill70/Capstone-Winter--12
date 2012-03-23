#pragma once
#include <vector>
#include "IInput.h"

enum Mode
{
	FREE, EXPLORE
}

class ModeFlag
{
private:
	Mode CurrentMode;
	vector<IInput*> watchers;
	
	void NotifyWatchers()
	{
		for(int i = 0; i < watchers.length(); i++)
		{
			watchers[i]->ModeChange();
		}
	}

public:
	ModeFlag(){}
	
	void AddWatcher(IInput* watcher)
	{
		watchers.push_back(watcher);
	}

	void SetMode(Mode m)
	{
		CurrentMode = m;
		NotifyWatchers();
	}

	Mode GetMode()
	{
		return CurrentMode;
	}
};