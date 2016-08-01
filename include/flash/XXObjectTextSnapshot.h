// XXObjectTextSnapshot.h: interface for the XXObjectTextSnapshot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTTEXTSNAPSHOT_H__414CEAFB_3A33_49F2_8E61_0A8E487EA99B__INCLUDED_)
#define AFX_XXOBJECTTEXTSNAPSHOT_H__414CEAFB_3A33_49F2_8E61_0A8E487EA99B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectTextSnapshot : public XXObject  
{
public:
	//XBOOL InitObject(){return XTRUE;}
	XXObjectTextSnapshot(ScriptPlayer*pRoot);
	virtual ~XXObjectTextSnapshot();

};

#endif // !defined(AFX_XXOBJECTTEXTSNAPSHOT_H__414CEAFB_3A33_49F2_8E61_0A8E487EA99B__INCLUDED_)
