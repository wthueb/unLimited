#pragma once

#include <imgui.h>

#include "options.hpp"

namespace gui
{
	static void draw_gui()
	{
		ImGui::SetNextWindowSize(ImVec2(850, 500));

		if (ImGui::Begin("unLimited", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Checkbox("backtracking", &options::misc::backtracking);
			ImGui::Checkbox("bactracking visual", &options::misc::backtracking_vis);

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

		style.WindowTitleAlign = { .5f, .5f }; // center title text

		style.WindowRounding = 3.f;

		style.Colors[ImGuiCol_WindowBg] = ImVec4(.85f, .85f, .85f, 1.f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.f, 1.f, 1.f, 1.f);
		style.Colors[ImGuiCol_Text] = ImVec4(.1f, .1f, .1f, 1.f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(.2f, .2f, .2f, 1.f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(.5f, .5f, .5f, 1.f);
	}
}
