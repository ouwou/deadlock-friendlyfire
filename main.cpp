#include <string>
#include <fstream>
#include <sstream>

#include <eiface.h>
#include <entity2/entitysystem.h>
#include <icvar.h>
#include <convar.h>
#include <igameevents.h>
#include <utlvector.h>
#include <entity2/entityclass.h>
#include <server_class.h>

#include <sourcehook.h>
#include <sourcehook_impl.h>

#include <funchook.h>

#include "globals.hpp"

#include "sigscan.hpp"
#include "deadlock/deadlock.hpp"
#include <queue>

#pragma comment(lib, "psapi")

SourceHook::Plugin g_PLID = 0x6D656F77 + 1;
SourceHook::Impl::CSourceHookImpl g_SHImp;
SourceHook::ISourceHook *g_SHPtr = &g_SHImp;

SH_DECL_HOOK4_void(ISource2GameClients, ClientPutInServer, SH_NOATTRIB, 0, CPlayerSlot, char const *, int, uint64);
SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);
SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);

CTakeDamageInfo *Hook_CTakeDamageInfo_Ctor(CTakeDamageInfo *thisptr,
                                           CBaseEntity *inflictor,
                                           CBaseEntity *attacker,
                                           CBaseEntity *ability,
                                           Vector *vecDamageForce,
                                           Vector *vecDamagePosition,
                                           Vector *vecReportedPosition,
                                           float flDamage,
                                           int32_t bitsDamageType,
                                           int32_t iDamageCustom) {
    // ConColorMsg(Color(255, 0, 0), "CTakeDamageInfo = %llx (%s:%s)\n", thisptr, inflictor ? ((CEntityInstance *)inflictor)->GetClassname() : "", attacker ? ((CEntityInstance *)attacker)->GetClassname() : "");
    g_pfnCTakeDamageInfo_CtorOrig(thisptr, inflictor, attacker, ability, vecDamageForce, vecDamagePosition, vecReportedPosition, flDamage, bitsDamageType, iDamageCustom);
    auto *dmg = reinterpret_cast<deadlock::CTakeDamageInfo *>(thisptr);
    return thisptr;
}

constexpr static uint64_t Citadel_Team_Amber = 0x080000000;
constexpr static uint64_t Citadel_Team_Sapphire = 0x100000000;

