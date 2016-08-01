// XXObjectStage.h: interface for the XXObjectStage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTSTAGE_H__7C085036_396F_48FE_ACB4_98FB460F34BF__INCLUDED_)
#define AFX_XXOBJECTSTAGE_H__7C085036_396F_48FE_ACB4_98FB460F34BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

const XPCTSTR _strAligns[]=
{
	"none",
	"left",
	"right",
	"center"
};

class XXObjectStage : public XXObject//Event  
{
	XDEC_XOBJECT(XXObjectStage)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);

	//void alignGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void alignSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void scaleModeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void scaleModeSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void showMenuGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void showMenuSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void widthGetStage(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void heightGetStage(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void addListenerStage(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void removeListenerStage(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	XXObjectStage(ScriptPlayer*pRoot);
	virtual ~XXObjectStage();
protected:
	void InitObject();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_STAGE;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
};

#endif // !defined(AFX_XXOBJECTSTAGE_H__7C085036_396F_48FE_ACB4_98FB460F34BF__INCLUDED_)
