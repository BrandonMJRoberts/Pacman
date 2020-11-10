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

private:
	AudioManager();

	static AudioManager* mInstance;

	S2D::SoundEffect*    mDotCollectionSFX;
	S2D::SoundEffect*    mEatingCollectableSFX;
	S2D::SoundEffect*    mExtraLifeAllocationSFX;
};


#endif