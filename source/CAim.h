#pragma once

#include "SDK.h"
#include "Globals.h"
#include <DirectXMath.h>
#include <vector>
#include <deque>

#define NUM_OF_TICKS 12



struct StoredData
{
	float m_flSimulationTime;
	Vector m_vecHitboxPosition;
	matrix3x4 m_Matrix[128][64];
};

namespace gAim
{


	namespace LagCompensation
	{
		//Will eventually make it work.
		extern std::deque<StoredData> GetPlayerStoredData[64];
		void OnCreateMove();
		void PlaceHolder();
	}

	namespace AimAssist
	{
		// Add it yourself lol
		void Triggerbot();
	}
}