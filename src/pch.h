#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <format>
#define deprecated(x)
#define __attribute__(x)
#include <emscripten.h>
#include <emscripten/val.h>
#include <SDL/SDL.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#define SANITY(x, ...) if (!(x)) { glfwTerminate(); __VA_ARGS__; }
#include "common/types.h"
#include "common/vectors.h"

template <typename cT, typename vT>
inline cT cast_value(vT v)
{
    return static_cast<cT>(v);
}