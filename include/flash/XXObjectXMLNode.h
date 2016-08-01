// XXObjectXMLNode.h: interface for the XXObjectXMLNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTXMLNODE_H__C5D39A20_5464_411D_A3BC_E19F981BA51B__INCLUDED_)
#define AFX_XXOBJECTXMLNODE_H__C5D39A20_5464_411D_A3BC_E19F981BA51B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectXMLNode : public XXObject  
{
public:
	XXObjectXMLNode(ScriptPlayer*pRoot);
	virtual ~XXObjectXMLNode();

};

#endif // !defined(AFX_XXOBJECTXMLNODE_H__C5D39A20_5464_411D_A3BC_E19F981BA51B__INCLUDED_)
