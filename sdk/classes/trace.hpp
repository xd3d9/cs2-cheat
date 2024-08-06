#pragma once

#include <cstdint>

#include "../math/vec3_t.hpp"
#include "entities.hpp"

class c_ray
{
public:
	vec3_t start;
	vec3_t end;
	vec3_t mins;
	vec3_t maxs;
	std::uint32_t N000002C7;
	std::uint8_t N000002BE;
};
static_assert(sizeof(c_ray) == 0x38);

class c_trace_filter
{
public:
	std::uint64_t trace_mask;
	std::uint64_t null_it1;
	std::uint64_t null_it2;
	std::uint32_t skip_handle1;
	std::uint32_t skip_handle2;
	std::uint32_t skip_handle3;
	std::uint32_t skip_handle4;
	std::uint16_t collision1;
	std::uint16_t collision2;
	std::uint16_t N0000011C;
	std::uint8_t layer;
	std::uint8_t N00000104;
	std::uint8_t null_it3;

	virtual ~c_trace_filter()
	{

	}

	virtual bool function()
	{
		return true;
	}

	c_trace_filter(std::uint64_t trace_mask, player_t* local_player, std::uint8_t layer);
};
static_assert(sizeof(c_trace_filter) == 0x40);

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define trace_MEM_PAD(sz)                               \
private:                                            \
    std::uint8_t COMBINE2(pad_, __COUNTER__)[sz];   \
public:

class c_game_trace
{
public:
	void* m_pSurface;
	entity_t* m_pHitEntity;
	void* m_pHitboxData;
	trace_MEM_PAD(0x38);
	std::uint32_t m_uContents;
	trace_MEM_PAD(0x24);
	vec3_t m_vecStartPos;
	vec3_t m_vecEndPos;
	vec3_t m_vecNormal;
	vec3_t m_vecPosition;
	trace_MEM_PAD(0x4);
	float m_flFraction;
	trace_MEM_PAD(0x6);
	bool m_bAllSolid;
	trace_MEM_PAD(0x4D)
};