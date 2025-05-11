#pragma once

#include <string>

#include "Scenes/ButtonOptionsScene.h"

class PacmanStartScene : public ButtonOptionsScene
{
public:
	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;
	virtual const std::string& GetSceneName() const override;

private:

};