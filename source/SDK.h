#pragma once

#include <windows.h>
#include <math.h>
#include <xstring>
#include "Vector.h"
#include "getvfunc.h"
#include "dt_recv2.h"
#include "CSignature.h"
#include "WeaponList.h"
#include "CGlobalVars.h"
#include "VMTHooks.h"
#include "Log.h"
#include "CRecV.h"
#include "Color.h"
#include "studio.h"
#include "bspflags.h"
#include "Fonts.h"
#include "Font.h"
#include "IconsFontAwesome.h"
#include <d3dx9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "ConVar.h"
#include "CSignature.h"
#include "CUtlVector.h"
#include "CNetvarManager.h"

using namespace std;

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

#define VMTManager toolkit::VMTManager
#define VMTBaseManager toolkit::VMTBaseManager

#define WIN32_LEAN_AND_MEAN
#pragma optimize("gsy",on)
#pragma warning( disable : 4244 ) //Possible loss of data

typedef float matrix3x4[3][4];
class CGameTrace;
typedef CGameTrace trace_t;
class CBaseCombatWeapon;

#define me gInts.Engine->GetLocalPlayer()
#define GetBaseEntity gInts.EntList->GetClientEntity
#define MASK_AIMBOT 0x200400B
#define	FL_ONGROUND (1<<0)
#define FL_DUCKING (1<<1)
#define CONTENTS_HITBOX 0x40000000
#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define square( x ) ( x * x )
#define RADPI 57.295779513082f
#define SQUARE( a ) a*a
#define BLU_TEAM 3
#define RED_TEAM 2



typedef struct player_info_s
{
	char			name[32];
	int				userID;
	char			guid[33];
	unsigned long	friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	unsigned long	customFiles[4];
	unsigned char	filesDownloaded;
} player_info_t;

class ClientClass
{
private:
	BYTE _chPadding[8];
public:
	char* chName;
	RecvTable* Table;
	ClientClass* pNextClass;
	int iClassID;
};

class CHLClient
{
public:
	ClientClass* GetAllClasses( void )
	{
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID ); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return getvfunc<OriginalFn>( this, 8 )( this ); //Return the pointer to the head CClientClass.
	}
};

class CGlobals
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};


class IVModelInfo
{
public:
	DWORD *GetModel(int index)
	{
		typedef DWORD*(__thiscall* GetModelFn)(void*, int);
		return getvfunc<GetModelFn>(this, 1)(this, index);
	}

	int	GetModelIndex(const char* name)
	{
		typedef int(__thiscall* GetModelIndexFn)(void*, const char*);
		return getvfunc< GetModelIndexFn >(this, 2)(this, name);
	}

	const char* GetModelName(const DWORD* model)
	{
		typedef const char* (__thiscall* GetModelNameFn)(void*, const DWORD*);
		return getvfunc< GetModelNameFn >(this, 3)(this, model);
	}

	studiohdr_t* GetStudiomodel(const DWORD *mod)
	{
		typedef studiohdr_t* (__thiscall* GetStudiomodelFn)(void*, const DWORD*);
		return getvfunc< GetStudiomodelFn >(this, 28)(this, mod);
	}
};

class CUserCmd
{
public:
	virtual ~CUserCmd() {}; //Destructor 0
	int command_number; //4
	int tick_count; //8
	Vector viewangles; //C
	float forwardmove; //18
	float sidemove; //1C
	float upmove; //20
	int	buttons; //24
	byte impulse; //28
	int weaponselect; //2C
	int weaponsubtype; //30
	int random_seed; //34
	short mousedx; //38
	short mousedy; //3A
	bool hasbeenpredicted; //3C;
};

class CBaseEntity
{
public:
	Vector& GetAbsOrigin( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 9)(this);
	}
	Vector& GetAbsAngles( )
	{
		typedef Vector& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 10)(this);
	}
	void GetWorldSpaceCenter( Vector& vWorldSpaceCenter)
	{
		Vector vMin, vMax;
		this->GetRenderBounds( vMin, vMax );
		vWorldSpaceCenter = this->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2;
	}


	matrix3x4& GetRgflCoordinateFrame()
	{
		PVOID pRenderable = static_cast<PVOID>(this + 0x4);
		typedef matrix3x4 &(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pRenderable, 34)(pRenderable);
	}

	DWORD* GetModel( )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 9 )( pRenderable );
	}
	bool SetupBones( matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef bool ( __thiscall* OriginalFn )( PVOID, matrix3x4*, int, int, float );
		return getvfunc<OriginalFn>( pRenderable, 16 )( pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
	ClientClass* GetClientClass( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}
	bool IsDormant( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 8 )( pNetworkable );
	}
	int GetIndex( )
	{
		PVOID pNetworkable = (PVOID)(this + 0x8);
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 9 )( pNetworkable );
	}
	void GetRenderBounds( Vector& mins, Vector& maxs )
	{
		PVOID pRenderable = (PVOID)(this + 0x4);
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& , Vector& );
		getvfunc<OriginalFn>( pRenderable, 20 )( pRenderable, mins, maxs );
	}
	int GetMaxHealth()
	{
		typedef int(__thiscall *OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 107)(this);
	}
	

	Vector GetCollideableMins();
	Vector GetCollideableMaxs();
	int & GetTickBase();
	Vector GetEyePosition();
	Vector GetOrigin();
	int GetHealth();
	float GetSimulationTime();
	int GetTeamNum();
	int GetFlags();
	BYTE GetLifeState();
	int GetClassNum();
	char * szGetClass();
	void UpdateGlowEffect();
	void DestroyGlowEffect();
	int GetCond();
	bool & GlowEnabled();
	void SetGlow(bool value);
	void SetAbsOrigin(const Vector & v);
	CBaseCombatWeapon* GetActiveWeapon();
	Vector GetHitboxPosition(int iHitbox);
};

