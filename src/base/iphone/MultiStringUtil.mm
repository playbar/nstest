/*
 *  MultiStringUtil.mm
 *  jqbar
 *
 *  Created by lining on 10-10-15.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MultiStringUtil.h"
#include <assert.h>
#include <iconv.h>
#ifdef __APPLE__
#include "wchar.h"
#endif

void* unicodeToNSString(const unsigned short* unicode)
{
	assert(unicode);
	if (!unicode) return 0;
	int unicode_len = 0;
	for (; unicode[unicode_len]; ++unicode_len)
	{
		
	}
	
	
	NSData* data = [[NSData alloc] initWithBytes:unicode length:unicode_len * 2];
	NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
	[data release];
	return str;
	
}

char* NSStringToUtf8(void* nsstring)
{
	NSString* str = (NSString*)nsstring;
	
	int retLen = [str maximumLengthOfBytesUsingEncoding:NSUTF8StringEncoding];
	char* utf8 = new char[retLen + 1];
	[str getCString:utf8 maxLength:retLen + 1 encoding:NSUTF8StringEncoding];
	utf8[retLen] = 0;
	return utf8;
}

char* NSStringToGbk(void* nsstring)
{
	NSString* str = (NSString*)nsstring;
	
	int retLen = [str maximumLengthOfBytesUsingEncoding:NSUTF8StringEncoding];
	char* utf8 = new char[retLen + 1];
	[str getCString:utf8 maxLength:retLen + 1 encoding:NSUTF8StringEncoding];
	utf8[retLen] = 0;

	char* gbk = utf8ToGbk(utf8);
	delete[] utf8;
	return gbk;
	
}

void* gbkToNSString(const char* gbk)
{
	int len = strlen(gbk);
	const unsigned short* unicode = gbkToUnicode(gbk, len);
	void* res = unicodeToNSString(unicode);
	delete[] unicode;
	return res;}

unsigned short* gbkToUnicode(const char* gbk, int& len)
{	
	size_t utf8_len = len * 2;
	char* utf8 = new char[utf8_len + 1];
	memset(utf8, 0, utf8_len + 1);
	iconv_t cd = iconv_open("utf-8", "gb2312");
	assert(cd);
	char* temp_gbk = (char*)gbk;
	char* temp_utf8 = utf8;
	int res = iconv(cd, (char**)&temp_gbk,  (size_t*)&len, &temp_utf8, &utf8_len);
	if (res == -1) 
	{
		//		assert(false);
		iconv_close(cd);
		unsigned short* buffer = new unsigned short[len + 1];
		for (int i = 0; i < len; ++i)
		{
			buffer[i] = gbk[i];
		}
		buffer[len] = 0;
		return buffer;
	}
	iconv_close(cd);
	
	unsigned short* unicode = utf8ToUnicode(utf8);
	delete[] utf8;
	len = wcslen((const wchar_t*)unicode)*2;
	return unicode;
	
//	NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_2312_80);;
//	NSString* str = [[NSString alloc] initWithCString:gbk encoding:enc];
//	int len = [str maximumLengthOfBytesUsingEncoding:NSUTF16LittleEndianStringEncoding];
//	unsigned short* _ret = new unsigned short[len / 2 + 1];
//	[str getCString:(char*)_ret maxLength:len + 2 encoding:NSUTF16LittleEndianStringEncoding];
//	_ret[len / 2] = 0;
//	[str release];
//	return _ret;
}

char* utf8ToGbk(const char* utf8)
{
	size_t utf8_len = strlen(utf8);
	char* gbk = new char[utf8_len * 2 + 1];
	memset(gbk, 0, utf8_len * 2 + 1);
	iconv_t cd = iconv_open("gb2312", "utf-8");
	assert(cd);
	size_t gbk_len = utf8_len * 2;
	char* temp_gbk = gbk;
	char* temp_utf8 = (char*)utf8;
	int errorNo = iconv(cd, &temp_utf8, &utf8_len, (char**)&temp_gbk, &gbk_len);
	if (errorNo == -1) 
	{
		memcpy(gbk, utf8, utf8_len + 1);
//		assert(false);
	}
	iconv_close(cd);
	return gbk;
}

char* gbkToUtf8(const char* gbk)
{
	size_t gbk_len = strlen(gbk);
	char* utf8 = new char[gbk_len * 2 + 1];
	memset(utf8, 0, gbk_len * 2 + 1);
	iconv_t cd = iconv_open("utf-8", "gb2312");
	APPLEASSERT(cd);
	size_t utf8_len = gbk_len * 2;
	char* temp_gbk = (char*)gbk;
	char* temp_utf8 = utf8;
	int errorNo = iconv(cd, (char**)&temp_gbk, &gbk_len, &temp_utf8, &utf8_len);
	if (errorNo == -1)
	{
		memcpy(utf8, gbk, gbk_len + 1);
//		APPLEASSERT(false);
	}
	iconv_close(cd);
	return utf8;
}

char* unicodeToGbk(const unsigned short* unicode)
{
	char* utf8 = unicodeToUtf8(unicode);
	
	size_t utf8_len = strlen(utf8);
	char* gbk = new char[utf8_len * 2 + 1];
	memset(gbk, 0, utf8_len * 2 + 1);
	iconv_t cd = iconv_open("gb2312", "utf-8");
	assert(cd);
	size_t gbk_len = utf8_len * 2;
	char* temp_gbk = gbk;
	char* temp_utf8 = utf8;
	int errorNo = iconv(cd, &temp_utf8, &utf8_len, (char**)&temp_gbk, &gbk_len);
	if (errorNo == -1) 
	{
		memcpy(gbk, utf8, utf8_len + 1);
	}
	iconv_close(cd);
	
	delete[] utf8;
	return gbk;
	
//	assert(unicode);
//	if (!unicode) return 0;
//	int unicode_len = 0;
//	for (; unicode[unicode_len]; ++unicode_len)
//	{
//		
//	}
	
	
	
//	NSStringEncoding out_enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_2312_80);
//	out_enc = NSUTF8StringEncoding;
//	NSData* data = [[NSData alloc] initWithBytes:unicode length:unicode_len * 2];
//	NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
//	int retLen = [str maximumLengthOfBytesUsingEncoding:out_enc];
//	char* _ret = new char[retLen + 1];
//	[str getCString:_ret maxLength:retLen + 1 encoding:out_enc];
//	_ret[retLen] = 0;
//	[str release];
//	[data release];
//	return _ret;
}

unsigned short* utf8ToUnicode(const char* utf8)
{
	NSStringEncoding enc = NSUTF8StringEncoding;
	NSString* str = [[NSString alloc] initWithCString:utf8 encoding:enc];
	int len = [str maximumLengthOfBytesUsingEncoding:NSUTF16LittleEndianStringEncoding];
	unsigned short* _ret = new unsigned short[len / 2 + 1];
	[str getCString:(char*)_ret maxLength:len + 2 encoding:NSUTF16LittleEndianStringEncoding];
	_ret[len / 2] = 0;
	[str release];
	return _ret;
}

char* unicodeToUtf8(const unsigned short* unicode)
{
	assert(unicode);
	if (!unicode) return 0;
	int unicode_len = 0;
	for (; unicode[unicode_len]; ++unicode_len)
	{
		
	}
	
	NSStringEncoding out_enc = NSUTF8StringEncoding;
	
	NSData* data = [[NSData alloc] initWithBytes:unicode length:unicode_len * 2];
	NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
	int retLen = [str maximumLengthOfBytesUsingEncoding:out_enc];
	char* _ret = new char[retLen + 1];
	[str getCString:_ret maxLength:retLen + 1 encoding:out_enc];
	_ret[retLen] = 0;
	[str release];
	[data release];
	return _ret;
}