#include "CVisuals.h"
#include "Globals.h"
#include "Math.h"
#include <string>
#include <time.h>

void gVisuals::Do(CBaseEntity* pLocal)
{
	
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (pEntity->GetLifeState() != LIFE_ALIVE)
			continue;

		if (pEntity->GetTeamNum() == g::g_LocalPlayer->GetTeamNum() && !gMenu::Visuals::RenderOnTeammates)
			continue;

		gVisuals::RunPlayerVisuals(pEntity);
	}
}


// cringe

Color clrDynamic(CBaseEntity* pPlayerEntity)
{
	if (pPlayerEntity->GetTeamNum() == 2) // red
	{
		return Color(236, 56, 57, 200);
	}
	else if (pPlayerEntity->GetTeamNum() == 3) // blu
	{
		return Color(29, 124, 190, 200);
	}

	return Color(0, 0, 0, 0);
}

Color clrDynamicHealthColor(CBaseEntity* pPlayerEntity)
{
	auto iMaxHealth = pPlayerEntity->GetMaxHealth();
	auto iHalfHealth = iMaxHealth / 2;
	auto iThirdOfHealth = iHalfHealth / 2;

	// could add like a gradient thing
	// nah, hardcode 4 life
	if (pPlayerEntity->GetHealth() > iHalfHealth && pPlayerEntity->GetHealth() <= iMaxHealth)
	{
		return Color::Green();
	}
	else if (pPlayerEntity->GetHealth() > iThirdOfHealth && pPlayerEntity->GetHealth() <= iHalfHealth)
	{
		return Color::Yellow();
	}
	else if (pPlayerEntity->GetHealth() > 0 && pPlayerEntity->GetHealth() <= iThirdOfHealth)
	{
		return Color::Red();
	}

	return Color(0, 0, 0, 0);
}


void gVisuals::RunPlayerVisuals(CBaseEntity* pPlayerEntity)
{
	// Potassium. Yup. I dont really care tho.
	player_info_t pInfo;

	if (!gInts.Engine->GetPlayerInfo(pPlayerEntity->GetIndex(), &pInfo))
		return;

	const matrix3x4& vMatrix = pPlayerEntity->GetRgflCoordinateFrame();

	Vector vMin = pPlayerEntity->GetCollideableMins();
	Vector vMax = pPlayerEntity->GetCollideableMaxs();

	Vector vPointList[] = {
		Vector(vMin.x, vMin.y, vMin.z),
		Vector(vMin.x, vMax.y, vMin.z),
		Vector(vMax.x, vMax.y, vMin.z),
		Vector(vMax.x, vMin.y, vMin.z),
		Vector(vMax.x, vMax.y, vMax.z),
		Vector(vMin.x, vMax.y, vMax.z),
		Vector(vMin.x, vMin.y, vMax.z),
		Vector(vMax.x, vMin.y, vMax.z)
	};

	Vector vTransformed[8];

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			vTransformed[i][j] = vPointList[i].Dot((Vector&)vMatrix[j]) + vMatrix[j][3];

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if (!gDrawManager.WorldToScreen(vTransformed[3], flb) ||
		!gDrawManager.WorldToScreen(vTransformed[0], blb) ||
		!gDrawManager.WorldToScreen(vTransformed[2], frb) ||
		!gDrawManager.WorldToScreen(vTransformed[6], blt) ||
		!gDrawManager.WorldToScreen(vTransformed[5], brt) ||
		!gDrawManager.WorldToScreen(vTransformed[4], frt) ||
		!gDrawManager.WorldToScreen(vTransformed[1], brb) ||
		!gDrawManager.WorldToScreen(vTransformed[7], flt))
		return;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	float x = left;
	float y = bottom;
	float w = right - left;
	float h = top - bottom;

	x += ((right - left) / 8); 
	w -= ((right - left) / 8) * 2;

	Color clrPlayerCol = clrDynamic(pPlayerEntity);
	Color clrBoneCol = Color::White();
	int iY = 0;


	int iHp = pPlayerEntity->GetHealth(), iMaxHp = pPlayerEntity->GetMaxHealth();
	if (iHp > iMaxHp)
		iHp = iMaxHp;

	if (gMenu::Visuals::Box)
	{

		if (gMenu::Visuals::BoxStyle == 0)
		{
			gDrawManager.OutlineRect(x, y, w, h, clrDynamic(pPlayerEntity));
		}
		else if (gMenu::Visuals::BoxStyle == 1)
		{
			gDrawManager.OutlineRect(x - 1, y - 1, w + 2, h + 2, clrDynamic(pPlayerEntity));
			gDrawManager.OutlineRect(x, y, w, h, clrDynamic(pPlayerEntity));
			gDrawManager.OutlineRect(x + 1, y + 1, w - 2, h - 2, clrDynamic(pPlayerEntity));
		}
	}

	if (gMenu::Visuals::Glow)
	{
		DoGlow(pPlayerEntity);
	}


	if (gMenu::Visuals::Health)
	{
		auto iHealth = pPlayerEntity->GetHealth();
		auto iHealthHeight = h / iMaxHp * iHp;
		
		gDrawManager.DrawRect(x - 5, y + (h - (h / iMaxHp * iHp)) - 1, 2, iHealthHeight, clrDynamicHealthColor(pPlayerEntity));
		//gDrawManager.DrawString(x - 9, y + (h - (h / iMaxHp * iHp)) - 10, Color::White(), Font::Get().MenuText, "%d", iHealth);

	}

	if (gMenu::Visuals::Conditions)
	{
		// renders if enemy has conditions like uber ( and if theyre a medic, how much % ), cloaked, scoped, bonked etc.
		// a player can have multiple conditions thats why we will render all of the active ones. i didnt do this in absence.

		std::vector<const wchar_t*> GetConditions = { };

		int iCond = pPlayerEntity->GetCond();
		int iClass = pPlayerEntity->GetClassNum();
		int iUbercharge = pPlayerEntity->GetActiveWeapon()->GetUberChargeLevel();
		int iEntIndex = pPlayerEntity->GetIndex();

		std::string strUber = "Ubercharge: " + std::to_string((iUbercharge * 100)) + "%"; // Hopefully it prints "Ubercharge x %)

		if (iCond & TFCond_Ubercharged) // ubercharge is the first one to get drawn because i think its the most important one
		{
			GetConditions.push_back(L"Ubercharged");
			//gDrawManager.DrawString(x + w + 2, y + iY, clrBoneCol, Font::Get().Name, L"Ubercharged"); // using the function that takes a wchar as an argument. basically i want to minimize memory leaks
			//iY += gDrawManager.GetESPHeight();
		}
		if (iCond & TFCond_Cloaked) // cloaked.
		{
			GetConditions.push_back(L"Cloaked");
			//gDrawManager.DrawString(x + w + 2, y + iY + 2, clrBoneCol, Font::Get().ESP, L"Cloaked");
			//iY += gDrawManager.GetESPHeight();
		}
		if (iCond & TFCond_Bonked) // under the effect of BONK
		{ 
			GetConditions.push_back(L"Bonked");
			//gDrawManager.DrawString(x + w + 2, y + iY + 4, clrBoneCol, Font::Get().ESP, L"Bonked");
			//iY += gDrawManager.GetESPHeight();
		}
		if (iCond & TFCond_Zoomed)
		{
			GetConditions.push_back(L"Zoomed");
			//gDrawManager.DrawString(x + w + 2, y + iY + 6, clrBoneCol, Font::Get().ESP, L"Zoomed");
			//iY += gDrawManager.GetESPHeight();
		}


		for (int i = 0; i < GetConditions.size(); i++) //Loop through records.
		{
			int off = 0;
			gDrawManager.DrawString(x + w + 2, y + iY + off, clrBoneCol, Font::Get().Name, GetConditions.at(i));
			iY += gDrawManager.GetESPHeight();
			off += 2; //Double it after rendering.
		}
	}

	if (gMenu::Visuals::Skeleton)
	{
		static int iLeftArmBones[] = { 8, 7, 6, 4 };
		static int iRightArmBones[] = { 11, 10, 9, 4 };
		static int iHeadBones[] = { 0, 4, 1 };
		static int iLeftLegBones[] = { 14, 13, 1 };
		static int iRightLegBones[] = { 17, 16, 1 };

		DrawBone(pPlayerEntity, iLeftArmBones, 4, clrBoneCol);
		DrawBone(pPlayerEntity, iRightArmBones, 4, clrBoneCol);

		DrawBone(pPlayerEntity, iHeadBones, 3, clrBoneCol);

		DrawBone(pPlayerEntity, iLeftLegBones, 3, clrBoneCol);
		DrawBone(pPlayerEntity, iRightLegBones, 3, clrBoneCol);
	}
}




