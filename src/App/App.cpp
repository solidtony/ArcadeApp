#include "App/App.h"

#include "SDL.h"

#include <cassert>
#include <iostream>

#include "Scenes/ArcadeScene.h"
#include "Scenes/Scene.h"

App& App::Singleton()
{
	static App theApp;
	return theApp;
}

bool App::Init(uint32_t width, uint32_t height, uint32_t magnification)
{
	mnoptrWindow = mScreen.Init(width, height, magnification);

	std::unique_ptr<ArcadeScene> arcadeScene = std::make_unique<ArcadeScene>();

	PushScene(std::move(arcadeScene));

	return (mnoptrWindow != nullptr);
}

void App::Run()
{
	if (mnoptrWindow != nullptr)
	{
		bool running = true;

		uint32_t lastTick = SDL_GetTicks();
		uint32_t currentTick = lastTick;

		uint32_t dt = 10; // Update scene every 10ms
		uint32_t accumulator = 0;

		mInputController.Init([&running](uint32_t dt, InputState state)
		{
			running = false;
		});

		while (running)
		{
			currentTick = SDL_GetTicks();
			uint32_t frameTime = currentTick - lastTick;

			if (frameTime > 300)
			{
				frameTime = 300;
			}

			lastTick = currentTick;

			accumulator += frameTime; // Number of total ticks

			// Input
			mInputController.Update(dt);

			Scene* topScene = App::TopScene();
			assert(topScene && "Must have a scene");

			if (topScene)
			{
				// Update
				while (accumulator >= dt)
				{
					// Update current scene by dt
					topScene->Update(dt);
					accumulator -= dt;
				}

				// Render
				topScene->Draw(mScreen);
			}

			mScreen.SwapScreens();
		}
	}
}

void App::PushScene(std::unique_ptr<Scene> scene)
{
	assert(scene && "Don't push nullptr");
	{
		scene->Init();
		mInputController.SetGameController(scene->GetGameController());
		mSceneStack.emplace_back(std::move(scene));
		SDL_SetWindowTitle(mnoptrWindow, TopScene()->GetSceneName().c_str());
	}
}

void App::PopScene()
{
	if (mSceneStack.size() > 1)
	{
		mSceneStack.pop_back();
	}

	if (TopScene())
	{
		mInputController.SetGameController(TopScene()->GetGameController());
		SDL_SetWindowTitle(mnoptrWindow, TopScene()->GetSceneName().c_str());
	}
}

Scene* App::TopScene()
{
	if (mSceneStack.empty())
	{
		return nullptr;
	}

	return mSceneStack.back().get();
}