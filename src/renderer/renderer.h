#pragma once
#include "pch.h"
#include "util/bridge.h"
#include "imconfig.h"
#define IMGUI_IMPL_OPENGL_ES3 1
#include "imgui_internal.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

class callbacks
{
public:
	callbacks(bool active, std::function<void(bool&)> fn) : m_active(active), m_fn(fn) {}
	callbacks(std::function<void(bool&)> fn) : m_active(true), m_fn(fn) {}
    ~callbacks()
    {
        m_active = false;
        m_fn = nullptr;
    }
public:
	void invoke()
    {
		if (m_fn)
        {
			if (m_active)
            {
				m_fn(m_active);
			}
		}
	}
private:
	bool m_active;
	std::function<void(bool&)> m_fn;
};

inline void i_window(const std::string& name, bool* p_open, ImGuiWindowFlags flags, std::function<void()> callback)
{
    if (p_open && !(*p_open))
    {
        return;
    }
    ImGui::Begin(name.c_str(), p_open, flags);
    if (callback)
    {
        callback();
    }
    ImGui::End();
}

inline void im_bar(std::function<void()> callback)
{
    if (ImGui::BeginMenuBar())
    {
        if (callback)
        {
            callback();
        }
        ImGui::EndMenuBar();
    }
}

inline void im_menu(const std::string& name, std::function<void()> callback)
{
    if (ImGui::BeginMenu(name.c_str()))
    {
        if (callback)
        {
            callback();
        }
        ImGui::EndMenu();
    }
}

inline void im_item(const std::string& name, std::function<void()> callback)
{
    if (ImGui::MenuItem(name.c_str()))
    {
        if (callback)
        {
            callback();
        }
    }
}

template <typename ...A>
inline void i_text(const std::string& format, A... args)
{
    std::string msg{ std::vformat(format, std::make_format_args(args...)) };
    ImGui::Text(msg.c_str());
}

struct string
{
	string(std::string str, ImFont* font) :
        m_string(str), m_font(font)
    {}
	string(std::string str) :
        m_string(str), m_font(nullptr)
    {}
	string() = default;
	std::string m_string{};
	ImFont* m_font{};
};

class renderer;

class renderer final
{
public:
    renderer() = default;
    ~renderer() = default;
    bool create(const std::string& window_name);
    void get_sizes();
    void set_style();
    void destroy();

    void loop();
    std::vector<callbacks> m_callbacks{};
private:
    GLFWwindow* m_window{};
    ImGuiContext* m_gcontext{};
    Vector2 m_window_size{};
    Vector2 m_screen_size{};
    Vector2 m_canvas_size{};
    ImColor m_clear_color{ 30, 30, 30, 255 };
    bool m_canvas_fullscreen{};
};
inline renderer g_renderer{};

inline void i_popup(const std::string& window_name, std::vector<string> strings, float offset = 410.f)
{
	g_renderer.m_callbacks.push_back(callbacks([&](bool& active) {
		ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
		ImGui::SetNextWindowPos({ center.x - (470.f * 0.5f), center.y - (275.f * 0.5f) }, ImGuiCond_Always);
		ImGui::SetNextWindowSize({ 470.f, 275.f }, ImGuiCond_Always);
		if (ImGui::BeginPopupModal(window_name.c_str(), NULL, ImGuiWindowFlags_NoResize))
        {
			for (auto& s : strings)
            {
				//ImGui::PushFont(s.m_font);
				i_text(s.m_string);
				//ImGui::PopFont();
			}
			ImGui::SameLine(offset);
			ImGui::SetItemDefaultFocus();
			if (ImGui::Button("Close"))
            {
				ImGui::CloseCurrentPopup();
				active = false;
			}
			ImGui::EndPopup();
		}
		ImGui::OpenPopup(window_name.c_str());
	}));
}