#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "S2D/S2D.h"

class AudioManager final
{
public:
	~AudioManager();

	static AudioManager* GetInstance();

	void Update();

	void PlayDotCollectionSFX();
	void PlayEatingFruitSFX();
	void PlayExtraLifeSFX();

	void PlayPacmanDeathSFX_1();
	void PlayPacmanDeathSFX_2();

	bool GetPacmanDeathSFXFinishedPlaying();

	void PlayEatingGhostSFX();

	void PlayGhostGoingToHomeSFX();

	void PlayGhostFleeingSFX_1();
	void PlayGhostFleeingSFX_2();
	void PlayGhostFleeingSFX_3();
	void PlayGhostFleeingSFX_4();
	void PlayGhostFleeingSFX_5();

	void PauseGhostFleeingSFX_1();
	void PauseGhostFleeingSFX_2();
	void PauseGhostFleeingSFX_3();
	void PauseGhostFleeingSFX_4();
	void PauseGhostFleeingSFX_5();

	void PauseGhostGoingToHomeSFX() { if (mGhostGoingToHomeSFX) S2D::Audio::Pause(mGhostGoingToHomeSFX); }

	void PlayGameStartSFX();

	void StopAllAudio();
	void PauseAllAudio();
	void ResumeAllAudio();

private:
	AudioManager();

	void RemoveSoundFromCurrentlyPlayingList(S2D::SoundEffect* sfx);

	static AudioManager* mInstance;

	std::vector<S2D::SoundEffect*> mCurrentlyPlayingSounds;

	S2D::SoundEffect*    mDotCollectionSFX;
	S2D::SoundEffect*    mEatingFruitSFX;

	S2D::SoundEffect*    mExtraLifeAllocationSFX;
	S2D::SoundEffect*    mDeathSFX_1;
	S2D::SoundEffect*    mDeathSFX_2;

	S2D::SoundEffect*    mEatingGhostSFX;
	S2D::SoundEffect*    mGhostGoingToHomeSFX;

	S2D::SoundEffect*    mGhostFleeingSFX_1;
	S2D::SoundEffect*    mGhostFleeingSFX_2;
	S2D::SoundEffect*    mGhostFleeingSFX_3;
	S2D::SoundEffect*    mGhostFleeingSFX_4;
	S2D::SoundEffect*    mGhostFleeingSFX_5;

	S2D::SoundEffect*    mGameStartSFX;
};


#endif