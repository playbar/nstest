/*
 *  string_comp.cpp
 *  jqbar
 *
 *  Created by lining on 10-10-15.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "string_comp.h"
#include <assert.h>
/*
char *strupr(char *string)
{
	char *s;
	
	if (string)
	{
		for (s = string; *s; ++s)
			*s = toupper(*s);
	}
	return string;
} 

char *strlwr(char *string)
{
	char *s;
	
	if (string)
	{
		for (s = string; *s; ++s)
			*s = tolower(*s);
	}
	return string;
}

unsigned short* wcsupr(unsigned short* string)
{
	unsigned short *s;
	
	if (string)
	{
		for (s = string; *s; ++s)
			*s = toupper(*s);
	}
	return string;
}

unsigned short* wcslwr(unsigned short* string)
{
	unsigned short *s;
	
	if (string)
	{
		for (s = string; *s; ++s)
			*s = tolower(*s);
	}
	return string;
}
*/
int gg_wcslen(const unsigned short* string)
{
	const unsigned short *s;
	int _res = 0;
	if (string)
	{
		for (s = string; *s; ++s)
			_res++;
	}
	return _res;
}

int gg_wcsncmp(const unsigned short* lhs, const unsigned short* rhs, int len)
{
	if (len == 0) len = 0xfffff;
	if (lhs == 0)
	{
		if (rhs == 0)
			return 0;
		else
			return -1;
	}
	else if (rhs == 0)
		return 1;
	else {
		for (int i = 0; i < len; ++i)
		{
			int c = lhs[i];
			int diff = c - rhs[i];
			if (diff == 0)
			{
				if (!c)
					return 0;
			}
			else
			{
				return diff;
			}
		}
	}
	return 0;
}

int gg_wcscmp(const unsigned short* lhs, const unsigned short* rhs)
{
	return gg_wcsncmp(lhs, rhs, 0);
//	if (lhs == 0)
//	{
//		if (rhs == 0)
//			return 0;
//		else
//			return -1;
//	}
//	else if (rhs == 0)
//		return 1;
//	else {
//		for (; *lhs && *rhs; ++lhs, ++rhs)
//		{
//			if (*lhs != *rhs)
//				return *lhs - *rhs;
//		}
//		if (*lhs)
//			return 1;
//		else if (*rhs)
//			return -1;
//		else 
//			return 0;
//	}
}

const unsigned short* gg_wcschr(const unsigned short* string, unsigned short c)
{
	if (string)
	{
		for (string; *string; ++string)
		{
			if (*string == c)
				return string;
		}
	}
	return 0;
}

int strnicmp(const char* lhs, const char* rhs, int len)
{
	if (len == 0) len = 0xfffff;
	if (lhs == 0)
	{
		if (rhs == 0)
			return 0;
		else
			return -1;
	}
	else if (rhs == 0)
		return 1;
	else {
		for (int i = 0; i < len; ++i)
		{
			int c = toupper(lhs[i]);
			int diff = c-toupper(rhs[i]);//c - rhs[i];
			if(!diff)
				{
					if(!c) return diff;
				}
			else return diff;
//			switch (diff)
//			{
//				case -32:
//					if (c >= 'A' && c <= 'Z')
//						break;
//					return diff;
//				case 32:
//					if (c >= 'a' && c <= 'A')
//						break;
//					return diff;
//				case 0:
//					if (c)
//						break;
//					return 0;
//				default:
//					return diff;
//			}
		}
	}
	return 0;
	
}

int stricmp(const char* lhs, const char* rhs)
{
	return strnicmp(lhs, rhs, 0);
}
/*
int wcsnicmp(const unsigned short* lhs, const unsigned short* rhs, int len)
{
	if (len == 0) len = 0xfffff;
	if (lhs == 0)
	{
		if (rhs == 0)
			return 0;
		else
			return -1;
	}
	else if (rhs == 0)
		return 1;
	else {
		for (int i = 0; i < len; ++i)
		{
			int c = toupper(lhs[i]);
			int diff = c - toupper(rhs[i]);
			if(diff) return diff;
			else if(!c) return diff;
//			switch (diff)
//			{
//				case -32:
//					if (c >= 'A' && c <= 'Z')
//						break;
//					return diff;
//				case 32:
//					if (c >= 'a' && c <= 'A')
//						break;
//					return diff;
//				case 0:
//					if (c)
//						break;
//					return 0;
//				default:
//					return diff;
//			}
		}
	}
	assert(false);
	return 0;
	
}

int wcsicmp(const unsigned short* lhs, const unsigned short* rhs)
{
	return wcsnicmp(lhs, rhs, 0);
}
*/
unsigned short * gg_wcsstr ( 
						  const unsigned short * wcs1, 
						  const unsigned short * wcs2 
						  ) 
{ 
	unsigned short *cp = (unsigned short *) wcs1; 
	unsigned short *s1, *s2; 
	
	if ( !*wcs2) 
		return (unsigned short *)wcs1; 
	
	while (*cp) 
	{ 
		s1 = cp; 
		s2 = (unsigned short *) wcs2; 
		
		while ( *s1 && *s2 && !(*s1-*s2) ) 
			s1++, s2++; 
		
		if (!*s2) 
			return(cp); 
		
		cp++; 
	} 
	
	return 0; 
} 


