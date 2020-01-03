#pragma once

#include "SDK.h"
#include "VMT.h"

extern std::unique_ptr<VMTHook> clientModeHook;
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand);