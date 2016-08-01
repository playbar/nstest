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

size_t gg_wcslen(const unsigned short* string)
{
	const unsigned short *s;
	size_t _res = 0;
	if (string)
	{
		for (s = string; *s; ++s)
			_res++;
	}
	return _res;
}

int gg_wcsncmp(const unsigned short* first, const unsigned short* last, int count)
{
		if (!count)
                return(0);

        while (--count && *first && *first == *last)
        {
                first++;
                last++;
        }

        return((int)(*first - *last));
	/*if (len == 0) len = 0xfffff;
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
	return 0;*/
}

int gg_wcscmp(const unsigned short* dst, const unsigned short* src)
{
	int ret = 0 ;

    while( ! (ret = (int)(*src - *dst)) && *dst)
            ++src, ++dst;

    if ( ret < 0 )
            ret = -1 ;
    else if ( ret > 0 )
            ret = 1 ;

    return( ret );
//	return gg_wcsncmp(lhs, rhs, 0);
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

int strnicmp(const char* first, const char* last, int count)
{
	//if (len == 0) len = 0xfffff;
	//if (lhs == 0)
	//{
	//	if (rhs == 0)
	//		return 0;
	//	else
	//		return -1;
	//}
	//else if (rhs == 0)
	//	return 1;
	//else {
	//	for (int i = 0; i < len; ++i)
	//	{
	//		int diff = tolower(lhs[i]) - tolower(rhs[i]);
	//		if (diff != 0) return diff;
	//		if (lhs[i] == 0)
	//			return 0;
	//	}
	//}
	//return 0;
	
	if(count)
    {
        int f=0;
        int l=0;

        do
        {

            if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
                    (f <= 'Z') )
                f -= 'A' - 'a';

            if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
                    (l <= 'Z') )
                l -= 'A' - 'a';

        }
        while ( --count && f && (f == l) );

        return ( f - l );
    }
    else
    {
        return 0;
    }
}

int stricmp(const char* dst, const char* src)
{
	int f, l;

    do
    {
        if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
            l -= 'A' - 'a';
    }
    while ( f && (f == l) );

    return(f - l);
//	return strnicmp(lhs, rhs, 0);
}

int wcsnicmp(const unsigned short* first, const unsigned short* last, int count)
{
	//if (len == 0) len = 0xfffff;
	//if (lhs == 0)
	//{
	//	if (rhs == 0)
	//		return 0;
	//	else
	//		return -1;
	//}
	//else if (rhs == 0)
	//	return 1;
	//else {
	//	for (int i = 0; i < len; ++i)
	//	{
	//		int diff = tolower(lhs[i]) - tolower(rhs[i]);
	//		if (diff != 0) return diff;
	//		if (lhs[i] == 0)
	//			return 0;
	//	}
	//}
	//assert(false);
	//return 0;
	if(count)
    {
        int f=0;
        int l=0;

        do
        {

            if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
                    (f <= 'Z') )
                f -= 'A' - 'a';

            if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
                    (l <= 'Z') )
                l -= 'A' - 'a';

        }
        while ( --count && f && (f == l) );

        return ( f - l );
    }
    else
    {
        return 0;
    }
}

int wcsicmp(const unsigned short* dst, const unsigned short* src)
{
	int f, l;

    do
    {
        if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
            l -= 'A' - 'a';
    }
    while ( f && (f == l) );

    return(f - l);
//	return wcsnicmp(lhs, rhs, 0);
}

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
	
	return(NULL); 
} 


