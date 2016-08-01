#include "StdAfx.h"
#include "StaticUtil.h"
#include <stdio.h>
#include <string.h>


CStaticUtil::CStaticUtil(void)
{
}


CStaticUtil::~CStaticUtil(void)
{
}

int CStaticUtil::movefile(const char* dpath,const char* spath)
{
	if(dpath == NULL || spath == NULL) return Uerr;
	FILE *fld;
	FILE *fls;
	fld = fopen(dpath,"rb");
	if(fld == NULL) return Unofind;
	::DeleteFileA(spath);
	fls = fopen(spath,"ab");
	
	char* buf = new char[1024];
	int len = 0;
	while(!feof(fld))
	{
		len = fread(buf,1,1024,fld);
		fwrite(buf,1,len,fls);
	}

	fclose(fls);
	fclose(fld);
}

int CStaticUtil::removefile(const char* dpath)
{
	return ::DeleteFileA(dpath);
}

int CStaticUtil::findstr(const char* str1,const char* str2,int len)
{
	if(len<0) len = strlen(str2);
	char *p1,*p2;
	p1 = (char*)str1;
	p2 = (char*)str2;
	int len1 = strlen(str1);
	int d = len1 - len;
	if(d<0) return -1;
	while(p1-str1<=d)
	{
		int i(0);
		while(p1[i]==p2[i] && ++i<len);
		if(i>=len) return p1-str1;
		p1++;
	}
	return -1;
}

int CStaticUtil::findstr(const char* str1,const char* str2,int offset,bool style)
{
	if(style)
	{
		char* p1 = (char*)str1+offset;
		char* p2 = (char*)str2;
		int len = findstr(p1,p2);
		return len>0 ? len+offset : len;
	}
	else
	{
		int len1,len2;
		len1 = strlen(str1);
		len1-=offset;
		char *p1 = (char*)str1+len1-1;
		len2 = strlen(str2);
		char *p2 = (char*)str2;
		int d = len1-len2;
		if(d<=0) return -1;
		while(p1-str1>len2)
		{
			int i(0);
			while(*(p1+i) == *(p2+i) && i++<len2);
			if(i>=len2) return p1-str1;
			p1--;
		}
		return -1;
	}
}

bool CStaticUtil::replaceFileContent(const char* filename,const char* content,const char* src)
{
	FILE *fl = fopen(filename,"rb+");
	if(!fl) return false;
	::fseek(fl,0,SEEK_END);
	int flen = ::ftell(fl);
	fseek(fl,0,SEEK_SET);
	char *buf = new char[flen+1];
	memset(buf,0,flen+1);
	int clen = strlen(content);
	int slen = strlen(src);
	::fread(buf,1,flen,fl);
	fclose(fl);

	fl = fopen(filename,"wb+");
	int len(0);
	bool b(false);
	char *temp = buf;
	while(1)
	{
		len = findstr(temp,content,clen);
		if(len<0) break;
		::fwrite(temp,1,len,fl);
		::fwrite(src,1,slen,fl);
		temp+=len+clen;
		b = true;
	}
	::fwrite(temp,1,flen-(temp-buf),fl);
	fclose(fl);

	delete buf;
	return b;
}

int CStaticUtil::GbkToUtf8(char*& utf8,const char* gbk)
{
	if(!gbk) return -1;
	int len;
	len = ::MultiByteToWideChar(CP_ACP,0,(char*)gbk,-1,NULL,NULL);
	wchar_t* unicode = new wchar_t[len+1];
	len = ::MultiByteToWideChar(CP_ACP,0,(char*)gbk,-1,unicode,len);
	if(len <= 0)
		return -1;
	len = ::WideCharToMultiByte(CP_UTF8,0,unicode,-1,NULL,0,NULL,NULL);
	if(utf8)
		delete utf8;
	utf8 = new char[len+1];
	len = ::WideCharToMultiByte(CP_UTF8,0,unicode,-1,utf8,len,NULL,NULL);
	delete unicode;
	return len;
}