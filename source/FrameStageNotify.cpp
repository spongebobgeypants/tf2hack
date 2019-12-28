#include "FrameStageNotify.h"
#include "CVisuals.h"
#include "CMiscellaneous.h"

void __fastcall FrameStageNotify(PVOID CHLClient, void *_this, ClientFrameStage_t Stage)
{
	if (Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i <= 64; i++)
		{
			// call your resolver here.
		}
	}

	if (Stage == FRAME_RENDER_START)
	{
		if (g::g_LocalPlayer && g::g_LocalPlayer->GetLifeState() == LIFE_ALIVE)
		{
			for (int i = 1; i <= 64; i++)
			{
				auto pPlayerEntity = GetBaseEntity(i);

				if (pPlayerEntity == NULL || pPlayerEntity->IsDormant() || pPlayerEntity->GetLifeState() != LIFE_ALIVE || pPlayerEntity == g::g_LocalPlayer)
				{
					continue;
				}

				if (gMiscellaneous::ShouldDisableInterpolation == true)
				{
					pPlayerEntity->SetAbsOrigin(pPlayerEntity->GetOrigin()); // way of disabling interpolation?. saw it in the csgo section.
				}
			}
		}
	}

	VMTManager &FrameStage = VMTManager::GetHook(CHLClient);
	return FrameStage.GetMethod<void(__fastcall *)(PVOID, void *, ClientFrameStage_t)>(35)(CHLClient, _this, Stage);
}

