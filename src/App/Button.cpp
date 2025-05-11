#include "App/Button.h"

#include "Graphics/Screen.h"
#include "Graphics/BitmapFont.h"
#include "Utils/Vec2D.h"

Button::Button(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor) :
mBitmapFont(bitmapFont),  mTextColor(textColor), mHighlightColor(highlightColor), mTitle("")
{

}

void Button::Init(Vec2D topLeft, unsigned int width, unsigned height)
{
	mBBox = AARectangle(topLeft, width, height);
}

void Button::Draw(Screen& theScreen)
{
	theScreen.Draw(mBitmapFont, mTitle, mBitmapFont.GetDrawPosition(mTitle, mBBox, BFXA_CENTER, BFYA_CENTER), mTextColor);

	if (mIsHighlighted)
	{
		theScreen.Draw(mBBox, mHighlightColor);
	}
}

void Button::ExecuteAction()
{
	mAction();
}