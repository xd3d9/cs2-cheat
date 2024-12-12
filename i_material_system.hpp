#pragma once

// used: cbasehandle
#include "../entity_handle.h"
#include "./scanner.hpp"
#include "sdk/math/color_t.hpp"
#include <cstddef>
#include "virtual.hpp"

class CMaterial2
{
public:
	virtual const char* GetName() = 0;
	virtual const char* GetShareName() = 0;
};
#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define MEM_PAD(sz)                               \
private:                                            \
    std::uint8_t COMBINE2(pad_, __COUNTER__)[sz];   \
public:

class ByteColorRGBA {
public:
	unsigned char r, g, b, a;

	ByteColorRGBA(float _r = 0, float _g = 0, float _b = 0, float _a = 0) {
		r = static_cast<unsigned char>(_r);
		g = static_cast<unsigned char>(_g);
		b = static_cast<unsigned char>(_b);
		a = static_cast<unsigned char>(_a);
	}

	ByteColorRGBA operator+(ByteColorRGBA color) {
		return ByteColorRGBA(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	ByteColorRGBA operator+(unsigned char n) {
		return ByteColorRGBA(r + n, g + n, b + n, a + n);
	}

	ByteColorRGBA operator+=(ByteColorRGBA color) {
		r += color.r;
		g += color.g;
		b += color.b;
		a += color.a;

		return *this;
	}

	ByteColorRGBA operator+=(unsigned char n) {
		r += n;
		g += n;
		b += n;
		a += n;

		return *this;
	}

	ByteColorRGBA operator-(ByteColorRGBA color) {
		return ByteColorRGBA(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	ByteColorRGBA operator-(unsigned char n) {
		return ByteColorRGBA(r - n, g - n, b - n, a - n);
	}

	ByteColorRGBA operator-=(ByteColorRGBA color) {
		r -= color.r;
		g -= color.g;
		b -= color.b;
		a -= color.a;

		return *this;
	}

	ByteColorRGBA operator-=(unsigned char n) {
		r -= n;
		g -= n;
		b -= n;
		a -= n;

		return *this;
	}

	ByteColorRGBA operator/(ByteColorRGBA color) {
		return ByteColorRGBA(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	ByteColorRGBA operator/(unsigned char n) {
		return ByteColorRGBA(r / n, g / n, b / n, a / n);
	}

	ByteColorRGBA operator/=(ByteColorRGBA color) {
		r /= color.r;
		g /= color.g;
		b /= color.b;
		a /= color.a;

		return *this;
	}

	ByteColorRGBA operator/=(unsigned char n) {
		r /= n;
		g /= n;
		b /= n;
		a /= n;

		return *this;
	}

	ByteColorRGBA operator*(ByteColorRGBA color) {
		return ByteColorRGBA(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	ByteColorRGBA operator*(unsigned char n) {
		return ByteColorRGBA(r * n, g * n, b * n, a * n);
	}

	ByteColorRGBA operator*=(ByteColorRGBA color) {
		r *= color.r;
		g *= color.g;
		b *= color.b;
		a *= color.a;

		return *this;
	}

	ByteColorRGBA operator*=(unsigned char n) {
		r *= n;
		g *= n;
		b *= n;
		a *= n;

		return *this;
	}

	bool operator==(ByteColorRGBA color) {
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}

	bool operator!=(ByteColorRGBA color) {
		return !(*this == color);
	}
};

// idk
struct MaterialKeyVar_t
{
	std::uint64_t uKey;
	const char* szName;

	MaterialKeyVar_t(std::uint64_t uKey, const char* szName) :
		uKey(uKey), szName(szName) { }

	MaterialKeyVar_t(const char* szName, bool bShouldFindKey = false) :
		szName(szName)
	{
		uKey = bShouldFindKey ? FindKey(szName) : 0x0;
	}

	std::uint64_t FindKey(const char* szName)
	{
		using fnFindKeyVar = std::uint64_t(__fastcall*)(const char*, unsigned int, int);
		static auto oFindKeyVar = reinterpret_cast<fnFindKeyVar>(scanner::pattern_scan(L"particles.dll", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 33 C0 8B DA"));

#ifdef CS_PARANOID
		CS_ASSERT(oFindKeyVar != nullptr);
#endif
		return oFindKeyVar(szName, 0x12, 0x31415926);
	}
};

class CObjectInfo
{
	MEM_PAD(0xB0);
	int nId;
};

class CSceneAnimatableObject
{
	MEM_PAD(0xB0);
	CBaseHandle hOwner;
};

#define CS_DEBUG_BREAK() __debugbreak()

#define CS_ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (CS_DEBUG_BREAK(), 0))
class CMeshData
{
private:
	char pad_0000[0x18];
public:
	CSceneAnimatableObject* sceneObject;
	CMaterial2* pMaterial;
private:
	char pad_0020[0x18];
public:
	ByteColorRGBA colValue;
	CObjectInfo* pObjectInfo;
};

#include "memory.h"
class IMaterialSystem2
{
public:
	CMaterial2*** FindOrCreateFromResource(CMaterial2*** pOutMaterial, const char* szMaterialName)
	{
		return vt::CallMethod<CMaterial2***>(this, 14U, pOutMaterial, szMaterialName);
	}

	CMaterial2** CreateMaterial(CMaterial2*** pOutMaterial, const char* szMaterialName, CMeshData* pData)
	{
		return vt::CallMethod<CMaterial2**>(this, 29U, pOutMaterial, szMaterialName, pData, 0, 0, 0, 0, 0, 1);
	}

	void SetCreateDataByMaterial(const void* pData, CMaterial2*** const pInMaterial)
	{
		return MEM::CallVFunc<void, 37U>(this, pInMaterial, pData);
	}
};
