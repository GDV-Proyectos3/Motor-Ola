#pragma once
#include "Componente.h"
class FMODAudioManager;
using namespace std;
class AudioSource :	public Componente
{
public:
	AudioSource(int channel,FMODAudioManager*a,const char* s);
	~AudioSource();

	void play();
	void pause();
private:
	FMODAudioManager* aud;
	int _channel;
};

