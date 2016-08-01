/*
 * zcconvert.cpp
 *
 *  Created on: 2009-11-6
 *      Author: Administrator
 */

#include <charconv.h>  //Link against: charconv.lib  
#include <f32file.h>   //Link against: efsrv.lib 
#include <e32std.h>
#include <coemain.h>   //Link against: cone.lib
#include <string.h>		//for strlen()
#include <escapeutils.h>

#include "ZCConvert.h"
#include "symbiandebug.h"
#include <stdio.h>
#include <iconv.h>
#include <assert.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZCConvert::ZCConvert()
{
 
}

ZCConvert::~ZCConvert()
{
 
}
TText16* ZCConvert::ConvertToUnicode(const TText8* aSource,int len)
{
//����һ��ת������ת�����������ջ
	CCnvCharacterSetConverter* converter=CCnvCharacterSetConverter::NewLC();
	RFs fSession;
//��Ҫ���ļ�������
//	User::LeaveIfError(fSession.Connect());
	fSession.Connect();
//�жϴ����Դ�ַ��Ƿ�GBK\GB2123��
	if(converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierGbk,
			fSession)!=CCnvCharacterSetConverter::EAvailable)
		{
//�����GBK\GBK2123����˳�
		fSession.Close();
		CleanupStack::Pop(converter);
		delete converter;
		User::Leave(KErrNotSupported);
		}

	TInt state=CCnvCharacterSetConverter::KStateDefault;

	TPtrC8 source;
	if(len<=0)
		source.Set(aSource);
	else
		source.Set(aSource,len);
	int nMax=source.Length()+1;
	TText16* iInfoText=new TText16[nMax];
	memset(iInfoText,0,nMax*2);
	TPtr16 ptr(iInfoText,source.Length());

	if(CCnvCharacterSetConverter::EErrorIllFormedInput == converter->ConvertToUnicode(ptr, source, state))
		User::Leave(KErrArgument);
//ת����ɲ����ת���� 
	CleanupStack::PopAndDestroy();
	fSession.Close();
//����unicode�����������,�ǵ��ڵ������������ʾ���ĺ�Ҫɾ���������
//	*(iInfoText+source.Length())=0;
	return iInfoText;
}

TText8* ZCConvert::ConvertToGBK(const TText16* aSource)
{
//����һ��ת������ת�����������ջ
	CCnvCharacterSetConverter* converter=CCnvCharacterSetConverter::NewLC();
	RFs fSession;
//��Ҫ���ļ�������
//	User::LeaveIfError(fSession.Connect());
	fSession.Connect();
//�жϴ����Դ�ַ��Ƿ�GBK\GB2123��
	if(converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierGbk,
			fSession)!=CCnvCharacterSetConverter::EAvailable)
		{
//�����GBK\GBK2123����˳�
		fSession.Close();
		CleanupStack::Pop(converter);
		delete converter;
		User::Leave(KErrNotSupported);
		}

	//TInt state=CCnvCharacterSetConverter::KStateDefault;

	const TPtrC16 source( aSource );
	int nMax=source.Length()*2+1;
	TText8* iInfoText = new TText8[nMax];
	memset(iInfoText,0,nMax);
	TPtr8 ptr(iInfoText,source.Length()*2);

	if(CCnvCharacterSetConverter::EErrorIllFormedInput == converter->ConvertFromUnicode(ptr, source))
		User::Leave(KErrArgument);
//ת����ɲ����ת���� 
	CleanupStack::PopAndDestroy();
	fSession.Close();
//����GBK�����������,�ǵ��ڵ������������ʾ���ĺ�Ҫɾ���������
	iInfoText[nMax-1]=0;
	return iInfoText;
}

TText* ZCConvert::ConvertFromUTF8(const TText8* aSource,int len)
{
	/*
//����һ��ת������ת�����������ջ
	CCnvCharacterSetConverter* converter=CCnvCharacterSetConverter::NewLC();
	RFs fSession;
//���ļ�������
//	User::LeaveIfError(fSession.Connect());
	fSession.Connect();
//�жϴ����Դ�ַ��Ƿ�Utf8��
	if(converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8,
			fSession)!=CCnvCharacterSetConverter::EAvailable)
		{
//����Ǿ��˳�
		fSession.Close();
		CleanupStack::Pop(converter);
		delete converter;
		User::Leave(KErrNotSupported);
		}

	TInt state=CCnvCharacterSetConverter::KStateDefault;

	TPtrC8 source( aSource );
	int nMax=source.Length()+1;
	TText16* iInfoText=new TText16[nMax];
	memset(iInfoText,0,nMax*2);
	TPtr16 ptr(iInfoText,source.Length());

	if(CCnvCharacterSetConverter::EErrorIllFormedInput == converter->ConvertToUnicode(ptr, source, state))
		User::Leave(KErrArgument);
//ת����ɲ����ת���� 
	CleanupStack::PopAndDestroy();
	fSession.Close();
//����unicode�����������,�ǵ��ڵ������������ʾ���ĺ�Ҫɾ���������
//	iInfoText[source.Length()]=0;
	return iInfoText;
	*/
	if(len<=0)
		len = strlen((char*)aSource);
	TPtrC8 prt(aSource,len);
	HBufC *test=EscapeUtils::ConvertToUnicodeFromUtf8L(prt);
	len=test->Length();
	unsigned short *src=(unsigned short*)test->Des().Ptr();
	unsigned short *des=new unsigned short[len+1];
	memmove(des,src,len<<1);
	delete test;
	des[len]=0x0000;
	return des;
}

//////////////////////////////////////////////////////////////
//
//��8λ��GBKתΪ8λUTF8
//
//�ÿ�ָ����ܷ���ֵ��ʹ�ý������ܵ�ָ����delete����
TText8* ZCConvert::ConvertGBKtoUTF8(const TText8* aSource)
{
	iconv_t cd=NULL;
	size_t gbk_l = strlen((char*)aSource);
	size_t utf8_l = gbk_l<<1+1;
	cd=iconv_open("UTF-8","GBK");
	assert((size_t)cd != (size_t)-1);
	char* utf8_b = new char[utf8_l];
	char* gbk_temp=(char*)aSource;
	char* utf8_temp=utf8_b;
	memset(utf8_b,0,utf8_l);
	size_t size=iconv(cd,(const char**)&gbk_temp,&gbk_l,&utf8_temp,&utf8_l);
	iconv_close(cd);
	if(size == (size_t)-1)
	{
		memmove(utf8_b,aSource,gbk_l);
		utf8_b[gbk_l]=0;
	}
	return (TText8*)utf8_b;
}

//////////////////////////////////////////////////////////////
//utf-8 to gbk
TText8* ZCConvert::ConvertUTF8toGBK(const TText8* aSource)
{
	iconv_t cd=NULL;
	size_t utf8_l = strlen((char*)aSource);
	size_t gbk_l = utf8_l+1;
	cd=iconv_open("GBK","UTF-8");
	assert((size_t)cd != (size_t)-1);
	char* gbk_b = new char[gbk_l];
	memset(gbk_b,0,gbk_l);
	char* gbk_temp = gbk_b;
	const char* utf8_temp = (const char*)aSource;
	size_t size = iconv(cd,&utf8_temp,&utf8_l,&gbk_temp,&gbk_l);
	iconv_close(cd);
	if(size == (size_t)-1)
		{
		memmove(gbk_b,aSource,utf8_l);
		gbk_b[utf8_l] = 0;
		}
	return (TText8*)gbk_b;
}
