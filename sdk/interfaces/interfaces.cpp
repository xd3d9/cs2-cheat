#include "interfaces.hpp"
#include "../../pch.h"
#define NOMINMAX
#include <Windows.h>
#include <vector>

i_client* interfaces::client = nullptr;
i_csgo_input* interfaces::csgo_input = nullptr;
i_engine_client* interfaces::engine = nullptr;
i_entity_list* interfaces::entity_list = nullptr;
i_input_system* interfaces::input_system = nullptr;
i_renderer* interfaces::renderer = nullptr;
CSchemaSystem* interfaces::schema_system = nullptr;
i_trace* interfaces::trace = nullptr;
c_global_vars* interfaces::globals = nullptr;
IMaterialSystem2* interfaces::material_system = nullptr;
IMemAlloc* interfaces::memalloc = nullptr;

std::uint8_t* hadfhadh(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset)
{
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}

template<typename T>
T* get_interface(const wchar_t* module_name, const char* interface_name)
{
	if (!module_name || !interface_name)
	{
		return nullptr;
	}

	HMODULE module = GetModuleHandle(module_name);

	if (!module)
	{
		return nullptr;
	}

	std::uint8_t* create_interface = reinterpret_cast<std::uint8_t*>(GetProcAddress(module, "CreateInterface"));

	if (!create_interface)
	{
		return nullptr;
	}

	using interface_callback_fn = void*(__cdecl*)();

	typedef struct _interface_reg_t
	{
		interface_callback_fn callback;
		const char* name;
		_interface_reg_t* flink;
	} interface_reg_t;

	interface_reg_t* interface_list = *reinterpret_cast<interface_reg_t**>(hadfhadh(create_interface, 3, 7));

	if (!interface_list)
	{
		return nullptr;
	}

	for (interface_reg_t* it = interface_list; it; it = it->flink)
	{
		if (!strcmp(it->name, interface_name))
		{
			return reinterpret_cast<T*>(it->callback());
		}
	}

	return nullptr;
}


static std::vector<std::uint32_t> pattern_to_byte(const char* pattern)
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

std::uint8_t* pattern_scan(const wchar_t* module_name, const char* signature)
{
	HMODULE module_handle = GetModuleHandle(module_name);

	if (!module_handle)
	{
		return nullptr;
	}

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	std::vector<std::uint32_t> pattern_bytes = pattern_to_byte(signature);
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

std::uint8_t* aaaaaa(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset)
{
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}


int StringCompare(const char* tszLeft, const char* tszRight)
{
	if (tszLeft == nullptr)
		return -1;

	if (tszRight == nullptr)
		return 1;

	using ComparisonType_t = std::conditional_t<std::is_same_v<char, char>, std::uint8_t, std::conditional_t<sizeof(wchar_t) == 2U, std::int16_t, std::int32_t>>;

	ComparisonType_t nLeft, nRight;
	do
	{
		nLeft = static_cast<ComparisonType_t>(*tszLeft++);
		nRight = static_cast<ComparisonType_t>(*tszRight++);

		if (nLeft == char('\0'))
			break;
	} while (nLeft == nRight);

	return nLeft - nRight;
}

int StringCompareWCHAR(const wchar_t* tszLeft, const wchar_t* tszRight)
{
	if (tszLeft == nullptr)
		return -1;

	if (tszRight == nullptr)
		return 1;

	using ComparisonType_t = std::conditional_t<std::is_same_v<wchar_t, char>, std::uint8_t, std::conditional_t<sizeof(wchar_t) == 2U, std::int16_t, std::int32_t>>;

	ComparisonType_t nLeft, nRight;
	do
	{
		nLeft = static_cast<ComparisonType_t>(*tszLeft++);
		nRight = static_cast<ComparisonType_t>(*tszRight++);

		if (nLeft == wchar_t('\0'))
			break;
	} while (nLeft == nRight);

	return nLeft - nRight;
}

void* GetExportAddress(const void* hModuleBase, const char* szProcedureName)
{
	const auto pBaseAddress = static_cast<const std::uint8_t*>(hModuleBase);

	const auto pIDH = static_cast<const IMAGE_DOS_HEADER*>(hModuleBase);
	if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
		return nullptr;

	const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS64*>(pBaseAddress + pIDH->e_lfanew);
	if (pINH->Signature != IMAGE_NT_SIGNATURE)
		return nullptr;

	const IMAGE_OPTIONAL_HEADER64* pIOH = &pINH->OptionalHeader;
	const std::uintptr_t nExportDirectorySize = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	const std::uintptr_t uExportDirectoryAddress = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	if (nExportDirectorySize == 0U || uExportDirectoryAddress == 0U)
	{
		return nullptr;
	}

	const auto pIED = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(pBaseAddress + uExportDirectoryAddress);
	const auto pNamesRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfNames);
	const auto pNameOrdinalsRVA = reinterpret_cast<const std::uint16_t*>(pBaseAddress + pIED->AddressOfNameOrdinals);
	const auto pFunctionsRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfFunctions);

	std::size_t nRight = pIED->NumberOfNames, nLeft = 0U;
	while (nRight != nLeft)
	{
		const std::size_t uMiddle = nLeft + ((nRight - nLeft) >> 1U);
		const int iResult = StringCompare(szProcedureName, reinterpret_cast<const char*>(pBaseAddress + pNamesRVA[uMiddle]));

		if (iResult == 0)
		{
			const std::uint32_t uFunctionRVA = pFunctionsRVA[pNameOrdinalsRVA[uMiddle]];
			if (uFunctionRVA >= uExportDirectoryAddress && uFunctionRVA - uExportDirectoryAddress < nExportDirectorySize)
			{
				break;
			}

			return const_cast<std::uint8_t*>(pBaseAddress) + uFunctionRVA;
		}

		if (iResult > 0)
			nLeft = uMiddle + 1;
		else
			nRight = uMiddle;
	}
	return nullptr;
}
#include "../../pe64.h"
void*GetModuleBaseHandle(const wchar_t* wszModuleName)
{
	const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

	if (wszModuleName == nullptr)
		return pPEB->ImageBaseAddress;

	void* pModuleBase = nullptr;
	for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
	{
		const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (pEntry->FullDllName.Buffer != nullptr && StringCompareWCHAR(wszModuleName, pEntry->BaseDllName.Buffer) == 0)
		{
			pModuleBase = pEntry->DllBase;
			break;
		}
	}
	return pModuleBase;
}

