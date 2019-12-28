#include "SDK.h"
#include "CNetvarManager.h"
#include "Math.h"
#include "CDrawManager.h"

// most of this was pasted from potassium. thanks stick

Vector CBaseEntity::GetCollideableMins()
{
    NETVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
}

Vector CBaseEntity::GetCollideableMaxs()
{
	NETVAR_RETURN(Vector, this, "DT_BaseEntity", "m_Collision", "m_vecMaxs");
}

Vector CBaseEntity::GetEyePosition()
{
	NETVAR_RETURN(Vector, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]") + this->GetAbsOrigin();
}

Vector CBaseEntity::GetOrigin()
{
	NETVAR_RETURN(Vector, this, "DT_BaseEntity", "m_vecOrigin");
}

float CBaseEntity::GetUberChargeLevel()
{
	NETVAR_RETURN(float, this, "DT_WeaponMedigun", "NonLocalTFWeaponMedigunData", "m_flChargeLevel"); // not sure if this is the netvar for ubercharge
}

float CBaseEntity::GetSniperRifleChargeDamage()
{
	NETVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage"); 
}
int CBaseEntity::GetHealth()
{
	NETVAR_RETURN(int, this, "DT_BasePlayer", "m_iHealth");
}

float CBaseEntity::GetSimulationTime()
{
	NETVAR_RETURN(float, this, "DT_BaseEntity", "m_flSimulationTime");
}

int CBaseEntity::GetTeamNum()
{
	NETVAR_RETURN(int, this, "DT_BaseEntity", "m_iTeamNum");
}

int CBaseEntity::GetFlags()
{
	NETVAR_RETURN(int, this, "DT_BasePlayer", "m_fFlags");
}

BYTE CBaseEntity::GetLifeState()
{
	NETVAR_RETURN(BYTE, this, "DT_BasePlayer", "m_lifeState");
}

int CBaseEntity::GetClassNum()
{
	NETVAR_RETURN(int, this, "DT_TFPlayer", "m_PlayerClass", "m_iClass");
}

char* CBaseEntity::szGetClass()
{
	 NETVAR(iClass, int, "DT_TFPlayer", "m_PlayerClass", "m_iClass");

	switch (iClass.GetValue(this))
	{
	case TF2_Scout:
		return "Scout";
	case TF2_Soldier:
		return "Soldier";
	case TF2_Pyro:
		return "Pyro";
	case TF2_Demoman:
		return "Demoman";
	case TF2_Heavy:
		return "Heavy";
	case TF2_Engineer:
		return "Engineer";
	case TF2_Medic:
		return "Medic";
	case TF2_Sniper:
		return "Sniper";
	case TF2_Spy:
		return "Spy";
	default:
		return "Unknown class";
	}

	return "Unknown class"; //Just in case
}

void CBaseEntity::UpdateGlowEffect()
{
	typedef void(__thiscall* UpdateGlowEffectFn)(void*);
	getvfunc<UpdateGlowEffectFn>(this, 226)(this);
}

void CBaseEntity::DestroyGlowEffect()
{
	typedef void(__thiscall* DestroyGlowEffectFn)(void*);
	getvfunc<DestroyGlowEffectFn>(this, 227)(this);
}

int CBaseEntity::GetCond()
{
	NETVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
}


bool &CBaseEntity::GlowEnabled()
{
	static int m_bGlowEnabled = gNetvars->GetOffset("DT_TFPlayer", "m_bGlowEnabled");
	return *reinterpret_cast<bool*>(uintptr_t(this) + m_bGlowEnabled);
}

void CBaseEntity::SetGlow(bool value)
{
	static int m_bGlowEnabled = gNetvars->GetOffset("DT_TFPlayer", "m_bGlowEnabled");
	*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + m_bGlowEnabled) = value;
}

void CBaseEntity::SetAbsOrigin(const Vector &v) 
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
	static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)gSignatures.FindPatternEx("client", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07");
	return SetAbsOrigin(this, v);
}

Vector CBaseEntity::GetHitboxPosition(int iHitbox)
{
	DWORD *model = this->GetModel();
	if (!model)
		return Vector();

	studiohdr_t *hdr = gInts.ModelInfo->GetStudiomodel(model);
	if (!hdr)
		return Vector();

	matrix3x4 matrix[128];
	if (!this->SetupBones(matrix, 128, 0x100, 0))
		return Vector();

	int HitboxSetIndex = *(int *)((DWORD)hdr + 0xB0);
	if (!HitboxSetIndex)
		return Vector();

	mstudiohitboxset_t *pSet = (mstudiohitboxset_t *)(((PBYTE)hdr) + HitboxSetIndex);

	mstudiobbox_t* box = pSet->pHitbox(iHitbox);
	if (!box)
		return Vector();

	Vector vCenter = (box->bbmin + box->bbmax) * 0.5f;

	Vector vHitbox;

	gMath.VectorTransformS(vCenter, matrix[box->bone], vHitbox);

	return vHitbox;
}