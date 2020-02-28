#include "Scenes/ArcadeScene.h"

#include "Graphics/Color.h"
#include "Graphics/Screen.h"

#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Lines2D.h"
#include "Shapes/Star.h"
#include "Shapes/Triangle.h"

#include "Utils/Vec2D.h"

ArcadeScene::ArcadeScene()
{

}

void ArcadeScene::Init()
{

}

void ArcadeScene::Update(uint32_t dt)
{

}

void ArcadeScene::Draw(Screen& theScreen)
{
	Vec2D centerOfScreen = { static_cast<float>(theScreen.Width()) / 2, static_cast<float>(theScreen.Height()) / 2 };

	Star star(Vec2D(theScreen.Width() * 0.25, theScreen.Height() * 0.25), 50.f, 45.f, 20);

	Color starColor = Color(255, 255, 0, 150);
	AARectangle rect = { Vec2D(theScreen.Width() / 2 - 50, theScreen.Height() / 2 - 50), 50, 50 };

	Vec2D rotationPoint = star.GetCenterPoint() + Vec2D(0.0f, 0.0f);
	float deltaAngle = 0.0255f;

	theScreen.Draw(star, starColor, true, starColor);
	theScreen.Draw(rect, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

	star.Rotate(rotationPoint, deltaAngle);
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