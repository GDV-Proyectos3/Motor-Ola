#pragma once

#include <string>
#include "./utils/Singleton.h"


class AudioManager : public Singleton<AudioManager> {
public:
	AudioManager() {};
	virtual ~AudioManager() {};

	virtual void init() = 0;
	virtual void update() = 0;

	virtual bool loadMusic(int channel, const std::string& fileName) = 0;
	virtual void playMusic(int channel, bool loops) = 0;
	virtual void stopMusic(int channel) = 0;
	virtual void togglePause(int channel) = 0;
	virtual void setVolume(int chan, int volume) = 0;
	virtual void setPitch(int chan, int pitch) = 0;
	virtual void fadeIn(int chan) = 0;
	virtual void fadeOut(int chan) = 0;


};