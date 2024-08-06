#pragma once
#include "memory.h"
#include "../../virtual.hpp"
class i_engine_client
{
public:
	bool is_in_game()
	{
		return vt::CallMethod<bool>(this, 35U);
	}

	bool is_connected()
	{
		return vt::CallMethod<bool>(this, 36U);
	}

	void get_screen_size(std::int32_t& width, std::int32_t& height)
	{
		using function_t = void(__thiscall*)(i_engine_client*, int&, int&);
		return (*reinterpret_cast<function_t**>(this))[48](this, width, height);
	}

	const char* get_level_name()
	{
		using function_t = const char*(__thiscall*)(i_engine_client*);
		return (*reinterpret_cast<function_t**>(this))[51](this);
	}

	const char* get_level_name_short()
	{
		using function_t = const char* (__thiscall*)(i_engine_client*);
		return (*reinterpret_cast<function_t**>(this))[52](this);
	}
};