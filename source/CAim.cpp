#include "CAim.h"
#include "Math.h"
#include "CMiscellaneous.h"
#include "Vector.h"


#define TICK_INTERVAL			(gInts.gGlobals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
std::deque<StoredData> gAim::LagCompensation::GetPlayerStoredData[64];
StoredData BacktrackData[64][12];
// i know i know.
inline float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

void AngleVectors(const Vector &angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}



void gAim::LagCompensation::OnCreateMove()
{
	if (!gMenu::Aim::Backtracking)
	{
		return;
	}

	int bestTargetIndex = -1;
	float bestFov = FLT_MAX;

	if (g::g_LocalPlayer->GetHealth() <= 0)
	{
		return;
	}

	for (int i = 1; i <= 65; i++)
	{
		auto pPlayerEntity = GetBaseEntity(i); // cool macro

		if (pPlayerEntity == NULL || g::g_LocalPlayer == NULL || pPlayerEntity == g::g_LocalPlayer || pPlayerEntity->IsDormant() || pPlayerEntity->GetHealth() <= 0) /*|| gInts.pGameRules->IsPVEModeActive()*/
		{
			continue;
		}

		float flSimulationTime = pPlayerEntity->GetSimulationTime();
		Vector vecHitboxPosition = pPlayerEntity->GetHitboxPosition(0); // 0 = head
		
		BacktrackData[i][g::g_UserCmd->command_number % NUM_OF_TICKS] = StoredData{ flSimulationTime, vecHitboxPosition };
		Vector ViewDir;
		AngleVectors(g::g_UserCmd->viewangles, ViewDir);
		float FOVDistance = DistancePointToLine(vecHitboxPosition, g::g_LocalPlayer->GetEyePosition(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}
	}

	float bestTargetSimTime = -1;

	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;

		Vector ViewDir;

		AngleVectors(g::g_UserCmd->viewangles, ViewDir);

		for (int t = 0; t <= NUM_OF_TICKS; ++t)
		{
			float tempFOVDistance = DistancePointToLine(BacktrackData[bestTargetIndex][t].m_vecHitboxPosition, g::g_LocalPlayer->GetEyePosition(), ViewDir);

			if (tempFloat > tempFOVDistance && BacktrackData[bestTargetIndex][t].m_flSimulationTime > g::g_LocalPlayer->GetSimulationTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = BacktrackData[bestTargetIndex][t].m_flSimulationTime; 
			}
		}

		if (bestTargetSimTime >= 0 && g::g_UserCmd->buttons & IN_ATTACK)
			g::g_UserCmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
	}
}

