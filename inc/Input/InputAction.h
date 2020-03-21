#pragma once

#include <functional>
#include <stdint.h>

using InputKey = uint8_t;
using InputState = uint8_t;
using InputAction = std::function<void(uint32_t dt, InputState state)>;

struct ButtonAction
{
	InputKey key;
	InputAction action;
};
