#pragma once
#ifndef _SOUNDPLAYER_H_
#define _SOUNDPLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fmod.h>
// #include "inc/fmod.h"

#define TERR_MUSIC_1 0
#define TERR_MUSIC_2 1
#define TAV_MUSIC_1 2
#define TAV_MUSIC_2 3
#define EXPLOSION_SOUND 4
#define MAGMISS_VOICE 5

const int NUM_SOUND_FILES = 6;

const float sound_volume[] = {0.5, 0.5, 0.5, 0.5, 0.65, 0.65};
const float song_length[] = {240, 95, 207, 252};

class SoundPlayer
{
public:
	SoundPlayer();
	virtual ~SoundPlayer();
	void playBackgroundMusic(bool tav);
	void playVoice(int file);
	void playSoundEffect(int file);
	void stop(int ndx);
	void stopAll();
	void pause();
	void setVolume(float newVol, int channel);
	void checkTime();
private:
	int curBkgSong;
	time_t startTime, pauseStart;
	float pausedTime;
	FMOD_SYSTEM *sys;
	FMOD_SOUND *sounds[NUM_SOUND_FILES];
	FMOD_RESULT result;
	FMOD_CHANNEL *music;
	FMOD_CHANNEL *voice;
	FMOD_CHANNEL *soundEffect;
	bool pauseSound;
	bool success;
	void play(FMOD_CHANNEL **channel, int ndx);
	void loadFile(int ndx);
	int getRandInt(int limit);
};

#endif
