#pragma once
#include <cstddef>
#include <cstdint>
#include "scanner.hpp"
#include <windows.h>
#include <intrin.h>
#define COMBINEss(x, y) x##y
#define COMBINE2ss(x, y) COMBINEss(x, y)

#define MEM_PADss(sz)                               \
private:                                            \
    std::uint8_t COMBINE2ss(pad_, __COUNTER__)[sz];   \
public:

inline int StringComparssssse(const char* tszLeft, const char* tszRight)
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
inline void* GetExportAddresssssssssssss(const void* hModuleBase, const char* szProcedureName)
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
		const int iResult = StringComparssssse(szProcedureName, reinterpret_cast<const char*>(pBaseAddress + pNamesRVA[uMiddle]));

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
inline int StringCompareWCHARsssssssss(const wchar_t* tszLeft, const wchar_t* tszRight)
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

#include "pe64.h"
inline void* GetModuleBaseHandlssssssssse(const wchar_t* wszModuleName)
{
	const _PEB* pPEB = reinterpret_cast<_PEB*>(__readgsqword(0x60));

	if (wszModuleName == nullptr)
		return pPEB->ImageBaseAddress;

	void* pModuleBase = nullptr;
	for (LIST_ENTRY* pListEntry = pPEB->Ldr->InMemoryOrderModuleList.Flink; pListEntry != &pPEB->Ldr->InMemoryOrderModuleList; pListEntry = pListEntry->Flink)
	{
		const _LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(pListEntry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (pEntry->FullDllName.Buffer != nullptr && StringCompareWCHARsssssssss(wszModuleName, pEntry->BaseDllName.Buffer) == 0)
		{
			pModuleBase = pEntry->DllBase;
			break;
		}
	}
	return pModuleBase;
}

const auto pTier0Handle = GetModuleBaseHandlssssssssse(L"tier0.dll");

class CUtlBuffer
{
public:
    MEM_PADss(0x80);

    CUtlBuffer(int a1, int nSize, int a3)
    {
		using FnUtlBufferInit = void(__fastcall*)(CUtlBuffer*, int, int, int);
		static const FnUtlBufferInit fnUtlBufferInit = reinterpret_cast<decltype(fnUtlBufferInit)>(GetExportAddresssssssssssss(pTier0Handle, "??0CUtlBuffer@@QEAA@HHH@Z"));
        fnUtlBufferInit(this, a1, nSize, a3);
    }

    void EnsureCapacity(int nSize)
    {
#ifdef CS_PARANOID
        CS_ASSERT(Functions::fnUtlBufferEnsureCapacity != nullptr);
#endif
		using FnUtlBufferEnsureCapacity = void(__fastcall*)(CUtlBuffer*, int);
		static const FnUtlBufferEnsureCapacity fnUtlBufferEnsureCapacity = reinterpret_cast<decltype(fnUtlBufferEnsureCapacity)>(GetExportAddresssssssssssss(pTier0Handle, "?EnsureCapacity@CUtlBuffer@@QEAAXH@Z"));
        fnUtlBufferEnsureCapacity(this, nSize);
    }
};