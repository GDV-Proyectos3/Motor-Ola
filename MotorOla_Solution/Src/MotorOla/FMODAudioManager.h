#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include "AudioManager.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>

MOTOR_API class FMODAudioManager : public AudioManager {
public:
	FMODAudioManager();
	~FMODAudioManager();

	MOTOR_API void init() override;
	MOTOR_API void update() override;

	MOTOR_API virtual void loadMusic(int channel, const char* fileName)override;
	MOTOR_API virtual void playMusic(int channel, bool loops ) override;
	MOTOR_API virtual void stopMusic(int channel) override;
	MOTOR_API virtual void togglePause(int channel) override;
	MOTOR_API virtual void setVolume(int chan, int volume) override;
	MOTOR_API virtual void setPitch(int chan, int pitch) override;
	MOTOR_API virtual void fadeIn(int chan) override;
	MOTOR_API virtual void fadeOut(int chan)override;
	MOTOR_API void checkError(FMOD_RESULT result ) ;
	MOTOR_API int getCont();
	MOTOR_API bool getMute();
	MOTOR_API void setMute(bool m);
	MOTOR_API FMOD::Channel* getChannel(int i);

private:
	FMOD::System* system;
	FMOD_RESULT result;
	FMOD::Channel* channel[24];
	FMOD::Sound* sound[24];
	bool mute = false;
	int cont = 0;
};