#include <iostream>
bool interfaces::initialize()
{
	interfaces::client = get_interface<i_client>(L"client.dll", "Source2Client002");
	interfaces::csgo_input = *reinterpret_cast<i_csgo_input**>(aaaaaa(pattern_scan(L"client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 FF"), 3, 7));
	interfaces::engine = get_interface<i_engine_client>(L"engine2.dll", "Source2EngineToClient001");
	interfaces::entity_list = *reinterpret_cast<i_entity_list**>(aaaaaa(pattern_scan(L"client.dll", "48 8B 0D ?? ?? ?? ?? 48 89 7C 24 ?? 8B FA C1 EB"), 3, 7));
	interfaces::input_system = get_interface<i_input_system>(L"inputsystem.dll", "InputSystemVersion001");
	//interfaces::renderer = **reinterpret_cast<i_renderer***>(aaaaaa(pattern_scan(L"rendersystemdx11.dll", "66 0F 7F 05 ? ? ? ? 66 0F 7F 0D ? ? ? ? 48 89 35"), 4, 8));
	interfaces::schema_system = get_interface<CSchemaSystem>(L"schemasystem.dll", "SchemaSystem_001");
	interfaces::trace = *reinterpret_cast<i_trace**>(aaaaaa(pattern_scan(L"client.dll", "4C 8B 35 ? ? ? ? 24 C9"), 3, 7));//didnt supply startpos and endpos to Script TraceLine call
	interfaces::globals = *reinterpret_cast<c_global_vars**>(aaaaaa(pattern_scan(L"client.dll", "48 8B 35 ? ? ? ? 44 8B F0"), 3, 7)); //%3.1f  (%s,%d) <-- (%s)\n
	interfaces::material_system = get_interface<IMaterialSystem2>(L"materialsystem2.dll", "VMaterialSystem2_001");
	const auto pTier0Handle = GetModuleBaseHandle(L"tier0.dll");
	interfaces::memalloc = *reinterpret_cast<IMemAlloc**>(GetExportAddress(pTier0Handle, "g_pMemAlloc"));
	interfaces::fnLoadKV3 = *reinterpret_cast<decltype(fnLoadKV3)>(GetExportAddress(pTier0Handle, "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2@Z"));
	/*
	interfaces::fnUtlBufferInit = reinterpret_cast<decltype(fnUtlBufferInit)>(GetExportAddress(pTier0Handle, "??0CUtlBuffer@@QEAA@HHH@Z"));
	interfaces::fnUtlBufferPutString = reinterpret_cast<decltype(fnUtlBufferPutString)>(GetExportAddress(pTier0Handle, "?PutString@CUtlBuffer@@QEAAXPEBD@Z"));
	interfaces::fnUtlBufferEnsureCapacity = reinterpret_cast<decltype(fnUtlBufferEnsureCapacity)>(GetExportAddress(pTier0Handle, "?EnsureCapacity@CUtlBuffer@@QEAAXH@Z"));
	*/
	interfaces::fnCreateMaterial = reinterpret_cast<decltype(fnCreateMaterial)>(pattern_scan(L"materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ?")); //RESOURCE_TYPE_MATERIAL

	if (!interfaces::memalloc || !interfaces::fnLoadKV3 || !interfaces::material_system || !interfaces::csgo_input || !interfaces::engine || !interfaces::entity_list || !interfaces::input_system || !interfaces::schema_system || !interfaces::trace || !interfaces::globals || !interfaces::fnCreateMaterial)
	{
		std::cout << "racxa ver chaitvirta" << std::endl;
		std::cout << "csgo_input" << interfaces::csgo_input << std::endl;
		std::cout << "engine" << interfaces::engine << std::endl;
		std::cout << "entity_list" << interfaces::entity_list << std::endl;
		std::cout << "input_system" << interfaces::input_system << std::endl;
		std::cout << "globals" << interfaces::globals << std::endl;
		std::cout << "renderer" << interfaces::renderer << std::endl; 
		std::cout << "material_system" << interfaces::material_system << std::endl;
		std::cout << "fnLoadKV3 " << interfaces::fnLoadKV3 << std::endl;
		std::cout << "fnCreateMaterial " << interfaces::fnCreateMaterial << std::endl;
		std::cout << "interfaces::memalloc " << interfaces::memalloc << std::endl;
		std::cout << "pTier0Handle " << pTier0Handle << std::endl;
		return false;
	}

	return true;
}