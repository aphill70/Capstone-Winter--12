#include "ViscositySenseMode.h"

void ViscositySenseMode::Tick(void) {
}

IHapticMode* ViscositySenseMode::GetSingleton(void){
	if (! singleton) {
		singleton = new ViscositySenseMode();
	}
	return singleton;
}