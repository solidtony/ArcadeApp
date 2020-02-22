#include "Problems.h"

#include <random>
#include <vector>

#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Star.h"
#include "Shapes/Triangle.h"
#include "Utils/Vec2D.h"

namespace PracticeProblems
{

	void Section12PracticePrblem1(Screen* theScreen)
	{
		const int NUMBER_OF_STARS = 10;

		// Setup Random number
		std::random_device ranDevice;
		//std::default_random_engine ranEngine(ranDevice);

		//std::uniform_int_distribution<int> uniDistWidth(0, theScreen->Width());
		//std::uniform_int_distribution<int> uniDistHeight(0, theScreen->Height());
		//std::uniform_int_distribution<int> uniDistStarRadius(0, 20);
		//std::uniform_int_distribution<int> uniDistRotSpeed(0, 100);

		//std::vector<Star> stars;
		//std::vector<float> starsRotationSpeeds;

		//const int numberOfStarArms = 5;
		//for (int star = 0; star < NUMBER_OF_STARS; ++star)
		//{
		//	Vec2D starLocation = Vec2D(static_cast<float>(uniDistWidth(ranEngine)), static_cast<float>(uniDistHeight(ranEngine)) );
		//	float starRadius = static_cast<float>(uniDistStarRadius(ranEngine));
		//	float starThickness = starRadius * 0.50f;
		//	float starRotSpeed = static_cast<float>(uniDistRotSpeed(ranEngine))/1000.f;

		//	stars.push_back(Star(starLocation, starRadius, starThickness, numberOfStarArms));
		//	starsRotationSpeeds.push_back(starRotSpeed);
		//}

		//int frame = 0;
		//int endFrame = 100;
		//while (frame < endFrame)
		//{
		//	for (int star = 0; (star < static_cast<int>(stars.size())) && (star < static_cast<int>(starsRotationSpeeds.size())); ++star)
		//	{
		//		theScreen->Draw(stars[star], Color::Yellow(), true, Color::Yellow());
		//		stars[star].Rotate(stars[star].GetCenterPoint(), starsRotationSpeeds[star]);
		//	}

		//	theScreen->SwapScreens();
		//	++frame;
		//}
	}

}