class CAttribute
{
public:
	void *pad;
	uint16_t m_iAttributeDefinitionIndex;
	float m_flValue;
	unsigned int dpad;

	inline CAttribute(uint16_t iAttributeDefinitionIndex, float flValue)
	{
		m_iAttributeDefinitionIndex = iAttributeDefinitionIndex;
		m_flValue = flValue;
	}
};

class CAttributeList
{
public:
	void *pad;
	CUtlVector<CAttribute, CUtlMemory<CAttribute> > m_Attributes;

	inline void AddAttribute(int iIndex, float flValue)
	{
		//15 = MAX_ATTRIBUTES
		if (m_Attributes.Count() > 14)
			return;

		CAttribute attr(iIndex, flValue);

		m_Attributes.AddToTail(attr);
	}
};

class CBaseCombatWeapon : public CBaseEntity, public CAttributeList
{
public:
	int GetMaxClip1()
	{
		typedef int(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 318)(this);
	}

	int GetMaxClip2()
	{
		typedef int(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 319)(this);
	}

	int GetSlot()
	{
		typedef int(__thiscall *OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 327)(this);
	}

	char *GetName()
	{
		typedef char *(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 329)(this);
	}

	char *GetPrintName()
	{
		typedef char *(__thiscall * OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 330)(this);
	}

	float GetSniperRifleChargeDamage()
	{
		NETVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage");
	}

	float GetUberChargeLevel()
	{
		NETVAR_RETURN(float, this, "DT_WeaponMedigun", "NonLocalTFWeaponMedigunData", "m_flChargeLevel"); 
	}

	int* GetItemDefinitionIndex()
	{
		NETVAR_RETURN(int*, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}

	//Probably wrong.
	CAttributeList* GetAttributeList()
	{
		NETVAR_RETURN(CAttributeList*, this, "DT_BasePlayer ", "m_AttributeList");
	}

	CAttribute* GetAttribute()
	{
		NETVAR_RETURN(CAttribute*, this, "DT_AttributeList", "m_Attributes");
	}
};



class C_TFGameRules
{
public:
	//credits: blackfire62

	//Are we playing casual?
	bool IsMatchTypeCasual()
	{
		typedef bool(__thiscall* IsCasualFn)(void*);
		static IsCasualFn IsCasual = (IsCasualFn)(gSignatures.FindPatternEx("client.dll", "55 8B EC 51 56 6A 00 8B F1 E8 ? ? ? ? 8B C8 E8 ? ? ? ? 84 C0 74 0B 8B 86 ? ? ? ? 89 45 FC EB 07 C7 45 ? ? ? ? ? 8D 45 FC 50 E8 ? ? ? ? 83 C4 04 5E 85 C0 74 0B 83 78 28 03"));
		return IsCasual(this);
	}

	//Are we playing competitive?
	bool IsMatchTypeCompetitive()
	{
		typedef bool(__thiscall* IsCompetitiveFn)(void*);
		static IsCompetitiveFn IsCompetitive = (IsCompetitiveFn)(gSignatures.FindPatternEx("client.dll", "55 8B EC 51 56 6A 00 8B F1 E8 ? ? ? ? 8B C8 E8 ? ? ? ? 84 C0 74 0B 8B 86 ? ? ? ? 89 45 FC EB 07 C7 45 ? ? ? ? ? 8D 45 FC 50 E8 ? ? ? ? 83 C4 04 5E 85 C0 74 0B 83 78 28 02"));
		return IsCompetitive(this);
	}

	//Is PVE mode active ? (When fighting boss like merasmus/monoculus and you cant kill other players)
	bool IsPVEModeActive()
	{
		return *(bool*)(this + 1051);
	}

};

enum AttributeIDs
{
	//Apply these for australium item
	AT_is_australium_item = 2027,
	AT_loot_rarity = 2022,
	AT_item_style_override = 542,

	//This one and item style override 0 to golden pan
	AT_ancient_powers = 150,

	AT_uses_stattrack_module = 719,

	AT_is_festive = 2053,

	//Only ks sheen is visible
	AT_sheen = 2014,

	AT_unusual_effect = 134,
	AT_particle_effect = 370,

	AT_weapon_allow_inspect = 731,
};

enum WeaponEffects
{
	WE_hot = 701,
	WE_isotope = 702,
	WE_cool = 703,
	WE_energyorb = 704,
};


class EngineClient
{
public:
	void GetScreenSize( int& width, int& height )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, int& , int& );
		return getvfunc<OriginalFn>( this, 5 )( this, width, height );
	}
	bool GetPlayerInfo( int ent_num, player_info_t *pinfo )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID, int, player_info_t * );
		return getvfunc<OriginalFn>(this, 8)(this, ent_num, pinfo );
	}
	bool Con_IsVisible( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 11 )( this );
	}
	int GetLocalPlayer( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 12 )( this );
	}
	float Time( void )
	{
		typedef float ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 14 )( this );
	}
	void GetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 19 )( this, va );
	}
	void SetViewAngles( Vector& va )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, Vector& va );
		return getvfunc<OriginalFn>( this, 20 )( this, va );
	}
	int GetMaxClients( void )
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 21 )( this );
	}
	bool IsInGame( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 26 )( this );
	}
	bool IsConnected( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 27 )( this );
	}
	bool IsDrawingLoadingImage( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 28 )( this );
	}
	const matrix3x4& WorldToScreenMatrix( void )
	{
		typedef const matrix3x4& ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>(this, 36)(this);
	}
	bool IsTakingScreenshot( void )
	{
		typedef bool ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 85 )( this );
	}
	DWORD* GetNetChannelInfo( void )
	{
		typedef DWORD* ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 72 )( this );
	}
	void ClientCmd_Unrestricted( const char* chCommandString )
	{
		typedef void ( __thiscall* OriginalFn )( PVOID, const char * );
		return getvfunc<OriginalFn>( this, 106 )( this, chCommandString );
	}
};

