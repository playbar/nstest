// GGFlash.cpp: implementation of the GGFlash class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "GGFlash.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _SYMBIAN
#include "VMPI.h"
#endif
#include "XDibSrv.h"

XBOOL STDCALL XInitGGFlash()
{
#ifdef _SYMBIAN
	VMPI_tlsStart();
#endif
	return XTRUE;
}
void  STDCALL XReleaseGGFlash()
{
#ifdef _SYMBIAN
	VMPI_tlsClose();
#endif
	XDibSrv::ReleaseSrv();
	//RasterGL::Release();
	return;
}
