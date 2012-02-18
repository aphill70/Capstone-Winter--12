#include "AudioManager.h"

AudioManager::AudioManager(void) { }
AudioManager::~AudioManager(void) { }

void AudioManager::PlayZoom(bool ZoomIn) {
	if(ZoomIn)
		PlaySound("res/zoomin2.wav", NULL, SND_FILENAME | SND_ASYNC);
	else
		PlaySound("res/zoomout2.wav", NULL, SND_FILENAME | SND_ASYNC);
}