#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include <string>
#include <map>

#include "S2D/S2D.h"

class TextRenderer
{
public:
	TextRenderer() = delete;
	TextRenderer(std::string filePath, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight);
	~TextRenderer();

	void Render(std::string textToRender, unsigned int amountOfCharactersPerLine, S2D::Vector2 topLeftPos, unsigned int colourIndex);

private:
	void SetUpConversionTable();

	std::map<char, unsigned int> mConversionMap;

	S2D::Texture2D*              mSpriteSheet;
	S2D::Rect*                   mSourceRect;
	S2D::Rect*                   mDestRect;

	const unsigned int           mSpritesOnWidth;
	const unsigned int           mSpritesOnHeight;
	
	unsigned int                 mSingleSpriteWidth;
	unsigned int                 mSingleSpriteHeight;

};

#endif // !_TEXT_RENDERER_H_
