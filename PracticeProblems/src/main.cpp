#include "Graphics/Screen.h"

#include "Problems.h"
#include "Utils/Vec2D.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

int main()
{
	Screen theScreen;
	Vec2D centerOfScreen = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);

	PracticeProblems::Section12PracticePrblem1(&theScreen);

	return 0;
}