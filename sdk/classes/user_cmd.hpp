#pragma once
#undef max
#include "../math/vec3_t.hpp"
#include "../math/vec4_t.hpp"
#include <cstddef>
#include <iostream>
#include <limits>
#include <string_view>
#include <type_traits>
#include <cinttypes>
#include <cstdio>
#include<limits>
#include "../virtual.hpp"
#include "../../utlbuffer.h"
#include "../i_mem_alloc.hpp"

template <typename T>
struct RepeatedPtrField_t
{
	struct Rep_t
	{
		int nAllocatedSize;
		#undef max
		T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* pArena;
	int nCurrentSize;
	int nTotalSize;
	Rep_t* pRep;
};

// @source: master/game/shared/in_buttons.h
enum ECommandButtons : int
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_SECOND_ATTACK = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_LEFT_ALT = (1 << 14),
	IN_RIGHT_ALT = (1 << 15),
	IN_SCORE = (1 << 16),
	IN_SPEED = (1 << 17),
	IN_WALK = (1 << 18),
	IN_ZOOM = (1 << 19),
	IN_FIRST_WEAPON = (1 << 20),
	IN_SECOND_WEAPON = (1 << 21),
	IN_BULLRUSH = (1 << 22),
	IN_FIRST_GRENADE = (1 << 23),
	IN_SECOND_GRENADE = (1 << 24),
	IN_MIDDLE_ATTACK = (1 << 25),
	IN_USE_OR_RELOAD = (1 << 26)
};

enum ESubtickMoveStepBits : std::uint32_t
{
	MOVESTEP_BITS_BUTTON = 0x1U,
	MOVESTEP_BITS_PRESSED = 0x2U,
	MOVESTEP_BITS_WHEN = 0x4U,
	MOVESTEP_BITS_ANALOG_FORWARD_DELTA = 0x8U,
	MOVESTEP_BITS_ANALOG_LEFT_DELTA = 0x10U
};

enum EInputHistoryBits : std::uint32_t
{
	INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
	INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
	INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
	INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
	INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
	INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
	INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
	INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
	INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
	INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
	INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
	INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
	INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
	INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
	INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
};

enum EBaseCmdBits : std::uint32_t
{
	BASE_BITS_MOVE_CRC = 0x1U,
	BASE_BITS_BUTTONPB = 0x2U,
	BASE_BITS_VIEWANGLES = 0x4U,
	BASE_BITS_COMMAND_NUMBER = 0x8U,
	BASE_BITS_CLIENT_TICK = 0x10U,
	BASE_BITS_FORWARDMOVE = 0x20U,
	BASE_BITS_LEFTMOVE = 0x40U,
	BASE_BITS_UPMOVE = 0x80U,
	BASE_BITS_IMPULSE = 0x100U,
	BASE_BITS_WEAPON_SELECT = 0x200U,
	BASE_BITS_RANDOM_SEED = 0x400U,
	BASE_BITS_MOUSEDX = 0x800U,
	BASE_BITS_MOUSEDY = 0x1000U,
	BASE_BITS_CONSUMED_SERVER_ANGLE = 0x2000U,
	BASE_BITS_CMD_FLAGS = 0x4000U,
	BASE_BITS_ENTITY_HANDLE = 0x8000U
};

enum ECSGOUserCmdBits : std::uint32_t
{
	CSGOUSERCMD_BITS_BASECMD = 0x1U,
	CSGOUSERCMD_BITS_LEFTHAND = 0x2U,
	CSGOUSERCMD_BITS_ATTACK3START = 0x4U,
	CSGOUSERCMD_BITS_ATTACK1START = 0x8U,
	CSGOUSERCMD_BITS_ATTACK2START = 0x10U
};

enum EButtonStatePBBits : uint32_t
{
	BUTTON_STATE_PB_BITS_BUTTONSTATE1 = 0x1U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE2 = 0x2U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE3 = 0x4U
};

#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)

#define MEM_PAD(sz)                               \
private:                                            \
    std::uint8_t COMBINE2(pad_, __COUNTER__)[sz];   \
