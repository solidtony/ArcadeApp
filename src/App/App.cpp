#include "App/App.h"

#include <iostream>
#include "SDL.h"

#include "Graphics/Color.h"

#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Lines2D.h"
#include "Shapes/Star.h"
#include "Shapes/Triangle.h"


#include "Utils/Vec2D.h"

App& App::Singleton()
{
	static App theApp;
	return theApp;
}

bool App::Init(uint32_t width, uint32_t height, uint32_t magnification)
{
	mnoptrWindow = mScreen.Init(width, height, magnification);
	return (mnoptrWindow != nullptr);
}

void App::Run()
{
	if (mnoptrWindow != nullptr)
	{
		Vec2D centerOfScreen = { static_cast<float>(mScreen.Width()) / 2, static_cast<float>(mScreen.Height()) / 2 };

		//Triangle triangle = { Vec2D(60, 10), Vec2D(10, 110), Vec2D(110, 110) };
		//AARectangle rect = { Vec2D(mScreen.Width() / 2 - 50, mScreen.Height() / 2 - 50), 50, 50 };
		//Circle circle = { Vec2D(mScreen.Width() / 2 + 50, mScreen.Height() / 2 + 50), 50};
		//mScreen.Draw(triangle, Color::Red(), true, Color::Red());
		//mScreen.Draw(rect, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
		//mScreen.Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

		//mScreen.SwapScreens();

		//triangle.MoveTo(centerOfScreen);

		//mScreen.Draw(triangle, Color::Red(), true, Color::Red());
		//mScreen.Draw(rect, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
		//mScreen.Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

		//mScreen.SwapScreens();

		//rect.MoveTo(centerOfScreen);

		//mScreen.Draw(triangle, Color::Red(), true, Color::Red());
		//mScreen.Draw(rect, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
		//mScreen.Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

		//mScreen.SwapScreens();

		//=============================================================================\\

		Star star(Vec2D(mScreen.Width() * 0.25, mScreen.Height() * 0.25), 50.f, 45.f, 20);

		Color starColor = Color(255, 255, 0, 150);
		AARectangle rect = { Vec2D(mScreen.Width() / 2 - 50, mScreen.Height() / 2 - 50), 50, 50 };

		mScreen.Draw(star, starColor, true, starColor);
		mScreen.SwapScreens();

		star.MoveTo(centerOfScreen);
		mScreen.Draw(star, starColor, true, starColor);
		mScreen.SwapScreens();

		Vec2D rotationPoint = star.GetCenterPoint() + Vec2D(0.0f, 0.0f);
		float deltaAngle = 0.0255f;
		do
		{
			mScreen.Draw(star, starColor, true, starColor);
			mScreen.Draw(rect, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
			mScreen.SwapScreens();

			star.Rotate(rotationPoint, deltaAngle);
		} while (true);

		//Line2D line = { Vec2D(mScreen.Width() / 2, mScreen.Height() / 2), Vec2D(mScreen.Width() * 3 / 4, mScreen.Height() * 1 / 2) };
		//Vec2D rotationPoint = line.MidPoint() + Vec2D(0.0, 50.0);
		//do
		//{
		//	mScreen.Draw(line, Color::Yellow());
		//	mScreen.SwapScreens();

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
	}
}