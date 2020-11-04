#include "AudioManager.h"

#include <iostream>

AudioManager* AudioManager::mInstance = nullptr;

// -------------------------------------------------------------------------- //

AudioManager::AudioManager()
{
	mDotCollectionSFX = new S2D::SoundEffect();
	mDotCollectionSFX->Load("Audio/ShortenedChomp.wav");

	if (!mDotCollectionSFX)
		std::cout << "Failed to load in the dot collection SFX" << std::endl;

	mEatingCollectableSFX = new S2D::SoundEffect();
	mEatingCollectableSFX->Load("Audio/pacman_eatfruit.wav");

	if (!mEatingCollectableSFX)
		std::cout << "Failed to load in the fruit eating SFX" << std::endl;

	mExtraLifeAllocationSFX = new S2D::SoundEffect();
	mExtraLifeAllocationSFX->Load("Audio/pacman_extrapac.wav");

	if(!mExtraLifeAllocationSFX)
		std::cout << "Failed to load in the extra life gain SFX" << std::endl;
}

// -------------------------------------------------------------------------- //

AudioManager::~AudioManager()
{
	delete mDotCollectionSFX;
	mDotCollectionSFX = nullptr;

	delete mInstance;
	mInstance = nullptr;
}

// -------------------------------------------------------------------------- //

AudioManager* AudioManager::GetInstance()
{
	if (!mInstance)
		mInstance = new AudioManager;

	return mInstance;
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayDotCollectionSFX()
{
	//if(mDotCollectionSFX->GetState() != S2D::SoundEffectState::PLAYING && mDotCollectionSFX)
	//	S2D::Audio::Play(mDotCollectionSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayEatingFruitSFX()
{
	if (mEatingCollectableSFX->GetState() != S2D::SoundEffectState::PLAYING && mEatingCollectableSFX)
		S2D::Audio::Play(mEatingCollectableSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayExtraLifeSFX()
{
	if (mExtraLifeAllocationSFX->GetState() != S2D::SoundEffectState::PLAYING && mExtraLifeAllocationSFX)
		S2D::Audio::Play(mExtraLifeAllocationSFX);
}

// -------------------------------------------------------------------------- //