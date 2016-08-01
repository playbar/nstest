/*
 *  MultiStringUtil.h
 *  jqbar
 *
 *  Created by lining on 10-10-15.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MULTI_STRING_UTIL_H_
#define _MULTI_STRING_UTIL_H_

#if defined(__cplusplus) && __cplusplus
extern "C" {
#endif
#ifdef __APPLE__
void* gbkToNSString(const char* gbk);
void* unicodeToNSString(const unsigned short* unicode);
//char* NSStringToGbk(void*);
char* NSStringToUtf8(void*);
char* NSStringToGbk(void*);
#endif
unsigned short* gbkToUnicode(const char* gbk, int& len);
char* unicodeToGbk(const unsigned short* unicode);
unsigned short* utf8ToUnicode(const char* utf8);
char* unicodeToUtf8(const unsigned short* unicode);
char* utf8ToGbk(const char* utf8);
char* gbkToUtf8(const char* gbk);
#if defined(__cplusplus) && __cplusplus
}
#endif
	


#endif