void gVisuals::DoGlow(CBaseEntity* pPlayerEntity)
{
	if (pPlayerEntity == NULL || pPlayerEntity->IsDormant() || pPlayerEntity->GetLifeState() != LIFE_ALIVE || pPlayerEntity == g::g_LocalPlayer)
	{
		// we dont want to do glow on these people.
		pPlayerEntity->GlowEnabled() = false;
		pPlayerEntity->DestroyGlowEffect();
 		return;
	}

	//Fool proof garbage.
	//static auto* glow_outline_effect_enable = gInts.Cvar->FindVar("glow_outline_effect_enable");
	//if (glow_outline_effect_enable->GetInt() != 1)
		//glow_outline_effect_enable->SetValue(1);
	
	pPlayerEntity->UpdateGlowEffect();
	pPlayerEntity->GlowEnabled() = true;
	//if it doesnt work, make sure you are playing on dx9 and have glow outlines enabled in settings.
}

void gVisuals::DoSkeleton(CBaseEntity* pPlayerEntity)
{


	static int iLeftArmBones[] = { 8, 7, 6, 4 };
	static int iRightArmBones[] = { 11, 10, 9, 4 };
	static int iHeadBones[] = { 0, 4, 1 };
	static int iLeftLegBones[] = { 14, 13, 1 };
	static int iRightLegBones[] = { 17, 16, 1 };

	DrawBone(pPlayerEntity, iLeftArmBones, 4, Color::White());
	DrawBone(pPlayerEntity, iRightArmBones, 4, Color::White());

	DrawBone(pPlayerEntity, iHeadBones, 3, Color::White());

	DrawBone(pPlayerEntity, iLeftLegBones, 3, Color::White());
	DrawBone(pPlayerEntity, iRightLegBones, 3, Color::White());
}



void gVisuals::DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color clrCol)
{
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
			continue;

		Vector vBone1 = pEntity->GetHitboxPosition(iBones[i]);
		Vector vBone2 = pEntity->GetHitboxPosition(iBones[i + 1]);

		Vector vScr1, vScr2;

		if (!gDrawManager.WorldToScreen(vBone1, vScr1) || !gDrawManager.WorldToScreen(vBone2, vScr2))
			continue;

		gDrawManager.DrawLine(vScr1.x, vScr1.y, vScr2.x, vScr2.y, clrCol);
	}
}