class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, unsigned int );
		return getvfunc<OriginalFn>( this, 36 )( this, vguiPanel );
	}
	void SetMouseInputEnabled(unsigned int panel, bool state)
	{
		getvfunc<void(__thiscall *)(void*, int, bool)>(this, 32)(this, panel, state);
	}
};




class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 11)(this, r, g, b, a);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 12)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 14)(this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* DrawLineFn)(void*, int, int, int, int);
		getvfunc<DrawLineFn>(this, 15)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		getvfunc<OriginalFn>(this, 17)(this, font);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 19)(this, r, g, b, a);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		getvfunc<OriginalFn>(this, 20)(this, x, y);
	}
	void DrawPrintText(const wchar_t *text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t *, int, int);
		return getvfunc<OriginalFn>(this, 22)(this, text, textLen, 0);
	}
	unsigned long CreateFont()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 66)(this);
	}
	void SetFontGlyphSet(unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		getvfunc<OriginalFn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t *, int&, int&);
		getvfunc<OriginalFn>(this, 75)(this, font, text, wide, tall);
	}
};

class CEntList
{
public:
	CBaseEntity* GetClientEntity( int entnum )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 3 )( this, entnum );
	}
	CBaseEntity* GetClientEntityFromHandle( int hEnt )
	{
		typedef CBaseEntity* ( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc<OriginalFn>( this, 4 )( this, hEnt );
	}
	int GetHighestEntityIndex(void)
	{
		typedef int ( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 6 )( this );
	}
};


enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,            // (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

struct Ray_t
{
	VectorAligned   m_Start;
	VectorAligned   m_Delta;
	VectorAligned   m_StartOffset;
	VectorAligned   m_Extents;

	bool    m_IsRay;
	bool    m_IsSwept;

	void Init(Vector& start, Vector& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();
		m_IsRay = true;

		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector& start, Vector& end, Vector& mins, Vector& maxs)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start - m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

struct cplane_t
{
	Vector normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
};

struct csurface_t
{
	const char		*name;
	short			surfaceProps;
	unsigned short	flags;
};

enum SurfaceFlags_t
{
	DISPSURF_FLAG_SURFACE = (1 << 0),
	DISPSURF_FLAG_WALKABLE = (1 << 1),
	DISPSURF_FLAG_BUILDABLE = (1 << 2),
	DISPSURF_FLAG_SURFPROP1 = (1 << 3),
	DISPSURF_FLAG_SURFPROP2 = (1 << 4),
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void *pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		CBaseEntity *pEntity = (CBaseEntity *)pEntityHandle;

