#include "sdk.hpp"
controller_t* sdk::local_controller = nullptr;
player_t* sdk::local_player = nullptr;
std::int32_t sdk::screen_width = 0;
std::int32_t sdk::screen_height = 0;

void sdk::update_local_controller()
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		sdk::local_controller = nullptr;
		return;
	}

	sdk::local_controller = interfaces::entity_list->get_local_controller();
}

void sdk::update_local_player()
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		sdk::local_player = nullptr;
		return;
	}

	sdk::local_player = interfaces::entity_list->get_local_player();
}
std::uint8_t* erahaerhaerh(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset)
{
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}
void sdk::update_global_vars()
{
	interfaces::globals = *reinterpret_cast<c_global_vars**>(scanner::resolve_rip(scanner::pattern_scan(L"client.dll", "48 8B 35 ? ? ? ? 44 8B F0"), 3, 7));
}

void sdk::update_screen_size()
{
	interfaces::engine->get_screen_size(sdk::screen_width, sdk::screen_height);
}