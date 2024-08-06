#include "math.hpp"
#include "../../pch.h"

#include "../sdk.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <vector>
#include <imgui.h>

#define deg_to_rad(x)  ((float)(x) * (float)(M_PI / 180.f))
#define rad_to_deg(x)  ((float)(x) * (float)(180.f / M_PI))

static std::vector<std::uint32_t> asdasdpattern_to_byte(const char* pattern)
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

std::uint8_t* ongod_pattern_scan(const wchar_t* module_name, const char* signature)
{
	HMODULE module_handle = GetModuleHandle(module_name);

	if (!module_handle)
	{
		return nullptr;
	}

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	std::vector<std::uint32_t> pattern_bytes = asdasdpattern_to_byte(signature);
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

void math::UpdateViewMatrix(VMatrix* viewMatrix) {
    if (!viewMatrix) return;
    m_ViewMatrix = *viewMatrix;
}


bool math::world_to_screen(vec3_t& in, vec2_t& out)
{
	/*
	using function_t = bool(__fastcall*)(vec3_t&, vec3_t&);
	static function_t function = reinterpret_cast<function_t>(ongod_pattern_scan(L"client.dll", WORLD_TO_SCREEN));

	vec3_t out = { };
	if (function(origin, out))
	{
		return false;
	}
	*/
	if (!ImGui::GetCurrentContext()) return false;

	const float z = m_ViewMatrix[3][0] * in.x + m_ViewMatrix[3][1] * in.y + m_ViewMatrix[3][2] * in.z + m_ViewMatrix[3][3];
	if (z < 0.001f) return false;

	out = vec2_t(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	out.x *= 1.0f + (m_ViewMatrix[0][0] * in.x + m_ViewMatrix[0][1] * in.y + m_ViewMatrix[0][2] * in.z + m_ViewMatrix[0][3]) / z;
	out.y *= 1.0f - (m_ViewMatrix[1][0] * in.x + m_ViewMatrix[1][1] * in.y + m_ViewMatrix[1][2] * in.z + m_ViewMatrix[1][3]) / z;

	return true;
}

vec3_t math::calculate_angle(vec3_t& source, vec3_t& destination, vec3_t& view_angles)
{
	vec3_t delta = source - destination;
	vec3_t angles;

	angles.x = rad_to_deg(std::atanf(delta.z / std::hypotf(delta.x, delta.y))) - view_angles.x;
	angles.y = rad_to_deg(std::atanf(delta.y / delta.x)) - view_angles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
	{
		angles.y += 180.0f;
	}

	return angles;
}

void math::correct_movement(vec3_t old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove)
{
	float delta_view;
	float f1 = 0.f;
	float f2 = 0.f;

	if (old_angles.y < 0.f)
	{
		f1 = 360.0f + old_angles.y;
	}
	else
	{
		f1 = old_angles.y;
	}

	if (cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.y < 0.0f)
	{
		f2 = 360.0f + cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.y;
	}
	else
	{
		f2 = cmd->m_csgoUserCmd.m_pBaseCmd->m_pViewangles->m_angValue.y;
	}

	if (f2 < f1)
	{
		delta_view = abs(f2 - f1);
	}
	else
	{
		delta_view = 360.0f - abs(f1 - f2);
	}

	delta_view = 360.0f - delta_view;

	cmd->m_csgoUserCmd.m_pBaseCmd->m_flForwardMove = std::cos(deg_to_rad(delta_view)) * old_forwardmove + std::cos(deg_to_rad(delta_view + 90.f)) * old_sidemove;
	cmd->m_csgoUserCmd.m_pBaseCmd->m_flSideMove = std::sin(deg_to_rad(delta_view)) * old_forwardmove + std::sin(deg_to_rad(delta_view + 90.f)) * old_sidemove;
}
