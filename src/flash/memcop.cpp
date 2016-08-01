#include "StdAfxflash.h"
#include "flashmemcop.h"

//--------------------------------------------------------------------------------
// defines/constants
#ifdef _MAC

const Size kTempMemSafetyMargin = 0x00080000; 	// 512 K
const Size kAppMemSafetyMargin 	= 50000; 		// between 48K and 49K

#endif													

//--------------------------------------------------------------------------------
// globals
#ifdef _MAC
BOOL allocFailed = false;
BOOL allocFailedOnce = false;

BOOL TempMemSeemsLow(Size size)
{
	BOOL seemsLow = TempFreeMem() < kTempMemSafetyMargin + size;

	if (seemsLow)
	{
		// Force a heap compaction, and look again.
		Size ignored = 0;
		TempMaxMem(&ignored);
		
		seemsLow = TempFreeMem() < kTempMemSafetyMargin + size;
	}

	return seemsLow;
}

BOOL AppMemSeemsLow(Size size)
{
	return  ( FreeMem() < kAppMemSafetyMargin + size ) ;
}

#endif                     

