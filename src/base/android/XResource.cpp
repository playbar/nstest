// XResource.cpp: implementation of the XResource class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XResource.h"
#include "XImage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//static XPCTSTR _strResClass[]={("TXT"),("GIF"),("DAT")};
#ifdef _SYMBIAN
#include <stringloader.h>
#include <BARSREAD.h>
#include <EIKENV.h>

#include "GGClientN82.rpp"
#include "XSort.h"
static int _resCount=sizeof(_rssMapIndex)/sizeof(_RSSMapindex);

class _XResFinder:public XSort
	{
public:
	XINT  Compare(void* iSrt,void* iDst,XU8 mode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,mode);
	}
	XU32  GetCount()
	{
		return _resCount;
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

#endif

//XBOOL XResource::LoadImage(XImage *pImage, XPCTSTR strRes,XBOOL)
//{
////#ifdef _SYMBIAN
////	XINT nSize;
////	XU8*pData=_FindResource(strRes,nSize);
////	if(pData==XNULL) return XFALSE;
////	return pImage->Create(pData,nSize);
////#elif define(_WIN32)
////    HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[1]);
////	int nSize=SizeofResource(NULL,hrsrc);
////	if(hrsrc==NULL) return XFALSE;
////	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
////	if(hObj==NULL) return XFALSE;  
////	void* pData=LockResource(hObj);
////	pImage->Append(pData,nSize);
////	UnlockResource(pData);
////	//img.Create(hObj);
////	DeleteObject(hObj);
////	return pImage->Final(bBitmap);
////#endif
//	return NULL;
//}


//XBOOL XResource::LoadText(XString8 &strTxt, XPCTSTR strRes)
//{
////    HRSRC hrsrc=::FindResource(NULL,strRes,_strResClass[0]);
////#ifdef _SYMBIAN
////	int nSize;
////	XU8* pData=_FindResource(strRes,nSize);
////	if(pData==XNULL) return XFALSE;
////	strTxt.SetString((XPCTSTR)pData,nSize);
////	return XTRUE;
////#elif define(_WIN32)
////	HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[0]);
////	if(hrsrc==NULL) return FALSE;
////	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
////	if(hObj==NULL) return FALSE;  
////	strTxt=(XPCTSTR)::LockResource(hObj);//new BYTE[ file . GetLength( ) ];
////	DeleteObject(hObj);
////	return XTRUE;
////#endif
//	return XTRUE;
//}

//XBOOL XResource::LoadData(XU8Array&data, XPCTSTR strRes)
//{
//#ifdef _SYMBIAN
//	/*TResourceReader reader;
//	CEikonEnv::Static()->CreateResourceReaderLC(reader,*((int*)strRes));
//	int nSize=reader.ReadHBufC8L()->Size();
//	XU8 *pData=(XU8*)reader.Ptr();
//	data.Append(pData,nSize);
//	CleanupStack::PopAndDestroy();*/
//	XINT nSize;
//	XU8*pData=_FindResource(strRes,nSize);
//	if(pData==XNULL) return XFALSE;
//	data.Append(pData,nSize);
//	return XTRUE;
//#elif defined(_WIN32)	
//    HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[2]);
//	int nSize=SizeofResource(NULL,hrsrc);
//	if(hrsrc==NULL) return XFALSE;
//	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
//	if(hObj==NULL) return XFALSE; 
//	XU8* pData=(XU8*)LockResource(hObj);
//	data.Append(pData,nSize);
//	
//	UnlockResource(pData);
//	//img.Create(hObj);
//	DeleteObject(hObj);
//#endif
//	return XTRUE;
//}
