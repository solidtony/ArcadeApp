#pragma once

#include <string>
#include <vector>

#include "App/Button.h"
#include "Scenes/Scene.h"

class ButtonOptionsScene : public Scene
{
public:
	ButtonOptionsScene(const std::vector<std::string>& optionNames, const Color& textColor);

	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;

	void SetButtonActions(const std::vector<Button::ButtonAction>& buttonActions);

private:
	void SetNextButtonHighlighted();
	void SetPreviousButtonHighlighted();
	void ExecuteCurrentButtonAction();
	void HighlightCurrentButton();

	std::vector<Button> mButtons;
	int mHighlightedOption;

};