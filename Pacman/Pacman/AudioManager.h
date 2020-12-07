#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "S2D/S2D.h"

class AudioManager final
{
public:
	~AudioManager();

	static AudioManager* GetInstance();

	void PlayDotCollectionSFX();
	void PlayEatingFruitSFX();
	void PlayExtraLifeSFX();

	void PlayPacmanDeathSFX_1();
	void PlayPacmanDeathSFX_2();

	void PlayEatingGhostSFX();

	void PlayGhostGoingToHomeSFX();

	void PlayGhostFleeingSFX_1();
	void PlayGhostFleeingSFX_2();
	void PlayGhostFleeingSFX_3();
	void PlayGhostFleeingSFX_4();
	void PlayGhostFleeingSFX_5();

	void PauseGhostFleeingSFX_1() { if (mGhostFleeingSFX_1) S2D::Audio::Pause(mGhostFleeingSFX_1); }
	void PauseGhostFleeingSFX_2() { if (mGhostFleeingSFX_2) S2D::Audio::Pause(mGhostFleeingSFX_2); }
	void PauseGhostFleeingSFX_3() { if (mGhostFleeingSFX_3) S2D::Audio::Pause(mGhostFleeingSFX_3); }
	void PauseGhostFleeingSFX_4() { if (mGhostFleeingSFX_4) S2D::Audio::Pause(mGhostFleeingSFX_4); }
	void PauseGhostFleeingSFX_5() { if (mGhostFleeingSFX_5) S2D::Audio::Pause(mGhostFleeingSFX_5); }

	void PlayGameStartSFX();

	void StopAllAudio();

private:
	AudioManager();

	static AudioManager* mInstance;

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