#include "Scenes/ArcadeScene.h"

#include <iostream>

#include "Input/GameController.h"

#include "Graphics/Color.h"
#include "Graphics/Screen.h"

#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Lines2D.h"
#include "Shapes/Star.h"
#include "Shapes/Triangle.h"

#include "Utils/Vec2D.h"

#include "App/App.h" // TODO REMOVE after testing

ArcadeScene::ArcadeScene()
{

}

void ArcadeScene::Init()
{
	mTempSS.Load("ArcadeFont");

	ButtonAction action;
	action.key = GameController::ActionKey();
	action.action = [](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			std::cout << "Action button was pressed!" << std::endl;
		}
	};

	mGameController.AddInputActionForKey(action);
	
	{
		MouseButtonAction mouseAction;

		mouseAction.mouseButton = GameController::LeftMouseButton();
		mouseAction.mouseInputAction = [](InputState state, const MousePosition& position)
		{
			if (GameController::IsPressed(state))
			{
				std::cout << "Left Mouse button pressed!" << std::endl;
			}
		};
		mGameController.AddMouseButtonAction(mouseAction);

		mouseAction.mouseButton = GameController::RightMouseButton();
		mouseAction.mouseInputAction = [](InputState state, const MousePosition& position)
		{
			if (GameController::IsPressed(state))
			{
				std::cout << "Right Mouse button pressed!" << std::endl;
			}
		};
		mGameController.AddMouseButtonAction(mouseAction);
	}

	mGameController.SetMouseMovedAction([](const MousePosition& mousePosition)
	{
		std::cout << "Mouse position x: " << mousePosition.xPos << ", y: " << mousePosition.yPos << std::endl;
	});

}

void ArcadeScene::Update(uint32_t dt)
{

}

void ArcadeScene::Draw(Screen& theScreen)
{
	theScreen.Draw(mTempSS, "=", Vec2D::Zero());

	//Vec2D centerOfScreen = { static_cast<float>(theScreen.Width()) / 2, static_cast<float>(theScreen.Height()) / 2 };

	//Star star(Vec2D(theScreen.Width() * 0.25f, theScreen.Height() * 0.25f), 50.f, 45.f, 20);

	//Color starColor = Color(255, 255, 0, 150);
	//AARectangle rect = { Vec2D(theScreen.Width() / 2.f - 50.f, theScreen.Height() / 2.f - 50.f), 50, 50 };

	//Vec2D rotationPoint = star.GetCenterPoint() + Vec2D(0.0f, 0.0f);
	//float deltaAngle = 0.0255f;

	//theScreen.Draw(star, starColor, true, starColor);
	//theScreen.Draw(rect, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

	//star.Rotate(rotationPoint, deltaAngle);
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName; // TODO complete later
}

std::unique_ptr<Scene> ArcadeScene::GetScene(eGame game)
{
	switch (game)
	{
		case TETRIS:
		{

		}
		break;

		case BREAK_OUT:
		{

		}
		break;

		case ASTEROIDS:
		{

		}
		break;

		case PACMAN:
		{

		}
		break;

		default:
		{

		}
		break;
	}

	return nullptr;
}