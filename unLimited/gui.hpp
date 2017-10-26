#pragma once

#include <imgui.h>

#include "font.hpp"
#include "options.hpp"

namespace gui
{
	static void draw_gui()
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(850, 500));

		if (ImGui::Begin("unLimited", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Checkbox("backtracking", &options::misc::backtracking);
			ImGui::Checkbox("backtracking visual", &options::misc::backtracking_vis);

			ImGui::Separator();

			ImGui::Checkbox("airstuck", &options::misc::airstuck);

			ImGui::Columns(1);

			ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::CalcTextSize("http://wi1.us.to/").y - 10);
			ImGui::Text("unLimited by wi1");
			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("http://wi1.us.to/").x - 10);
			ImGui::Text("http://wi1.us.to/");
			
			ImGui::End();
		}
	}

	static void setup_style()
	{
		auto &style = ImGui::GetStyle();

		auto &io = ImGui::GetIO();

		io.Fonts->AddFontDefault();
		auto font = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoFont_compressed_data, RobotoFont_compressed_size, 14.f);
		io.FontDefault = font;

		style.WindowTitleAlign = { .5f, .5f };
		style.WindowRounding = 3.f;

		auto theme_color = ImVec4(.65f, .85f, .95f, 1.f);

		style.Colors[ImGuiCol_Text] = ImVec4(.05f, .05f, .05f, 1.f);
		style.Colors[ImGuiCol_TextDisabled] = style.Colors[ImGuiCol_Text];
		style.Colors[ImGuiCol_WindowBg] = ImVec4(.9f, .9f, .9f, 1.f);
		style.Colors[ImGuiCol_ChildWindowBg] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_PopupBg] = ImVec4(.6f, .6f, .6f, 1.f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.f, 0.f, 0.f, 1.f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(.2f, .2f, .2f, 1.f);
		style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_FrameBg];
		style.Colors[ImGuiCol_FrameBgActive] = style.Colors[ImGuiCol_FrameBg];
		style.Colors[ImGuiCol_TitleBg] = theme_color;
		style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_TitleBg];
		style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_TitleBg];
		style.Colors[ImGuiCol_MenuBarBg] = style.Colors[ImGuiCol_TitleBg];
		style.Colors[ImGuiCol_ScrollbarBg] = style.Colors[ImGuiCol_TitleBg];
		style.Colors[ImGuiCol_ScrollbarGrab] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = style.Colors[ImGuiCol_ScrollbarGrab];
		style.Colors[ImGuiCol_ScrollbarGrabActive] = style.Colors[ImGuiCol_ScrollbarGrab];
		style.Colors[ImGuiCol_ComboBg] = style.Colors[ImGuiCol_FrameBg];
		style.Colors[ImGuiCol_CheckMark] = style.Colors[ImGuiCol_TitleBg];
		// FIXMEW: finish style setup, this is boring lol
	}
}
