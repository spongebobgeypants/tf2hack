#pragma once
#include "SDK.h"
#include "Globals.h"

namespace gMiscellaneous
{
	void BypassPure();
	void OptimizeNetworking();
	bool IsVisible(void * pLocal, void * pEntity, Vector vStart, Vector vEnd);
	extern bool ShouldDisableInterpolation;
}