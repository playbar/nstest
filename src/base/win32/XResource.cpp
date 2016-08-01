// XResource.cpp: implementation of the XResource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XResource.h"
#include "XImage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static XPCTSTR _strResClass[]={("TXT"),("GIF"),("DAT")};

XBOOL XResource::LoadImage(XImage *pImage, XPCTSTR strRes,XBOOL bBitmap)
{
    HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[1]);
	int nSize=SizeofResource(NULL,hrsrc);
	if(hrsrc==NULL) return XFALSE;
	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
	if(hObj==NULL) return XFALSE;  
	void* pData=LockResource(hObj);
	pImage->Append(pData,nSize);
	UnlockResource(pData);
	//img.Create(hObj);
	DeleteObject(hObj);
	return pImage->Final(bBitmap);
}


XBOOL XResource::LoadText(XString8 &strTxt, XPCTSTR strRes)
{
//    HRSRC hrsrc=::FindResource(NULL,strRes,_strResClass[0]);
	HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[0]);
	if(hrsrc==NULL) return FALSE;
	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
	if(hObj==NULL) return FALSE;  
	strTxt=(XPCTSTR)::LockResource(hObj);//new BYTE[ file . GetLength( ) ];
	DeleteObject(hObj);
	return XTRUE;
}

XBOOL XResource::LoadData(XU8Array&data, XPCTSTR strRes)
{
    HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[2]);
	int nSize=SizeofResource(NULL,hrsrc);
	if(hrsrc==NULL) return XFALSE;
	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
	if(hObj==NULL) return XFALSE; 
	XU8* pData=(XU8*)LockResource(hObj);
	data.Append(pData,nSize);
	
	UnlockResource(pData);
	//img.Create(hObj);
	DeleteObject(hObj);
	return XTRUE;
}

XPCWSTR XResource::LoadString(XU32 nID)
{

};

