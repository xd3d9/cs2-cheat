#include "esp.hpp"
#include "sdk/interfaces/interfaces.hpp"
#include "sdk/sdk.hpp"
#include "renderer.hpp"
#include <iostream>

typedef struct _box_t
{
	float x, y, w, h;
} box_t;


static bool get_bounding_box(player_t* player, box_t& box)
{
	vec3_t origin = player->game_scene_node()->abs_origin();

	vec2_t flb, brt, blb, frt, frb, brb, blt, flt = { };
	float left, top, right, bottom = 0.f;

	vec3_t min = player->collision_property()->mins() + origin;
	vec3_t max = player->collision_property()->maxs() + origin;

	vec3_t points[8] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, min.y, max.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, max.y, max.z) };

	if (!math::world_to_screen(points[0], flb)
		|| !math::world_to_screen(points[5], brt)
		|| !math::world_to_screen(points[0], blb)
		|| !math::world_to_screen(points[4], frt)
		|| !math::world_to_screen(points[2], frb)
		|| !math::world_to_screen(points[1], brb)
		|| !math::world_to_screen(points[6], blt)
		|| !math::world_to_screen(points[7], flt))
	{
		return false;
	}

	vec2_t arr[8] = {flb, brt, blb, frt, frb, brb, blt, flt};

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (std::int32_t i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
		{
			left = arr[i].x;
		}

		if (bottom < arr[i].y)
		{
			bottom = arr[i].y;
		}

		if (right < arr[i].x)
		{
			right = arr[i].x;
		}

		if (top > arr[i].y)
		{
			top = arr[i].y;
		}
	}

	box.x = left;
	box.y = top;
	box.w = right - left;
	box.h = bottom - top;

	return true;
}

static void draw_box(box_t& box)
{
	renderer::rect(box.x - 1, box.y - 1, box.w + 2, box.h + 2, color_t::black());
	renderer::rect(box.x + 1, box.y + 1, box.w - 2, box.h - 2, color_t::black());
	renderer::rect(box.x, box.y, box.w, box.h, color_t::white());
}

static void draw_health(box_t& box, std::int32_t player_health)
{
	if (!esp::health) {
		return;
	}

	float bar_height = player_health * box.h / 100.f;
	std::uint8_t color_scale = static_cast<std::uint8_t>(player_health * 2.55);

	box_t bar_box = { box.x - 5.f, box.y, 3.f, box.h };
	box_t health_bar = { box.x - 4.f, box.y + box.h - bar_height, 1.f, bar_height };

	renderer::filled_rect(bar_box.x, bar_box.y, bar_box.w, bar_box.h, color_t::black());
	renderer::filled_rect(health_bar.x, health_bar.y, health_bar.w, health_bar.h, color_t(255 - color_scale, color_scale, 0));
}



static void draw_skeleton(player_t* player)
{
	if (!esp::skeleton) {
		return;
	}
	constexpr std::uint8_t tr[][2] =
	{
		{ 84, 26 },{ 26, 25 },{ 25, 0 },	// right leg
		{ 82, 23 },{ 23, 22 },{ 22, 0 },	// left leg
		{ 5, 92 },{ 92, 11 },{ 11, 12 },	// left arm
		{ 5, 96 },{ 96, 16 },{ 16, 17 },	// right arm
		{ 0, 21 },{ 21, 5 },{ 5, 6 }		// spine
	};

	constexpr std::uint8_t ct[][2] =
	{
		{ 97, 27 },{ 27, 26 },{ 26, 0 },	// right leg
		{ 95, 24 },{ 24, 23 },{ 23, 0 },	// left leg
		{ 5, 55 },{ 55, 11 },{ 11, 12 },	// left arm
		{ 5, 105 },{ 105, 16 },{ 16, 17 },	// right arm
		{ 0, 21 },{ 21, 5 },{ 5, 6 }		// spine
	};

	for (std::uint8_t i = 0; i < 15; ++i)
	{
		vec3_t rotation1, rotation2 = { };
		vec2_t position1, position2 = { };
		vec3_t from = { };
		vec3_t to = { };

		player->get_bone_position(player->team() == 2 ? tr[i][0] : ct[i][0], from, rotation1);
		player->get_bone_position(player->team() == 2 ? tr[i][1] : ct[i][1], to, rotation2);

		if (math::world_to_screen(from, position1) && math::world_to_screen(to, position2))
		{
			renderer::line(position1.x, position1.y, position2.x, position2.y, color_t::white());
		}
	}
	
}

std::uint8_t* hadfhadfh(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset)
{
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}

static void draw_name(box_t& box, const char* player_name)
{

	std::string name(player_name);

	if (name.empty())
	{
		return;
	}

	if (name.size() > 15)
	{
		name = name.substr(0, 15) + "...";
	}

	renderer::text_centered(box.x, box.y, box.w, box.h, color_t::white(), name.c_str());
}

void esp::exec() {

	if (!esp::esp_on) {
		return;
	}


	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	if (!sdk::local_player)
	{
		return;
	}

	box_t box = { };


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

		if (!player || !player->is_alive() || player == sdk::local_player)
		{
			continue;
		}

		if (player->team() == sdk::local_player->team())
		{
			continue;
		}

		if (!get_bounding_box(player, box))
		{
			continue;
		}

		draw_box(box);
		draw_health(box, player->m_iHealth());
		draw_name(box, controller->ControllerName());
		draw_skeleton(player);
	}
}