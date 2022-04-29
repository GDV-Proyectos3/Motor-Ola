#include "AudioSource.h"
#include "FMODAudioManager.h"

AudioSource::AudioSource(int channel,FMODAudioManager* a,const char* s):aud(a),_channel(channel)
{
	aud->loadMusic(_channel, s);
}

AudioSource::~AudioSource()
{
}

void AudioSource::play()
{
	aud->playMusic(_channel, false);
}

void AudioSource::pause()
{
	aud->stopMusic(_channel);
}
