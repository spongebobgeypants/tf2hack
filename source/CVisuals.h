#pragma once
#include "SDK.h"
#include "CDrawManager.h"

namespace gVisuals
{
	void DrawPlayers(int i);
	void DrawItems(int i);
	void Do(CBaseEntity * pLocal);
	void RunPlayerVisuals(CBaseEntity * pPlayerEntity);
	void ShowSniperDamage();
	void DrawCurrentTime();
	void ShowPickups(CBaseEntity * pBaseEntity, int x, int y);
	void DoGlow(CBaseEntity * pPlayerEntity);
	void DoSkeleton(CBaseEntity * pPlayerEntity);
	void DrawBox(RECT rect, Color Col);
	RECT DrawDynamicBox(CBaseEntity * pPlayer, bool & bPVS);
	void DrawBacktrackSkeleton(CBaseEntity * pEntity, int * iBones, int count, Color clrCol);
	void DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color clrCol);

}