// idk what to call this nor do i really understand what it does. a3 is some struct that holds some data that eventually is used to construct a trace filter
// to find: only xref to function with "SERVER[%d]: Trace result hits %d\n"
using MagicFn = void *(__fastcall *)(int a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7, void *a8, void *a9, void *a10, void *a11, void *a12, ...);
MagicFn g_pfnMagic_Orig = nullptr;
void *Hook_Magic(int a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7, void *a8, void *a9, void *a10, void *a11, void *a12, ...) {
    if (!g_FriendlyFireEnabled) return g_pfnMagic_Orig(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);

    auto *handle = reinterpret_cast<CHandle<deadlock::CCitadelPlayerPawn> *>(reinterpret_cast<uintptr_t>(a3) + 0x8);
    uint64_t *exclude = reinterpret_cast<uint64_t *>(reinterpret_cast<uintptr_t>(a3) + 0x78);
    *exclude &= ~(Citadel_Team_Amber | Citadel_Team_Sapphire);
    *reinterpret_cast<int32_t *>(reinterpret_cast<uintptr_t>(a3) + 0xC) = handle->m_data;

    auto *r = g_pfnMagic_Orig(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
    return r;
}

// this probably isnt actually what executes a trace but it goes too much further deep for me to care enough since it catches most things
// to find:
// "NPC collides with" and its in the first subroutine called within an if statement going up
// or sluggishly trawl through the callstack of CBaseEntity::TakeDamage

// a1 = CVPhys2World : IVPhysics2World
// a2 and a6 are interesting
using ExecuteTraceFn = void *(__fastcall *)(void *a1, void *a2, void *posMaybe, CTraceFilter *filter, int a5, void *a6, char a7);
ExecuteTraceFn g_pfnExecuteTrace_Orig = nullptr;
void *Hook_ExecuteTrace(void *a1, void *a2, void *posMaybe, CTraceFilter *filter, int a5, void *a6, char a7) {
    if (!g_FriendlyFireEnabled) return g_pfnExecuteTrace_Orig(a1, a2, posMaybe, filter, a5, a6, a7);

    if (g_PrintStuff) {
        Msg("IVPhys2World trace:\n");
        Msg("  - InteractsExclude %llx\n", filter->m_nInteractsExclude);
        Msg("  - InteractsWith %llx\n", filter->m_nInteractsWith);
        Msg("  - InteractsAs %llx\n", filter->m_nInteractsAs);
    }

    filter->m_nInteractsExclude &= ~(Citadel_Team_Amber | Citadel_Team_Sapphire);
    if (filter->m_nInteractsWith & (Citadel_Team_Amber | Citadel_Team_Sapphire)) {
        filter->m_nInteractsWith |= (Citadel_Team_Amber | Citadel_Team_Sapphire);
    }

    return g_pfnExecuteTrace_Orig(a1, a2, posMaybe, filter, a5, a6, a7);
}

#pragma pack(1)
struct TraceResult {
    int32_t nHits;
    uint8_t pad0[0xC];
    deadlock::CBaseEntity **pHitEntities;
    uint8_t pad1[24];
};

using TraceShapeFn = bool(__fastcall *)(void *pVPhys, Ray_t *ray, Vector *vStart, Vector *vEnd, CTraceFilter *filter, TraceResult *result);
TraceShapeFn g_pfnTraceShape_Orig = nullptr;
bool Hook_TraceShape(void *pVPhys, Ray_t *ray, Vector *vStart, Vector *vEnd, CTraceFilter *filter, TraceResult *result) {
    if (!g_FriendlyFireEnabled) return g_pfnTraceShape_Orig(pVPhys, ray, vStart, vEnd, filter, result);
    /*
    switch (ray->m_eType) {
        case RayType_t::RAY_TYPE_CAPSULE:
            Msg("Trace Capsule\n");
            break;
        case RayType_t::RAY_TYPE_HULL:
            Msg("Trace Hull\n");
            break;
        case RayType_t::RAY_TYPE_LINE:
            Msg("Trace Line\n");
            break;
        case RayType_t::RAY_TYPE_MESH:
            Msg("Trace Mesh\n");
            break;
        case RayType_t::RAY_TYPE_SPHERE:
            Msg("Trace Sphere\n");
            break;
    }
    */

    filter->m_nInteractsExclude &= ~(Citadel_Team_Amber | Citadel_Team_Sapphire);

    return g_pfnTraceShape_Orig(pVPhys, ray, vStart, vEnd, filter, result);
}

using TraceLineWithExcludedEntityFn = bool(__fastcall *)(void *pVPhys, Vector *vStart, Vector *vEnd, deadlock::CBaseEntity *ent, uint64_t interactsWith, uint8_t collisionGroup, TraceResult *result);
TraceLineWithExcludedEntityFn g_pfnTraceLineWithExcludedEntity_Orig = nullptr;
bool Hook_TraceLineWithExcludedEntity(void *pVPhys, Vector *vStart, Vector *vEnd, deadlock::CBaseEntity *ent, uint64_t interactsWith, uint8_t collisionGroup, TraceResult *result) {
    if (!g_FriendlyFireEnabled) return g_pfnTraceLineWithExcludedEntity_Orig(pVPhys, vStart, vEnd, ent, interactsWith, collisionGroup, result);

    return g_pfnTraceLineWithExcludedEntity_Orig(pVPhys, vStart, vEnd, ent, interactsWith, collisionGroup, result);
}

// string: C:\\buildworker\\citadel_rel_win64\\build\\src\\game\\shared\\citadel\\abilities\\tier_2_boss_abilities.cpp:
// go to second one
// call immediately before V_sincosf
// also CCitadel_Ability_IceBeam vfunc 224 (212 "CCitadel_Ability_IceBeamVData" + 12) buried about half way down

using ExecuteRaySegmentTraceFn = bool(__fastcall *)(void *pVPhys, void *a2, Vector *vStart, Vector *vEnd, CTraceFilter *filter, void *a6);
ExecuteRaySegmentTraceFn g_pfnExecuteRaySegmentTrace_Orig = nullptr;
bool Hook_ExecuteRaySegmentTrace(void *pVPhys, void *a2, Vector *vStart, Vector *vEnd, CTraceFilter *filter, TraceResult *a6) {
    if (!g_FriendlyFireEnabled) return g_pfnExecuteRaySegmentTrace_Orig(pVPhys, a2, vStart, vEnd, filter, a6);

    filter->m_nInteractsExclude &= ~(Citadel_Team_Amber | Citadel_Team_Sapphire);
    if (filter->m_nInteractsWith & (Citadel_Team_Amber | Citadel_Team_Sapphire)) {
        filter->m_nInteractsWith |= (Citadel_Team_Amber | Citadel_Team_Sapphire);
    }

    if (g_PrintStuff) Msg("trace=%llx %llx:%llx:%llx\n", filter, filter->m_nInteractsWith, filter->m_nInteractsAs, filter->m_nInteractsExclude);

    auto r = g_pfnExecuteRaySegmentTrace_Orig(pVPhys, a2, vStart, vEnd, filter, a6);

    if (g_PrintStuff) Msg("  nHits=%d\n", a6->nHits);

    return r;
}

using CCitadel_Ability_IceBeam_DmgFn = void *(__fastcall *)(void *thisptr);
CCitadel_Ability_IceBeam_DmgFn g_pfnCCitadel_Ability_IceBeam_Dmg_Orig = nullptr;
void *Hook_CCitadel_Ability_IceBeam_Dmg(deadlock::CBaseEntity *thisptr) {
    if (!g_FriendlyFireEnabled) return g_pfnCCitadel_Ability_IceBeam_Dmg_Orig(thisptr);

    // ummmm meow ? (i think this double checks team for some reason and i dont wanna deal with it)
    int original_team_num = 0;
    deadlock::CBaseEntity *owner = thisptr->GetOwner();
    if (owner) {
        original_team_num = owner->m_iTeamNum;
        owner->m_iTeamNum = 4;
    }

    auto *r = g_pfnCCitadel_Ability_IceBeam_Dmg_Orig(thisptr);

    if (owner) {
        owner->m_iTeamNum = original_team_num;
    }

    return r;
}

using CCitadel_Ability_Viscous_Telepunch_ImpactFn = void(__fastcall *)(void *thisptr, void *ent, float *a3, uint8_t a4);
CCitadel_Ability_Viscous_Telepunch_ImpactFn g_pfnCCitadel_Ability_Viscous_Telepunch_Impact_Orig = nullptr;
void Hook_CCitadel_Ability_Viscous_Telepunch_Impact(void *thisptr, deadlock::CBaseEntity *ent, float *a3, uint8_t a4) {
    if (!g_FriendlyFireEnabled) return g_pfnCCitadel_Ability_Viscous_Telepunch_Impact_Orig(thisptr, ent, a3, a4);

    int original_team_num = ent->m_iTeamNum;
    ent->m_iTeamNum = 4;
    g_pfnCCitadel_Ability_Viscous_Telepunch_Impact_Orig(thisptr, ent, a3, a4);
    ent->m_iTeamNum = original_team_num;
}

using CCitadel_Ability_PowerSlash_Cast2Fn = int(__fastcall *)(void *thisptr, void *a2);
CCitadel_Ability_PowerSlash_Cast2Fn g_pfnCCitadel_Ability_PowerSlash_Cast2_Orig = nullptr;
int Hook_CCitadel_Ability_PowerSlash_Cast2(deadlock::CBaseEntity *thisptr, void *a2) {
    if (!g_FriendlyFireEnabled) return g_pfnCCitadel_Ability_PowerSlash_Cast2_Orig(thisptr, a2);

    int original_team_num = 0;
    deadlock::CBaseEntity *owner = thisptr->GetOwner();
    if (owner) {
        original_team_num = owner->m_iTeamNum;
        owner->m_iTeamNum = 4;
        thisptr->m_iTeamNum = 4;
    }

    int r = g_pfnCCitadel_Ability_PowerSlash_Cast2_Orig(thisptr, a2);

    if (owner) {
        owner->m_iTeamNum = original_team_num;
        thisptr->m_iTeamNum = original_team_num;
    }

    return r;
}

using g_pfnUnkAbilityFuncFn = void(__fastcall *)(void *thisptr);
g_pfnUnkAbilityFuncFn g_pfnUnkAbilityFunc_Orig = nullptr;
void Hook_UnkAbilityFunc(deadlock::CBaseEntity *thisptr) {
    if (!g_FriendlyFireEnabled) return g_pfnUnkAbilityFunc_Orig(thisptr);

    // i see no traces
    int original_team_num = 0;
    deadlock::CBaseEntity *owner = thisptr->GetOwner();
    if (owner) {
        original_team_num = owner->m_iTeamNum;
        owner->m_iTeamNum = 4;
    }

    g_pfnUnkAbilityFunc_Orig(thisptr);

    if (owner) {
        owner->m_iTeamNum = original_team_num;
    }
}

using CBaseEntity_TakeDamageOldFn = void *(__fastcall *)(void *thisptr, void *dmg);
CBaseEntity_TakeDamageOldFn g_pfnCBaseEntity_TakeDamageOld_Orig = nullptr;
void *Hook_CBaseEntity_TakeDamageOld(deadlock::CBaseEntity *thisptr, deadlock::CTakeDamageInfo *dmg) {
    auto *attacker = dmg->GetAttacker();
    auto *inflictor = dmg->GetInflictor();
    auto *ability = dmg->GetAbility();
    auto *originator = dmg->GetOriginator();

    if (g_PrintStuff && strcmp(thisptr->AsInstance()->GetClassname(), "npc_trooper")) {
        ConMsg("%s:%s took damage from %s:%s:%s:%s\n", thisptr->AsInstance()->GetClassname(), reinterpret_cast<CEntityInstance *>(thisptr)->m_pEntity->m_designerName.String(),
               attacker ? attacker->AsInstance()->GetClassname() : "",
               inflictor ? inflictor->AsInstance()->GetClassname() : "",
               ability ? ability->AsInstance()->GetClassname() : "",
               originator ? originator->AsInstance()->GetClassname() : "");
        ConMsg("%llx:%llx:%llx\n", attacker, inflictor, ability);
        ConMsg(" - Ability owner: %llx (%s)\n", ability ? ability->GetOwner() : nullptr, ability ? ability->GetOwner()->AsInstance()->GetClassname() : "");
    }

    if (!g_FriendlyFireEnabled) return g_pfnCBaseEntity_TakeDamageOld_Orig(thisptr, dmg);

    // abilities should not hurt their casters
    if (ability && ability->GetOwner() == thisptr) {
        // exceptions for lady geist
        if (!strcmp(ability->AsInstance()->GetClassname(), "ability_blood_bomb")) {
            if (static_cast<uint64_t>(dmg->m_nDamageFlags) & static_cast<uint64_t>(deadlock::TakeDamageFlags_t::DFLAG_TRIGGER_FLINCH)) return nullptr; // ignore explosion
            if (static_cast<uint64_t>(dmg->m_bitsDamageType) & static_cast<uint64_t>(deadlock::DamageTypes_t::DMG_GROUND_AURA)) return nullptr;        // ignore aura damage
        } else if (strcmp(ability->AsInstance()->GetClassname(), "ability_blood_shards")) {
            return nullptr;
        }
    }

    return g_pfnCBaseEntity_TakeDamageOld_Orig(thisptr, dmg);
}

using CCitadelModifierAura_SearchFn = void(__fastcall *)(void *thisptr, CUtlVector<CHandle<deadlock::CBaseEntity>> *out);
CCitadelModifierAura_SearchFn g_pfnCCitadelModifierAura_Search_Orig = nullptr;
void Hook_CCitadelModifierAura_Search(void *thisptr, CUtlVector<CHandle<deadlock::CBaseEntity>> *out) {
    if (!g_FriendlyFireEnabled) return g_pfnCCitadelModifierAura_Search_Orig(thisptr, out);

    g_pfnCCitadelModifierAura_Search_Orig(thisptr, out);

    auto *mod = reinterpret_cast<deadlock::CBaseModifier *>(thisptr);
    auto *ability = mod->GetAbility();
    auto *caster = mod->GetCaster();

    if (ability && caster) {
        if (!strcmp(ability->AsInstance()->GetClassname(), "citadel_ability_self_vacuum")) {
            if (ability->GetOwner() == caster) {
                int idx_to_remove = -1;
                for (int i = 0; i < out->Count(); i++) {
                    auto &hdl = (*out)[i];
                    if (hdl.Get() == caster) {
                        idx_to_remove = i;
                    }
                }
                if (idx_to_remove >= 0) {
                    out->Remove(idx_to_remove);
                }
            }
        }
    }
}

using CCitadel_Ability_HoldMelee_unkAttackFn = void *(__fastcall *)(void *thisptr);
CCitadel_Ability_HoldMelee_unkAttackFn g_pfnCCitadel_Ability_HoldMelee_unkAttack_Orig = nullptr;
void *Hook_CCitadel_Ability_HoldMelee_unkAttack(deadlock::CBaseEntity *thisptr) {
    if (!g_FriendlyFireEnabled) return g_pfnCCitadel_Ability_HoldMelee_unkAttack_Orig(thisptr);

    auto *owner = thisptr->GetOwner();
    int original_team_num = 0;
    if (owner) {
        original_team_num = owner->m_iTeamNum;
        owner->m_iTeamNum = 4;
    }
    auto *r = g_pfnCCitadel_Ability_HoldMelee_unkAttack_Orig(thisptr);
    if (owner) owner->m_iTeamNum = original_team_num;
    return r;
}

using CCitadel_Ability_HoldMelee_unkTraceFn = bool (__fastcall*)(void* thisptr, int *a2, float a3);
CCitadel_Ability_HoldMelee_unkTraceFn g_pfnCCitadel_Ability_HoldMelee_unkTrace_Orig = nullptr;
bool Hook_CCitadel_Ability_HoldMelee_unkTrace(deadlock::CBaseEntity *thisptr, int *a2, float a3) {
	if (!g_FriendlyFireEnabled) return g_pfnCCitadel_Ability_HoldMelee_unkTrace_Orig(thisptr, a2, a3);

    auto *owner = thisptr->GetOwner();
	int original_team_num = 0;
	if (owner) {
        original_team_num = owner->m_iTeamNum;
        owner->m_iTeamNum = 4;
    }
    bool r = g_pfnCCitadel_Ability_HoldMelee_unkTrace_Orig(thisptr, a2, a3);
	if (owner) owner->m_iTeamNum = original_team_num;
	return r;
}

CON_COMMAND(owo_friendlyfire, "Enable or disable friendly fire") {
    if (args.ArgC() < 2) {
        ConMsg("owo_friendlyfire = %d\n", g_FriendlyFireEnabled);
    } else {
        g_FriendlyFireEnabled = atoi(args.Arg(1)) != 0;
        ConColorMsg(Color(0, 255, 0, 255), g_FriendlyFireEnabled ? "Friendly fire enabled\n" : "Friendly fire disabled\n");
    }
}

CON_COMMAND(owo_debug_print, "Enable or disable spammy debug print stuff") {
    if (args.ArgC() < 2) {
        ConMsg("owo_debug_print = %d\n", g_PrintStuff);
    } else {
        g_PrintStuff = atoi(args.Arg(1)) != 0;
    }
}

CON_COMMAND_F(dbg_printme, "", FCVAR_GAMEDLL | FCVAR_CLIENT_CAN_EXECUTE) {
    auto *controller = (deadlock::CCitadelPlayerController *)GameEntitySystem()->GetEntityInstance(CEntityIndex(context.GetPlayerSlot().Get() + 1));
    deadlock::CCitadelPlayerPawn *pawn = controller ? controller->GetPawn() : nullptr;
    ConMsg("%llx %llx\n", controller, pawn);

    if (pawn) {
        auto *abilities = pawn->GetAbilityComponent()->GetVecAbilities();
        for (int i = 0; i < abilities->Count(); i++) {
            auto *ability = (*abilities)[i].Get();
            if (ability) {
                const char *classname = ability->AsInstance()->GetClassname();
                Msg("%s\n", classname);
                if (!strcmp(classname, "citadel_item")) {
                    Msg("  - %s\n", ability->AsInstance()->m_pEntity->m_name.String());
                }
            }
        }
    }
}

CON_COMMAND(cvar_unhide, "") {
    uint64_t mask = FCVAR_HIDDEN | FCVAR_DEVELOPMENTONLY | FCVAR_MISSING3;
    int num_unhidden_concommands = 0;
    int num_unhidden_convars = 0;

    {
        ConCommandHandle hdl;
        auto invalid = interfaces::g_pCvar->GetCommand(hdl);
        int idx = 0;
        while (true) {
            hdl.Set(idx++);
            auto concommand = interfaces::g_pCvar->GetCommand(hdl);
            if (concommand == invalid) break;

            if (concommand->GetFlags() & mask) {
                concommand->RemoveFlags(mask);
                num_unhidden_concommands++;
            }
        }
    }

    {
        ConVarHandle hdl;
        auto invalid = interfaces::g_pCvar->GetConVar(hdl);
        int idx = 0;
        while (true) {
            hdl.Set(idx++);
            auto convar = interfaces::g_pCvar->GetConVar(hdl);
            if (convar == invalid) break;

            if (convar->flags & mask) {
                convar->flags &= ~mask;
                num_unhidden_convars++;
            }
        }
    }

    ConColorMsg(Color(255, 0, 255, 255), "%d concommands unhidden\n", num_unhidden_concommands);
    ConColorMsg(Color(255, 0, 255, 255), "%d convars unhidden\n", num_unhidden_convars);
}

bool Connect(IAppSystem *pAppSystem, CreateInterfaceFn fnCreateInterface) {
    auto result = g_pfnServerConfigConnect(pAppSystem, fnCreateInterface);

    g_pfnEngineCreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA("engine2"), "CreateInterface"));

    interfaces::g_pGameResourceService = reinterpret_cast<CGameResourceService *>(g_pfnEngineCreateInterface(GAMERESOURCESERVICESERVER_INTERFACE_VERSION, nullptr));
    if (!interfaces::g_pGameResourceService) {
        Plat_FatalErrorFunc("Could not get GameResourceService");
    }

    interfaces::g_pSource2GameClients = reinterpret_cast<ISource2GameClients *>(g_pfnServerCreateInterface(SOURCE2GAMECLIENTS_INTERFACE_VERSION, nullptr));
    if (!interfaces::g_pSource2GameClients) {
        Plat_FatalErrorFunc("Could not get Source2GameClients");
    }

    interfaces::g_pSource2Server = reinterpret_cast<ISource2Server *>(g_pfnServerCreateInterface(SOURCE2SERVER_INTERFACE_VERSION, nullptr));
    if (!interfaces::g_pSource2Server) {
        Plat_FatalErrorFunc("Could not get Source2Server");
    }

    interfaces::g_pCvar = reinterpret_cast<ICvar *>(fnCreateInterface("VEngineCvar007", nullptr));
    if (!interfaces::g_pCvar) {
        Plat_FatalErrorFunc("Could not get ICvar");
    }

    {
        static const auto addr = sigscan::scan(g_hServerModule, "\x40\x53\x48\x83\xEC\x20\x49\x8B\xC0\x48\x8D\x0D", "xxxxxxxxxxxx");
        static const auto displacement = *reinterpret_cast<int32_t *>(addr + 12);
        interfaces::g_pCitadelHeroVDataManager = reinterpret_cast<deadlock::CCitadelHeroVDataManager *>(addr + displacement + 0x10);
    }

    {
        static const auto addr = sigscan::scan(g_hServerModule, "\x48\x89\x05\x00\x00\x00\x00\x33\xDB", "xxx????xx");
        static const auto displacement = *reinterpret_cast<int32_t *>(addr + 3);
        interfaces::g_pCitadelHeroManager = reinterpret_cast<deadlock::CCitadelHeroManager *>(addr + displacement + 0x7);
    }

    {
        static const auto CTakeDamageInfo_CtorAddr = sigscan::scan(g_hServerModule, "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x20\x4D\x8B\xF1", "xxxx?xxxx?xxxx?xxxxxxxxxxxx");
        g_pfnCTakeDamageInfo_CtorOrig = reinterpret_cast<CTakeDamageInfo_CtorFn>(CTakeDamageInfo_CtorAddr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCTakeDamageInfo_CtorOrig), &Hook_CTakeDamageInfo_Ctor);
        funchook_install(hook, 0);
    }

    {
        static const auto Magic_Addr = sigscan::scan(g_hServerModule, "\x48\x8B\xC4\x4C\x89\x48\x00\x4C\x89\x40\x00\x48\x89\x50\x00\x89\x48\x00\x55\x53", "xxxxxx?xxx?xxx?xx?xx");
        g_pfnMagic_Orig = reinterpret_cast<MagicFn>(Magic_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnMagic_Orig), &Hook_Magic);
        funchook_install(hook, 0);
    }

    {
        static const auto ExecuteTrace_Addr = sigscan::scan(g_hServerModule, "\x4C\x89\x44\x24\x00\x55\x53\x56\x41\x55\x41\x56", "xxxx?xxxxxxx");
        g_pfnExecuteTrace_Orig = reinterpret_cast<ExecuteTraceFn>(ExecuteTrace_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnExecuteTrace_Orig), &Hook_ExecuteTrace);
        funchook_install(hook, 0);
    }

    {
        static const auto ExecuteRaySegmentTrace_Addr = sigscan::scan(g_hServerModule, "\x4C\x89\x44\x24\x00\x48\x89\x54\x24\x00\x55\x53\x57\x41\x54\x41\x55\x41\x57", "xxxx?xxxx?xxxxxxxxx");
        g_pfnExecuteRaySegmentTrace_Orig = reinterpret_cast<ExecuteRaySegmentTraceFn>(ExecuteRaySegmentTrace_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnExecuteRaySegmentTrace_Orig), &Hook_ExecuteRaySegmentTrace);
        funchook_install(hook, 0);
    }

    {
        static const auto TraceLineWithExcludedEntity_Addr = sigscan::scan(g_hServerModule, "\x4C\x8B\xDC\x49\x89\x5B\x00\x49\x89\x6B\x00\x49\x89\x73\x00\x57\x41\x56\x41\x57\x48\x81\xEC\xA0\x00\x00\x00", "xxxxxx?xxx?xxx?xxxxxxxxxxxx");
        g_pfnTraceLineWithExcludedEntity_Orig = reinterpret_cast<TraceLineWithExcludedEntityFn>(TraceLineWithExcludedEntity_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnTraceLineWithExcludedEntity_Orig), &Hook_TraceLineWithExcludedEntity);
        funchook_install(hook, 0);
    }

    {
        static const auto TraceShape_Addr = sigscan::scan(g_hServerModule, "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x41\x55", "xxxx?xxxx?xxxx");
        g_pfnTraceShape_Orig = reinterpret_cast<TraceShapeFn>(TraceShape_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnTraceShape_Orig), &Hook_TraceShape);
        funchook_install(hook, 0);
    }

    {
        static const auto CCitadel_Ability_IceBeam_Dmg_Addr = sigscan::scan(g_hServerModule, "\x48\x89\x4C\x24\x00\x55\x41\x54\x48\x8D\xAC\x24", "xxxx?xxxxxxx");
        g_pfnCCitadel_Ability_IceBeam_Dmg_Orig = reinterpret_cast<CCitadel_Ability_IceBeam_DmgFn>(CCitadel_Ability_IceBeam_Dmg_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCCitadel_Ability_IceBeam_Dmg_Orig), &Hook_CCitadel_Ability_IceBeam_Dmg);
        funchook_install(hook, 0);
    }
    {
        static const auto CCitadel_Ability_Viscous_Telepunch_Impact_Addr = sigscan::scan(g_hServerModule, "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x70\x00\x48\x89\x78\x00\x4C\x89\x70\x00\x55\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x01\x00\x00\x0F\x29\x70\x00\x48\x8D\x99", "xxxxxx?xxx?xxx?xxx?xxxx????xxx?xxxxxx?xxx");
        g_pfnCCitadel_Ability_Viscous_Telepunch_Impact_Orig = reinterpret_cast<CCitadel_Ability_Viscous_Telepunch_ImpactFn>(CCitadel_Ability_Viscous_Telepunch_Impact_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCCitadel_Ability_Viscous_Telepunch_Impact_Orig), &Hook_CCitadel_Ability_Viscous_Telepunch_Impact);
        funchook_install(hook, 0);
    }

    {
        static const auto CCitadel_Ability_PowerSlash_Cast2_Addr = sigscan::scan(g_hServerModule, "\x48\x89\x5C\x24\x00\x57\x48\x81\xEC\x80\x00\x00\x00\x83\xB9", "xxxx?xxxxxxxxxx");
        g_pfnCCitadel_Ability_PowerSlash_Cast2_Orig = reinterpret_cast<CCitadel_Ability_PowerSlash_Cast2Fn>(CCitadel_Ability_PowerSlash_Cast2_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCCitadel_Ability_PowerSlash_Cast2_Orig), &Hook_CCitadel_Ability_PowerSlash_Cast2);
        funchook_install(hook, 0);
    }

    {
        /*
        static const auto UnkAbilityFunc_Addr = sigscan::scan(g_hServerModule, "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x00\x00\x00\x00\x80\xBB\x00\x00\x00\x00\x00\x74\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x85\xC0", "xxxxxxxxxx????xx?????x?xxxx????xxx");
        g_pfnUnkAbilityFunc_Orig = reinterpret_cast<g_pfnUnkAbilityFuncFn>(UnkAbilityFunc_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnUnkAbilityFunc_Orig), &Hook_UnkAbilityFunc);
        funchook_install(hook, 0);
        */
    }

    {
        static const auto CBaseEntity_TakeDamageOld_Addr = sigscan::scan(g_hServerModule, "\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x41", "xxxx?xxxxxxxxxxx");
        g_pfnCBaseEntity_TakeDamageOld_Orig = reinterpret_cast<CBaseEntity_TakeDamageOldFn>(CBaseEntity_TakeDamageOld_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCBaseEntity_TakeDamageOld_Orig), &Hook_CBaseEntity_TakeDamageOld);
        funchook_install(hook, 0);
    }

    {
        // CCitadelModifierAura vfunc 106
        static const auto CCitadelModifierAura_Search_Addr = sigscan::scan(g_hServerModule, "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x83\xEC\x60\x0F\x29\x74\x24\x00\x4C\x8B\xF2", "xxxx?xxxx?xxxx?xxxx?xxxxxxxxxx?xxx");
        g_pfnCCitadelModifierAura_Search_Orig = reinterpret_cast<CCitadelModifierAura_SearchFn>(CCitadelModifierAura_Search_Addr);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCCitadelModifierAura_Search_Orig), &Hook_CCitadelModifierAura_Search);
        funchook_install(hook, 0);
    }

    {
        // callstack
        static const auto CCitadel_Ability_HoldMelee_unkAttack = sigscan::scan(g_hServerModule, "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\xE0\x01\x00\x00\x0F\x29\x70\x00\x48\x8B\xF1\x0F\x29\x78\x00\xE8", "xxxxxx?xxxxxxxxxxxxxx????xxxxxxxxxx?xxxxxx?x");
        g_pfnCCitadel_Ability_HoldMelee_unkAttack_Orig = reinterpret_cast<CCitadel_Ability_HoldMelee_unkAttackFn>(CCitadel_Ability_HoldMelee_unkAttack);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCCitadel_Ability_HoldMelee_unkAttack_Orig), &Hook_CCitadel_Ability_HoldMelee_unkAttack);
        funchook_install(hook, 0);
    }

    {
        // callstack
        static const auto CCitadel_Ability_HoldMee_unkTrace = sigscan::scan(g_hServerModule, "\x40\x55\x41\x54\x41\x56\x41\x57\x48\x8D\xAC\x24", "xxxxxxxxxxxx");
        g_pfnCCitadel_Ability_HoldMelee_unkTrace_Orig = reinterpret_cast<CCitadel_Ability_HoldMelee_unkTraceFn>(CCitadel_Ability_HoldMee_unkTrace);

        auto hook = funchook_create();
        funchook_prepare(hook, reinterpret_cast<void **>(&g_pfnCCitadel_Ability_HoldMelee_unkTrace_Orig), &Hook_CCitadel_Ability_HoldMelee_unkTrace);
		funchook_install(hook, 0);
    }

    interfaces::g_pCvar->RegisterConCommand(&owo_friendlyfire_command);
    interfaces::g_pCvar->RegisterConCommand(&dbg_printme_command);
    interfaces::g_pCvar->RegisterConCommand(&owo_debug_print_command);
    interfaces::g_pCvar->RegisterConCommand(&cvar_unhide_command);

    return result;
}