		switch (pEntity->GetClientClass()->iClassID)
		{
		case 55: // Portal Window
		case 64: // Spawn Door visualizers
		case 117: // Sniper Dots
		case 225: // Medigun Shield
			return false;
			break;
		}

		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

enum class classId : int
{
	CTFWearableLevelableItem = 326,
	CTFWearableDemoShield = 324,
	CTFBaseRocket = 184,
	CTFWeaponBaseMerasmusGrenade = 313,
	CTFWeaponBaseMelee = 312,
	CTFWeaponBaseGun = 311,
	CTFWeaponBaseGrenadeProj = 310,
	CTFWeaponBase = 309,
	CTFWearableRobotArm = 327,
	CTFRobotArm = 277,
	CTFWrench = 329,
	CTFProjectile_ThrowableBreadMonster = 269,
	CTFProjectile_ThrowableBrick = 270,
	CTFProjectile_ThrowableRepel = 271,
	CTFProjectile_Throwable = 268,
	CTFThrowable = 307,
	CTFSyringeGun = 303,
	CTFKatana = 219,
	CTFSword = 302,
	CSniperDot = 117,
	CTFSniperRifleClassic = 296,
	CTFSniperRifleDecap = 297,
	CTFSniperRifle = 295,
	CTFChargedSMG = 194,
	CTFSMG = 294,
	CTFShovel = 293,
	CTFShotgunBuildingRescue = 292,
	CTFPEPBrawlerBlaster = 235,
	CTFSodaPopper = 298,
	CTFShotgun_Revenge = 290,
	CTFScatterGun = 286,
	CTFShotgun_Pyro = 289,
	CTFShotgun_HWG = 288,
	CTFShotgun_Soldier = 291,
	CTFShotgun = 287,
	CTFCrossbow = 198,
	CTFRocketLauncher_Mortar = 285,
	CTFRocketLauncher_AirStrike = 283,
	CTFRocketLauncher_DirectHit = 284,
	CTFRocketLauncher = 282,
	CTFRevolver = 276,
	CTFDRGPomson = 199,
	CTFRaygun = 274,
	CTFPistol_ScoutSecondary = 240,
	CTFPistol_ScoutPrimary = 239,
	CTFPistol_Scout = 238,
	CTFPistol = 237,
	CTFPipebombLauncher = 236,
	CTFWeaponPDA_Spy = 319,
	CTFWeaponPDA_Engineer_Destroy = 318,
	CTFWeaponPDA_Engineer_Build = 317,
	CTFWeaponPDAExpansion_Teleporter = 321,
	CTFWeaponPDAExpansion_Dispenser = 320,
	CTFWeaponPDA = 316,
	CTFParticleCannon = 233,
	CTFParachute_Secondary = 232,
	CTFParachute_Primary = 231,
	CTFParachute = 230,
	CTFMinigun = 228,
	CTFMedigunShield = 225,
	CWeaponMedigun = 337,
	CTFMechanicalArm = 224,
	CTFLunchBox_Drink = 223,
	CTFLunchBox = 222,
	CLaserDot = 78,
	CTFLaserPointer = 221,
	CTFKnife = 220,
	CTFProjectile_Cleaver = 246,
	CTFProjectile_JarMilk = 253,
	CTFProjectile_Jar = 252,
	CTFCleaver = 195,
	CTFJarMilk = 218,
	CTFJar = 217,
	CTFWeaponInvis = 315,
	CTFCannon = 193,
	CTFGrenadeLauncher = 211,
	CTFGrenadePipebombProjectile = 212,
	CTFGrapplingHook = 210,
	CTFFlareGun_Revenge = 206,
	CTFFlareGun = 205,
	CTFFlameRocket = 203,
	CTFFlameThrower = 204,
	CTFFists = 202,
	CTFFireAxe = 201,
	CTFCompoundBow = 197,
	CTFClub = 196,
	CTFBuffItem = 192,
	CTFStickBomb = 300,
	CTFBottle = 191,
	CTFBonesaw = 189,
	CTFBall_Ornament = 181,
	CTFStunBall = 301,
	CTFBat_Giftwrap = 187,
	CTFBat_Wood = 188,
	CTFBat_Fish = 186,
	CTFBat = 185,
	CTFProjectile_EnergyRing = 248,
	CTFDroppedWeapon = 200,
	CTFWeaponSapper = 322,
	CTFWeaponBuilder = 314,
	C_TFWeaponBuilder = 0,
	CTFProjectile_Rocket = 254,
	CTFProjectile_Flare = 249,
	CTFProjectile_EnergyBall = 247,
	CTFProjectile_GrapplingHook = 250,
	CTFProjectile_HealingBolt = 251,
	CTFProjectile_Arrow = 245,
	CMannVsMachineStats = 80,
	CTFTankBoss = 304,
	CTFBaseBoss = 182,
	CBossAlpha = 0,
	NextBotCombatCharacter = 342,
	CTFProjectile_SpellKartBats = 258,
	CTFProjectile_SpellKartOrb = 259,
	CTFHellZap = 215,
	CTFProjectile_SpellLightningOrb = 260,
	CTFProjectile_SpellTransposeTeleport = 267,
	CTFProjectile_SpellMeteorShower = 261,
	CTFProjectile_SpellSpawnBoss = 264,
	CTFProjectile_SpellMirv = 262,
	CTFProjectile_SpellPumpkin = 263,
	CTFProjectile_SpellSpawnHorde = 265,
	CTFProjectile_SpellSpawnZombie = 266,
	CTFProjectile_SpellBats = 256,
	CTFProjectile_SpellFireball = 257,
	CTFSpellBook = 299,
	CHightower_TeleportVortex = 74,
	CTeleportVortex = 159,
	CZombie = 339,
	CMerasmusDancer = 83,
	CMerasmus = 82,
	CHeadlessHatman = 73,
	CEyeballBoss = 48,
	CTFBotHintEngineerNest = 190,
	CBotNPCMinion = 0,
	CBotNPC = 0,
	CPasstimeGun = 94,
	CTFViewModel = 308,
	CRobotDispenser = 111,
	CTFRobotDestruction_Robot = 278,
	CTFReviveMarker = 275,
	CTFPumpkinBomb = 272,
	CTFBaseProjectile = 183,
	CBaseObjectUpgrade = 11,
	CTFRobotDestructionLogic = 281,
	CTFRobotDestruction_RobotGroup = 279,
	CTFRobotDestruction_RobotSpawn = 280,
	CTFPlayerDestructionLogic = 242,
	CPlayerDestructionDispenser = 101,
	CTFMinigameLogic = 227,
	CTFHalloweenMinigame_FallingPlatforms = 214,
	CTFHalloweenMinigame = 213,
	CTFMiniGame = 226,
	CTFPowerupBottle = 244,
	CTFItem = 216,
	CHalloweenSoulPack = 71,
	CTFGenericBomb = 208,
	CBonusRoundLogic = 23,
	CTFGameRulesProxy = 207,
	CTETFParticleEffect = 178,
	CTETFExplosion = 177,
	CTETFBlood = 176,
	CHalloweenGiftPickup = 69,
	CBonusDuckPickup = 21,
	CHalloweenPickup = 70,
	CCaptureFlagReturnIcon = 27,
	CCaptureFlag = 26,
	CBonusPack = 22,
	CTFTeam = 306,
	CTFTauntProp = 305,
	CTFPlayerResource = 243,
	CTFPlayer = 241,
	CTFRagdoll = 273,
	CTEPlayerAnimEvent = 164,
	CTFPasstimeLogic = 234,
	CPasstimeBall = 93,
	CTFObjectiveResource = 229,
	CTFGlow = 209,
	CTEFireBullets = 151,
	CTFBuffBanner = 0,
	CTFAmmoPack = 180,
	CObjectTeleporter = 89,
	CObjectSentrygun = 88,
	CTFProjectile_SentryRocket = 255,
	CObjectSapper = 87,
	CObjectCartDispenser = 85,
	CObjectDispenser = 86,
	CMonsterResource = 84,
	CFuncRespawnRoomVisualizer = 64,
	CFuncRespawnRoom = 63,
	CFuncPasstimeGoal = 61,
	CFuncForceField = 57,
	CCaptureZone = 28,
	CCurrencyPack = 31,
	CBaseObject = 10,
	CTestTraceline = 175,
	CTEWorldDecal = 179,
	CTESpriteSpray = 173,
	CTESprite = 172,
	CTESparks = 171,
	CTESmoke = 170,
	CTEShowLine = 168,
	CTEProjectedDecal = 166,
	CTEPlayerDecal = 165,
	CTEPhysicsProp = 163,
	CTEParticleSystem = 162,
	CTEMuzzleFlash = 161,
	CTELargeFunnel = 158,
	CTEKillPlayerAttachments = 157,
	CTEImpact = 156,
	CTEGlowSprite = 155,
	CTEShatterSurface = 167,
	CTEFootprintDecal = 153,
	CTEFizz = 152,
	CTEExplosion = 150,
	CTEEnergySplash = 149,
	CTEEffectDispatch = 148,
	CTEDynamicLight = 147,
	CTEDecal = 145,
	CTEClientProjectile = 144,
	CTEBubbleTrail = 143,
	CTEBubbles = 142,
	CTEBSPDecal = 141,
	CTEBreakModel = 140,
	CTEBloodStream = 139,
	CTEBloodSprite = 138,
	CTEBeamSpline = 137,
	CTEBeamRingPoint = 136,
	CTEBeamRing = 135,
	CTEBeamPoints = 134,
	CTEBeamLaser = 133,
	CTEBeamFollow = 132,
	CTEBeamEnts = 131,
	CTEBeamEntPoint = 130,
	CTEBaseBeam = 129,
	CTEArmorRicochet = 128,
	CTEMetalSparks = 160,
	CSteamJet = 122,
	CSmokeStack = 116,
	DustTrail = 340,
	CFireTrail = 50,
	SporeTrail = 347,
	SporeExplosion = 346,
	RocketTrail = 344,
	SmokeTrail = 345,
	CPropVehicleDriveable = 107,
	ParticleSmokeGrenade = 343,
	CParticleFire = 90,
	MovieExplosion = 341,
	CTEGaussExplosion = 154,
	CEnvQuadraticBeam = 43,
	CEmbers = 36,
	CEnvWind = 47,
	CPrecipitation = 106,
	CBaseTempEntity = 17,
	CWeaponIFMSteadyCam = 336,
	CWeaponIFMBaseCamera = 335,
	CWeaponIFMBase = 334,
	CTFWearableVM = 328,
	CTFWearable = 323,
	CTFWearableItem = 325,
	CEconWearable = 35,
	CBaseAttributableItem = 3,
	CEconEntity = 34,
	CHandleTest = 72,
	CTeamplayRoundBasedRulesProxy = 125,
	CTeamRoundTimer = 126,
	CSpriteTrail = 121,
	CSpriteOriented = 120,
	CSprite = 119,
	CRagdollPropAttached = 110,
	CRagdollProp = 109,
	CPoseController = 105,
	CGameRulesProxy = 68,
	CInfoLadderDismount = 75,
	CFuncLadder = 58,
	CEnvDetailController = 40,
	CWorld = 338,
	CWaterLODControl = 333,
	CWaterBullet = 332,
	CVoteController = 331,
	CVGuiScreen = 330,
	CPropJeep = 0,
	CPropVehicleChoreoGeneric = 0,
	CTest_ProxyToggle_Networkable = 174,
	CTesla = 169,
	CTeamTrainWatcher = 127,
	CBaseTeamObjectiveResource = 16,
	CTeam = 124,
	CSun = 123,
	CParticlePerformanceMonitor = 91,
	CSpotlightEnd = 118,
	CSlideshowDisplay = 115,
	CShadowControl = 114,
	CSceneEntity = 113,
	CRopeKeyframe = 112,
	CRagdollManager = 108,
	CPhysicsPropMultiplayer = 98,
	CPhysBoxMultiplayer = 96,
	CBasePropDoor = 15,
	CDynamicProp = 33,
	CPointCommentaryNode = 104,
	CPointCamera = 103,
	CPlayerResource = 102,
	CPlasma = 100,
	CPhysMagnet = 99,
	CPhysicsProp = 97,
	CPhysBox = 95,
	CParticleSystem = 92,
	CMaterialModifyControl = 81,
	CLightGlow = 79,
	CInfoOverlayAccessor = 77,
	CFuncTrackTrain = 67,
	CFuncSmokeVolume = 66,
	CFuncRotating = 65,
	CFuncReflectiveGlass = 62,
	CFuncOccluder = 60,
	CFuncMonitor = 59,
	CFunc_LOD = 54,
	CTEDust = 146,
	CFunc_Dust = 53,
	CFuncConveyor = 56,
	CBreakableSurface = 25,
	CFuncAreaPortalWindow = 55,
	CFish = 51,
	CEntityFlame = 38,
	CFireSmoke = 49,
	CEnvTonemapController = 46,
	CEnvScreenEffect = 44,
	CEnvScreenOverlay = 45,
	CEnvProjectedTexture = 42,
	CEnvParticleScript = 41,
	CFogController = 52,
	CEntityParticleTrail = 39,
	CEntityDissolve = 37,
	CDynamicLight = 32,
	CColorCorrectionVolume = 30,
	CColorCorrection = 29,
	CBreakableProp = 24,
	CBasePlayer = 13,
	CBaseFlex = 8,
	CBaseEntity = 7,
	CBaseDoor = 6,
	CBaseCombatCharacter = 4,
	CBaseAnimatingOverlay = 2,
	CBoneFollower = 20,
	CBaseAnimating = 1,
	CInfoLightingRelative = 76,
	CAI_BaseNPC = 0,
	CBeam = 19,
	CBaseViewModel = 18,
	CBaseProjectile = 14,
	CBaseParticleEntity = 12,
	CBaseGrenade = 9,
	CBaseCombatWeapon = 5,
};

class IGameEvent
{
public:
	virtual ~IGameEvent()
	{
	};
	virtual const char *GetName() const = 0; // get event name

