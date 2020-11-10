#include "TextRenderer.h"

#include <iostream>

// ------------------------------------------------------------------------------------------------------------------------------------------ //

TextRenderer::~TextRenderer()
{
	delete mSpriteSheet;
	mSpriteSheet = nullptr;

	delete mSourceRect;
	mSourceRect = nullptr;

	delete mDestRect;
	mDestRect = nullptr;
}

// ------------------------------------------------------------------------------------------------------------------------------------------ //

TextRenderer::TextRenderer(std::string filePath, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight) : mSpritesOnWidth(spritesOnWidth), mSpritesOnHeight(spritesOnHeight)
{
	mSpriteSheet = new S2D::Texture2D();
	mSpriteSheet->Load(filePath.c_str(), false);
	if (!mSpriteSheet)
		std::cout << "Failed to load in the text renderer sprite sheet." << std::endl;

	mSingleSpriteWidth  = mSpriteSheet->GetWidth()  / mSpritesOnWidth;
	mSingleSpriteHeight = mSpriteSheet->GetHeight() / mSpritesOnHeight;

	mSourceRect         = new S2D::Rect();
	mSourceRect->Width  = mSingleSpriteWidth;
	mSourceRect->Height = mSingleSpriteHeight;

	mDestRect           = new S2D::Rect();
	mDestRect->Width    = mSingleSpriteWidth;
	mDestRect->Height   = mSingleSpriteHeight;

	SetUpConversionTable();
}

// ------------------------------------------------------------------------------------------------------------------------------------------ //

void TextRenderer::RenderFromRight(std::string textToRender, unsigned int amountOfCharactersPerLine, S2D::Vector2 topRightPos, unsigned int colourIndex)
{
	mDestRect->X = topRightPos.X;
	mDestRect->Y = topRightPos.Y;

	// Make sure that the colour of the text is a valid index
	colourIndex %= 7;

	// Loop through all of the text we need to render
	for (int charID = textToRender.size() - 1; charID >= 0; charID--)
	{
		if ((textToRender.size() - charID) % amountOfCharactersPerLine == 0 && charID != textToRender.size() - 1)
		{
			// Reset to the starting render position
			mDestRect->Y += mSingleSpriteHeight;
			mDestRect->X  = topRightPos.X;
		}

		// Now calculate the source render rect based on the current character
		mSourceRect->X =    float(     (mConversionMap[textToRender[charID]] % mSpritesOnWidth)  * mSingleSpriteWidth);
		mSourceRect->Y =    float(((int(mConversionMap[textToRender[charID]] / mSpritesOnWidth)) * mSingleSpriteHeight) + (colourIndex * (3 * mSingleSpriteHeight)));

		// Now do the draw call
		S2D::SpriteBatch::Draw(mSpriteSheet, mDestRect, mSourceRect);

		// Increment the render position
		mDestRect->X -= mSingleSpriteWidth;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------ //

void TextRenderer::RenderFromLeft(std::string textToRender, unsigned int amountOfCharactersPerLine, S2D::Vector2 topLeftPos, unsigned int colourIndex)
{
	mDestRect->X = topLeftPos.X;
	mDestRect->Y = topLeftPos.Y;

	// Make sure that the colour of the text is a valid index
	colourIndex %= 7;

	// Loop through all of the text we need to render
	for (int charID = 0; charID < textToRender.size(); charID++)
	{
		if (charID % amountOfCharactersPerLine == 0 && charID != textToRender.size() - 1)
		{
			// Reset to the starting render position
			mDestRect->Y += mSingleSpriteHeight;
			mDestRect->X = topLeftPos.X;
		}

		// Now calculate the source render rect based on the current character
		mSourceRect->X = float((mConversionMap[textToRender[charID]] % mSpritesOnWidth) * mSingleSpriteWidth);
		mSourceRect->Y = float(((int(mConversionMap[textToRender[charID]] / mSpritesOnWidth)) * mSingleSpriteHeight) + (colourIndex * (3 * mSingleSpriteHeight)));

		// Now do the draw call
		S2D::SpriteBatch::Draw(mSpriteSheet, mDestRect, mSourceRect);

		// Increment the render position
		mDestRect->X += mSingleSpriteWidth;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------ //

void TextRenderer::SetUpConversionTable()
{
	char character = '/';

	// Loop through all of the main characters
	while (character != 'Z')
	{
		if ((unsigned int)character < 58)
			mConversionMap[character] = (unsigned int)(character - 44);
		else if (character > 64 && character < 91)
			mConversionMap[character] = (unsigned int) (character - 51);

		character++;
	}

	// Now add special cases
	mConversionMap['!'] = 0;
	mConversionMap['"'] = 1;
	mConversionMap['-'] = 2;
	mConversionMap['@'] = 40;
	mConversionMap[' '] = 41;
}

// ------------------------------------------------------------------------------------------------------------------------------------------ //