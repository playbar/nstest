// XResource.cpp: implementation of the XResource class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XResource.h"
#include "XImage.h"
#include "XFile.h"
#ifdef _WINEMU
	#include "win32.rpp"
#elif defined(__APPLE__)
    #include "IPhoneDeviceData.h"
	#ifdef __IPHONE__
        #include "iphone.rpp"
	#elif defined(__IPAD__)
        #include "ipad.rpp"
	#else
		@Error@	// Please Define Device Macro
	#endif
#elif defined(__ANDROID__)
    #include "android.rpp"
#else
	@Error@ //Please Define Device Macro
#endif
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//static XPCTSTR _strResClass[]={("TXT"),("GIF"),("DAT")};
#ifdef _SYMBIAN
#include <stringloader.h>
#include <BARSREAD.h>
#include <EIKENV.h>
#endif

#include "XSort.h"
//static int _resCount=sizeof(_rssMapIndex)/sizeof(_RSSMapindex);

#ifdef __APPLE__
extern iOSDeviceRelateData g_deviceData;
//#ifdef __IPHONE__
//extern XString8 g_bundlePath_iPhone;
//#elif defined(__IPAD__)
extern XString8 g_bundlePath_ios;
//#endif
#endif

class _XResFinder:public XSort
	{
public:
	XINT  Compare(void* iSrt,void* iDst,XU8 mode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,mode);
	}
	XU32  GetCount()
	{
		return _RESCOUNT;//sizeof(_rssMapIndex)/sizeof(_RSSMapindex);//_resCount;
	}
	void* GetData(XU32 iData)
	{
		return (void*)_rssMapIndex[iData].strName;
	};
	};

static XU8* _FindResource(XPCTSTR strName,int&len)
{
	_XResFinder find;
	int id=find.Index((void*)strName,XString8::XLS_NOCASE);
	if(id<0) return XNULL;
	len=_rssMapIndex[id].nLength;
	return _rssMapIndex[id].pData;
}

XBOOL XResource::LoadImage(XImage *pImage, XPCTSTR strRes,XBOOL bBitmap)
{
	XINT nSize;
	XU8*pData=_FindResource(strRes,nSize);
	if(pData==XNULL) return XFALSE;
	pImage->Append(pData,nSize);
	return pImage->Final(bBitmap);
}


XBOOL XResource::LoadText(XString8 &strTxt, XPCTSTR strRes)
{
	int nSize = 0;
	XU8* pData=_FindResource(strRes,nSize);
	if(pData==XNULL) return XFALSE;
	if(pData[0]==0xef&&pData[1]==0xbb)
		strTxt.SetString((XPCTSTR)pData+2,nSize-2);
	else
		strTxt.SetString((XPCTSTR)pData,nSize);
	return XTRUE;
}

XBOOL XResource::LoadData(XU8Array&data, XPCTSTR strRes)
{
	XINT nSize;
	XU8*pData=_FindResource(strRes,nSize);
	if(pData==XNULL) return XFALSE;
	data.Append(pData,nSize);
	return XTRUE;
}

XPCWSTR XResource::LoadWString(XU32 nID)
{
	if(nID>=XIDS_MAX)
		return _strResString[XIDS_EMPTY];
	return _strResString[nID];
}

#ifdef __APPLE__
void translateFileNameToBundlePath(XPCTSTR sourceFile, XString8& destFile)
{
	const char* p = sourceFile;
	if (!p) return;
	while(*p)
	{
		if (*p == ':')
		{
			destFile = p + 1;
			p++;
			break;
		}
		p++;
	};

	if (*p)
	{
//#ifdef __IPHONE__
//            destFile = g_bundlePath_iPhone + destFile;
//#elif defined(__IPAD__)
            destFile = g_bundlePath_ios + destFile;
//#endif
	}
	else
	{
//#ifdef __IPHONE__
//            destFile = g_bundlePath_iPhone + sourceFile;
//#elif defined(__IPAD__)
            destFile = g_bundlePath_ios + sourceFile;
//#endif
	}
}

XBOOL XResource::Open(XString8 &strPath)
{
	if(hHandle) Close();

	XString8 destPath;
	translateFileNameToBundlePath(strPath, destPath);
	hHandle = fopen(destPath, "rb");
	if (!hHandle) return XFALSE;
	return XTRUE;
}

int XResource::Read(void *pData, int nLen)
{
	if(!hHandle) return 0;
	return fread(pData, 1, nLen, (FILE*)hHandle);
}

int XResource::Seek(int pos, int type)
{
	if(hHandle==XNULL) return XFALSE;
	int nType = type;
	switch(nType)
	{
		default:nType=SEEK_CUR;break;
		case XFile::XSEEK_SET:nType=SEEK_SET;break;
		case XFile::XSEEK_CUR:nType=SEEK_CUR;break;
		case XFile::XSEEK_END:nType=SEEK_END;break;
	}
	return ::fseek((FILE*)hHandle,pos,nType)==0;
}

void XResource::Close()
{
	if(hHandle)
	{
		fclose((FILE*)hHandle);
	}
}
#else
XBOOL XResource::Open(XString8 &strPath)
{
	if(hHandle) Close();
	XFile* file=new XFile;
	if(!file) return XFALSE;
	if(!file->Open(strPath,XFile::XREAD|XFile::XBINARY))
	{
		delete file;
		return XFALSE;
	}
	hHandle=(XHANDLE)file;
	return XTRUE;
}

int XResource::Read(void *pData, int nLen)
{
	if(!hHandle) return 0;
	XFile*file=(XFile*)hHandle;
	return file->Read(pData,nLen);
}

int XResource::Seek(int pos, int type)
{
	if(!hHandle) return 0;
	XFile* file=(XFile*)hHandle;
	return file->Seek(pos,type);
}

void XResource::Close()
{
	if(hHandle)
	{
		((XFile*)hHandle)->Close();
		delete (XFile*)hHandle;
	}
}
#endif

XResource::XResource()
{
	hHandle=0;
}
