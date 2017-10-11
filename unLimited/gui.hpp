#pragma once

#include <imgui.h>

namespace gui
{
	void draw_gui()
	{
		ImGui::SetNextWindowSize(ImVec2(850, 500));

		if (ImGui::Begin("unLimited", nullptr, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings))
		{
			// FIXMEW: do some dope stuff here

			ImGui::Columns(1);

			ImGui::Text("unLimited by wi1");
			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("http://wi1.us.to/").x - 17);
			ImGui::Text("http://wi1.us.to/");

			ImGui::End();
		}
	}

	void setup_style()
	{
		auto &style = ImGui::GetStyle();

		auto &io = ImGui::GetIO();

		io.Fonts->AddFontDefault();
	}
}
