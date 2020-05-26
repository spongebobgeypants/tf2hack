
#include "Menu.h"
#include "Globals.h"

const char* BoxesType[] = { "Defualt", "Outline" };

void CMenu::DrawMenuInterface()
{
	auto& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2{ 100, 100 }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 450, 280 }, ImGuiSetCond_Once);
	style.Alpha = 0.9f;

	ImGui::Begin("TEAMSENSE", &g::b_MenuEnable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginChild(("first penis"), ImVec2(400, 260), true);
		{
			ImGui::Separator();
			ImGui::Text(("ESP"));
			ImGui::Separator();
			ImGui::Checkbox("Box", &gMenu::Visuals::Box);
			ImGui::SameLine(150);
			ImGui::Checkbox(("HP"), &gMenu::Visuals::Health);
			ImGui::SameLine(300);
			ImGui::Checkbox(("Skeleton"), &gMenu::Visuals::Skeleton);
			ImGui::Separator();
			ImGui::Text(("ESP Filter"));
			ImGui::Checkbox("Render on teammates", &gMenu::Visuals::RenderOnTeammates);
			ImGui::Separator();
			ImGui::Text(("ESP Options"));
			ImGui::Combo("##Type", &gMenu::Visuals::BoxStyle, BoxesType, ARRAYSIZE(BoxesType));
			ImGui::Separator();
			ImGui::Text(("Removals"));
			ImGui::Separator();
			ImGui::Checkbox("Remove scope", &gMenu::Visuals::RemoveScope);
			ImGui::Separator();
			ImGui::Text(("Miscellaneous"));
			ImGui::Separator();
			ImGui::Checkbox("Conditions", &gMenu::Visuals::Conditions);
			ImGui::SameLine(150);
			ImGui::Checkbox("Glow", &gMenu::Visuals::Glow);
			ImGui::Separator();
			ImGui::Checkbox("Backtracking", &gMenu::Aim::Backtracking);
			ImGui::SameLine(150);
			ImGui::Checkbox("Auto-jump", &gMenu::Movement::Autojump);
			ImGui::SameLine(300);
			ImGui::Checkbox("Auto-strafe", &gMenu::Movement::Autostrafe);		
			ImGui::Checkbox("Bypass sv_pure", &gMenu::Miscellaneous::BypassPure);
			ImGui::Separator();

		}
		ImGui::EndChild();
	}
	ImGui::End(); //End main window
}

