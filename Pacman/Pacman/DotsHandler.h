#ifndef _DOTS_HANDLER_H_
#define _DOTS_HANDLER_H_

#include <iostream>
#include <vector>
#include "S2D/S2D.h"

// ------------------------------------------------------------------------- //

enum class DOT_TYPE : char
{
	SMALL = 0x00,
	LARGE = 0x01
};

// ------------------------------------------------------------------------- //

struct Dot
{
public:
	Dot(S2D::Vector2 startPos, DOT_TYPE DotType) { mPosition = startPos; mDotType = DotType; }

	S2D::Vector2  mPosition;
	DOT_TYPE      mDotType;
};

// ------------------------------------------------------------------------- //

class DotsHandler final
{
public:
	DotsHandler();
	~DotsHandler();

	void Update(const S2D::Vector2 pacmanPosition, const float pacmanDimensions);
	void Render(const unsigned int currentFrameCount);

	std::vector<S2D::Vector2> GetLargeDotPositions() { return mLargeDotsPositions; }
	S2D::Vector2              GetRandomDotPosition();

	unsigned int GetRemainingDotsCount() { return mDots.size(); }

private:
	void LoadInDotData();

	unsigned int      mTotalDots;
	int               mRandomDotIndex;

	std::vector<Dot*> mDots;
	std::vector<S2D::Vector2> mLargeDotsPositions;

	S2D::Texture2D*   mSmallDotSpriteSheet;
	S2D::Texture2D*   mLargeDotSpriteSheet;

	S2D::Rect         mSourceRectSmallDot;
	S2D::Rect         mSourceRectLargeDot;
};

#endif