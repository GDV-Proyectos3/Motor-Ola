#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <string>
#include "./utils/Singleton.h"

MOTOR_API class AudioManager : public Singleton<AudioManager> {
	friend Singleton<AudioManager>;
public:
	AudioManager() {};
	virtual ~AudioManager() {};

	MOTOR_API virtual void init() = 0;
	MOTOR_API virtual void update() = 0;

	MOTOR_API virtual void loadMusic(int channel, const char* fileName) = 0;
	MOTOR_API virtual void playMusic(int channel, bool loops) = 0;
	MOTOR_API virtual void stopMusic(int channel) = 0;
	MOTOR_API virtual void togglePause(int channel) = 0;
	MOTOR_API virtual void setVolume(int chan, int volume) = 0;
	MOTOR_API virtual void setPitch(int chan, int pitch) = 0;
	MOTOR_API virtual void fadeIn(int chan) = 0;
	MOTOR_API virtual void fadeOut(int chan) = 0;
};