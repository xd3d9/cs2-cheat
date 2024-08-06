#pragma once

#include <cstdint>
#include "../../schema_system.hpp"
#include "../math/vec3_t.hpp"
#include "skeleton.hpp"
#include "../../entity_handle.h"
#include <vector>
inline static std::vector<std::uint32_t> dfadfadaf(const char* pattern)
{
    std::vector<std::uint32_t> bytes;
    char* start = const_cast<char*>(pattern);
    char* end = const_cast<char*>(pattern) + std::strlen(pattern);

    for (char* current = start; current < end; current++)
    {
        if (*current == '?')
        {
            current++;

            if (*current == '?')
            {
                current++;
            }

            bytes.push_back(-1);
        }
        else
        {
            bytes.push_back(std::strtoul(current, &current, 16));
        }
    }

    return bytes;
}
#define NOMINMAX
#include <windows.h>
inline std::uint8_t* adfhadhadfhcan(const wchar_t* module_name, const char* signature)
{
    HMODULE module_handle = GetModuleHandle(module_name);

    if (!module_handle)
    {
        return nullptr;
    }

    PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    std::vector<std::uint32_t> pattern_bytes = dfadfadaf(signature);
    std::uint8_t* image_base = reinterpret_cast<std::uint8_t*>(module_handle);

    std::size_t pattern_size = pattern_bytes.size();
    std::uint32_t* array_of_bytes = pattern_bytes.data();

    for (std::size_t i = 0; i < size_of_image - pattern_size; i++)
    {
        bool found = true;

        for (std::size_t j = 0; j < pattern_size; j++)
        {
            if (image_base[i + j] != array_of_bytes[j] && array_of_bytes[j] != -1)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            return &image_base[i];
        }
    }

    return nullptr;
}

enum buttons_t : std::uint32_t
{
    in_attack = 1,
    in_jump = 2,
    in_duck = 4,
    in_attack2 = 2048,
    in_moveforward = 8,
    in_moveback = 16,
    in_moveleft = 512,
    in_moveright = 1024
};

enum movetype_t : std::uint32_t
{
    movetype_none = 0,
    movetype_isometric,
    movetype_walk,
    movetype_step,
    movetype_fly,
    movetype_flygravity,
    movetype_vphysics,
    movetype_push,
    movetype_noclip,
    movetype_ladder,
    movetype_observer,
    movetype_custom,
    movetype_last = movetype_custom,
    movetype_max_bits = 4
};

enum flags_t : std::uint32_t
{
    fl_onground = (1 << 0),
    fl_ducking = (1 << 1),
    fl_waterjump = (1 << 2),
    fl_ontrain = (1 << 3),
    fl_inrain = (1 << 4),
    fl_frozen = (1 << 5),
    fl_atcontrols = (1 << 6),
    fl_client = (1 << 7),
    fl_fakeclient = (1 << 8),
    fl_inwater = (1 << 9),
    fl_fly = (1 << 10),
    fl_swim = (1 << 11),
    fl_conveyor = (1 << 12),
    fl_npc = (1 << 13),
    fl_godmode = (1 << 14),
    fl_notarget = (1 << 15),
    fl_aimtarget = (1 << 16),
    fl_partialground = (1 << 17),
    fl_staticprop = (1 << 18),
    fl_graphed = (1 << 19),
    fl_grenade = (1 << 20),
    fl_stepmovement = (1 << 21),
    fl_donttouch = (1 << 22),
    fl_basevelocity = (1 << 23),
    fl_worldbrush = (1 << 24),
    fl_object = (1 << 25),
    fl_killme = (1 << 26),
    fl_onfire = (1 << 27),
    fl_dissolving = (1 << 28),
    fl_transragdoll = (1 << 29),
    fl_unblockable_by_player = (1 << 30)
};

enum frame_stage_t : std::int32_t
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};

class game_scene_node_t
{
public:
    SCHEMA(vec3_t, abs_origin, "CGameSceneNode", "m_vecAbsOrigin");
    SCHEMA(vec3_t, vec_origin, "CGameSceneNode", "m_vecOrigin");
    SCHEMA(bool, dormant, "CGameSceneNode", "m_bDormant");
};

class collision_property_t
{
public:
    SCHEMA(vec3_t, mins, "CCollisionProperty", "m_vecMins");
    SCHEMA(vec3_t, maxs, "CCollisionProperty", "m_vecMaxs");
    std::uint16_t get_collision_mask()
    {
        return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38);
    }
};




class entity_t
{
public:


    SCHEMA(flags_t, flags, "C_BaseEntity", "m_fFlags");
    SCHEMA(game_scene_node_t*, game_scene_node, "C_BaseEntity", "m_pGameSceneNode");
    SCHEMA(collision_property_t*, collision_property, "C_BaseEntity", "m_pCollision");
    SCHEMA(movetype_t, move_type, "C_BaseEntity", "m_MoveType");


};



class weapon_t : public entity_t
{
public:

};


class weapon_services_t
{
public:
    SCHEMA(std::uint32_t, active_weapon, "CPlayer_WeaponServices", "m_hActiveWeapon");
    // access weapon data
    /*
    weapon_vdata* get_weapon_data() { // weapon_v_data
        // ida: 48 8B 81 ? ? ? ? 48 8B 88 ? ? ? ? 48 8D 05 ? ? ? ? 48 85 C9 48 0F 45 C1 C3 CC CC CC 49 8B C8 + 0x3
        // 48 8B 81 ? ? ? ? 8B 80 ? ? ? ? C3 CC CC 66 89 54 24 ? - directly

        //static auto offset = *reinterpret_cast<int*>(scanner::pattern_scan(L"client.dll", "48 8B 81 ? ? ? ? 48 8B 88 ? ? ? ? 48 8D 05 ? ? ? ? 48 85 C9 48 0F 45 C1 C3 CC CC CC 49 8B C8")+0x3);
        return *reinterpret_cast<weapon_vdata**>((uintptr_t)this + 0x368);
    }
    */
};

class weapon_vdata
{
public:
};

class player_t : public entity_t
{
public:
    SCHEMA(weapon_services_t*, weapon_services, "C_BasePlayerPawn", "m_pWeaponServices");
    SCHEMA(int, m_iHealth, "C_BaseEntity", "m_iHealth");
    SCHEMA(std::uint8_t, team, "C_BaseEntity", "m_iTeamNum");
    SCHEMA(vec3_t, view_offset, "C_BaseModelEntity", "m_vecViewOffset");
    SCHEMA(bool, has_gun_immunity, "C_CSPlayerPawnBase", "m_bGunGameImmunity");

    bool is_alive()
    {
        return this->m_iHealth() > 0;
    }

    void get_bone_position(std::uint32_t bone_index, vec3_t& position, vec3_t& rotation)
    {
        using function_t = std::int64_t(__fastcall*)(player_t*, std::uint32_t, vec3_t*, vec3_t*);
        static function_t fn = reinterpret_cast<function_t>(adfhadhadfhcan(L"client.dll", "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1"));

        fn(this, bone_index, &position, &rotation);
    }

    vec3_t get_eye_position()
    {
        return this->game_scene_node()->vec_origin() + this->view_offset();
    }
};

class controller_t
{
public:
    SCHEMA(const char*, ControllerName, "CCSPlayerController", "m_sSanitizedPlayerName");
    SCHEMA(CBaseHandle, GetPawnHandle, "CBasePlayerController","m_hPawn");
};