public:

class CBasePB
{
	MEM_PAD(0x8) // 0x0 VTABLE
	std::uint32_t nHasBits; // 0x8
	std::uint64_t nCachedBits; // 0xC

	// @note: this function is used to check if the bits are set and set them if they are not
	void CheckAndSetBits(std::uint64_t nBits)
	{
		if (!(nCachedBits & nBits))
			nCachedBits |= nBits;
	}
};
static_assert(sizeof(CBasePB) == 0x18);

class CMsgQAngle : public CBasePB
{
public:
	vec3_t m_angValue; // 0x18
};

class CMsgVector : public CBasePB
{
public:
	vec4_t m_vecValue; // 0x18
};

class CCSGOInterpolationInfoPB : public CBasePB
{
public:
	float m_flFraction; // 0x18
	int m_nSrcTick; // 0x1C
	int m_nDstTick; // 0x20
};

class CCSGOInputHistoryEntryPB : public CBasePB
{
public:
	CMsgQAngle* m_pViewCmd; // 0x18
	CMsgVector* m_pShootOriginCmd; // 0x20
	CMsgVector* m_pTargetHeadOriginCmd; // 0x28
	CMsgVector* m_pTargetAbsOriginCmd; // 0x30
	CMsgQAngle* m_pTargetViewCmd; // 0x38
	CCSGOInterpolationInfoPB* m_cl_interp; // 0x40
	CCSGOInterpolationInfoPB* m_sv_interp0; // 0x48
	CCSGOInterpolationInfoPB* m_sv_interp1; // 0x50
	CCSGOInterpolationInfoPB* m_player_interp; // 0x58
	int m_nRenderTickCount; // 0x60
	float m_flRenderTickFraction; // 0x64
	int m_nPlayerTickCount; // 0x68
	float m_flPlayerTickFraction; // 0x6C
	int m_nFrameNumber; // 0x70
	int m_nTargetEntIndex; // 0x74
};

struct CInButtonStatePB : CBasePB
{
	uint64_t m_nValue;
	uint64_t m_nValueChanged;
	uint64_t m_nValueScroll;
};
static_assert(sizeof(CInButtonStatePB) == 0x30);

struct CSubtickMoveStep : CBasePB
{
public:
	std::uint64_t nButton;
	bool bPressed;
	float flWhen;
	float flAnalogForwardDelta;
	float flAnalogLeftDelta;
};
static_assert(sizeof(CSubtickMoveStep) == 0x30);

#include "../pe64.h"
inline void* GetModuleBaseHandlsssssss111sse(const wchar_t* wszModuleName)
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

