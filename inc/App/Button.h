#pragma once

#include <string>
#include "Shapes/AARectangle.h"
#include <functional>
#include "Graphics/Color.h"
#include "Graphics/BitmapFont.h"

class Screen;

class Button
{
public:
	using ButtonAction = std::function<void(void)>;

	Button(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor = Color::White());
	void Init(Vec2D topLeft, unsigned int width, unsigned height);
	void Draw(Screen& theScreen);
	void ExecuteAction();

	inline void SetButtonText(const std::string& text) { mTitle = text; }
	inline const std::string& GetButtonText() const { return mTitle; }
	inline void SetIsHighlighted(bool isHighlighted) { mIsHighlighted = isHighlighted; }
	inline bool IsHighlighted() const { return mIsHighlighted; }
	inline void SetButtonAction(ButtonAction buttonAction) { mAction = buttonAction; }

private:
	const BitmapFont& mBitmapFont;
	std::string mTitle;
	AARectangle mBBox;

	bool mIsHighlighted = false;
	ButtonAction mAction;
	Color mHighlightColor;
	Color mTextColor;
};