#pragma once
#undef __attribute__
#include "pch.h"

inline std::function<void(const char* name, const uint8_t* data, int size)> g_loadfile_cb{};
extern "C"
{
    void emsc_js_load(void);
    inline EMSCRIPTEN_KEEPALIVE int emsc_loadfile(const char* name, const uint8_t* data, int size)
    {
        if (g_loadfile_cb)
        {
            g_loadfile_cb(name, data, size);
            g_loadfile_cb = nullptr;
        }
        return 0;
    }
}
inline void emsc_load_file(decltype(g_loadfile_cb) callback)
{
    g_loadfile_cb = callback;
    emsc_js_load();
}