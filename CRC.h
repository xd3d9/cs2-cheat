#include "sdk/classes/user_cmd.hpp"

struct CInButtonStateNoVTable
{
public:
	std::uint64_t m_nValue;
	std::uint64_t m_nValueChanged;
	std::uint64_t m_nValueScroll;
};

namespace CRCInformation
{
	struct SavedData_t
	{
		CInButtonStateNoVTable m_nButtons;
		CMsgQAngle m_angView;
	};

	inline SavedData_t m_SavedData;
	inline CInButtonStateNoVTable m_nButtons;

	inline void GetOriginalButtons(CInButtonState nButtons)
	{
		m_nButtons.m_nValue = nButtons.m_nValue;
		m_nButtons.m_nValueChanged = nButtons.nValueChanged;
		m_nButtons.m_nValueScroll = nButtons.nValueScroll;
	}

	inline void Save(CBaseUserCmdPB* pBaseCmd)
	{
		CMsgQAngle* viewAngles = pBaseCmd->m_pViewangles;
		if (viewAngles != nullptr)
			m_SavedData.m_angView.m_angValue = viewAngles->m_angValue;


		m_SavedData.m_nButtons.m_nValue = pBaseCmd->pInButtonState->m_nValue;
		m_SavedData.m_nButtons.m_nValueChanged = pBaseCmd->pInButtonState->m_nValueChanged;
		m_SavedData.m_nButtons.m_nValueScroll = pBaseCmd->pInButtonState->m_nValueScroll;

		m_nButtons.~CInButtonStateNoVTable();
	}

	inline void Apply(CUserCmd* pCmd)
	{
		CBaseUserCmdPB* pBaseCmd = pCmd->m_csgoUserCmd.m_pBaseCmd;

		pCmd->m_nButtons.m_nValue = m_SavedData.m_nButtons.m_nValue;
		pCmd->m_nButtons.nValueChanged = m_SavedData.m_nButtons.m_nValueChanged;
		pCmd->m_nButtons.nValueScroll = m_SavedData.m_nButtons.m_nValueScroll;

		if (pBaseCmd->m_pViewangles != nullptr)
			pBaseCmd->m_pViewangles->m_angValue = m_SavedData.m_angView.m_angValue;
	}
};