	virtual bool IsReliable() const = 0;				  // if event handled reliable
	virtual bool IsLocal() const = 0;					  // if event is never networked
	virtual bool IsEmpty(const char *keyName = NULL) = 0; // check if data field exists

														  // Data access
	virtual bool GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char *GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
};
class IAppSystem
{
public:
	// Here's where the app systems get to learn about each other
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;

	// Here's where systems can access other interfaces implemented by this object
	// Returns NULL if it doesn't implement the requested interface
	virtual void *QueryInterface(const char *pInterfaceName) = 0;

	// Init, shutdown
	virtual int Init() = 0;
	virtual void Shutdown(char* reason) = 0;

};

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

enum AnalogCode_t //needed but not rly
{

};

class InputEvent_t;

class IInputSystem : public IAppSystem
{
public:
	// Attach, detach input system from a particular window
	// This window should be the root window for the application
	// Only 1 window should be attached at any given time.
	virtual void AttachToWindow(void* hWnd) = 0;
	virtual void DetachFromWindow() = 0;

	// Enables/disables input. PollInputState will not update current 
	// button/analog states when it is called if the system is disabled.
	virtual void EnableInput(bool bEnable) = 0;

	// Enables/disables the windows message pump. PollInputState will not
	// Peek/Dispatch messages if this is disabled
	virtual void EnableMessagePump(bool bEnable) = 0;

