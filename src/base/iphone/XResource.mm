// XResource.cpp: implementation of the XResource class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XResource.h"
#include "XImage.h"

#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//static XPCTSTR _strResClass[]={("TXT"),("GIF"),("DAT")};

#include <map>
#include <string>

typedef std::map<std::string, std::string> ResourceMap;

ResourceMap g_resourceMap;

void initResourceMap()
{
	g_resourceMap["ani"] = "er.gif";
	g_resourceMap["connect"] = "connect.gif";
	g_resourceMap["cursor"] = "cursor.gif";
	g_resourceMap["disk"] = "disk.gif";
	g_resourceMap["failed"] = "failed.gif";
	g_resourceMap["file"] = "file.gif";
	g_resourceMap["flash"] = "flash.gif";
	g_resourceMap["folder"] = "folder.gif";
	g_resourceMap["gogo"] = "gogo.gif";
	g_resourceMap["home"] = "home.gif";
	g_resourceMap["image"] = "image.gif";
	g_resourceMap["keya"] = "a_on.png";
	g_resourceMap["keyb"] = "b_on.png";
	g_resourceMap["keydown"] = "down_on.png";
	g_resourceMap["keyup"] = "up_on.png";
	g_resourceMap["keyleft"] = "left_on.png";
	g_resourceMap["keyright"] = "right_on.png";
	g_resourceMap["leftsoft"] = "keya1.bin";
	g_resourceMap["loading"] = "loading[1].gif";
	g_resourceMap["mainicon"] = "icon.gif";
	g_resourceMap["media"] = "media.gif";
	g_resourceMap["mobile"] = "mobile.gif";
	g_resourceMap["pdf"] = "pdf.gif";
	g_resourceMap["rar"] = "rar.gif";
	g_resourceMap["rightsoft"] = "keyb1.bin";
	g_resourceMap["select"] = "select.png";
	g_resourceMap["text"] = "text.gif";
	g_resourceMap["wml"] = "wml.gif";
	g_resourceMap["word"] = "word.gif";
	
	g_resourceMap["sorry"] = "dat1.bin";
	g_resourceMap["config"] = "config.wml";
	g_resourceMap["flashconfig"] = "flashcfg.xml";
	g_resourceMap["uiconfig"] = "uiconfig.txt";
}

std::string* getRealNameFromResourceName(XPCTSTR strRes)
{
	std::string name = strRes;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	ResourceMap::iterator iter = g_resourceMap.find(name);
	assert(iter != g_resourceMap.end());
	if (iter == g_resourceMap.end())
		return 0;
	return &(g_resourceMap[name]);
}

XBOOL XResource::LoadImage(XImage *pImage, XPCTSTR strRes,XBOOL bBitmap)
{
#ifdef __APPLE__
	std::string* realName = getRealNameFromResourceName(strRes);
	if (!realName) return XFALSE;
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSString* path = [[NSBundle mainBundle] bundlePath];
	NSString* filename = [[NSString alloc] initWithCString:realName->c_str()];
	
	NSString* filepath = [path stringByAppendingPathComponent:filename];
	NSFileHandle* handle = [NSFileHandle fileHandleForReadingAtPath:filepath];
	[filename release];
	assert(handle);
	if (!handle) 
	{
		[pool release];
		return XFALSE;
	}
	NSData* data = [handle availableData];
	if (!data) 
	{
		[pool release];
		return XFALSE;
	}
	pImage->Append([data bytes], [data length]);
	XBOOL ret = pImage->Final(bBitmap);
	[handle closeFile];
	 [pool release];
	 return ret;
#else
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
#endif
}


XBOOL XResource::LoadText(XString8 &strTxt, XPCTSTR strRes)
{
#ifdef __APPLE__
	std::string* realName = getRealNameFromResourceName(strRes);
	if (!realName) return XFALSE;
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSString* path = [[NSBundle mainBundle] bundlePath];
	
	NSString* filename = [[NSString alloc] initWithCString:realName->c_str()];
	
	NSString* filepath = [path stringByAppendingPathComponent:filename];
	NSFileHandle* handle = [NSFileHandle fileHandleForReadingAtPath:filepath];
	[filename release];
	assert(handle);
	if (!handle) 
	{
		[pool release];
		return XFALSE;
	}
	NSData* data = [handle availableData];
	if (!data) 
	{
		[pool release];
		return XFALSE;
	}
	
	strTxt.SetString((char*)[data bytes], [data length]);
	
	[handle closeFile];
	
	[pool release];
	return XTRUE;
#else
//    HRSRC hrsrc=::FindResource(NULL,strRes,_strResClass[0]);
	HRSRC hrsrc=::FindResourceA(NULL,strRes,_strResClass[0]);
	if(hrsrc==NULL) return FALSE;
	HGLOBAL hObj=::LoadResource(NULL,hrsrc);
	if(hObj==NULL) return FALSE;  
	strTxt=(XPCTSTR)::LockResource(hObj);//new BYTE[ file . GetLength( ) ];
	DeleteObject(hObj);
	return XTRUE;
#endif
}

XBOOL XResource::LoadData(XU8Array&data, XPCTSTR strRes)
{
#ifdef __APPLE__
	std::string* realName = getRealNameFromResourceName(strRes);
	if (!realName) return XFALSE;
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSString* path = [[NSBundle mainBundle] bundlePath];
	
	NSString* filename = [[NSString alloc] initWithCString:realName->c_str()];
	
	NSString* filepath = [path stringByAppendingPathComponent:filename];
	NSFileHandle* handle = [NSFileHandle fileHandleForReadingAtPath:filepath];
	[filename release];
	assert(handle);
	if (!handle) 
	{
		[pool release];
		return XFALSE;
	}
	
	NSData* ns_data = [handle availableData];
	if (!ns_data) 
	{
		[pool release];
		return XFALSE;
	}
	
	data.Append((unsigned char*)[ns_data bytes], [ns_data length]);
	
	[handle closeFile];
	
	[pool release];
	return XTRUE;
#else
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
#endif
}

XPCWSTR XResource::LoadWString(XU32 nID)
{
	
}