EXPORT void *CreateInterface(const char *pName, int *pReturnCode) {
    if (!g_pfnServerCreateInterface) {
        std::string game_directory = Plat_GetGameDirectory();
        std::string lib_path = game_directory + "\\citadel\\bin\\win64\\server.dll";
        g_hServerModule = LoadLibraryA(lib_path.c_str());

        if (!g_hServerModule) {
            Plat_FatalErrorFunc("Failed to load server module from %s: %d\n", lib_path.c_str(), GetLastError());
        }

        g_pfnServerCreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(g_hServerModule, "CreateInterface"));
        if (!g_pfnServerCreateInterface) {
            Plat_FatalErrorFunc("Failed to find CreateInterface in server module: %d\n", GetLastError());
        }
    }

    void *original = g_pfnServerCreateInterface(pName, pReturnCode);
    if (!strcmp(pName, "Source2ServerConfig001")) {
        void **vtable = *(void ***)original;

        DWORD dwOldProtect = 0;
        if (!VirtualProtect(vtable, sizeof(void **), PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
            Plat_FatalErrorFunc("VirtualProtect failed: %d\n", GetLastError());
        }

        g_pfnServerConfigConnect = reinterpret_cast<AppSystemConnectFn>(vtable[0]);
        vtable[0] = &Connect;

        if (!VirtualProtect(vtable, sizeof(void **), dwOldProtect, &dwOldProtect)) {
            Plat_FatalErrorFunc("VirtualProtect failed: %d\n", GetLastError());
        }
    }

    return original;
}
