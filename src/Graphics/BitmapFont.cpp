#include "Graphics/BitmapFont.h"

#include "Shapes/AARectangle.h"
#include "Utils/Vec2D.h"

bool BitmapFont::Load(const std::string& name)
{
	return mFontSheet.Load(name);
}

Size BitmapFont::GetSizeOf(const std::string& str) const
{
	Size textSize = { 0,0 };

	size_t length = str.length();

	int i = 0;
	for (char c : str)
	{
		if (c == ' ')
		{
			textSize.width += GetFontSpacingBetweenWords();
			++i;
			continue;
		}

		Sprite sprite = mFontSheet.GetSprite(std::string("") + c);
		textSize.height = textSize.height < sprite.height ? sprite.height : textSize.height;
		textSize.width += sprite.width;

		if (i + 1 < length)
		{
			textSize.width += GetFontSpacingBetweenLetters();
		}

		++i;
	}

	return textSize;
}

Vec2D BitmapFont::GetDrawPosition(const std::string& str, const AARectangle& box, BitmapFontXAlignment xAlign, BitmapFontYAlignment yAlign) const
{
	Size textSize = GetSizeOf(str);

	uint32_t x = 0;
	uint32_t y = 0;

	switch (xAlign)
	{
	case BFXA_CENTER:
		x = box.GetWidth() / 2 - textSize.width / 2;
		break;
	case BFXA_RIGHT:
		x = box.GetWidth() - textSize.width;
		break;
	case BFXA_LEFT:
	default:
		y = 0;
		break;
	}

	x += box.GetTopLeftPoint().GetX();

	switch (yAlign)
	{
	case BFYA_CENTER:
		y = box.GetHeight() / 2 - textSize.height / 2;
		break;
	case BFYA_BOTTOM:
		y = box.GetHeight() - textSize.height;
		break;
	case BFYA_TOP:
		y = 0;
	default:
		break;
	}

	y += box.GetTopLeftPoint().GetY();

	return Vec2D(x, y);
}
