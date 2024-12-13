#pragma once
#include <cstdint>
#include <dxgi.h>
#include "i_material_system.hpp"

inline bool bhop = false;

class VMatrix {
public:
	auto operator[](int i) const { return m[i]; }

	float m[4][4];
};
inline VMatrix m_ViewMatrix;
namespace hooks
{
	namespace level_init
	{
		using function_t = std::int64_t(__fastcall*)(std::int64_t, std::int64_t);
		std::int64_t __fastcall hook(std::int64_t a1, std::int64_t a2);
	}
	namespace create_move
	{
		using function_t = bool(__fastcall*)(void*, std::uint32_t, std::uint8_t);
		bool __fastcall hook(void* input, std::uint32_t a2, std::uint8_t a3);

	}
	namespace GetMatricesForView
	{
		using function_t = void(__fastcall*)(void* rcx, void* view, VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection,
			VMatrix* pWorldToPixels);
		void __fastcall hook(void* rcx, void* view, VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection, VMatrix* pWorldToPixels);
	}
	namespace swap_chain_present
	{
		using function_t = HRESULT(__fastcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t);
		HRESULT __fastcall hook(IDXGISwapChain* swap_chain, std::uint32_t sync_interval, std::uint32_t flags);
	}
	namespace draw_object
	{
		using function_t = void(__fastcall*)(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
		static function_t draw_object_original = nullptr;
		void __fastcall hook(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
	}
	bool initialize();
}