	// Polls the current input state
	virtual void PollInputState() = 0;

	// Gets the time of the last polling in ms
	virtual int GetPollTick() const = 0;

	// Is a button down? "Buttons" are binary-state input devices (mouse buttons, keyboard keys)
	virtual bool IsButtonDown(ButtonCode_t code) const = 0;

	// Returns the tick at which the button was pressed and released
	virtual int GetButtonPressedTick(ButtonCode_t code) const = 0;
	virtual int GetButtonReleasedTick(ButtonCode_t code) const = 0;

	// Gets the value of an analog input device this frame
	// Includes joysticks, mousewheel, mouse
	virtual int GetAnalogValue(AnalogCode_t code) const = 0;

	// Gets the change in a particular analog input device this frame
	// Includes joysticks, mousewheel, mouse
	virtual int GetAnalogDelta(AnalogCode_t code) const = 0;

	// Returns the input events since the last poll
	virtual int GetEventCount() const = 0;
	virtual const InputEvent_t* GetEventData() const = 0;

	// Posts a user-defined event into the event queue; this is expected
	// to be called in overridden wndprocs connected to the root panel.
	virtual void PostUserEvent(const InputEvent_t &event) = 0;

	// Returns the number of joysticks
	virtual int GetJoystickCount() const = 0;

	// Enable/disable joystick, it has perf costs
	virtual void EnableJoystickInput(int nJoystick, bool bEnable) = 0;

	// Enable/disable diagonal joystick POV (simultaneous POV buttons down)
	virtual void EnableJoystickDiagonalPOV(int nJoystick, bool bEnable) = 0;

	// Sample the joystick and append events to the input queue
	virtual void SampleDevices(void) = 0;

	// FIXME: Currently force-feedback is only supported on the Xbox 360
	virtual void SetRumble(float fLeftMotor, float fRightMotor, int userId = -1) = 0;
	virtual void StopRumble(void) = 0;

	// Resets the input state
	virtual void ResetInputState() = 0;

	// Sets a player as the primary user - all other controllers will be ignored.
	virtual void SetPrimaryUserId(int userId) = 0;

	// Convert back + forth between ButtonCode/AnalogCode + strings
	virtual const char *ButtonCodeToString(ButtonCode_t code) const = 0;
	virtual const char *AnalogCodeToString(AnalogCode_t code) const = 0;
	virtual ButtonCode_t StringToButtonCode(const char *pString) const = 0;
	virtual AnalogCode_t StringToAnalogCode(const char *pString) const = 0;

	// Sleeps until input happens. Pass a negative number to sleep infinitely
	virtual void SleepUntilInput(int nMaxSleepTimeMS = -1) = 0;

	// Convert back + forth between virtual codes + button codes
	// FIXME: This is a temporary piece of code
	virtual ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey) const = 0;
	virtual int ButtonCodeToVirtualKey(ButtonCode_t code) const = 0;
	virtual ButtonCode_t ScanCodeToButtonCode(int lParam) const = 0;

	// How many times have we called PollInputState?
	virtual int GetPollCount() const = 0;

	// Sets the cursor position
	virtual void SetCursorPosition(int x, int y) = 0;

	// NVNT get address to haptics interface
	virtual void *GetHapticsInterfaceAddress() const = 0;

	virtual void SetNovintPure(bool bPure) = 0;

	// read and clear accumulated raw input values
	virtual bool GetRawMouseAccumulators(int& accumX, int& accumY) = 0;

	// tell the input system that we're not a game, we're console text mode.
	// this is used for dedicated servers to not initialize joystick system.
	// this needs to be called before CInputSystem::Init (e.g. in PreInit of
	// some system) if you want ot prevent the joystick system from ever
	// being initialized.
	virtual void SetConsoleTextMode(bool bConsoleTextMode) = 0;
};
class ICvar : public IAppSystem
{
public:
	// Allocate a unique DLL identifier
	virtual int AllocateDLLIdentifier() = 0;

	// Register, unregister commands
	virtual void			RegisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void			UnregisterConCommands(int id) = 0;

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	virtual const char*		GetCommandLineValue(const char *pVariableName) = 0;

	// Try to find the cvar pointer by name
	virtual ConCommandBase *FindCommandBase(const char *name) = 0;
	virtual const ConCommandBase *FindCommandBase(const char *name) const = 0;
	virtual ConVar			*FindVar(const char *var_name) = 0;
	virtual const ConVar	*FindVar(const char *var_name) const = 0;
	virtual ConCommand		*FindCommand(const char *name) = 0;
	virtual const ConCommand *FindCommand(const char *name) const = 0;

	// Get first ConCommandBase to allow iteration
	virtual ConCommandBase	*GetCommands(void) = 0;
	virtual const ConCommandBase *GetCommands(void) const = 0;

