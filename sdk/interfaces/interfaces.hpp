#pragma once

#include "i_client.hpp"
#include "i_csgo_input.hpp"
#include "i_engine_client.hpp"
#include "i_entity_list.hpp"
#include "i_input_system.hpp"
#include "i_renderer.hpp"
#include "i_schema_system.hpp"
#include "i_trace.hpp"
#include "../classes/global_vars.hpp"
#include "../../i_mem_alloc.hpp"

class CKeyValues3
{
public:
	std::uint64_t uKey;
	void* pValue;

	std::byte pad[0x8];
};

struct KV3ID_t
{
	const char* szName;
	std::uint64_t unk0;
	std::uint64_t unk1;
};

namespace interfaces
{
	extern i_client* client;
	extern i_csgo_input* csgo_input;
	extern i_engine_client* engine;
	extern i_entity_list* entity_list;
	extern i_input_system* input_system;
	extern i_renderer* renderer;
	extern CSchemaSystem* schema_system;
	extern i_trace* trace;
	extern c_global_vars* globals;
	extern IMaterialSystem2* material_system;
	inline bool(__fastcall* fnLoadKV3)(CKeyValues3*, void*, const char*, const KV3ID_t*, const char*);
	inline std::int64_t(__fastcall* fnCreateMaterial)(void*, void*, const char*, void*, unsigned int, unsigned int);
	extern IMemAlloc* memalloc;
	bool initialize();
}