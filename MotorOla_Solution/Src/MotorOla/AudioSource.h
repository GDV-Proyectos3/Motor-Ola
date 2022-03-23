#pragma once
#include "Componente.h"
class FMODAudioManager;
//#include <iostream>
//#include <string>
using namespace std;
class AudioSource :	public Componente
{
public:
	AudioSource(int channel,FMODAudioManager*a,const char* s);
	~AudioSource();

	void play();
	void pause();
private:
	//string nom;
	FMODAudioManager* aud;
	int _channel;
};

