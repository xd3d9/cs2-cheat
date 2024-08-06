#include "aimbot.hpp"

#include "../sdk/sdk.hpp"

#include <cmath>
#include <vector>

static std::vector<std::uint32_t> bones;

static bool can_see_player_position(player_t* player, vec3_t& position)
{
    //interfaces::trace = *reinterpret_cast<i_trace**>(scanner::resolve_rip(scanner::pattern_scan(L"client.dll", "4C 8B 3D ? ? ? ? 24 C9 0C 49 66 0F 7F 45 ?"), 3, 7));
    c_trace_filter filter(0x1C3003, sdk::local_player, 4);
    c_ray ray = { };
    c_game_trace trace = { };

    vec3_t eye_position = sdk::local_player->get_eye_position();
    interfaces::trace->TraceShape(&ray, eye_position, position, &filter, &trace);
    //std::cout << "fraction " << trace.m_flFraction << std::endl;
    return trace.m_pHitEntity == player || trace.m_flFraction > 0.97f;
}


void aimbot::exec(CUserCmd* cmd)
{
    
    if (!aimbot::aim_on)
    {
        return;
    }

    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
    {
        return;
    }

    if (!sdk::local_player || !sdk::local_player->is_alive())
    {
        return;
    }

    if (sdk::local_player->move_type() == movetype_t::movetype_ladder || sdk::local_player->move_type() == movetype_t::movetype_noclip)
    {
        return;
    }

    float target_fov = aimbot::legit_fov;
    vec3_t best_target = vec3_t();
    vec3_t local_eye_position = sdk::local_player->get_eye_position();
    vec3_t cmd_angles = cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue;

    if (aimbot::hitboxes[0])
    {
        bones.emplace_back(5); // neck
        bones.emplace_back(6); // head
    }

    if (aimbot::hitboxes[1])
    {
        bones.emplace_back(0); // pelvis
        bones.emplace_back(1); // spine_0
        bones.emplace_back(2); // spine_1
        bones.emplace_back(3); // spine_2
        bones.emplace_back(4); // spine_3
    }
    
    if (aimbot::hitboxes[2])
    {
        bones.emplace_back(10); // arm_upper_l
        bones.emplace_back(11); // arm_lower_l
        bones.emplace_back(15); // arm_upper_r
        bones.emplace_back(16); // arm_lower_r
    }

    if (aimbot::hitboxes[3])
    {
        bones.emplace_back(23); // leg_upper_l
        bones.emplace_back(24); // leg_lower_l
        bones.emplace_back(26); // leg_upper_r
        bones.emplace_back(27); // leg_lower_r
    }
    

    //interfaces::globals = *reinterpret_cast<c_global_vars**>(scanner::resolve_rip(scanner::pattern_scan(L"client.dll", "48 89 0D ? ? ? ? 48 89 41"), 3, 7));
    // ^^^^^^^^ needs seperate updater unda movhukot karoche raghac da es unda chavsvat mand  ### GAMOSWOREBULIA,  CHAANACVLA LEVEL INITMA

    for (std::uint32_t i = 0; i < interfaces::globals->max_clients; i++)
    {
        controller_t* controller = interfaces::entity_list->GetEntityByIndex(i);

		if (!controller)
		{
			continue;
		}
		CBaseHandle handle = controller->GetPawnHandle();

		if (!handle.IsValid())
		{
			continue;
		}

		player_t* player = interfaces::entity_list->ongod(controller->GetPawnHandle());

        if (!player)
        {
            continue;
        }

        if (!player || !player->is_alive() || player->game_scene_node()->dormant() || player->has_gun_immunity())
        {
            continue;
        }

        if (player->team() == sdk::local_player->team())
        {
            continue;

        }
        vec3_t bone_position = { };
        vec3_t bone_rotation = { };

        for (std::uint32_t bone_id : bones)
        {
            player->get_bone_position(bone_id, bone_position, bone_rotation);

            if (!can_see_player_position(player, bone_position))
            {
                continue;
            }

            vec3_t angle = math::calculate_angle(local_eye_position, bone_position, cmd_angles);
            angle.clamp();

            float fov = std::hypotf(angle.x, angle.y);
            //std::cout << "fov " << fov << std::endl;
            if (fov < target_fov)
            {
                best_target = bone_position;
                target_fov = fov;
            }
        }
    }

    bones.clear();

    if (best_target.is_zero())
    {
        return;
    }

    vec3_t angle = math::calculate_angle(local_eye_position, best_target, cmd_angles);
    angle.clamp();

    angle /= aimbot::smooth;
    cmd_angles += angle;
    //std::cout << (cmd->m_nButtons.m_nValue & IN_ATTACK) << std::endl;
    //cmd->SetSubTickAngle(cmd_angles);
    if (check_attack && !(cmd->m_nButtons.m_nValue & IN_ATTACK)) {
        return;
    }

    cmd->SetSubTickAngle(cmd_angles);
    /*
    cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.x = cmd_angles.x;
    cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.y = cmd_angles.y;
    cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.z = cmd_angles.z;
    cmd->m_csgoUserCmd.CheckAndSetBits(EBaseCmdBits::BASE_BITS_VIEWANGLES);
    */

    /*
    if (!config::context.silent_aim)
    {
        cmd->base->view->angles = cmd_angles;
        interfaces::csgo_input->set_view_angles(cmd->base->view->angles);
    }
    */
}
/*

 *  amaze mushaoba chirdeba
 *   auto pLocalWeapon = sdk::local_player->weapon_services();
 *   auto pLocalWeaponVData = pLocalWeapon->get_weapon_data();
 *
 *  auto seed = cmd->m_csgoUserCmd.m_pBaseCmd->m_nRandomSeed;
 *  auto inaccuracy = C_CSWeaponBaseGun_GetInaccuracy(pLocalWeapon);
 *  auto spread = C_CSWeaponBaseGun_GetSpread(pLocalWeapon);
 *  auto recoil_index = pLocalWeapon->m_flRecoilIndex();
 *
 *  auto spread_x = 0.f;
 *  auto spread_y = 0.f;
 *
 *  CalculateSpreadAngle(pLocalWeapon, pLocalWeaponVData, 1, seed, inaccuracy, spread, recoil_index, &spread_x, &spread_y);
 *  

 //mtavari shi for rcs
void adjust_recoil(C_CSPlayerPawn* player, CUserCmd* pCmd) noexcept {
    if (!player || !pCmd) return;

    auto cache = player->m_aimPunchCache();
    static QAngle_t prev = QAngle_t(0.f, 0.f, 0.f);
    auto pred_punch = cache.m_Size < 0x81 ? cache.m_Data[cache.m_Size - 1] * 2.f : player->m_aimPunchAngle() * 2.f;
    auto delta = cache.m_Size > 0 && cache.m_Size < 0x81 ? prev - pred_punch : QAngle_t(0, 0, 0);

    if (cache.m_Size > 0 && cache.m_Size < 0x81) {
        if (delta != QAngle_t(0, 0, 0)) {
            pCmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue += delta;
            pCmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.Clamp();
        }
        prev = pred_punch;
    }

}
*/