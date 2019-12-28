#pragma once
#include "SDK.h"

namespace g // same as in stickrpg. sue me.
{
	extern CBaseEntity* g_LocalPlayer;
	extern CUserCmd* g_UserCmd;
	extern bool b_MenuEnable;
	extern int i_MenuTab;

}

namespace gMenu
{
	extern bool bExample;

	namespace Aim
	{
		extern bool MasterSwitch;
		extern bool Enable;
		extern int  TargetSelection;
		extern int  FOV;
		extern int  Smooth;
		extern int  LockMethord;
		extern bool pSilent;
		extern int  Hitbox;
		extern int  Mode;
		extern bool Backtracking;
		extern bool Autoshoot;

	}

	namespace Visuals
	{
		extern bool Enable;
		extern bool RenderOnTeammates;
		extern bool Box;
		extern int  BoxStyle;
		extern bool Skeleton;
		extern bool BacktrackSkeleton;
		extern bool Glow;
		extern bool Name;
		extern bool Health;
		extern bool Conditions;
		extern bool RemoveScope;
		extern bool ShowSniperChargeDamage;
	}

	namespace Movement
	{
		extern bool Enable;
		extern bool Autojump;
		extern bool Autostrafe;
	}

	namespace Miscellaneous
	{
		extern bool Enable;
		extern bool BypassPure;
		extern int OptimizeNetworking;

	}
}