	// Install a global change callback (to be called when any convar changes)
	virtual void			InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			CallGlobalChangeCallbacks(ConVar *var, const char *pOldString, float flOldValue) = 0;

	// Install a console printer
	virtual void			InstallConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void			RemoveConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void			ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;
	virtual void			ConsolePrintf(const char *pFormat, ...) const = 0;
	virtual void			ConsoleDPrintf(const char *pFormat, ...) const = 0;

	// Reverts cvars which contain a specific flag
	virtual void			RevertFlaggedConVars(int nFlag) = 0;

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void			InstallCVarQuery(void *pQuery) = 0;

#if defined( _X360 )
	virtual void			PublishToVXConsole() = 0;
#endif
	virtual bool			IsMaterialThreadSetAllowed() const = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, const char *pValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, int nValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar *pConVar, float flValue) = 0;
	virtual bool			HasQueuedMaterialThreadConVarSets() const = 0;
	virtual int				ProcessQueuedMaterialThreadConVarSets() = 0;

protected:	class ICVarIteratorInternal;
public:
	/// Iteration over all cvars.
	/// (THIS IS A SLOW OPERATION AND YOU SHOULD AVOID IT.)
	/// usage:
	/// { ICVar::Iterator iter(g_pCVar);
	///   for ( iter.SetFirst() ; iter.IsValid() ; iter.Next() )
	///   {
	///       ConCommandBase *cmd = iter.Get();
	///   }
	/// }
	/// The Iterator class actually wraps the internal factory methods
	/// so you don't need to worry about new/delete -- scope takes care
	//  of it.
	/// We need an iterator like this because we can't simply return a
	/// pointer to the internal data type that contains the cvars --
	/// it's a custom, protected class with unusual semantics and is
	/// prone to change.
	class Iterator
	{
	public:
		inline Iterator(ICvar *icvar);
		inline ~Iterator(void);
		inline void		SetFirst(void);
		inline void		Next(void);
		inline bool		IsValid(void);
		inline ConCommandBase *Get(void);
	private:
		ICVarIteratorInternal *m_pIter;
	};

protected:
	// internals for  ICVarIterator
	class ICVarIteratorInternal
	{
	public:
		// warning: delete called on 'ICvar::ICVarIteratorInternal' that is abstract but has non-virtual destructor [-Wdelete-non-virtual-dtor]
		virtual ~ICVarIteratorInternal()
		{
		}
		virtual void		SetFirst(void) = 0;
		virtual void		Next(void) = 0;
		virtual	bool		IsValid(void) = 0;
		virtual ConCommandBase *Get(void) = 0;
	};

	virtual ICVarIteratorInternal	*FactoryInternalIterator(void) = 0;
	friend class Iterator;
};

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}

private:
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const
	{
		return fraction < 1 || allsolid || startsolid;
	}

public:
	float			fractionleftsolid;
	csurface_t		surface;

	int				hitgroup;

	short			physicsbone;

	CBaseEntity*	m_pEnt;
	int				hitbox;

	CGameTrace() {}
	CGameTrace(const CGameTrace& vOther);
};


