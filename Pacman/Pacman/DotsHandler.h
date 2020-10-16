#ifndef _DOTS_HANDLER_H_
#define _DOTS_HANDLER_H_

#include <iostream>
#include <vector>
#include "S2D/S2D.h"

// ------------------------------------------------------------------------- //

enum class DOT_TYPE 
{
	SMALL = 0,
	LARGE
};

// ------------------------------------------------------------------------- //

struct Dot
{
public:
	Dot(S2D::Vector2* startPos, DOT_TYPE DotType) { mPosition = startPos; mDotType = DotType; }
	~Dot()                                        { delete mPosition; mPosition = nullptr; }

	DOT_TYPE GetDotType()       { return mDotType; }
	S2D::Vector2* GetPosition() { return mPosition; }

private:
	DOT_TYPE      mDotType;
	S2D::Vector2* mPosition;
};

// ------------------------------------------------------------------------- //

class DotsHandler
{
public:
	DotsHandler();
	~DotsHandler();

	void Update(S2D::Vector2* pacmanPosition, const unsigned int pacmanDimensions);
	void Render(unsigned int currentFrameCount);

private:
	void LoadInDotData();

	std::vector<Dot*> mDots;

	S2D::Texture2D* mSmallDotSpriteSheet;
	S2D::Texture2D* mLargeDotSpriteSheet;

	S2D::Rect*      mSourceRectSmallDot;
	S2D::Rect*      mSourceRectLargeDot;
};

#endif