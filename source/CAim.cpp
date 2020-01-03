#include "CAim.h"
#include "Math.h"
#include "CMiscellaneous.h"
#include "Vector.h"

#define TICK_INTERVAL			(gInts.gGlobals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
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
Vector CalculateAngle(Vector src, Vector dst)
{
	Vector AimAngles, delta;
	float hyp;
	delta = src - dst;
	hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y)); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
	AimAngles.x = atanf(delta.z / hyp) * RADPI;
	AimAngles.y = atanf(delta.y / delta.x) * RADPI;
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
		AimAngles.y += 180.0f;
	return AimAngles;
}
void MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}
float GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = CalculateAngle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
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
	if (!gMenu::Aim::MasterSwitch || !gMenu::Aim::Backtracking)
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

		if (pPlayerEntity == NULL || g::g_LocalPlayer == NULL || pPlayerEntity == g::g_LocalPlayer || pPlayerEntity->IsDormant() || pPlayerEntity->GetHealth() <= 0 /*|| gInts.pGameRules->IsPVEModeActive()*/)
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