class IEngineTrace
{
public:	  //We really only need this I guess...
	void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)//5
	{
		typedef void(__thiscall* TraceRayFn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		return getvfunc<TraceRayFn>(this, 4)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

enum playercontrols
{
	IN_ATTACK = (1 << 0),
	IN_JUMP	= (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = (1 << 17),	// Player is holding the speed key
	IN_WALK = (1 << 18),	// Player holding walk key
	IN_ZOOM	= (1 << 19),	// Zoom key for HUD zoom
	IN_WEAPON1 = (1 << 20),	// weapon defines these bits
	IN_WEAPON2 = (1 << 21),	// weapon defines these bits
	IN_BULLRUSH = (1 << 22),
};

enum tf_cond 
{ 
    TFCond_Slowed = (1 << 0), //Toggled when a player is slowed down. 
    TFCond_Zoomed = (1 << 1), //Toggled when a player is zoomed. 
    TFCond_Disguising = (1 << 2), //Toggled when a Spy is disguising.  
    TFCond_Disguised = (1 << 3), //Toggled when a Spy is disguised. 
    TFCond_Cloaked = (1 << 4), //Toggled when a Spy is invisible. 
    TFCond_Ubercharged = (1 << 5), //Toggled when a player is ÜberCharged. 
    TFCond_TeleportedGlow = (1 << 6), //Toggled when someone leaves a teleporter and has glow beneath their feet. 
    TFCond_Taunting = (1 << 7), //Toggled when a player is taunting. 
    TFCond_UberchargeFading = (1 << 8), //Toggled when the ÜberCharge is fading. 
    TFCond_CloakFlicker = (1 << 9), //Toggled when a Spy is visible during cloak. 
    TFCond_Teleporting = (1 << 10), //Only activates for a brief second when the player is being teleported; not very useful. 
    TFCond_Kritzkrieged = (1 << 11), //Toggled when a player is being crit buffed by the KritzKrieg. 
    TFCond_TmpDamageBonus = (1 << 12), //Unknown what this is for. Name taken from the AlliedModders SDK. 
    TFCond_DeadRingered = (1 << 13), //Toggled when a player is taking reduced damage from the Deadringer. 
    TFCond_Bonked = (1 << 14), //Toggled when a player is under the effects of The Bonk! Atomic Punch. 
    TFCond_Stunned = (1 << 15), //Toggled when a player's speed is reduced from airblast or a Sandman ball. 
    TFCond_Buffed = (1 << 16), //Toggled when a player is within range of an activated Buff Banner. 
    TFCond_Charging = (1 << 17), //Toggled when a Demoman charges with the shield. 
    TFCond_DemoBuff = (1 << 18), //Toggled when a Demoman has heads from the Eyelander. 
    TFCond_CritCola = (1 << 19), //Toggled when the player is under the effect of The Crit-a-Cola. 
    TFCond_InHealRadius = (1 << 20), //Unused condition, name taken from AlliedModders SDK. 
    TFCond_Healing = (1 << 21), //Toggled when someone is being healed by a medic or a dispenser. 
    TFCond_OnFire = (1 << 22), //Toggled when a player is on fire. 
    TFCond_Overhealed = (1 << 23), //Toggled when a player has >100% health. 
    TFCond_Jarated = (1 << 24), //Toggled when a player is hit with a Sniper's Jarate. 
    TFCond_Bleeding = (1 << 25), //Toggled when a player is taking bleeding damage. 
    TFCond_DefenseBuffed = (1 << 26), //Toggled when a player is within range of an activated Battalion's Backup. 
    TFCond_Milked = (1 << 27), //Player was hit with a jar of Mad Milk. 
    TFCond_MegaHeal = (1 << 28), //Player is under the effect of Quick-Fix charge. 
    TFCond_RegenBuffed = (1 << 29), //Toggled when a player is within a Concheror's range. 
    TFCond_MarkedForDeath = (1 << 30), //Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 
	TFCond_NoHealingDamageBuff = (1 << 31), //Unknown what this is used for.

    TFCondEx_SpeedBuffAlly = (1 << 0), //Toggled when a player gets hit with the disciplinary action. 
    TFCondEx_HalloweenCritCandy = (1 << 1), //Only for Scream Fortress event maps that drop crit candy. 
	TFCondEx_CritCanteen = (1 << 2), //Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge = (1 << 3), //From demo's shield
	TFCondEx_CritHype = (1 << 4), //Soda Popper crits. 
    TFCondEx_CritOnFirstBlood = (1 << 5), //Arena first blood crit buff. 
    TFCondEx_CritOnWin = (1 << 6), //End of round crits. 
    TFCondEx_CritOnFlagCapture = (1 << 7), //CTF intelligence capture crits. 
    TFCondEx_CritOnKill = (1 << 8), //Unknown what this is for. 
    TFCondEx_RestrictToMelee = (1 << 9), //Unknown what this is for. 
	TFCondEx_DefenseBuffNoCritBlock = ( 1 << 10 ), //MvM Buff.
	TFCondEx_Reprogrammed = (1 << 11), //MvM Bot has been reprogrammed.
    TFCondEx_PyroCrits = (1 << 12), //Player is getting crits from the Mmmph charge. 
    TFCondEx_PyroHeal = (1 << 13), //Player is being healed from the Mmmph charge. 
	TFCondEx_FocusBuff = (1 << 14), //Player is getting a focus buff.
	TFCondEx_DisguisedRemoved = (1 << 15), //Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent = (1 << 16), //Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser = (1 << 17), //Bot is disguised as dispenser.
	TFCondEx_Sapped = (1 << 18), //MvM bot is being sapped.
	TFCondEx_UberchargedHidden = (1 << 19), //MvM Related
	TFCondEx_UberchargedCanteen = (1 << 20), //Player is receiving ÜberCharge from a canteen.
	TFCondEx_HalloweenBombHead = (1 << 21), //Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller = (1 << 22), //Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge = (1 << 26), //Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge = (1 << 27), //Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge = (1 << 28), //Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance = (1 << 29), //Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance = (1 << 30), //Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance = (1 << 31), //Player is receiving 10% reduced damage from fire.

	TFCondEx2_Stealthed = (1 << 0),
	TFCondEx2_MedigunDebuff = (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune = (1 << 3),
	TFCondEx2_BlastImmune = (1 << 4),
	TFCondEx2_FireImmune = (1 << 5),
	TFCondEx2_PreventDeath = (1 << 6),
	TFCondEx2_MVMBotRadiowave = (1 << 7),
	TFCondEx2_HalloweenSpeedBoost = (1 << 8), //Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal = (1 << 9), //Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant = (1 << 10), //Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny = (1 << 11), //Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell = (1 << 12), //Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode = (1 << 13), //Wheel has granted player ghost mode.
	TFCondEx2_Parachute = (1 << 16), //Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping = (1 << 17), //Player has sticky or rocket jumped.

    TFCond_MiniCrits = ( TFCond_Buffed | TFCond_CritCola ),
    TFCond_IgnoreStates = ( TFCond_Ubercharged | TFCond_Bonked ), 
    TFCondEx_IgnoreStates = ( TFCondEx_PyroHeal ) 
};

enum tf_classes
{
	TF2_Scout = 1,
	TF2_Soldier = 3,
	TF2_Pyro = 7,
	TF2_Demoman = 4,
	TF2_Heavy = 6,
	TF2_Engineer = 9,
	TF2_Medic = 5,
	TF2_Sniper = 2,
	TF2_Spy = 8,
};

enum source_lifestates
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

class ClientModeShared
{
public:
	bool IsChatPanelOutOfFocus(void)
	{
		typedef PVOID(__thiscall* OriginalFn)(PVOID);
		PVOID CHudChat = getvfunc<OriginalFn>(this, 19)(this);
		if (CHudChat)
		{
			return *(PFLOAT)((DWORD)CHudChat + 0xFC) == 0;
		}
		return false;
	}
};

class CInterfaces
{
public:
	CEntList* EntList;
	EngineClient* Engine;
	IPanel* Panels;
	ISurface* Surface;
	ClientModeShared* ClientMode;
	CHLClient* Client;
	IVModelInfo* ModelInfo;
	CGlobals* gGlobals;
	ICvar* Cvar;
	IEngineTrace* EngineTrace;
	C_TFGameRules* pGameRules;
};

extern CInterfaces gInts;
extern CPlayerVariables gPlayerVars;
