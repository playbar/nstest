// XXObjectContextMenuItem.h: interface for the XXObjectContextMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTCONTEXTMENUITEM_H__E66CB0CA_E1CA_4794_BB2E_58A36EB9EC26__INCLUDED_)
#define AFX_XXOBJECTCONTEXTMENUITEM_H__E66CB0CA_E1CA_4794_BB2E_58A36EB9EC26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectContextMenuItem : public XXObject  
{
public:
	XXObjectContextMenuItem(ScriptPlayer*pRoot);
	virtual ~XXObjectContextMenuItem();

};

#endif // !defined(AFX_XXOBJECTCONTEXTMENUITEM_H__E66CB0CA_E1CA_4794_BB2E_58A36EB9EC26__INCLUDED_)
