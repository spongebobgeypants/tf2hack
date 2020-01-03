
#include "Menu.h"
#include "Globals.h"

const char* BoxesType[] = { "Defualt", "Outline" };
const char* Aimtype[] = { "Legit", "Rage" };
const char* AimMove[] = { "Plain", "Silent", "Smooth" };

const char* Targetselection[] = { "Fov", "Distance" };
const char* Hitbox[] = { "Head", "Body", "Hitscan" };
const char* OMode[] = { "Off", "Low", "High" }; // low = adjust commands like cl_interp, cl_interp_ratio etc, high = both low mode and it also disables interpolation


void BtnNormal()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
}

void BtnActive()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
}

void Aim()
{
	ImGui::Checkbox("Enabled", &gMenu::Aim::MasterSwitch);
	ImGui::Checkbox("Backtracking", &gMenu::Aim::Backtracking);

}

void Movement()
{
	ImGui::Checkbox("Enable", &gMenu::Movement::Enable);
	ImGui::Checkbox("Auto-jump", &gMenu::Movement::Autojump);
	ImGui::Checkbox("Auto-strafe", &gMenu::Movement::Autostrafe);

}

void Visuals()
{
	ImGui::Checkbox("Enable", &gMenu::Visuals::Enable);
	ImGui::Checkbox("Box", &gMenu::Visuals::Box);
	ImGui::Combo("##Type", &gMenu::Visuals::BoxStyle, BoxesType, ARRAYSIZE(BoxesType));
	ImGui::Checkbox("Skeleton", &gMenu::Visuals::Skeleton);
	ImGui::Checkbox("Health", &gMenu::Visuals::Health);
	ImGui::Checkbox("Conditions", &gMenu::Visuals::Conditions);
	ImGui::Checkbox("Glow", &gMenu::Visuals::Glow);
	ImGui::Text(("Removals"));
	ImGui::Separator();
	ImGui::Checkbox("Remove scope", &gMenu::Visuals::RemoveScope);
	ImGui::Text(("Miscellaneous"));
	ImGui::Separator();
	ImGui::Checkbox("Render on teammates", &gMenu::Visuals::RenderOnTeammates); 

}

void Miscellaneous()
{
	ImGui::Checkbox("Bypass sv_pure", &gMenu::Miscellaneous::BypassPure);
	ImGui::Checkbox("Force australiums", &gMenu::Miscellaneous::Australium);


}


void CMenu::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	ImGui::Begin("TEAMSENSE", &g::b_MenuEnable, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginChild("##TABCHILD", ImVec2(56, -1), true);
		{
			if (g::i_MenuTab == 0) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_CROSSHAIRS, ImVec2(-1, 30))) { g::i_MenuTab = 0; }

			if (g::i_MenuTab == 1) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_EYE, ImVec2(-1, 30))) { g::i_MenuTab = 1; }

			if (g::i_MenuTab == 2) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_EJECT, ImVec2(-1, 30))) { g::i_MenuTab = 2; }

			if (g::i_MenuTab == 3) BtnActive(); else BtnNormal();
			if (ImGui::Button(ICON_FA_COG, ImVec2(-1, 30))) { g::i_MenuTab = 3; }

			BtnNormal();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("##FEATURESCHILD", ImVec2(-1, -1), true);
		{
			if (g::i_MenuTab == 0) { Aim(); }
			if (g::i_MenuTab == 1) { Visuals(); }
			if (g::i_MenuTab == 2) { Movement(); }
			if (g::i_MenuTab == 3) { Miscellaneous(); }
		}
		ImGui::EndChild();
	}
	ImGui::End();
}