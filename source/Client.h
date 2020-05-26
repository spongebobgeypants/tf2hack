#pragma once

#include "SDK.h"
#include "VMT.h"

using CreateMove_t = bool(__thiscall*)(PVOID, float, CUserCmd*);
extern CreateMove_t oCreateMove; 
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand);