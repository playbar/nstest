// XXObjectLocal.h: interface for the XXObjectLocal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTLOCAL_H__22CB3E7F_59D9_46D3_96EC_3852259C697C__INCLUDED_)
#define AFX_XXOBJECTLOCAL_H__22CB3E7F_59D9_46D3_96EC_3852259C697C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectLocal : public XXObject  
{
public:
	XXObjectLocal(ScriptPlayer*pRoot);
	virtual ~XXObjectLocal();

};

#endif // !defined(AFX_XXOBJECTLOCAL_H__22CB3E7F_59D9_46D3_96EC_3852259C697C__INCLUDED_)
