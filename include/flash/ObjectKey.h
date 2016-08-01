// ObjectKey.h: interface for the XXObjectKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTKEY_H__81408877_98D6_45F0_853F_DF2E31E273C8__INCLUDED_)
#define AFX_OBJECTKEY_H__81408877_98D6_45F0_853F_DF2E31E273C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

enum 
{
	XXBACKSPACE	=8,
	XXCAPSLOCK	=20,
	XXCONTROL	=17,
	XXDELETE	=46,
	XXDOWN		=40,
	XXEND		=35,
	XXENTER		=13,
	XXESCAPE	=27,
	XXHOME		=36,
	XXINSERT	=45,
	XXLEFT		=37,
	XXPAGEDOWN	=34,
	XXPAGEUP	=33,
	XXRIGHT		=39,
	XXSHIFT		=16,
	XXSPACE		=32,
	XXTAB		=9,
	XXUP		=38
};

class XXObjectKey : public XXObject//Event  
{
	XDEC_XOBJECT(XXObjectKey);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	XXObjectKey(ScriptPlayer*pRoot);
	virtual ~XXObjectKey();
	//void listenersGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void addListener(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void getAscii(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void getCode(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void isAccessible(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void isDown(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void isToggled(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void removeLisenter(XSWFCONTEXT&cnt,XXVARLIST&list);
protected:
	virtual XU32 GetObjectType()
	{
		return XXOBJ_KEY;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	void InitObject();
};

#endif // !defined(AFX_OBJECTKEY_H__81408877_98D6_45F0_853F_DF2E31E273C8__INCLUDED_)
