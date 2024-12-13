#include "hooks.hpp"
#include "pch.h"

#include "CRC.h"

//sdk
#include "sdk/classes/user_cmd.hpp"
#include "sdk/interfaces/interfaces.hpp"

//math
#include "sdk/math/math.hpp"
#include "sdk/sdk.hpp"
#include <iostream>
#include <array>

//bibliotekebi
#include "minhook/MinHook.h"
#include <vector>
#include "aimbot.hpp"
#include "load_materials.hpp"
#include "chams.hpp"

//features


static void* get_virtual(void* class_pointer, std::uint32_t index)
{
	void** vtable = *static_cast<void***>(class_pointer);
	return vtable[index];
}

static hooks::create_move::function_t create_move_original = nullptr;
static hooks::GetMatricesForView::function_t get_matrices_for_view_original = nullptr;
static hooks::swap_chain_present::function_t swap_chain_present_original = nullptr;
static hooks::level_init::function_t level_init_original = nullptr;


static std::vector<std::uint32_t> hadfhdafhpattern_to_byte(const char* pattern)
{
	std::vector<std::uint32_t> bytes;
	char* start = const_cast<char*>(pattern);
	char* end = const_cast<char*>(pattern) + std::strlen(pattern);

	for (char* current = start; current < end; current++)
	{
		if (*current == '?')
		{
			current++;

			if (*current == '?')
			{
				current++;
			}

			bytes.push_back(-1);
		}
		else
		{
			bytes.push_back(std::strtoul(current, &current, 16));
		}
	}

	return bytes;
}

std::uint8_t* fasf_pattern_scan(const wchar_t* module_name, const char* signature)
{
	HMODULE module_handle = GetModuleHandle(module_name);

	if (!module_handle)
	{
		return nullptr;
	}

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	std::vector<std::uint32_t> pattern_bytes = hadfhdafhpattern_to_byte(signature);
	std::uint8_t* image_base = reinterpret_cast<std::uint8_t*>(module_handle);

	std::size_t pattern_size = pattern_bytes.size();
	std::uint32_t* array_of_bytes = pattern_bytes.data();

	for (std::size_t i = 0; i < size_of_image - pattern_size; i++)
	{
		bool found = true;

		for (std::size_t j = 0; j < pattern_size; j++)
		{
			if (image_base[i + j] != array_of_bytes[j] && array_of_bytes[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return &image_base[i];
		}
	}

	return nullptr;
}

bool hooks::initialize()
{
	void* level_init_target = fasf_pattern_scan(L"client.dll", "48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2");
	void* create_move_target = get_virtual(interfaces::csgo_input, csgo_input_vtable::CREATEMOVE);
	void* get_matrices_for_view_target = fasf_pattern_scan(L"client.dll", "40 53 48 81 EC ? ? ? ? 49 8B C1");
	void* draw_object = fasf_pattern_scan(L"scenesystem.dll", "48 8B C4 48 89 50 ? 53");



	if (MH_Initialize() != MH_OK)
	{
		std::cout << "minhook ver gaiara inicilazia :D" << std::endl;
		return false;
	}

	if (MH_CreateHook(level_init_target, &hooks::level_init::hook, reinterpret_cast<void**>(&level_init_original)) != MH_OK)
	{
		std::cout << "karoche ver movhuket raa leveli" << std::endl;
		return false;
	}

	if (MH_CreateHook(create_move_target, &hooks::create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
	{
		std::cout << "karoche ver movhuket raa createmove" << std::endl;
		return false;
	}
	if (MH_CreateHook(get_matrices_for_view_target, &hooks::GetMatricesForView::hook, reinterpret_cast<void**>(&get_matrices_for_view_original)) != MH_OK)
	{
		std::cout << "karoche ver movhuket raa get_matrices_for_view" << std::endl;
		return false;
	}

	if (MH_CreateHook(draw_object, &hooks::draw_object::hook, reinterpret_cast<void**>(&hooks::draw_object::draw_object_original)) != MH_OK)
	{
		std::cout << "karoche ver movhuket raa draw_object" << std::endl;
		return false;
	}
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		std::cout << "hookebi chaflavda vwv" << std::endl;
		return false;
	}
	return true;
}


void __fastcall hooks::draw_object::hook(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2)
{

	if (!arrMeshDraw || !arrMeshDraw->m_pMaterial || !arrMeshDraw->m_pObjectInfo)
		return draw_object_original(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
	
	switch (const auto id =
		*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(arrMeshDraw->m_pObjectInfo) + 176u); // aq agdebs errors radganac pObjectInfo-a gasasworebeli :d shemdeg commitshi davfixav
	id) {
	case 113:
	case 104: {
		if (!chams::chartuli) break;
		arrMeshDraw->m_pMaterial = materials::invisible_material;
		arrMeshDraw->m_Color = color_t(int(chams::kedlebis_color[0] * 255), int(chams::kedlebis_color[1] * 255), int(chams::kedlebis_color[2] * 255), 255);

		draw_object_original(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk,
			pUnk2);

		arrMeshDraw->m_pMaterial = materials::visible_material;
		arrMeshDraw->m_Color = color_t(int(chams::visible_color[0] * 255), int(chams::visible_color[1] * 255), int(chams::visible_color[2] * 255), 255);
	} break;
	default:
		break;
	}
	return draw_object_original(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk,
		pUnk2);
}

std::int64_t __fastcall hooks::level_init::hook(std::int64_t a1, std::int64_t a2)
{
	sdk::update_global_vars();
	return level_init_original(a1, a2);
}


void __fastcall hooks::GetMatricesForView::hook(void* rcx, void* view, VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection,
	VMatrix* pWorldToPixels)
{
	get_matrices_for_view_original(rcx, view, pWorldToView, pViewToProjection, pWorldToProjection, pWorldToPixels);
	math::UpdateViewMatrix(pWorldToProjection);
}

#include <cmath>
#include <algorithm>

bool __fastcall hooks::create_move::hook(void* a1, std::uint32_t a2, std::uint8_t a3)
{
	create_move_original(a1, a2, a3);

	//sdk::update_local_controller();
	sdk::update_local_player();
	//esp::exec();

	CUserCmd* cmd = interfaces::csgo_input->GetUserCmd();

	if (!cmd)
	{
		std::cout << "no cmd" << std::endl;
		return create_move_original(a1, a2, a3);
	}

	//amjamad areulia :d
	CBaseUserCmdPB* pBaseCmd = cmd->m_csgoUserCmd.m_pBaseCmd;
	if (pBaseCmd == nullptr)
		return create_move_original(a1, a2, a3);

	aimbot::exec(cmd);
	//std::cout << sdk::local_player->flags() << " " << fl_onground << std::endl;
	if (bhop) {
		if (sdk::local_player->flags() & fl_onground)
		{

			pBaseCmd->pInButtonState->CheckAndSetBits(EButtonStatePBBits::BUTTON_STATE_PB_BITS_BUTTONSTATE1);
			pBaseCmd->pInButtonState->m_nValue &= ~IN_JUMP;
		}
	}
	/*
	* dasafixia uechveli funqcionaladac da isec
	CRCInformation::Save(pBaseCmd);
	if (pBaseCmd->CalculateCRC())
		CRCInformation::Apply(cmd);
		*/

	return false;
}