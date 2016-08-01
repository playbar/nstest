#include "StdAfxGGBase.h"
#include "GGBase.h"
#include "GGType.h"
#include "XGraphics.h"
#include "XWindow.h"
#include "XString.h"
#include "XResource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static XPCTSTR _strIcons[XMAX_ICONS]={("LOADING"),("FAILED"),("CONNECT"),
("HOME"),("ANI"),("MAINICON"),"GREENBLOCK","BLUEBLOCK","GRAYBLOCK","BUTTON", "PAGELOADING" };

iOSDeviceRelateData g_deviceData;

XBOOL XInitGGBase()
{
	XGraphics g(1, 1);
	XSize sz;
	
	g.MeasureString(XResource::LoadWString(XIDS_HELLO), sz, 0);
	XWindow::m_nTextHeight = (XU8)sz.cy;
	for(int i=0;i<XMAX_ICONS;i++)
	{
		if(_strIcons[i]==XNULL) break;
		XWindow::GetIcon(i)->CreateFromResource(_strIcons[i]);
	}
	
	return XTRUE;
}

void  _stdcall XReleaseGGBase()
{
	for(int i=0;i<XMAX_ICONS;i++)
		XWindow::GetIcon(i)->Release();
}


