// XXObjectSelection.h: interface for the XXObjectSelection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTSELECTION_H__67C6EF2B_D57C_4E3D_87CF_4F498FCB7C11__INCLUDED_)
#define AFX_XXOBJECTSELECTION_H__67C6EF2B_D57C_4E3D_87CF_4F498FCB7C11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectSelection : public XXObject  
{
//	XDEC_XOBJECT(XXObjectSelection)
public:
	XXObjectSelection(ScriptPlayer*pRoot);
	virtual ~XXObjectSelection();

};

#endif // !defined(AFX_XXOBJECTSELECTION_H__67C6EF2B_D57C_4E3D_87CF_4F498FCB7C11__INCLUDED_)
