#pragma once

#include "../classes/trace.hpp"
#include "../math/vec3_t.hpp"
#include <string>
#include <stdarg.h>
#include <intrin.h>

class i_trace
{
public:
	template <typename T = std::uint8_t>
	[[nodiscard]] T* GetAbsoluteAddress(T* pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
	{
		pRelativeAddress += nPreOffset;
		pRelativeAddress += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(pRelativeAddress);
		pRelativeAddress += nPostOffset;
		return pRelativeAddress;
	}


	__forceinline bool TraceShape(c_ray* pRay, vec3_t vecStart, vec3_t vecEnd, c_trace_filter* pFilter, c_game_trace* pGameTrace)
	{
		using fnTraceShape = bool(__fastcall*)(void*, void*, const vec3_t&, const vec3_t&, void*, void*);
		static auto oTraceShape = reinterpret_cast<fnTraceShape>(GetAbsoluteAddress(scanner::pattern_scan(L"client.dll", "E8 ? ? ? ? 80 7D ? ? 75 ? F3 0F 10 45"), 0x1, 0x0));

#ifdef CS_PARANOID
		CS_ASSERT(oTraceShape != nullptr);
#endif

		return oTraceShape(this, pRay, vecStart, vecEnd, pFilter, pGameTrace);
	}
};