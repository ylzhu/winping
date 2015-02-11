#pragma once


// CAutoCompleteComboBox

class CAutoCompleteComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CAutoCompleteComboBox)

public:
	CAutoCompleteComboBox();
	virtual ~CAutoCompleteComboBox();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
	BOOL m_bAutoComplete;

protected:
	afx_msg void OnEditUpdate();

	DECLARE_MESSAGE_MAP()
};


