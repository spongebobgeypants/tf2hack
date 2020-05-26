#pragma once
#include "SDK.h"
#include "CDrawManager.h"

namespace gVisuals
{
	void Do(CBaseEntity * pLocal);
	void RunPlayerVisuals(CBaseEntity * pPlayerEntity);
	void ShowSniperDamage();
	void DoGlow(CBaseEntity * pPlayerEntity);
	void DoSkeleton(CBaseEntity * pPlayerEntity);
	void DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color clrCol);

}