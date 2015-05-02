#pragma once
#ifndef _SOUNDPLAYER_H_
#define _SOUNDPLAYER_H_

#include <stdio.h>
#include "inc/fmod.h"
// #include "fmod.h"

#define TAV_MUSIC 0
#define TRAIL_MUSIC 1

class SoundPlayer
{
public:
	SoundPlayer();
	virtual ~SoundPlayer();
	void loadFile(int ndx);
	void play();
	void stop();
	void pause();
private:
	FMOD_SYSTEM *sys;
	FMOD_SOUND *audioStream;
	FMOD_RESULT result;
	FMOD_CHANNEL *channel;
	bool pauseSound;
	bool success;
};

#endif