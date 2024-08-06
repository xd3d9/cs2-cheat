#pragma once

#include "../sdk/classes/user_cmd.hpp"
#include "../sdk/interfaces/i_csgo_input.hpp"


namespace aimbot
{
	void exec(CUserCmd* cmd);

	inline bool aim_on = false;
	inline float legit_fov = 30;
	inline float smooth = 10;
	inline bool hitboxes[4] = { false, false, false, false };
	inline bool check_attack = false;
}