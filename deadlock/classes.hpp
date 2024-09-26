#pragma once

#include "defs.hpp"
#include "enums.hpp"

#include "../globals.hpp"
#include "../sigscan.hpp"

#include <source2sdk/client/CTakeDamageInfo.hpp>
#include <source2sdk/client/ECurrencySource.hpp>
#include <source2sdk/client/ECurrencyType.hpp>
#include <source2sdk/client/CEntitySubclassVDataBase.hpp>
#include <source2sdk/server/CBaseModifier.hpp>
#include <source2sdk/server/CBodyComponent.hpp>
#include <source2sdk/server/CGameSceneNode.hpp>
#include <source2sdk/server/CCitadelModifierVData.hpp>
#include <source2sdk/server/CCitadelPlayerController.hpp>
#include <source2sdk/server/CCitadelPlayerPawn.hpp>
#include <source2sdk/server/CitadelHeroData_t.hpp>
#include <source2sdk/server/CModifierProperty.hpp>

#include <entity2/entitysystem.h>
#include <keyvalues3.h>
#include <utlstring.h>

template<typename T>
T vfunc(void *vft, int i) {
    return (*reinterpret_cast<T **>(vft))[i];
}

class CGameResourceService {
public:
    CGameEntitySystem *GetGameEntitySystem() {
        return *reinterpret_cast<CGameEntitySystem **>(reinterpret_cast<uintptr_t>(this) + 88);
    }
};

template<typename T>
struct CHandle {
    T *Get() {
        return reinterpret_cast<T *>(GameEntitySystem()->GetEntityInstance(CEntityIndex(m_data & 0x7fff)));
    }

    int32_t m_data;
};

// global funcs
namespace deadlock {
// maybe there is a better name for this
source2sdk::client::CEntitySubclassVDataBase *GetVDataInstanceByName(EntitySubclassScope_t type, const char *name);
} // namespace deadlock

// structs
namespace deadlock {
struct CitadelHeroData_t : source2sdk::server::CitadelHeroData_t {
    __declspec(noinline) float *GetMapStartingStat(deadlock::EStatsType type);
};
} // namespace deadlock

// deadlock-specific interfaces
namespace deadlock {
struct CCitadelHeroManager {
    __declspec(noinline) deadlock::CitadelHeroData_t *GetHeroDataFromID(HeroID_t hero_id);
    __declspec(noinline) deadlock::CitadelHeroData_t *GetHeroDataFromName(const char *hero_name);
    __declspec(noinline) deadlock::HeroID_t GetHeroIDFromName(const char *hero_name);
};

struct CCitadelHeroVDataManager {
    // deadlock::CitadelHeroData_t *GetHeroDataFromName(const char *name);
};
} // namespace deadlock

// classes
namespace deadlock {
struct CBaseEntity
    : source2sdk::server::CBaseEntity {
    deadlock::CBaseEntity *GetOwner() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hOwnerEntity)->Get();
    }

    ::CEntityInstance *AsInstance() {
        return reinterpret_cast<::CEntityInstance *>(this);
    }
};

struct CModifierProperty : public source2sdk::server::CModifierProperty {
    void AddModifier(source2sdk::server::CBaseEntity *entity, source2sdk::server::CCitadelModifierVData *vdata, KeyValues3 *kv3);
};

struct CCitadelAbilityComponent : public source2sdk::server::CCitadelAbilityComponent {
    CUtlVector<CHandle<deadlock::CBaseEntity>> *GetVecAbilities();
};

struct CCitadelPlayerPawn : public source2sdk::server::CCitadelPlayerPawn {
    void GiveCurrency(deadlock::ECurrencyType currency_type, int amount);
    void GiveItem(const char *item_name);
    void AddModifier(const char *name, float duration = 0.f);
    HeroID_t GetHeroID();
    CCitadelAbilityComponent *GetAbilityComponent();
};

struct CCitadelPlayerController : public source2sdk::server::CCitadelPlayerController {
    CCitadelPlayerPawn *GetPawn();
};

struct CTakeDamageInfo : public source2sdk::client::CTakeDamageInfo {
    deadlock::CBaseEntity *GetInflictor() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hInflictor)->Get();
    }

    deadlock::CBaseEntity *GetAttacker() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hAttacker)->Get();
    }

    deadlock::CBaseEntity *GetAbility() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hAbility)->Get();
    }

    deadlock::CBaseEntity* GetOriginator() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hOriginator)->Get();
    }
};

struct CBaseModifier : public source2sdk::server::CBaseModifier {
    deadlock::CBaseEntity *GetCaster() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hCaster)->Get();
    }

    deadlock::CBaseEntity* GetAbility() {
        return reinterpret_cast<CHandle<deadlock::CBaseEntity> *>(m_hAbility)->Get();
    }
};
} // namespace deadlock
