#include "pch.h"
#include "renderer/renderer.h"
#include "filesystem/filesystem_js.h"

using emscripten::val;

void loop()
{
    g_renderer.loop();
}

int main()
{
    emsc_js_init();
    emsc_js_load();
    std::cout << "Main function started" << std::endl;
    g_renderer.create("Vali | fuckk.lol");
    emscripten_set_main_loop(loop, 0, true);
    g_renderer.destroy();
    return 0;
}