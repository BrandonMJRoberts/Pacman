#include "AudioManager.h"

#include <iostream>

AudioManager* AudioManager::mInstance = nullptr;

// -------------------------------------------------------------------------- //

AudioManager::AudioManager()
{
	static AudioManager* mInstance;

	mDotCollectionSFX       = new S2D::SoundEffect();
	mDotCollectionSFX->Load("Audio/credit.wav");

	if (!mDotCollectionSFX)
		std::cout << "Failed to load the dot collection SFX" << std::endl;

	mEatingFruitSFX         = new S2D::SoundEffect();
	mEatingFruitSFX->Load("Audio/eat_fruit.wav");

	if (!mEatingFruitSFX)
		std::cout << "Failed to load the eating friut SFX" << std::endl;

	mExtraLifeAllocationSFX = new S2D::SoundEffect();
	mExtraLifeAllocationSFX->Load("Audio/extend.wav");

	if (!mExtraLifeAllocationSFX)
		std::cout << "Failed to load the extra life allocation SFX" << std::endl;

	mDeathSFX_1             = new S2D::SoundEffect();
	mDeathSFX_1->Load("Audio/death_1.wav");

	if (!mDeathSFX_1)
		std::cout << "Failed to load the death 2 SFX" << std::endl;

	mDeathSFX_2             = new S2D::SoundEffect();
	mDeathSFX_2->Load("Audio/death_2.wav");

	if (!mDeathSFX_2)
		std::cout << "Failed to load the death 2 SFX" << std::endl;

	mEatingGhostSFX         = new S2D::SoundEffect();
	mEatingGhostSFX->Load("Audio/eat_ghost.wav");

	if (!mEatingGhostSFX)
		std::cout << "Failed to load the eating ghost SFX" << std::endl;

	mGhostGoingToHomeSFX    = new S2D::SoundEffect();
	mGhostGoingToHomeSFX->Load("Audio/retreating.wav");

	if (!mGhostGoingToHomeSFX)
		std::cout << "Failed to load the ghost going home SFX" << std::endl;

	mGhostFleeingSFX_1      = new S2D::SoundEffect();
	mGhostFleeingSFX_1->Load("Audio/siren_1.wav");

	if (!mGhostFleeingSFX_1)
		std::cout << "Failed to load the ghost fleeing 1 SFX" << std::endl;

	mGhostFleeingSFX_2      = new S2D::SoundEffect();
	mGhostFleeingSFX_2->Load("Audio/siren_2.wav");

	if (!mGhostFleeingSFX_2)
		std::cout << "Failed to load the ghost fleeing 2 SFX" << std::endl;

	mGhostFleeingSFX_3      = new S2D::SoundEffect();
	mGhostFleeingSFX_3->Load("Audio/siren_3.wav");

	if (!mGhostFleeingSFX_3)
		std::cout << "Failed to load the ghost fleeing 3 SFX" << std::endl;

	mGhostFleeingSFX_4      = new S2D::SoundEffect();
	mGhostFleeingSFX_4->Load("Audio/siren_4.wav");

	if (!mGhostFleeingSFX_4)
		std::cout << "Failed to load the ghost fleeing 4 SFX" << std::endl;

	mGhostFleeingSFX_5      = new S2D::SoundEffect();
	mGhostFleeingSFX_5->Load("Audio/siren_5.wav");

	if (!mGhostFleeingSFX_5)
		std::cout << "Failed to load the ghost fleeing 5 SFX" << std::endl;

	mGameStartSFX           = new S2D::SoundEffect();
	mGameStartSFX->Load("Audio/game_start.wav");

	if (!mGameStartSFX)
		std::cout << "Failed to load the game start SFX" << std::endl;
}

// -------------------------------------------------------------------------- //

