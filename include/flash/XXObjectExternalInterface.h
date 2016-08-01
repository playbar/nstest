// XXObjectExternalInterface.h: interface for the XXObjectExternalInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTEXTERNALINTERFACE_H__4595530B_EEE7_4745_B7E1_D06D003BBF8E__INCLUDED_)
#define AFX_XXOBJECTEXTERNALINTERFACE_H__4595530B_EEE7_4745_B7E1_D06D003BBF8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectExternalInterface : public XXObject  
{
public:
	XXObjectExternalInterface(ScriptPlayer*pRoot);
	virtual ~XXObjectExternalInterface();

};

#endif // !defined(AFX_XXOBJECTEXTERNALINTERFACE_H__4595530B_EEE7_4745_B7E1_D06D003BBF8E__INCLUDED_)
