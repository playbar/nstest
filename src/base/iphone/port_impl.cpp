/*
 *  port_impl.cpp
 *  jqbar
 *
 *  Created by lining on 10-10-14.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GGType.h"
#include <assert.h>
#include <stdio.h>
#define MAX_INIT_WCHAR_STRING 0x1000

static XU16* memList[MAX_INIT_WCHAR_STRING];
static XU16 numAllocedString = 0;

const XU16* _convertWchar_toU16(const wchar_t* str)
{
	assert(str);
	if (!str) return 0;
	assert(numAllocedString < MAX_INIT_WCHAR_STRING);
	XU32 len = 0;
	for (const wchar_t* p = str; *p; ++p)
		len++;
	memList[numAllocedString] = new XU16[len + 1];
	XU16* _ret = memList[numAllocedString];
	_ret[len] = 0;
	for (XU32 i = 0; i < len; ++i)
	{
		_ret[i] = str[i];
	}
	numAllocedString++;

	return _ret;
}

void release_wchar_mem()
{
	for (XU16 i = 0; i < numAllocedString; ++i)
	{
		delete[] memList[numAllocedString];
	}
	numAllocedString = 0;
}