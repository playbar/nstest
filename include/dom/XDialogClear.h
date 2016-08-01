// XDialogClear.h: interface for the XDialogClear class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDIALOGCLEAR_H__D1FFA217_B0FD_4FB5_835D_9C8597ED290A__INCLUDED_)
#define AFX_XDIALOGCLEAR_H__D1FFA217_B0FD_4FB5_835D_9C8597ED290A__INCLUDED_

#include "XCatch.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __ANDROID__
#else

class XDialogClear : public XMessageBox,XBase
{
public:
	void SetProcCount(XU32 nCount);
	void SetCount(XU32 nCount);
	//XU32 DoModal();
	static XDialogClear* Clear(XPCTSTR strWorkPath,XU8 bAll)
	{
	   return new XDialogClear(strWorkPath,bAll);
	}
	virtual ~XDialogClear();
	XBOOL NeedStop();
	void EndDialog(XU32);
protected:
	XDialogClear(XPCTSTR strWorkPath,XU8 bAll);
	XUINT CallBack(XU32);
	
	XEvent m_stop;
	
	XThread m_thread;
	XU8 m_bAll;
	XBOOL OnPaint(XGraphics&g);
	XCatch m_catch;
	XUINT m_nCount,m_nProcCount;
};
#endif

#endif // !defined(AFX_XDIALOGCLEAR_H__D1FFA217_B0FD_4FB5_835D_9C8597ED290A__INCLUDED_)
