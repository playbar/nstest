#pragma once

#define Unoneerr	0;
#define Uerr		-1;
#define Unofind		-2;


class CStaticUtil
{
public:
	CStaticUtil(void);
	~CStaticUtil(void);
public:
	static int movefile(const char* dpath,const char* spath);
	static int removefile(const char* dpath);
	//在str1中找str2，返回str2在str1中的位置
	static int findstr(const char* str1,const char* str2,int len = -1);
	//在str1中找str2，返回str2在str1中从左到右的位置，offset为偏移量，style为true从左到右寻找，false，从右到左寻找
	static int findstr(const char* str1,const char* str2,int offset,bool style);
	//替换文件中的目标字段，content替换为src
	static bool replaceFileContent(const char* filename,const char* content,const char* src);
	//gbk转utf8,返回utf8串的长度
	static int GbkToUtf8(char*& utf8,const char* gbk);
};

