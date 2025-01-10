#include "pch.h"
#include "renderer.h"
#define STBVOX_CONFIG_MODE 0
#define STB_VOXEL_RENDER_IMPLEMENTATION
#include <stb_voxel_render.h>
#include "filesystem/filesystem.h"
#include "util/sid.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
}


bool renderer::create(const std::string& window_name)
{
    SANITY(glfwInit(), return false);
    get_sizes();
    em_set_window_title(window_name);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    SANITY(m_window = glfwCreateWindow(cast_value<int>(m_window_size.x), cast_value<int>(m_window_size.y), window_name.c_str(), NULL, NULL), return false);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    m_gcontext = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_gcontext);
    ImGuiIO& io{ ImGui::GetIO() };
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(m_window, "#canvas");
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();
    set_style();
    return true;
}
void renderer::get_sizes()
{
    m_window_size = em_get_screen_size();
    auto canvas_size = em_get_canvas_size();
    m_canvas_size = canvas_size.first;
    m_canvas_fullscreen = canvas_size.second;
}
void renderer::set_style()
{
    ImGuiStyle& style{ ImGui::GetStyle() };
    style.Alpha = 1.f;
    style.DisabledAlpha = 0.95f;
	style.WindowPadding = { 10.f, 10.f };
	style.WindowRounding = 0.f;
	style.WindowBorderSize = 1.f;
    style.WindowMinSize = { 5.f, 5.f};
	style.WindowTitleAlign = { 0.5f, 0.5f };
    style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.f;
	style.ChildBorderSize = 0.f;
	style.PopupRounding = 0.f;
	style.PopupBorderSize = 1.f;
	style.FramePadding = { 8.f, 4.f };
	style.FrameRounding = 0.f;
	style.FrameBorderSize = 0.f;
	style.ItemSpacing = { 10.f, 8.f };
	style.ItemInnerSpacing = { 6.f, 6.f };
	style.TouchExtraPadding = { 0.f, 0.f };
	style.IndentSpacing = 21.f;
	style.ScrollbarSize = 15.f;
	style.ScrollbarRounding = 0.f;
	style.GrabMinSize = 8.f;
	style.GrabRounding = 0.f;
	style.TabRounding = 0.f;
	style.TabBorderSize = 0.f;
    style.TabBarBorderSize = 0.5f;
    style.TabBarOverlineSize = 0.f;
	style.ButtonTextAlign = { 0.5f, 0.5f };
	style.DisplaySafeAreaPadding = { 3.f, 3.f };
    style.MouseCursorScale = 1.f;

	ImVec4* colors{ style.Colors };
    colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
    colors[ImGuiCol_TextDisabled] = ImColor(255, 230, 49, 255);
    colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 127);
    colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_PopupBg] = ImColor(20, 20, 20, 240);
    colors[ImGuiCol_Border] = ImColor(76, 76, 76, 127);
    colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_FrameBg] = ImColor(54, 54, 54, 138);
    colors[ImGuiCol_FrameBgHovered] = ImColor(54, 54, 54, 199);
    colors[ImGuiCol_FrameBgActive] = ImColor(71, 69, 69, 138);
    colors[ImGuiCol_TitleBg] = ImColor(43, 43, 43, 255);
    colors[ImGuiCol_TitleBgActive] = ImColor(48, 48, 48, 255);
    colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);
    colors[ImGuiCol_MenuBarBg] = ImColor(36, 36, 36, 255);
    colors[ImGuiCol_ScrollbarBg] = ImColor(5, 5, 5, 135);
    colors[ImGuiCol_ScrollbarGrab] = ImColor(79, 79, 79, 255);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(104, 104, 104, 255);
    colors[ImGuiCol_ScrollbarGrabActive] = ImColor(130, 130, 130, 255);
    colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
    colors[ImGuiCol_SliderGrab] = ImColor(87, 87, 87, 255);
    colors[ImGuiCol_SliderGrabActive] = ImColor(99, 97, 97, 255);
    colors[ImGuiCol_Button] = ImColor(104, 104, 104, 189);
    colors[ImGuiCol_ButtonHovered] = ImColor(104, 104, 104, 199);
    colors[ImGuiCol_ButtonActive] = ImColor(104, 104, 104, 222);
    colors[ImGuiCol_Header] = ImColor(94, 94, 94, 79);
    colors[ImGuiCol_HeaderHovered] = ImColor(97, 97, 97, 94);
    colors[ImGuiCol_HeaderActive] = ImColor(94, 94, 94, 130);
    colors[ImGuiCol_Separator] = ImColor(97, 97, 97, 127);
    colors[ImGuiCol_SeparatorHovered] = ImColor(117, 117, 117, 127);
    colors[ImGuiCol_SeparatorActive] = ImColor(117, 117, 117, 163);
    colors[ImGuiCol_ResizeGrip] = ImColor(66, 66, 66, 255);
    colors[ImGuiCol_ResizeGripHovered] = ImColor(79, 79, 79, 255);
    colors[ImGuiCol_ResizeGripActive] = ImColor(89, 89, 89, 255);
    colors[ImGuiCol_Tab] = ImColor(54, 54, 54, 219);
    colors[ImGuiCol_TabHovered] = ImColor(69, 69, 69, 219);
    colors[ImGuiCol_TabActive] = ImColor(87, 87, 87, 219);
    colors[ImGuiCol_TabUnfocused] = ImColor(25, 25, 25, 247);
    colors[ImGuiCol_TabUnfocusedActive] = ImColor(38, 38, 38, 255);
    colors[ImGuiCol_PlotLines] = ImColor(155, 155, 155, 255);
    colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 110, 89, 255);
    colors[ImGuiCol_PlotHistogram] = ImColor(229, 179, 0, 255);
    colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 153, 0, 255);
    colors[ImGuiCol_TextSelectedBg] = ImColor(66, 150, 250, 89);
    colors[ImGuiCol_DragDropTarget] = ImColor(255, 255, 0, 230);
    colors[ImGuiCol_NavHighlight] = ImColor(66, 150, 250, 255);
    colors[ImGuiCol_NavWindowingHighlight] = ImColor(255, 255, 255, 179);
    colors[ImGuiCol_NavWindowingDimBg] = ImColor(204, 204, 204, 51);
    colors[ImGuiCol_ModalWindowDimBg] = ImColor(204, 204, 204, 89);
}
void renderer::destroy()
{
    m_callbacks.clear();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void load_sid(sid& sid)
{
    emsc_load_file([&](const char* name, const uint8_t* data, int size) {
        sid.set_data(eC64Model::mPerfered, 44100, 0);
        sid.load(name, const_cast<uint8_t*>(data), size);
        sid.setup_sdl();
    });
}

std::string add_leading_zero(int v)
{
    return (v < 10 && v >= 0 ? "0" : "") + std::to_string(v);
}

void renderer::loop()
{
    glfwPollEvents();
    if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    m_window_size = { display_w, display_h };
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
	for (auto& c : m_callbacks)
		c.invoke();
    ImVec4 clear_color{ m_clear_color };
    ImGuiIO& io{ ImGui::GetIO() };
    ImGui::SetNextWindowPos({ 0.f, 0.f }, ImGuiCond_Always);
    ImGui::SetNextWindowSize(m_window_size, ImGuiCond_Always);
    i_window("canvas", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar, [&] {
        im_bar([&] {
            im_menu("SID Player", [&] {
                i_text("A very very basic SID player that is currently incomplete");
                i_text("Currently only has support for one tune");
                ImGui::Separator();
                static sid _sid{};
                if (ImGui::Button("Load SID"))
                { 
                    load_sid(_sid);
                }
                if (_sid.m_current_subtune.id != -1)
                {
                    i_text("Author: {}", _sid.metadata.author);
                    i_text("Title: {}", _sid.metadata.title);
                    i_text("Info: {}", _sid.metadata.info);
                    i_text("Default tune: {}", _sid.metadata.default_tune);
                    i_text("Total tunes: {}", _sid.metadata.total_tunes);
                    int miliseconds = (int)(_sid.m_time_elapsed * 1000);
                    int seconds = (int)(_sid.m_time_elapsed);
                    int minutes = seconds / 60;
                    int hours = minutes / 60;
                    i_text("Time: {}:{}:{}", add_leading_zero(hours), add_leading_zero(minutes - (hours * 60)), add_leading_zero(seconds - (minutes * 60)));
                    ImGui::SliderFloat("Volume", &_sid.m_current_subtune.volume, 0.f, 1.f);
                    if (ImGui::InputInt("Frequency", &_sid.m_samplerate, 1, 100))
                    {
                        //_sid.reset();
                    }
                    if (!_sid.m_current_subtune.active)
                    {
                        if (ImGui::Button("Play"))
                        {
                            _sid.play();
                        }
                    }
                    else
                    {
                        if (ImGui::Button("Pause"))
                        {
                            _sid.pause();
                        }
                    }
                    ImGui::Checkbox("Auto-start on load", &_sid.m_autostart_on_load);
                    if (ImGui::Button("Reset"))
                    {
                        _sid.reset();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Load a different SID"))
                    {
                        _sid.reset();
                        load_sid(_sid);
                    }
                }
                else
                {
                    i_text("No controls avaliable! Please load a .SID (MOS 6581/8580 Sound Interfacie Device) file");
                }
            });
            im_menu("Theme", [] {
                ImGui::ShowStyleEditor();
            });
            im_item("SpaceHey", [] {
                emscripten_run_script(R".(window.open("https://spacehey.com/vali0004", "_blank")).");
            });
            im_item("About", [] {
			    //std::vector<string> strings{};
			    //strings.push_back(string("fuck.lol"/*, s_ArialBD21*/));
			    //strings.push_back(string("\n"));
			    //strings.push_back(string("My personal website, sort of like a bio"));
			    //strings.push_back(string("\n\n\n\n\n\n\n\n"));
			    //strings.push_back(string("Copyright Vali (2021-2025)"/*, s_Arial15*/));
			    //i_popup("About", strings);
            });
            ImGui::SetCursorPosX(m_window_size.x - 90.f);         
            i_text("Hello, there.");
        });
        i_text("Right now, this site doesn't have much.\nBut! that'll change soon. Just check back in a couple of days :)");
        ImGui::SetCursorPos({ m_window_size.x - 215.f, m_window_size.y - 25.f });
        i_text("Copyright (c) Vali | 2021-2025");
    });
    // Trick to allow us to use ImColor
    m_clear_color = clear_color;
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}