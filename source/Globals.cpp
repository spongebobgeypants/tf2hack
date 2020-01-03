#include "Globals.h"


namespace g
{
	CBaseEntity* g_LocalPlayer = nullptr;
	CUserCmd* g_UserCmd = nullptr;
	bool b_MenuEnable = false;
	int i_MenuTab = 0;
	int iCreateMoveOffset = 21, iPaintTraverseOffset = 41, iFrameStageNotifyOffset = 35;

}

namespace gMenu
{
	namespace Aim
	{
		bool MasterSwitch;
		bool Enable;
		bool Backtracking;
		int  TargetSelection;
		int  LockMethord;
		bool pSilent;
		int  FOV;
		bool  Triggerbot;
		int  Smooth;
		int  Mode;
		int  Hitbox;
		bool Autoshoot;
	}

	namespace Visuals
	{
		bool Enable;
		bool Box;
		bool Skeleton;
		bool BacktrackSkeleton;
		bool Glow;
		int  BoxStyle;
		bool Health;
		bool Name;
		bool Conditions;
		bool RemoveScope;
		bool ShowSniperChargeDamage;
		bool RenderOnTeammates;
	}

	namespace Movement
	{
		bool Enable;
		bool Autojump;
		bool Autostrafe;
	}

	namespace Miscellaneous
	{
		bool Enable;
		bool BypassPure;
		int OptimizeNetworking;
		bool Australium;
	}
}