AudioManager::~AudioManager()
{
	delete mDotCollectionSFX;
	mDotCollectionSFX = nullptr;

	delete mEatingFruitSFX;
	mEatingFruitSFX = nullptr;

	delete mExtraLifeAllocationSFX;
	mExtraLifeAllocationSFX = nullptr;

	delete mDeathSFX_1;
	mDeathSFX_1 = nullptr;

	delete mDeathSFX_2;
	mDeathSFX_2 = nullptr;

	delete mEatingGhostSFX;
	mEatingGhostSFX = nullptr;

	delete mGhostGoingToHomeSFX;
	mGhostGoingToHomeSFX = nullptr;

	delete mGhostFleeingSFX_1;
	mGhostFleeingSFX_1 = nullptr;

	delete mGhostFleeingSFX_2;
	mGhostFleeingSFX_2 = nullptr;

	delete mGhostFleeingSFX_3;
	mGhostFleeingSFX_3 = nullptr;

	delete mGhostFleeingSFX_4;
	mGhostFleeingSFX_4 = nullptr;

	delete mGhostFleeingSFX_5;
	mGhostFleeingSFX_5 = nullptr;

	delete mGameStartSFX;
	mGameStartSFX = nullptr;
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
	if(mDotCollectionSFX->GetState() != S2D::SoundEffectState::PLAYING && mDotCollectionSFX)
		S2D::Audio::Play(mDotCollectionSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayEatingFruitSFX()
{
	if (mEatingFruitSFX->GetState() != S2D::SoundEffectState::PLAYING && mEatingFruitSFX)
		S2D::Audio::Play(mEatingFruitSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayExtraLifeSFX()
{
	if (mExtraLifeAllocationSFX->GetState() != S2D::SoundEffectState::PLAYING && mExtraLifeAllocationSFX)
		S2D::Audio::Play(mExtraLifeAllocationSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayPacmanDeathSFX_1()
{
	if (mDeathSFX_1->GetState() != S2D::SoundEffectState::PLAYING && mDeathSFX_1)
		S2D::Audio::Play(mDeathSFX_1);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayPacmanDeathSFX_2()
{
	if (mDeathSFX_2->GetState() != S2D::SoundEffectState::PLAYING && mDeathSFX_2)
		S2D::Audio::Play(mDeathSFX_2);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayEatingGhostSFX()
{
	if (mEatingGhostSFX->GetState() != S2D::SoundEffectState::PLAYING && mEatingGhostSFX)
		S2D::Audio::Play(mEatingGhostSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGhostGoingToHomeSFX()
{
	if (mGhostGoingToHomeSFX->GetState() != S2D::SoundEffectState::PLAYING && mGhostGoingToHomeSFX)
		S2D::Audio::Play(mGhostGoingToHomeSFX);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGhostFleeingSFX_1()
{
	if (mGhostFleeingSFX_1->GetState() != S2D::SoundEffectState::PLAYING && mGhostFleeingSFX_1)
		S2D::Audio::Play(mGhostFleeingSFX_1);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGhostFleeingSFX_2()
{
	if (mGhostFleeingSFX_2->GetState() != S2D::SoundEffectState::PLAYING && mGhostFleeingSFX_2)
		S2D::Audio::Play(mGhostFleeingSFX_2);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGhostFleeingSFX_3()
{
	if (mGhostFleeingSFX_3->GetState() != S2D::SoundEffectState::PLAYING && mGhostFleeingSFX_3)
		S2D::Audio::Play(mGhostFleeingSFX_3);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGhostFleeingSFX_4()
{
	if (mGhostFleeingSFX_4->GetState() != S2D::SoundEffectState::PLAYING && mGhostFleeingSFX_4)
		S2D::Audio::Play(mGhostFleeingSFX_4);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGhostFleeingSFX_5()
{
	if (mGhostFleeingSFX_5->GetState() != S2D::SoundEffectState::PLAYING && mGhostFleeingSFX_5)
		S2D::Audio::Play(mGhostFleeingSFX_5);
}

// -------------------------------------------------------------------------- //

void AudioManager::PlayGameStartSFX()
{
	if (mGameStartSFX->GetState() != S2D::SoundEffectState::PLAYING && mGameStartSFX)
		S2D::Audio::Play(mGameStartSFX);
}

// -------------------------------------------------------------------------- //