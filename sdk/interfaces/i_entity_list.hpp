#pragma once
#include <windows.h>
const uintptr_t Client = (uintptr_t)GetModuleHandle(L"client.dll");
#include "../classes/entities.hpp"

#include "../../scanner.hpp"
#include <vector>
#include "../../hooks.hpp"
#include "../../offsets.h"

class i_entity_list
{
public:

	controller_t* GetEntityByIndex(int index)
	{
		
		using fnGetBaseEntity = controller_t*(__fastcall*)(void*, int);
		static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(scanner::pattern_scan(L"client.dll", "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01"));
		return GetBaseEntity(this, index);
		
	}
	player_t* ongod(const CBaseHandle hHandle)
	{
		if (!hHandle.IsValid())
			return nullptr;

		return reinterpret_cast<player_t*>(this->GetEntityByIndex(hHandle.GetEntryIndex()));
	}

	player_t* get_player_from_controller(controller_t* controller)
	{
		/*
		using function_t = player_t*(__fastcall*)(controller_t*);
		static function_t fn = reinterpret_cast<function_t>(scanner::pattern_scan(L"client.dll", GET_PLAYER_FROM_CONTROLLER));

		return fn(controller);
		*/
	}

	player_t* get_local_player()
	{
		using function_t = void* (__fastcall*)(std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(scanner::pattern_scan(L"client.dll", "48 83 EC ? 83 F9 ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? 48 8B 01 FF 90 ? ? ? ? 8B 08 48 63 C1 48 8D 0D ? ? ? ? 48 8B 0C C1"));

		return static_cast<player_t*>(fn(0));
	}

	controller_t* get_local_controller()
	{
		static controller_t* controller = reinterpret_cast<controller_t*>(Client + LocalPlayerPawn);
		return controller;
	}

	entity_t* get_entity_from_handle(CBaseHandle handle)
	{
		using function_t = entity_t*(__fastcall*)(i_entity_list*, CBaseHandle);
		static function_t fn = reinterpret_cast<function_t>(scanner::pattern_scan(L"client.dll", "83 FA ? 77 ? 8B C2 25 ? ? ? ? 44 8B C0 48 C1 E8 ? 48 8B 4C C1"));

		return fn(this, handle);
	}

	std::uint32_t get_handle_from_entity(entity_t* entity)
	{
		/*using function_t = std::uint32_t(__fastcall*)(entity_t*);
		static function_t fn = reinterpret_cast<function_t>(utilities::pattern_scan(L"client.dll", GET_HANDLE_FROM_ENTITY));

		return fn(entity);*/
		return true;
	}
};