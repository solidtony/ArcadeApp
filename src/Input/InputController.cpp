#include "Input/InputController.h"

#include <SDL2/SDL.h>
#include "Input/GameController.h"

InputController::InputController() :mQuit(nullptr), mnoptrCurrentController(nullptr)
{

}

void InputController::Init(InputAction quitAction)
{
	mQuit = quitAction;
}

void InputController::Update(uint32_t dt)
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
		{
			mQuit(dt, SDL_PRESSED);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			if (mnoptrCurrentController != nullptr)
			{
				if (MouseMovedAction mouseMoved = mnoptrCurrentController->GetMouseMovedAction())
				{
					MousePosition position;
					position.xPos = sdlEvent.motion.x;
					position.yPos = sdlEvent.motion.y;
					mouseMoved(position);
				}
			}
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		{
			if (mnoptrCurrentController != nullptr)
			{
				MouseInputAction action = mnoptrCurrentController->GetMouseButtonActionForMouseButton(static_cast<MouseButton>(sdlEvent.button.button));
				MousePosition position;
				position.xPos = sdlEvent.button.x;
				position.yPos = sdlEvent.button.y;

				action(static_cast<InputState>(sdlEvent.button.state), position);
			}
			break;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			if (mnoptrCurrentController != nullptr)
			{
				InputAction action = mnoptrCurrentController->GetActionForKey(sdlEvent.key.keysym.sym);

				action(dt, static_cast<InputState>(sdlEvent.key.state));
			}
			break;
		}
		}
	}
}

void InputController::SetGameController(GameController* controller)
{
	mnoptrCurrentController = controller;
}