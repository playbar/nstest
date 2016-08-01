#if !defined(AFX_SWFSHOW_H__9AC1FBD9_A151_4CD0_B149_373511FA0057__INCLUDED_)
#define AFX_SWFSHOW_H__9AC1FBD9_A151_4CD0_B149_373511FA0057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SWFShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSWFShow window

class XSWFShow : public CButton
{
public:
	XSWFShow();
	virtual ~XSWFShow();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void ShowObject(SCharacter*ch);
	void ShowObject(CString&strFile);
	static XBOOL LoadDib(XDrawDib&dib,CString&strFile);

public:
	XDrawDib m_dib;
	XSWFPlayer* m_pPlayer;
	SCharacter* m_showObj;
	int miType;
	bool mbFullScreen;

	DECLARE_MESSAGE_MAP()
};


#endif 