class CBaseUserCmdPB : public CBasePB
{
public:
	RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField;
	std::string* strMoveCrc;
	CInButtonStatePB* pInButtonState;
	CMsgQAngle* m_pViewangles;
	std::int32_t nCommandNumber;
	std::int32_t nClientTick;
	float m_flForwardMove;
	float m_flSideMove;
	float flUpMove;
	std::int32_t nImpulse;
	std::int32_t nWeaponSelect;
	std::int32_t nRandomSeed;
	std::int32_t nMousedX;
	std::int32_t nMousedY;
	std::uint32_t nConsumedServerAngleChanges;
	std::int32_t nCmdFlags;
	std::uint32_t nPawnEntityHandle;
	/**/
	bool CalculateCRC()
	{
		int nCalcualtedCRCSize = this->CalculateCmdCRCSize();
		CUtlBuffer protobufBuffer = CUtlBuffer::CUtlBuffer(0, 0, 0);
		protobufBuffer.EnsureCapacity(nCalcualtedCRCSize + 1);
		using fnSerializePartialToArray = bool(__fastcall*)(CBaseUserCmdPB*, CUtlBuffer, int);
		static const fnSerializePartialToArray SerializePartialToArray = reinterpret_cast<fnSerializePartialToArray>(scanner::pattern_scan(L"client.dll", "48 89 5C 24 18 55 56 57 48 81 EC 90 00 00 00 48"));

		const auto pTier0Handle = GetModuleBaseHandlsssssss111sse(L"tier0.dll");
		IMemAlloc* alloc = *reinterpret_cast<IMemAlloc**>(GetExportAddresssssssssssss(pTier0Handle, "g_pMemAlloc"));

		if (SerializePartialToArray(this, protobufBuffer, nCalcualtedCRCSize))
		{
			std::uintptr_t* pMessage = reinterpret_cast<uintptr_t*>(alloc->Alloc(0x18));
			this->nCachedBits |= 1;
			auto nHasBits = static_cast<uint32_t>(this->nHasBits & 0xFFFFFFFFFFFFFFFC);
			if ((this->nHasBits & 1) != 0)
				nHasBits = static_cast<uint32_t>(nHasBits);
			using fnWriteMessage = void(__fastcall*)(std::uintptr_t*, CUtlBuffer, int);
			static const fnWriteMessage WriteMessage = reinterpret_cast<fnWriteMessage>(scanner::pattern_scan(L"client.dll", "48 89 5C 24 10 48 89 6C 24 18 48 89 7C 24 20 41 56 48 83 EC 20 48 BF"));

			using fnSetMessageData = std::string* (__fastcall*)(void*, std::uintptr_t*, void*);
			static const fnSetMessageData SetMessageData = reinterpret_cast<fnSetMessageData>(scanner::pattern_scan(L"client.dll", "48 89 5C 24 20 55 56 57 48 83 EC 30 49"));

			WriteMessage(pMessage, protobufBuffer, nCalcualtedCRCSize);
			this->strMoveCrc = SetMessageData(&this->strMoveCrc, pMessage, &nHasBits);
			alloc->Free(pMessage);

			return true;
		}

		return false;
	}

	int CalculateCmdCRCSize()
	{
		return vt::CallMethod<int>(this, 7U);
		//return MEM::CallVFunc<int, 7U>(this);
	}
};
static_assert(sizeof(CBaseUserCmdPB) == 0x80);

class CCSGOUserCmdPB
{
public:
	std::uint32_t m_nHasBits;
	std::uint64_t m_nCachedSize;
	RepeatedPtrField_t<CCSGOInputHistoryEntryPB> m_inputHistoryField;
	CBaseUserCmdPB* m_pBaseCmd;
	int32_t m_nAttack3StartHhistoryIndex;
	int32_t m_nAttack1StartHhistoryIndex;
	int32_t m_nAttack2StartHhistoryIndex;

	void CheckAndSetBits(std::uint32_t nBits)
	{
		if (!(m_nHasBits & nBits))
			m_nHasBits |= nBits;
	}
};



struct CInButtonState
{
	MEM_PAD(0x8) // 0x0 VTABLE
	std::uint64_t m_nValue; // 0x8
	std::uint64_t nValueChanged; // 0x10
	std::uint64_t nValueScroll; // 0x18
};
static_assert(sizeof(CInButtonStatePB) == 0x30);



class CUserCmd
{
public:
	MEM_PAD(0x8) // 0x0 VTABLE
	CCSGOUserCmdPB m_csgoUserCmd;
	CInButtonState m_nButtons;
	MEM_PAD(0x20);

	CCSGOInputHistoryEntryPB* GetInputHistoryEntry(int nIndex)
	{
		if (nIndex >= m_csgoUserCmd.m_inputHistoryField.pRep->nAllocatedSize || nIndex >= m_csgoUserCmd.m_inputHistoryField.nCurrentSize)
			return nullptr;

		return m_csgoUserCmd.m_inputHistoryField.pRep->tElements[nIndex];
	}

	void SetSubTickAngle(const vec3_t& angView)
	{
		for (int i = 0; i < this->m_csgoUserCmd.m_inputHistoryField.pRep->nAllocatedSize; i++)
		{
			CCSGOInputHistoryEntryPB* pInputEntry = this->GetInputHistoryEntry(i);
			if (!pInputEntry || !pInputEntry->m_pViewCmd)
				continue;

			pInputEntry->m_pViewCmd->m_angValue = angView;
			pInputEntry->CheckAndSetBits(EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES);
		}
	}
};
static_assert(sizeof(CUserCmd) == 0x88);