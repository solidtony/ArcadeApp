#include "Scenes/ButtonOptionsScene.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "App/App.h"
#include "App/Button.h"
#include "Graphics/BitmapFont.h"
#include "Utils/Utils.h"

ButtonOptionsScene::ButtonOptionsScene(const std::vector<std::string>& optionNames, const Color& textColor):
	mHighlightedOption(0)
{
	const BitmapFont& font = App::Singleton().GetFont();

	for (const auto& name : optionNames)
	{
		mButtons.push_back(Button(font, textColor));
		mButtons.back().SetButtonText(name);
	}

	if (optionNames.size() > 0)
	{
		mButtons[mHighlightedOption].SetIsHighlighted(true);
	}
}

void ButtonOptionsScene::Init()
{
	ButtonAction upAction;
	upAction.key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			SetPreviousButtonHighlighted();
		}
	};
	mGameController.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			SetNextButtonHighlighted();
		}
	};
	mGameController.AddInputActionForKey(downAction);

	ButtonAction acceptAction;
	acceptAction.key = GameController::ActionKey();
	acceptAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			ExecuteCurrentButtonAction();
		}
	};
	mGameController.AddInputActionForKey(acceptAction);



	uint32_t height = App::Singleton().Height();
	uint32_t width = App::Singleton().Width();

	const BitmapFont& font = App::Singleton().GetFont();

	Size fontSize = font.GetSizeOf(mButtons[0].GetButtonText());

	const int BUTTON_PAD = 10;
	unsigned int buttonHeight = fontSize.height + BUTTON_PAD * 2;
	uint32_t maxButtonWidth = fontSize.width;

	for (const auto & button : mButtons)
	{
		Size s = font.GetSizeOf(button.GetButtonText());

		if (s.width > maxButtonWidth)
		{
			maxButtonWidth = s.width;
		}
	}

	maxButtonWidth += BUTTON_PAD * 2;

	const uint32_t Y_PAD = 1;
	uint32_t yOffset = height / 2 - ((buttonHeight + Y_PAD) * static_cast<uint32_t>(mButtons.size())) / 2;

	for (auto& button : mButtons)
	{
		button.Init(Vec2D(width / 2 - maxButtonWidth / 2, yOffset), maxButtonWidth, buttonHeight);

		yOffset += buttonHeight + Y_PAD;
	}

	mButtons[mHighlightedOption].SetIsHighlighted(true);
}

void ButtonOptionsScene::Update(uint32_t dt)
{

}

void ButtonOptionsScene::Draw(Screen& theScreen)
{
	for (auto button : mButtons)
	{
		button.Draw(theScreen);
	}
}

void ButtonOptionsScene::SetButtonActions(const std::vector<Button::ButtonAction>& buttonActions)
{
	size_t size = std::min(mButtons.size(), buttonActions.size());
	for (size_t i = 0; i < size; ++i)
	{
		mButtons[i].SetButtonAction(buttonActions[i]);
	}
}

void ButtonOptionsScene::SetNextButtonHighlighted()
{
	mHighlightedOption = (mHighlightedOption + 1) % mButtons.size();

	HighlightCurrentButton();
}

void ButtonOptionsScene::SetPreviousButtonHighlighted()
{
	--mHighlightedOption;

	if (mHighlightedOption < 0)
	{
		mHighlightedOption = static_cast<int>(mButtons.size()) - 1;
	}

	HighlightCurrentButton();
}

void ButtonOptionsScene::ExecuteCurrentButtonAction()
{
	mButtons[mHighlightedOption].ExecuteAction();
}

void ButtonOptionsScene::HighlightCurrentButton()
{
	for (auto& button : mButtons)
	{
		button.SetIsHighlighted(false);
	}

	mButtons[mHighlightedOption].SetIsHighlighted(true);
}
