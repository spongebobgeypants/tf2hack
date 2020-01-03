#pragma once

#include "SDK.h"
#include "Globals.h"
#include <DirectXMath.h>



#define NUM_OF_TICKS 12

struct StoredData
{
	float m_flSimulationTime;
	Vector m_vecHitboxPosition;
};
extern StoredData BacktrackData[64][NUM_OF_TICKS]; // Idfk

namespace gAim
{
	namespace LagCompensation
	{
		void OnCreateMove();
	}

	namespace AimAssist
	{
		// Add it yourself lol
		void Triggerbot();
	}
}