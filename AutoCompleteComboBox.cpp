// AutoCompleteComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "WinPing.h"
#include "AutoCompleteComboBox.h"


// CAutoCompleteComboBox

IMPLEMENT_DYNAMIC(CAutoCompleteComboBox, CComboBox)

CAutoCompleteComboBox::CAutoCompleteComboBox()
{
	m_bAutoComplete = TRUE;
}

CAutoCompleteComboBox::~CAutoCompleteComboBox()
{
}


BEGIN_MESSAGE_MAP(CAutoCompleteComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &CAutoCompleteComboBox::OnEditUpdate)
END_MESSAGE_MAP()



// CAutoCompleteComboBox message handlers
BOOL CAutoCompleteComboBox::PreTranslateMessage(MSG *pMsg)
{
	if(WM_KEYDOWN == pMsg->message)
	{
		m_bAutoComplete = TRUE;
		if((VK_DELETE == pMsg->wParam) || (VK_BACK == pMsg->wParam))
		{
			m_bAutoComplete = FALSE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

void CAutoCompleteComboBox::OnEditUpdate()
{
	if(!m_bAutoComplete)
	{
		return ;
	}
	CString strText(_T(""));
	GetWindowText(strText);
	int nLen = strText.GetLength();

	DWORD dwCurSel = GetEditSel();
	WORD wStart = LOWORD(dwCurSel);
	WORD wEnd = HIWORD(dwCurSel);

	if(CB_ERR == SelectString(-1, strText))
	{
		SetWindowText(strText);
		if(CB_ERR != dwCurSel)
		{
			SetEditSel(wStart, wEnd);
		}
	}
	if((wEnd < nLen) && (CB_ERR != dwCurSel))
	{
		SetEditSel(wStart, wEnd);
	}
	else
	{
		SetEditSel(nLen, -1);
	}
}
