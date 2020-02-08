#include <iostream>
#include "SDL.h"

#include "Graphics/Color.h"
#include "Graphics/Screen.h"
#include "Shapes/Lines2D.h"
#include "Utils/Vec2D.h"
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Star.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

int main(int argc, char *argv[])
{
	Screen theScreen;

	theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);

	Triangle triangle = { Vec2D(60, 10), Vec2D(10, 110), Vec2D(110, 110) };
	AARectangle rect = { Vec2D(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25), 50, 50 };
	Circle circle = { Vec2D(SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 + 50), 50};
	theScreen.Draw(triangle, Color::Red(), true);
	theScreen.Draw(rect, Color::Blue(), true);
	theScreen.Draw(circle, Color::Pink(), true);

	theScreen.SwapScreens();


	Star star(Vec2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 50.f, 45.f, 20);
	Vec2D rotationPoint = star.GetCenterPoint()+Vec2D(0.0f, 0.0f);
	//float deltaAngle = 0.0015f;
	//do
	//{
	//	theScreen.Draw(star, Color::Yellow());
	//	theScreen.SwapScreens();

	//	star.Rotate(rotationPoint, deltaAngle);
	//} while (false);

	//Line2D line = { Vec2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), Vec2D(SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT * 1 / 2) };
	//Vec2D rotationPoint = line.MidPoint() + Vec2D(0.0, 50.0);
	//do
	//{
	//	theScreen.Draw(line, Color::Yellow());
	//	theScreen.SwapScreens();

	//	float deltaAngle = 0.0025;
	//	line.Rotate(rotationPoint, deltaAngle);
	//} while (true);

	//Vec2D vecA(2.0, 4.0);
	//Vec2D vecB(1.0, 2.0);
	//std::cout << "vecA: " << vecA << "; vecB: " << vecB << std::endl;
	//std::cout << "0.0 * vecA: " << 0.0f*vecA << std::endl;
	//std::cout << "vecA + vecB: " << vecA + vecB << std::endl;
	//std::cout << "vecA - vecB: " << vecA - vecB << std::endl;
	//vecB -= vecA;
	//std::cout << "vecA: " << vecA << "; vecB: " << vecB << std::endl;
	//Vec2D normVec(5.f, 5.f);
	//std::cout << "normVec: " << normVec << std::endl;
	//std::cout << "Get unit vector: " << normVec.GetUnitVector() << std::endl;
	//normVec.Normalize();
	//std::cout << "normVec: " << normVec << std::endl;
	//Vec2D zeroVec(0.f, 0.f);
	//std::cout << "zeroVec: " << zeroVec << std::endl;
	//std::cout << "Get unit vector: " << zeroVec.GetUnitVector() << std::endl;
	//zeroVec.Normalize();
	//std::cout << "zeroVec: " << zeroVec << std::endl;
	//std::cout << "Zero magnitude: " << zeroVec.Mag() << std::endl;
	//Vec2D vec1(3, 4);
	//Vec2D vec2(1, 0);
	//std::cout << "vec1: " << vec1 << "; vec2: " << vec2 << std::endl;
	//std::cout << "vec1 projected onto vec2: " << vec1.ProjectOnto(vec2) << std::endl;
	//vec2.SetX(0.f);
	//vec2.SetY(1.f);
	//std::cout << "vec1: " << vec1 << "; vec2: " << vec2 << std::endl;
	//std::cout << "vec1 projected onto vec2: " << vec1.ProjectOnto(vec2) << std::endl;

	SDL_Event sdlEvent;
	bool running = true;

	while (running)
	{
		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}