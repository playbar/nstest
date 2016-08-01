// XXObjectXML.h: interface for the XXObjectXML class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTXML_H__5E30BBC0_8E4D_439B_B7C0_69D6A9CF8D89__INCLUDED_)
#define AFX_XXOBJECTXML_H__5E30BBC0_8E4D_439B_B7C0_69D6A9CF8D89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectXML : public XXObject  
{
public:
	XXObjectXML(ScriptPlayer*pRoot);
	virtual ~XXObjectXML();

};

#endif // !defined(AFX_XXOBJECTXML_H__5E30BBC0_8E4D_439B_B7C0_69D6A9CF8D89__INCLUDED_)
