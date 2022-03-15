#include "FMODAudioManager.h"

FMODAudioManager::FMODAudioManager()
{
}

FMODAudioManager::~FMODAudioManager()
{
	result = system->release();
	checkError(result);
}

void FMODAudioManager::init()
{
	result = FMOD::System_Create(&system);
	checkError(result);
	result = system->init(128, FMOD_INIT_NORMAL, 0);
	checkError(result);

}

void FMODAudioManager::update()
{
	result = system->update();
	checkError(result);
}

bool FMODAudioManager::loadMusic(int channel, const std::string& fileName)
{
	result = system->createSound(
		fileName, // path al archivo de sonido
		FMOD_DEFAULT, 
		0, 
		&sound[channel]);
	checkError(result);
}

void FMODAudioManager::playMusic(int chan, bool loops)
{
	result = system->playSound(sound[chan], 0, false, &channel[chan]);
	checkError(result);

	if (loops)
	{
		result = channel[chan]->setMode(FMOD_LOOP_NORMAL);
		checkError(result);
	}

}

void FMODAudioManager::stopMusic(int chan)
{
	result = channel[chan]->stop();
	checkError(result);

}

void FMODAudioManager::togglePause(int chan)
{
	bool paused;
	result = channel[chan]->getPaused(&paused); 
	checkError(result);
	result = channel[chan]->setPaused(!paused);
	checkError(result);

}

void FMODAudioManager::setVolume( int chan, int volume)
{

	result = channel[chan]->setVolume(volume);
	checkError(result);
}

void FMODAudioManager::setPitch(int chan, int pitch)
{
	result = channel[chan]->setPitch(pitch);
	checkError(result);
}

void FMODAudioManager::fadeIn(int chan)
{
	unsigned long long parentclock;
	result = channel[chan]->getDSPClock(NULL, &parentclock);
	result = channel[chan]->addFadePoint(parentclock, 0.0f);
	result = channel[chan]->addFadePoint(parentclock + 500000, 1.0f);
}

void FMODAudioManager::fadeOut(int chan)
{
	unsigned long long parentclock;
	float vol;
	channel[chan]->getVolume(&vol);
	result = channel[chan]->getDSPClock(NULL, &parentclock);
	result = channel[chan]->addFadePoint(parentclock, vol);
	result = channel[chan]->addFadePoint(parentclock + 500000, 0.0f);
}

void FMODAudioManager::checkError(FMOD::FMOD_RESULT result)
{
	if (result != FMOD::FMOD_OK) {
		std::cout << FMOD::FMOD_ErrorString(result) << std::endl;
		// printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
		exit(-1);
	}
	
}

