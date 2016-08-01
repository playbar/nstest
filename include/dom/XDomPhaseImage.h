// XDomPhaseImage.h: interface for the XDomPhaseImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMPHASEIMAGE_H__7AB4E907_8963_4E17_86B5_655FEB6BCC2B__INCLUDED_)
#define AFX_XDOMPHASEIMAGE_H__7AB4E907_8963_4E17_86B5_655FEB6BCC2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomPhase.h"

class XDomPhaseImage : public XDomPhase  
{
public:
	XBOOL PhaseData(XTCHAR*pData,int nSize,bool bFlash);
	XDomPhaseImage(XDom*pDom);
	virtual ~XDomPhaseImage();

};

#endif // !defined(AFX_XDOMPHASEIMAGE_H__7AB4E907_8963_4E17_86B5_655FEB6BCC2B__INCLUDED_)
