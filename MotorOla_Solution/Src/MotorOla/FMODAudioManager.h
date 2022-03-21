//#pragma once
//
//#include "AudioManager.h"
//#include <fmod.hpp>
//#include <fmod_errors.h>
//#include <map>
//
//class FMODAudioManager : public AudioManager {
//public:
//	FMODAudioManager();
//	~FMODAudioManager();
//
//	void init() override;
//	void update() override;
//
//	virtual void loadMusic(int channel, const char* fileName)override;
//	virtual void playMusic(int channel, bool loops ) override;
//	virtual void stopMusic(int channel) override;
//	virtual void togglePause(int channel) override;
//	virtual void setVolume(int chan, int volume) override;
//	virtual void setPitch(int chan, int pitch) override;
//	virtual void fadeIn(int chan) override;
//	virtual void fadeOut(int chan)override;
//	void checkError(FMOD_RESULT result ) ;
//
//private:
//	FMOD::System* system;
//	FMOD_RESULT result;
//	FMOD::Channel* channel[24];
//	FMOD::Sound* sound[24];
//};