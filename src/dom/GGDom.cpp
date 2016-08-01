// GGDom.cpp: implementation of the GGDom class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "GGDom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


XBOOL STDCALL XInitGGDom()
{
	XInitGGBase();
	/*for(int i=0;i<XMAX_ICONS;i++)
	{
		if(icons[i]==XNULL) break;
		XMainWnd::GetIcon(i)->CreateFromResource(strRes,icons[i]);
	}*/
	XInitGGFlash();
	return XTRUE;
}

XBOOL STDCALL XReleaseGGDom()
{
	/*for(int i=0;i<XMAX_ICONS;i++)
		XMainWnd::GetIcon(i)->Release();*/
	XReleaseGGFlash();
	XReleaseGGBase();
	return XTRUE;
}
