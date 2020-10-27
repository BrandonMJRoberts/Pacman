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
	~Dot() { ; }

	DOT_TYPE      GetDotType()                    { return mDotType; }
	S2D::Vector2  GetPosition()                   { return mPosition; }

private:
	S2D::Vector2  mPosition;
	DOT_TYPE      mDotType;
};

// ------------------------------------------------------------------------- //

class DotsHandler final
{
public:
	DotsHandler();
	~DotsHandler();

	bool Update(const S2D::Vector2 pacmanPosition, const unsigned int pacmanDimensions);
	void Render(const unsigned int currentFrameCount);

private:
	void LoadInDotData();

	std::vector<Dot*> mDots;

	S2D::Texture2D*   mSmallDotSpriteSheet;
	S2D::Texture2D*   mLargeDotSpriteSheet;

	S2D::Rect*        mSourceRectSmallDot;
	S2D::Rect*        mSourceRectLargeDot;
};

#endif