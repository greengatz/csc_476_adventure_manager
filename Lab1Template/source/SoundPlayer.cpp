#include "SoundPlayer.h"

const char *files[] = {"assets/music/trailMusic1.mp3",
					   "assets/music/trailMusic2.mp3",
					   "assets/music/tavernMusic1.mp3",
					   "assets/music/tavernMusic2.mp3",
                       "assets/music/magicMissile.mp3",
                       "assets/music/explosion.mp3"};

SoundPlayer::SoundPlayer()
{
	success = true;
	//creates the sytesm
	result = FMOD_System_Create(&sys);
	if (result == FMOD_OK) {
		result = FMOD_System_Init(sys, 3, FMOD_INIT_NORMAL, 0);
	}
	else {
		printf("Failed to create sound system\n");
		success = false;
	}

	if (result == FMOD_OK) {
		 FMOD_Channel_SetVolume(music, 0.15f);
		 FMOD_Channel_SetVolume(voice, 0.65f);
		 FMOD_Channel_SetVolume(soundEffect, 0.65f);
	}
	else {
		printf("Failed to initialze sound system\n");
		success = false;
	}

	if (success) {
		printf("Successful initialization of sound system\n");
	}

	for (int iter = 0; iter < NUM_SOUND_FILES; iter++) {
		loadFile(iter);
	}

	pauseSound = false;
	pausedTime = 0;
}

SoundPlayer::~SoundPlayer()
{
	stopAll();
}

void SoundPlayer::playBackgroundMusic(bool tav)
{
	//gets a 0 or 1 if trail, 2 or 3 if tavern
	curBkgSong = (tav) ? getRandInt(2) + 1 : getRandInt(2) - 1;
	play(&music, curBkgSong);
	startTime = time(0);
	pausedTime = 0;
}

void SoundPlayer::playSoundEffect(int file)
{
	play(&soundEffect, file);
}

void SoundPlayer::playVoice(int file)
{
	play(&voice, file);
}

void SoundPlayer::loadFile(int ndx)
{
	if (success) {
		result = FMOD_Sound_Release(sounds[ndx]);
		result = FMOD_System_CreateStream(sys, files[ndx], FMOD_SOFTWARE, NULL, &(sounds[ndx]));
		if (result != FMOD_OK) {
			success = false;
			printf("Failed to load audio file: %s\n", files[ndx]);
			printf("Error: %d\n", result);
		}
	}
}

void SoundPlayer::play(FMOD_CHANNEL **channel, int ndx)
{
	if (success) {
		FMOD_Channel_SetPaused((*channel), true);
		result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sounds[ndx], false, &(*channel));
		// printf("Did not play sound because %d\n", result);
		FMOD_Channel_SetMode((*channel), FMOD_LOOP_OFF);
		FMOD_System_Update(sys);
		FMOD_Channel_SetVolume((*channel), sound_volume[ndx]);
		FMOD_Channel_SetPaused((*channel), false);
	}
}

void SoundPlayer::checkTime()
{
	float curPoint = (float) difftime(time(0), startTime) - pausedTime;
	if (!pauseSound && song_length[curBkgSong] < curPoint) {
		curBkgSong += (curBkgSong % 2) ? 1 : -1;
		play(&music, curBkgSong);
		startTime = time(0);
		pausedTime = 0;
	}
}

void SoundPlayer::stop(int ndx)
{
	if (success) {
		result = FMOD_Sound_Release(sounds[ndx]);
	}
}

void SoundPlayer::stopAll()
{
	if (success) {
		for (int iter = 0; iter < NUM_SOUND_FILES; iter++) {
			stop(iter);
		}
	}
}

void SoundPlayer::pause()
{
	pauseSound = !pauseSound;
	FMOD_Channel_SetPaused(music, pauseSound);
	FMOD_Channel_SetPaused(voice, pauseSound);
	FMOD_Channel_SetPaused(soundEffect, pauseSound);
	if (pauseSound) {
		pauseStart = time(0);
	}
	else {
		pausedTime += (float) difftime(time(0), pauseStart);
	}
}

//channels 0: music, 1: voice, 2: sound effects
void SoundPlayer::setVolume(float newVol, int channel)
{
	if (success) {
		switch (channel) {
			case 0:
				FMOD_Channel_SetVolume(music, newVol);
				break;
			case 1:
				FMOD_Channel_SetVolume(voice, newVol);
				break;
			case 2:
				FMOD_Channel_SetVolume(soundEffect, newVol);
				break;
		}
	}
}

//between [1, limit]
int SoundPlayer::getRandInt(int limit)
{
	return rand() % limit + 1;
}