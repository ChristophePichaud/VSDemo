#pragma once

class CFindComboButton : public CMFCToolBarComboBoxButton
{
	DECLARE_SERIAL(CFindComboButton)

// Construction
public:
	CFindComboButton() : CMFCToolBarComboBoxButton(ID_EDIT_FIND_COMBO, GetCmdMgr()->GetCmdImage(ID_EDIT_FIND), CBS_DROPDOWN)
	{
	}

// Attributes:
public:
	static BOOL HasFocus()
	{
		return m_bHasFocus;
	}

protected:
	static BOOL m_bHasFocus;

// Overrides
protected:
	virtual BOOL NotifyCommand(int iNotifyCode);
};

