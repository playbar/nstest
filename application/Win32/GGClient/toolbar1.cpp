// ToolBar1.cpp : implementation file
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "afxpriv.h"
#include "Toolbar1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStandardBar

CStandardBar::CStandardBar() : m_pTBButtons(NULL)
{

}

CStandardBar::~CStandardBar()
{
	if (m_pTBButtons)
		delete []m_pTBButtons;

}


BEGIN_MESSAGE_MAP(CStandardBar, CToolBarCtrl)
	//{{AFX_MSG_MAP(CStandardBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CStandardBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	BOOL bRet = CToolBarCtrl::Create(dwStyle, rect, pParentWnd, nID);

	m_nButtonCount = ID_CONTEXTHELP - ID_NEW + 1;

	VERIFY(AddBitmap(m_nButtonCount,IDR_STANDARDBAR) != -1);

	m_pTBButtons = new TBBUTTON[m_nButtonCount];

	for (int nIndex = 0; nIndex < m_nButtonCount; nIndex++)
	{
		CString string;
		string.LoadString(nIndex + ID_NEW);

		// Add second '\0'
		int nStringLength = string.GetLength() + 1;
		TCHAR * pString = string.GetBufferSetLength(nStringLength);
		pString[nStringLength] = 0;

		VERIFY((m_pTBButtons[nIndex].iString = AddStrings(pString)) != -1);

		string.ReleaseBuffer();


		m_pTBButtons[nIndex].fsState = TBSTATE_ENABLED;
		m_pTBButtons[nIndex].fsStyle = TBSTYLE_BUTTON;
		m_pTBButtons[nIndex].dwData = 0;
		m_pTBButtons[nIndex].iBitmap = nIndex;
		m_pTBButtons[nIndex].idCommand = nIndex + ID_NEW;

	}

	TBBUTTON sepButton;
	sepButton.idCommand = 0;
	sepButton.fsStyle = TBSTYLE_SEP;
	sepButton.fsState = TBSTATE_ENABLED;
	sepButton.iString = 0;
	sepButton.iBitmap = 0;
	sepButton.dwData = 0;

	for (nIndex = 0; nIndex < m_nButtonCount; nIndex++)
	{
		VERIFY(AddButtons(1,&m_pTBButtons[nIndex]));
		if (!((nIndex +1) % 3))
		{
			VERIFY(AddButtons(1,&sepButton));
		}
	}


	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
// CStandardBar message handlers

// MFC routes the notifications sent to the parent of the control to
// the control before the parent can process the notification.
// The control object can handle the notification or ignore it.
// If the notification is handled then return TRUE. Otherwise MFC
// will route it to the parent of the control.

BOOL CStandardBar::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	if (message == WM_NOTIFY)
	{
		NMHDR *pNMHDR = (NMHDR *) lParam;
		switch (pNMHDR->code)
		{
		case TBN_BEGINADJUST :
			return BeginAdjust(wParam,  lParam,  pLResult);

		case TBN_BEGINDRAG:
			return BeginDrag(wParam,  lParam,  pLResult);

		case TBN_CUSTHELP:
			return CustomizeHelp(wParam,  lParam,  pLResult);

		case TBN_ENDADJUST:
			return EndAdjust(wParam,  lParam,  pLResult);

		case TBN_ENDDRAG:
			return EndDrag(wParam,  lParam,  pLResult);

		case TBN_GETBUTTONINFO:
			return GetButtonInfo(wParam,  lParam,  pLResult);

		case TBN_QUERYDELETE:
			return QueryDelete(wParam,  lParam,  pLResult);

		case TBN_QUERYINSERT:
			return QueryInsert(wParam,  lParam,  pLResult);

		case TBN_RESET:
			return Reset(wParam, lParam,  pLResult);

		case TBN_TOOLBARCHANGE:
			return ToolBarChange(wParam, lParam, pLResult);
		}
	}

	return CToolBarCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CStandardBar::BeginAdjust(WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	TRACE(_T("TBN_BEGINADJUST\n"));

	// the customize dialog box is about to be displayed

	// save toolbar state before customization using the dialog
	// Use this information to restore the state if reset button is pressed
	SaveState(HKEY_CURRENT_USER,_T("Software\\Microsoft\\VC40\\Samples\\CtrlDemo"),_T("Standard Tool Bar"));

	return TRUE;
}

BOOL CStandardBar::BeginDrag(WPARAM wParam, LPARAM  lParam, LRESULT* pLResult)
{
	TRACE(_T("TBN_BEGINDRAG\n"));

	// we are not implementing custon drag and drop
	* pLResult = FALSE;
	return FALSE;
}



BOOL CStandardBar::EndAdjust(WPARAM wParam, LPARAM  lParam, LRESULT* pLResult)
{
	TRACE(_T("TBN_ENDADJUST\n"));

	// the customize dialog box has been closed

	return TRUE;
}

BOOL CStandardBar::EndDrag(WPARAM wParam, LPARAM  lParam, LRESULT* pLResult)
{
	TRACE(_T("TBN_ENDDRAG\n"));

	// Code to handle custom drag and drop. This message indicates that
	// the item is being dropped
	* pLResult = FALSE;
	return TRUE;
}

BOOL CStandardBar::GetButtonInfo(WPARAM wParam, LPARAM  lParam, LRESULT* pLResult)
{
	// This notification message has to be handled correctly if
	// all operations in the custom dialogbox has to function correctly
	// We have to supply information for the button specified by pTBN->tbButton
	//
	// This notification is sent in the following cases
	//
	// After TBN_BEGINADJUST the control sends these notifications until
	// * pLResult is TRUE. We have to supply valid values when this value is
	// set to TRUE. Here the control is collecting information for all
	// the buttons that have to be displayed in the dialog box
	//
	// The control sends this notification to get information about
	// a button if the user is trying to add it to the toolbar or
	// rearranging the buttons on the toolbar from within the dialog

	TRACE(_T("TBN_GETBUTTONINFO\n"));

	TBNOTIFY *pTBN = (TBNOTIFY *) lParam;

	if (pTBN->iItem >= m_nButtonCount)
	{
		* pLResult = FALSE;
	}
	else
	{
		CString buffer;
		buffer.LoadString(pTBN->iItem + ID_NEW);

		// set the string for the button
		// truncate the string if its length is greater than the buffer
		// supplied by the toolbar
		_tcsncpy(pTBN->pszText, buffer, pTBN->cchText - 1);
		pTBN->pszText[pTBN->cchText - 1] = '\0';

		// set the button info
		pTBN->tbButton = m_pTBButtons[pTBN->iItem];

		// valid values are structure
		*pLResult = TRUE;
	}

	return TRUE;
}

// Helper function for tooltips



///////////////////////////////////////////////////////////////////////
// This has been overridden so we can handle the tooltip TTN_NEEDTEXT//
// notification message                                              //
///////////////////////////////////////////////////////////////////////


