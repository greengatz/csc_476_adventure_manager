#include "SoundPlayer.h"

const char *files[] = {"assets/music/bikeMusic.mp3",
                       "doesNotExistYet.mp3"};

SoundPlayer::SoundPlayer()
{
	success = true;
	//creates the sytesm
	result = FMOD_System_Create(&sys);
	if (result == FMOD_OK) {
		result = FMOD_System_Init(sys, 2, FMOD_INIT_NORMAL, 0);
	}
	else {
		printf("Failed to create sound system\n");
		success = false;
	}

	if (result == FMOD_OK) {
		 FMOD_Channel_SetVolume(channel, 0.5f);
	}
	else {
		printf("Failed to initialze sound system\n");
		success = false;
	}

	if (success) {
		printf("Successful initialization of sound system\n");
	}

	pauseSound = false;
}

SoundPlayer::~SoundPlayer()
{
	stop();
}

void SoundPlayer::loadFile(int ndx)
{
	if (success) {
		result = FMOD_Sound_Release(audioStream);
		result = FMOD_System_CreateStream(sys, files[ndx], FMOD_SOFTWARE, NULL, &audioStream);
		if (result != FMOD_OK) {
			success = false;
			printf("Failed to load audio file: %s\n", files[ndx]);
			printf("Error: %d\n", result);
		}
	}
}

void SoundPlayer::play()
{
	if (success) {
		result = FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, audioStream, false, &channel);
		printf("Did not play sound because %d\n", result);
		FMOD_Channel_SetMode(channel, FMOD_LOOP_NORMAL);
	}
}

void SoundPlayer::stop()
{
	if (success) {
		result = FMOD_Sound_Release(audioStream);
	}
}

void SoundPlayer::pause()
{
	pauseSound = !pauseSound;
	FMOD_Channel_SetPaused(channel, pauseSound);
	// FSOUND_SetPaused(handle, pauseSound);
}