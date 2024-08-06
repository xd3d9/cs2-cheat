#pragma once


#include "../math/vec3_t.hpp"
#include "../classes/user_cmd.hpp"

#define MULTIPLAYER_BACKUP 150

enum csgo_input_vtable
{
	CREATEMOVE = 5,
	MOUSE_INPUT = 10
};

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define MEM_PAD(sz)                               \
private:                                            \
    std::uint8_t COMBINE2(pad_, __COUNTER__)[sz];   \
public:


class i_csgo_input
{
public:
    MEM_PAD(0x250);
    CUserCmd m_arrCommands[MULTIPLAYER_BACKUP];
    MEM_PAD(0x1);
    bool m_bInThirdPerson;
    vec3_t m_angThirdPersonAngles;
    MEM_PAD(0x14)
    std::int32_t m_nSequenceNumber;
    CUserCmd* GetUserCmd()
    {
        return &m_arrCommands[m_nSequenceNumber % MULTIPLAYER_BACKUP];
    }

    void set_view_angles(vec3_t& angles)
    {
        /*
        using function_t = std::int64_t(__fastcall*)(i_csgo_input*, std::int32_t, vec3_t&);
        static function_t fn = reinterpret_cast<function_t>(utilities::pattern_scan(L"client.dll", SET_VIEW_ANGLES));

        fn(this, 0, angles);
        */
    }

    void get_view_angles(vec3_t& angles)
    {
        /*
        using function_t = void*(__fastcall*)(i_csgo_input*, std::int32_t);
        static function_t fn = reinterpret_cast<function_t>(scanner::pattern_scan(L"client.dll", GET_VIEW_ANGLES));

        angles = *reinterpret_cast<vec3_t*>(fn(this, 0));
        */
    }
};