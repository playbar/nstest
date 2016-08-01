// XDomOptGroup.h: interface for the XDomOptGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMOPTGROUP_H__FADEEC82_FE95_4293_8366_2A964C88AB9E__INCLUDED_)
#define AFX_XDOMOPTGROUP_H__FADEEC82_FE95_4293_8366_2A964C88AB9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomFieldset.h"

class _XEXT_CLASS XDomOptGroup : public XDomFieldset  
{
	XDEC_CREATE(XDomOptGroup)
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomOptGroup(XU16 ID=XDOM_OPTGROUP):XDomFieldset(ID)
	{
	}
};

#endif // !defined(AFX_XDOMOPTGROUP_H__FADEEC82_FE95_4293_8366_2A964C88AB9E__INCLUDED_)
