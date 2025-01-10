#pragma once
#include "pch.h"

inline Vector2 em_get_screen_size()
{  
    int x{}, y{};
    emscripten_get_screen_size(&x, &y);
    return Vector2(x, y);
}
inline std::pair<Vector2, bool> em_get_canvas_size()
{  
    int x{}, y{};
    int fullscreen{};
    emscripten_get_canvas_size(&x, &y, &fullscreen);
    return { Vector2(x, y), fullscreen ? true : false };
}
inline void em_set_window_title(const std::string& title)
{
    emscripten_set_window_title(title.c_str());
}