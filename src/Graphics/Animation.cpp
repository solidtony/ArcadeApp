#include "Graphics/Animation.h"
#include "Utils/FileCommandLoader.h"

Animation::Animation() : mSize(Vec2D::Zero()), mSpriteSheetName(""), mAnimationName(""), mFPS(0)
{

}

AnimationFrame Animation::GetAnimationFrame(uint32_t frameNum) const
{
	AnimationFrame frame;
	
	if (frameNum > mFrames.size())
	{
		return frame;
	}

	frame.frame = mFrames[frameNum];
	
	if (frameNum < mFrameColors.size())
	{
		frame.frameColor = mFrameColors[frameNum];
		frame.frameColorSet = 1;
	}

	if (mOverlay.size() > 0)
	{
		frame.overlay = mOverlay;
	}

	if (frameNum < mOverlayColors.size())
	{
		frame.overlayColor = mOverlayColors[frameNum];
	}

	if (frameNum < mFrameOffsets.size())
	{
		frame.offset = mFrameOffsets[frameNum];
	}

	frame.size = mSize;

	return frame;
}

std::vector<Animation> Animation::LoadAnimations(const std::string& animationFilePath)
{
	std::vector<Animation> animations;

	FileCommandLoader fileLoader;

	Command animationCommand;
	animationCommand.command = "animation";
	animationCommand.parseFunc = [&](ParseFuncParams params)
	{
		Animation newAnimation;
		newAnimation.SetName(FileCommandLoader::ReadString(params));
		animations.push_back(newAnimation);
	};
	fileLoader.AddCommand(animationCommand);

	Command spriteSheetCommand;
	spriteSheetCommand.command = "sprite_sheet";
	spriteSheetCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().SetSpriteSheetName(FileCommandLoader::ReadString(params));
	};
	fileLoader.AddCommand(spriteSheetCommand);

	Command sizeCommand;
	sizeCommand.command = "size";
	sizeCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().SetSize(FileCommandLoader::ReadSize(params));
	};
	fileLoader.AddCommand(sizeCommand);
}